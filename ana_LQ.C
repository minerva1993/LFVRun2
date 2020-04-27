#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "ROOT/RDF/RInterface.hxx"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLatex.h"
#include "TLegend.h"
#include "Math/Vector4Dfwd.h"
#include "TStyle.h"
#include <vector>
#include <ROOT/RSnapshotOptions.hxx>

using namespace ROOT::VecOps;
using rvec_f = RVec<float>;
using rvec_b = RVec<bool>;
using rvec_i = RVec<int>;

template <typename T>
void plot(T hist, TString name){
  TCanvas * c = new TCanvas("c",Form("c_%s", name.Data()));
  hist->Write();
 // c->Print(Form("%s.pdf",name.Data()));
}

//good jets index
vector<int> goodjets_idx(rvec_i g){ 
  vector<int> out;
  for(int i = 0; i < g.size(); i++){
    if( g[i] ) out.push_back( i );
  }
  return out; 
}
//categorize events ( 0:lf, 1:b, 2:bb, 3:c, 4:, cc )
int categorize(int g){
  int out;
  if( g == 0 ) out = 0;
  else if( g == 51 || g == 52 ) out = 1;
  else if( g == 53 || g == 54 || g == 55 ) out = 2;
  else if( g == 41 || g == 42 ) out = 3;
  else if( g == 43 || g == 44 || g == 45 ) out = 4;
  return out;
}

void ana_LQ(){

  ROOT::RDataFrame df("Events","/xrootd/store/mc/RunIIAutumn18NanoAODv6/TT_noSC_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/250000/1622691F-875A-1149-802C-3BA3B98B6C6C.root");
  ROOT::RDataFrame dfs("Events","/cms/ldap_home/ljw1015/public/LQ_Signals/LQ_2018_nano.root");
  
  //background events selection
  auto df_S1_ttbar = df.Filter("genTtbarId > 0", "ttbar events");
  auto df_S1_muon = df_S1_ttbar.Filter("nMuon >= 1", "Events with one lepton")
                                .Define("goodmuons","Muon_pt > 20  && abs(Muon_eta) < 2.4 && Muon_tightId && Muon_pfRelIso04_all < 0.15")
                                .Define("goodtaus","Tau_pt > 20 && abs(Tau_eta) < 2.3 && Tau_idMVAoldDM & 2");
  auto df_S1_goodmuon = df_S1_muon.Filter("Sum( goodmuons ) >=1 ","Events with at least a goood muon");
  auto df_S1_goodtau = df_S1_goodmuon.Filter("Sum ( goodtaus ) >=1", "Events with at least a good tau")
                                     .Define("ntaujets", "Sum( goodtaus )")  
                                     .Define("CvsB","Jet_btagDeepC/(Jet_btagDeepC+Jet_btagDeepB)")
                                     .Define("Jet_btagDeepLF", "1-(Jet_btagDeepC+Jet_btagDeepB)")
                                     .Define("CvsL", "Jet_btagDeepC/(Jet_btagDeepC+Jet_btagDeepLF)")
                                     .Define("goodcjets_m","Jet_pt > 20 && abs(Jet_eta) < 2.4 && CvsB >0.29 && CvsL > 0.137")
                                     .Define("goodcjets_t","Jet_pt > 20 && abs(Jet_eta) < 2.4 && CvsB >0.1 && CvsL > 0.66")
                                     .Define("goodbjets_m","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepB > 0.4184")
                                     .Define("goodbjets_t","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepB > 0.7527")
                                     .Define("goodjets", "Jet_pt > 20 && abs(Jet_eta) < 2.4");
  auto df_S1_goodjet = df_S1_goodtau.Filter("Sum( goodjets ) >= 3", "Events with at least a goodjet")                                  
                                    .Define("ngoodjets", "Sum( goodjets )")  
                                    .Define("jet_idx", goodjets_idx, {"goodjets"})
                                    .Define("jet_pt", "Jet_pt[goodjets][0]") // pt of first jet
  //auto df_S1_goodbjet = df_S1_goodjet.Filter("Sum( goodbjets_m ) >= 4", "Events with at least a medium b-jet")
                                    .Define("nbjets_m", "Sum( goodbjets_m )")
                                    .Define("nbjets_t", "Sum( goodbjets_t )")
                                    .Define("bjet_m_idx", goodjets_idx, {"goodbjets_m"})
                                    .Define("bjet_m_pt", "Jet_pt[goodbjets_m][0]")
                                    .Define("bjet_t_idx", goodjets_idx, {"goodbjets_t"})
                                    .Define("bjet_t_pt", "Jet_pt[goodbjets_t][0]")
//  //auto df_S1_goodcjet = df_S1_goodbjet.Filter("Sum( goodcjets_m ) >= 1", "Events with at least a medium c-jet")
                                    .Define("ncjets_m", "Sum( goodcjets_m )")
                                    .Define("cjet_m_idx", goodjets_idx, {"goodcjets_m"})
                                    .Define("cjet_m_pt", "Jet_pt[goodcjets_m][0]")
                                    .Define("ncjets_t", "Sum( goodcjets_t )")
                                    .Define("cjet_t_idx", goodjets_idx, {"goodcjets_t"})
                                    .Define("cjet_t_pt", "Jet_pt[goodcjets_t][0]")
                                    .Define("genTtbarId_num", "genTtbarId%100")
                                    .Define("category", categorize, {"genTtbarId_num"});
  auto df_S1_lf = df_S1_goodjet.Filter("category == 0", "ttbar events with only light flavour jets");
  auto df_S1_b = df_S1_goodjet.Filter("category == 1");
  auto df_S1_bb = df_S1_goodjet.Filter("category == 2");
  auto df_S1_c = df_S1_goodjet.Filter("category == 3");
  auto df_S1_cc = df_S1_goodjet.Filter("category == 4");

  //signal events selection
  auto dfs_S2_muon = dfs.Define("goodmuons","Muon_pt > 20  && abs(Muon_eta) < 2.4 && Muon_tightId && Muon_pfRelIso04_all < 0.15")
                        .Define("goodtaus","Tau_pt > 20 && abs(Tau_eta) < 2.3 && Tau_idMVAoldDM & 2");
  auto dfs_S2_goodmuon = dfs_S2_muon.Filter("Sum( goodmuons ) >=1 ","Events with at least a goood muon");
  auto dfs_S2_goodtau = dfs_S2_goodmuon.Filter("Sum ( goodtaus ) >=1", "Events with at least a good tau")
                                       .Define("ntaujets", "Sum( goodtaus )")  
                                       .Define("CvsB","Jet_btagDeepC/(Jet_btagDeepC+Jet_btagDeepB)")
                                       .Define("Jet_btagDeepLF", "1-(Jet_btagDeepC+Jet_btagDeepB)")
                                       .Define("CvsL", "Jet_btagDeepC/(Jet_btagDeepC+Jet_btagDeepLF)")
                                       .Define("goodcjets_m","Jet_pt > 20 && abs(Jet_eta) < 2.4 && CvsB >0.29 && CvsL > 0.137")
                                       .Define("goodcjets_t","Jet_pt > 20 && abs(Jet_eta) < 2.4 && CvsB >0.1 && CvsL > 0.66")
                                       .Define("goodbjets_m","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepB > 0.4184")
                                       .Define("goodbjets_t","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepB > 0.7527")
                                       .Define("goodjets", "Jet_pt > 20 && abs(Jet_eta) < 2.4");
  auto dfs_S2_goodjet = dfs_S2_goodtau.Filter("Sum( goodjets ) >= 3", "Events with at least a goodjet")                                  
                                      .Define("ngoodjets", "Sum( goodjets )")  
                                      .Define("jet_idx", goodjets_idx, {"goodjets"})
                                      .Define("jet_pt", "Jet_pt[goodjets][0]") // pt of first jet
  //auto dfs_S2_goodbjet = dfs_S2_goodjet.Filter("Sum( goodbjets_m ) >= 1", "Events with at least a medium b-jet")
                                      .Define("nbjets_m", "Sum( goodbjets_m )")
                                      .Define("nbjets_t", "Sum( goodbjets_t )")
                                      .Define("bjet_m_idx", goodjets_idx, {"goodbjets_m"})
                                      .Define("bjet_m_pt", "Jet_pt[goodbjets_m][0]")
                                      .Define("bjet_t_idx", goodjets_idx, {"goodbjets_t"})
                                      .Define("bjet_t_pt", "Jet_pt[goodbjets_t][0]")
  //auto dfs_S2_goodcjet = dfs_S2_goodbjet.Filter("Sum( goodcjets_m ) >= 1", "Events with at least a medium c-jet")
                                      .Define("ncjets_m", "Sum( goodcjets_m )")
                                      .Define("cjet_m_idx", goodjets_idx, {"goodcjets_m"})
                                      .Define("cjet_m_pt", "Jet_pt[goodcjets_m][0]")
                                      .Define("ncjets_t", "Sum( goodcjets_t )")
                                      .Define("cjet_t_idx", goodjets_idx, {"goodcjets_t"})
                                      .Define("cjet_t_pt", "Jet_pt[goodcjets_t][0]");
  
  //histograms 
  auto h_lf_nbjets_m = df_S1_lf.Histo1D({"h_lf_nbjets_m", "h_lf_nbjets_m", 8, 0, 8}, "nbjets_m");
  auto h_lf_nbjets_t = df_S1_lf.Histo1D({"h_lf_nbjets_t", "h_lf_nbjets_t", 6, 0, 6}, "nbjets_t");
  auto h_lf_ncjets_m = df_S1_lf.Histo1D({"h_lf_ncjets_m", "h_lf_ncjets_m", 10, 0, 10}, "ncjets_m");
  auto h_lf_ncjets_t = df_S1_lf.Histo1D({"h_lf_ncjets_t", "h_lf_ncjets_t", 6, 0, 6}, "ncjets_t");
  auto h_lf_ntaujets = df_S1_lf.Histo1D({"h_lf_ntaujets", "h_lf_ntaujets", 5, 0, 5}, "ntaujets");
  auto h_lf_jet_pt = df_S1_lf.Histo1D({"h_lf_jet_pt", "h_lf_jet_pt", 25, 0, 250}, "jet_pt");
  auto h_lf_bjet_m_pt = df_S1_lf.Histo1D({"h_lf_bjet_m_pt", "h_lf_bjet_m_pt", 20, 0, 200}, "bjet_m_pt");
  auto h_lf_bjet_t_pt = df_S1_lf.Histo1D({"h_lf_bjet_t_pt", "h_lf_bjet_t_pt", 20, 0, 200}, "bjet_t_pt");
  auto h_lf_cjet_m_pt = df_S1_lf.Histo1D({"h_lf_cjet_m_pt", "h_lf_cjet_m_pt", 20, 0, 200}, "cjet_m_pt");
  auto h_lf_cjet_t_pt = df_S1_lf.Histo1D({"h_lf_cjet_t_pt", "h_lf_cjet_t_pt", 20, 0, 200}, "cjet_t_pt");

  auto h_b_nbjets_m = df_S1_b.Histo1D({"h_b_nbjets_m", "h_b_nbjets_m", 8, 0, 8}, "nbjets_m");
  auto h_b_nbjets_t = df_S1_b.Histo1D({"h_b_nbjets_t", "h_b_nbjets_t", 6, 0, 6}, "nbjets_t");
  auto h_b_ncjets_m = df_S1_b.Histo1D({"h_b_ncjets_m", "h_b_ncjets_m", 10, 0, 10}, "ncjets_m");
  auto h_b_ncjets_t = df_S1_b.Histo1D({"h_b_ncjets_t", "h_b_ncjets_t", 6, 0, 6}, "ncjets_t");
  auto h_b_ntaujets = df_S1_b.Histo1D({"h_b_ntaujets", "h_b_ntaujets", 5, 0, 5}, "ntaujets");
  auto h_b_jet_pt = df_S1_b.Histo1D({"h_b_jet_pt", "h_b_jet_pt", 25, 0, 250}, "jet_pt");
  auto h_b_bjet_m_pt = df_S1_b.Histo1D({"h_b_bjet_m_pt", "h_b_bjet_m_pt", 20, 0, 200}, "bjet_m_pt");
  auto h_b_bjet_t_pt = df_S1_b.Histo1D({"h_b_bjet_t_pt", "h_b_bjet_t_pt", 20, 0, 200}, "bjet_t_pt");
  auto h_b_cjet_m_pt = df_S1_b.Histo1D({"h_b_cjet_m_pt", "h_b_cjet_m_pt", 20, 0, 200}, "cjet_m_pt");
  auto h_b_cjet_t_pt = df_S1_b.Histo1D({"h_b_cjet_t_pt", "h_b_cjet_t_pt", 20, 0, 200}, "cjet_t_pt");

  auto h_bb_nbjets_m = df_S1_bb.Histo1D({"h_bb_nbjets_m", "h_bb_nbjets_m", 8, 0, 8}, "nbjets_m");
  auto h_bb_nbjets_t = df_S1_bb.Histo1D({"h_bb_nbjets_t", "h_bb_nbjets_t", 6, 0, 6}, "nbjets_t");
  auto h_bb_ncjets_m = df_S1_bb.Histo1D({"h_bb_ncjets_m", "h_bb_ncjets_m", 10, 0, 10}, "ncjets_m");
  auto h_bb_ncjets_t = df_S1_bb.Histo1D({"h_bb_ncjets_t", "h_bb_ncjets_t", 6, 0, 6}, "ncjets_t");
  auto h_bb_ntaujets = df_S1_bb.Histo1D({"h_bb_ntaujets", "h_bb_ntaujets", 5, 0, 5}, "ntaujets");
  auto h_bb_jet_pt = df_S1_bb.Histo1D({"h_bb_jet_pt", "h_bb_jet_pt", 25, 0, 250}, "jet_pt");
  auto h_bb_bjet_m_pt = df_S1_bb.Histo1D({"h_bb_bjet_m_pt", "h_bb_bjet_m_pt", 20, 0, 200}, "bjet_m_pt");
  auto h_bb_bjet_t_pt = df_S1_bb.Histo1D({"h_bb_bjet_t_pt", "h_bb_bjet_t_pt", 20, 0, 200}, "bjet_t_pt");
  auto h_bb_cjet_m_pt = df_S1_bb.Histo1D({"h_bb_cjet_m_pt", "h_bb_cjet_m_pt", 20, 0, 200}, "cjet_m_pt");
  auto h_bb_cjet_t_pt = df_S1_bb.Histo1D({"h_bb_cjet_t_pt", "h_bb_cjet_t_pt", 20, 0, 200}, "cjet_t_pt");

  auto h_c_nbjets_m = df_S1_c.Histo1D({"h_c_nbjets_m", "h_c_nbjets_m", 8, 0, 8}, "nbjets_m");
  auto h_c_nbjets_t = df_S1_c.Histo1D({"h_c_nbjets_t", "h_c_nbjets_t", 6, 0, 6}, "nbjets_t");
  auto h_c_ncjets_m = df_S1_c.Histo1D({"h_c_ncjets_m", "h_c_ncjets_m", 10, 0, 10}, "ncjets_m");
  auto h_c_ncjets_t = df_S1_c.Histo1D({"h_c_ncjets_t", "h_c_ncjets_t", 6, 0, 6}, "ncjets_t");
  auto h_c_ntaujets = df_S1_c.Histo1D({"h_c_ntaujets", "h_c_ntaujets", 5, 0, 5}, "ntaujets");
  auto h_c_jet_pt = df_S1_c.Histo1D({"h_c_jet_pt", "h_c_jet_pt", 25, 0, 250}, "jet_pt");
  auto h_c_bjet_m_pt = df_S1_c.Histo1D({"h_c_bjet_m_pt", "h_c_bjet_m_pt", 20, 0, 200}, "bjet_m_pt");
  auto h_c_bjet_t_pt = df_S1_c.Histo1D({"h_c_bjet_t_pt", "h_c_bjet_t_pt", 20, 0, 200}, "bjet_t_pt");
  auto h_c_cjet_m_pt = df_S1_c.Histo1D({"h_c_cjet_m_pt", "h_c_cjet_m_pt", 20, 0, 200}, "cjet_m_pt");
  auto h_c_cjet_t_pt = df_S1_c.Histo1D({"h_c_cjet_t_pt", "h_c_cjet_t_pt", 20, 0, 200}, "cjet_t_pt");

  auto h_cc_nbjets_m = df_S1_cc.Histo1D({"h_cc_nbjets_m", "h_cc_nbjets_m", 8, 0, 8}, "nbjets_m");
  auto h_cc_nbjets_t = df_S1_cc.Histo1D({"h_cc_nbjets_t", "h_cc_nbjets_t", 6, 0, 6}, "nbjets_t");
  auto h_cc_ncjets_m = df_S1_cc.Histo1D({"h_cc_ncjets_m", "h_cc_ncjets_m", 10, 0, 10}, "ncjets_m");
  auto h_cc_ncjets_t = df_S1_cc.Histo1D({"h_cc_ncjets_t", "h_cc_ncjets_t", 6, 0, 6}, "ncjets_t");
  auto h_cc_ntaujets = df_S1_cc.Histo1D({"h_cc_ntaujets", "h_cc_ntaujets", 5, 0, 5}, "ntaujets");
  auto h_cc_jet_pt = df_S1_cc.Histo1D({"h_cc_jet_pt", "h_cc_jet_pt", 25, 0, 250}, "jet_pt");
  auto h_cc_bjet_m_pt = df_S1_cc.Histo1D({"h_cc_bjet_m_pt", "h_cc_bjet_m_pt", 20, 0, 200}, "bjet_m_pt");
  auto h_cc_bjet_t_pt = df_S1_cc.Histo1D({"h_cc_bjet_t_pt", "h_cc_bjet_t_pt", 20, 0, 200}, "bjet_t_pt");
  auto h_cc_cjet_m_pt = df_S1_cc.Histo1D({"h_cc_cjet_m_pt", "h_cc_cjet_m_pt", 20, 0, 200}, "cjet_m_pt");
  auto h_cc_cjet_t_pt = df_S1_cc.Histo1D({"h_cc_cjet_t_pt", "h_cc_cjet_t_pt", 20, 0, 200}, "cjet_t_pt");

  auto h_sig_nbjets_m = dfs_S2_goodjet.Histo1D({"h_sig_nbjets_m", "h_sig_nbjets_m", 8, 0, 8}, "nbjets_m");
  auto h_sig_nbjets_t = dfs_S2_goodjet.Histo1D({"h_sig_nbjets_t", "h_sig_nbjets_t", 6, 0, 6}, "nbjets_t");
  auto h_sig_ncjets_m = dfs_S2_goodjet.Histo1D({"h_sig_ncjets_m", "h_sig_ncjets_m", 10, 0, 10}, "ncjets_m");
  auto h_sig_ncjets_t = dfs_S2_goodjet.Histo1D({"h_sig_ncjets_t", "h_sig_ncjets_t", 6, 0, 6}, "ncjets_t");
  auto h_sig_ntaujets = dfs_S2_goodjet.Histo1D({"h_sig_ntaujets", "h_sig_ntaujets", 5, 0, 5}, "ntaujets");
  auto h_sig_jet_pt = dfs_S2_goodjet.Histo1D({"h_sig_jet_pt", "h_sig_jet_pt", 25, 0, 250}, "jet_pt");
  auto h_sig_bjet_m_pt = dfs_S2_goodjet.Histo1D({"h_sig_bjet_m_pt", "h_sig_bjet_m_pt", 20, 0, 200}, "bjet_m_pt");
  auto h_sig_bjet_t_pt = dfs_S2_goodjet.Histo1D({"h_sig_bjet_t_pt", "h_sig_bjet_t_pt", 20, 0, 200}, "bjet_t_pt");
  auto h_sig_cjet_m_pt = dfs_S2_goodjet.Histo1D({"h_sig_cjet_m_pt", "h_sig_cjet_m_pt", 20, 0, 200}, "cjet_m_pt");
  auto h_sig_cjet_t_pt = dfs_S2_goodjet.Histo1D({"h_sig_cjet_t_pt", "h_sig_cjet_t_pt", 20, 0, 200}, "cjet_t_pt");

  //save as root file
  ROOT::RDF::RSnapshotOptions opts;
  opts.fMode = "UPDATE";
  df_S1_goodjet.Snapshot("Event_category", "LQ_ana.root", {"category"});
  df_S1_lf.Snapshot("ttbar_lf", "LQ_ana.root", {"nbjets_m", "nbjets_t", "ncjets_m", "ncjets_t", "ntaujets", "ngoodjets", "jet_pt", "bjet_m_pt", "bjet_t_pt", "category"}, opts);
  df_S1_b.Snapshot("ttbar_b", "LQ_ana.root", {"nbjets_m", "nbjets_t", "ncjets_m", "ncjets_t", "ntaujets", "ngoodjets", "jet_pt", "bjet_m_pt", "bjet_t_pt", "category"}, opts);
  df_S1_bb.Snapshot("ttbar_bb", "LQ_ana.root", {"nbjets_m", "nbjets_t", "ncjets_m", "ncjets_t", "ntaujets", "ngoodjets", "jet_pt", "bjet_m_pt", "bjet_t_pt", "category"}, opts);
  df_S1_c.Snapshot("ttbar_c", "LQ_ana.root", {"nbjets_m", "nbjets_t", "ncjets_m", "ncjets_t", "ntaujets", "ngoodjets", "jet_pt", "bjet_m_pt", "bjet_t_pt", "category"}, opts);
  df_S1_cc.Snapshot("ttbar_cc", "LQ_ana.root", {"nbjets_m", "nbjets_t", "ncjets_m", "ncjets_t", "ntaujets", "ngoodjets", "jet_pt", "bjet_m_pt", "bjet_t_pt", "category"}, opts);
  dfs_S2_goodjet.Snapshot("siganl", "LQ_ana.root", {"nbjets_m", "nbjets_t", "ncjets_m", "ncjets_t", "ntaujets", "ngoodjets", "jet_pt", "bjet_m_pt", "bjet_t_pt"}, opts);

  TFile f("LQ_ana.root", "UPDATE");
    
    plot( h_lf_nbjets_m, "h_lf_nbjets_m");
	plot( h_lf_nbjets_t, "h_lf_nbjets_t");
	plot( h_lf_ncjets_m, "h_lf_ncjets_m");
	plot( h_lf_ncjets_t, "h_lf_ncjets_t");
	plot( h_lf_ntaujets, "h_lf_ntaujets");
	plot( h_lf_jet_pt, "h_lf_jet_pt");
	plot( h_lf_bjet_m_pt , "h_lf_bjet_m_pt");
	plot( h_lf_bjet_t_pt , "h_lf_bjet_t_pt");
	plot( h_lf_cjet_m_pt , "h_lf_cjet_m_pt");
	plot( h_lf_cjet_t_pt , "h_lf_cjet_t_pt");
	plot( h_b_nbjets_m, "h_b_nbjets_m");
	plot( h_b_nbjets_t, "h_b_nbjets_t");
	plot( h_b_ncjets_m, "h_b_ncjets_m");
	plot( h_b_ncjets_t, "h_b_ncjets_t");
	plot( h_b_ntaujets, "h_b_ntaujets");
	plot( h_b_jet_pt, "h_b_jet_pt");
	plot( h_b_bjet_m_pt, "h_b_bjet_m_pt");
	plot( h_b_bjet_t_pt, "h_b_bjet_t_pt");
	plot( h_b_cjet_m_pt, "h_b_cjet_m_pt");
	plot( h_b_cjet_t_pt, "h_b_cjet_t_pt");
	plot( h_bb_nbjets_m, "h_bb_nbjets_m");
	plot( h_bb_nbjets_t, "h_bb_nbjets_t");
	plot( h_bb_ncjets_m, "h_bb_ncjets_m");
	plot( h_bb_ncjets_t, "h_bb_ncjets_t");
	plot( h_bb_ntaujets, "h_bb_ntaujets");
	plot( h_bb_jet_pt, "h_bb_jet_pt");
	plot( h_bb_bjet_m_pt, "h_bb_bjet_m_pt");
	plot( h_bb_bjet_t_pt, "h_bb_bjet_t_pt");
	plot( h_bb_cjet_m_pt, "h_bb_cjet_m_pt");
	plot( h_bb_cjet_t_pt, "h_bb_cjet_t_pt");
	plot( h_c_nbjets_m, "h_c_nbjets_m");
	plot( h_c_nbjets_t, "h_c_nbjets_t");
	plot( h_c_ncjets_m, "h_c_ncjets_m");
	plot( h_c_ncjets_t, "h_c_ncjets_t");
	plot( h_c_ntaujets, "h_c_ntaujets");
	plot( h_c_jet_pt, "h_c_jet_pt");
	plot( h_c_bjet_m_pt, "h_c_bjet_m_pt");
	plot( h_c_bjet_t_pt, "h_c_bjet_t_pt");
	plot( h_c_cjet_m_pt, "h_c_cjet_m_pt");
	plot( h_c_cjet_t_pt, "h_c_cjet_t_pt");
	plot( h_cc_nbjets_m, "h_cc_nbjets_m");
	plot( h_cc_nbjets_t, "h_cc_nbjets_t");
	plot( h_cc_ncjets_m, "h_cc_ncjets_m");
	plot( h_cc_ncjets_t, "h_cc_ncjets_t");
	plot( h_cc_ntaujets, "h_cc_ntaujets");
	plot( h_cc_jet_pt, "h_cc_jet_pt");
	plot( h_cc_bjet_m_pt, "h_cc_bjet_m_pt");
	plot( h_cc_bjet_t_pt, "h_cc_bjet_t_pt");
	plot( h_cc_cjet_m_pt, "h_cc_cjet_m_pt");
	plot( h_cc_cjet_t_pt, "h_cc_cjet_t_pt");
	plot( h_sig_nbjets_m, "h_sig_nbjets_m");
	plot( h_sig_nbjets_t, "h_sig_nbjets_t");
	plot( h_sig_ncjets_m, "h_sig_ncjets_m");
	plot( h_sig_ncjets_t, "h_sig_ncjets_t");
	plot( h_sig_ntaujets, "h_sig_ntaujets");
	plot( h_sig_jet_pt, "h_sig_jet_pt");
	plot( h_sig_bjet_m_pt, "h_sig_bjet_m_pt");
	plot( h_sig_bjet_t_pt, "h_sig_bjet_t_pt");
	plot( h_sig_cjet_m_pt, "h_sig_cjet_m_pt");
	plot( h_sig_cjet_t_pt, "h_sig_cjet_t_pt");

  f.Close();

  //auto report = df_S1_bjets.Report();
  //report->Print();

}
