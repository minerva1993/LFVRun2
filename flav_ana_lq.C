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

using namespace ROOT::VecOps;
using rvec_f = RVec<float>;
using rvec_b = RVec<bool>;
using rvec_i = RVec<int>;

template <typename T>
void plot(T hist, TString name){
  TCanvas * c = new TCanvas("c",Form("c_%s", name.Data()));
  hist->Write();
  hist->DrawClone();
  c->Print(Form("%s.pdf",name.Data()));
}

vector<int> goodmuons_idx(rvec_i g){
  vector<int> out;
  for(int i = 0; i < g.size(); i++){
    if( g[i] ) out.push_back( i );
  }
  return out; 
}

void flav_ana_lq(){

  //ROOT::RDataFrame df("Events", "/xrootd/store/data/Run2018B/SingleMuon/NANOAOD/Nano14Dec2018-v1/10000/BCC1B466-EF27-9D40-A0B7-9FE64F456E13.root");
  //ROOT::RDataFrame df("Events", "signal_nano.root");
  // ROOT::RDataFrame df("Events", "/xrootd/store/user/ljw1015/LQ_Signals/LQ_2017_nano_v1.root");
  //ROOT::RDataFrame df("Events","/xrootd/store/mc/RunIISummer16NanoAODv6/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/NANOAODSIM/PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v2/30000/A7753E9B-A80C-9547-8729-17F953C5B5DE.root");
  //ROOT::RDataFrame df("Events", "/xrootd/store/user/ljw1015/LQ_Signals/LQ_2016_nano.root");
  

  ROOT::RDataFrame df("Events", "/cms/ldap_home/ljw1015/public/LQ_Signals/LQ_2016_nano.root");
  //ROOT::RDataFrame df("Events", "/cms/ldap_home/ljw1015/public/LQ_Signals/LQ_2017_nano.root");
  //ROOT::RDataFrame df("Events", "/cms/ldap_home/ljw1015/public/LQ_Signals/LQ_2018_nano.root");

  //good muon selection
  auto df_S1_muon = df.Filter("nMuon >= 1", "Events with one lepton")
                      .Define("goodmuons","Muon_pt > 20  && abs(Muon_eta) < 2.4 && Muon_tightId && Muon_pfRelIso04_all < 0.15")
                      .Define("goodtaus","Tau_pt > 30 && abs(Tau_eta) < 2.4 &&  Tau_idMVAoldDM & 2");
  auto df_S1_goodmuon = df_S1_muon.Filter("Sum( goodmuons ) >=1 ","Events with at least a goood muon")
                                  .Define("goodmuons_idx",goodmuons_idx,{"goodmuons"});
  auto df_S1_goodtau = df_S1_goodmuon.Filter("Sum( goodtaus ) >=1 ","Events with at least a goood tau")
                                     .Define("CvsB","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
				     .Define("Jet_btagDeepFlavLF","1-(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
                                     .Define("CvsL","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavLF)")
                                     .Define("goodcjets_l","Jet_pt > 20 && abs(Jet_eta) < 2.4 && CvsB > 0.4 && CvsL > 0.03") //(0.4,0.03)/same
				     .Define("goodcjets_m","Jet_pt > 20 && abs(Jet_eta) < 2.4 && CvsB > 0.29 && CvsL > 0.085") //(0.29,0.085)
				     .Define("goodcjets_t","Jet_pt > 20 && abs(Jet_eta) < 2.4 && CvsB > 0.05 && CvsL > 0.48 ") //(0.05,0.48)
                                     .Define("goodbjets_l","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.0614") //0.0614/0.0521/0.0494
                                     .Define("goodbjets_m","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.3093") //0.3093/0.3033//0.2770
                                     .Define("goodbjets_t","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.7221"); //0.7221/0.7489//0.7264
  auto df_S1_hadronFlav_b = df_S1_goodtau.Define("b_hadron", "Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_hadronFlavour == 5")
                                         .Filter("Sum(b_hadron)>=1","Jets with at least one b ghost hadron");
  auto df_S1_hadronFlav_c = df_S1_goodtau.Define("c_hadron", "Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_hadronFlavour == 4")
                                         .Filter("Sum(c_hadron)>=1","Jets with at least one c and no b ghost hadrons");
  auto df_S1_hadronFlav_lf = df_S1_goodtau.Define("lf_hadron", "Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_hadronFlavour == 0")
                                         .Filter("Sum(lf_hadron)>=1","Jets with no b or c hadrons");
  auto df_S1_bjets = df_S1_goodtau.Filter("Sum( goodbjets_m ) >=1","Events with at least a b jet");
  auto df_S1_ttb = df_S1_bjets.Filter("(genTtbarId/10)%10 == 5","ttb+bb event");

  //histograms 
  auto h_muon_pt = df_S1_goodmuon.Define("muon_pt","Muon_pt[goodmuons][0]").Histo1D({"h_muon_pt", "h_muon_pt", 100, 0, 100}, "muon_pt");
  auto h_n_selmuon = df_S1_goodmuon.Define("n_selmuon","Sum(goodmuons)").Histo1D({"h_n_selmuon", "h_n_selmuon", 5, 0, 5}, "n_selmuon");
  auto h_n_tau = df_S1_goodmuon.Histo1D({"h_n_tau", "h_n_tau", 5, 0, 5}, "nTau");
  auto h_n_seltau = df_S1_goodmuon.Define("ntaujets", "Sum(goodtaus)").Histo1D({"h_n_seltau", "h_n_seltau", 5, 0, 5}, "ntaujets");

  auto h_hadronFlav = df_S1_goodtau.Histo1D({"h_hadronFlav", "h_hadronFlav",10, 0,10}, "Jet_hadronFlavour");
  auto h_Jet_btagDeepFlavB_b = df_S1_hadronFlav_b.Histo1D({"h_Jet_btagDeepFlavB_b", "h_Jet_btagDeepFLavB_b", 100, 0, 1}, "Jet_btagDeepFlavB");
  auto h_Jet_btagDeepFlavB_c = df_S1_hadronFlav_c.Histo1D({"h_Jet_btagDeepFlavB_c", "h_Jet_btagDeepFlavB_c", 100, 0, 1}, "Jet_btagDeepFlavB");
  auto h_Jet_btagDeepFlavB_lf = df_S1_hadronFlav_lf.Histo1D({"h_Jet_btagDeepFlavB_lf", "h_Jet_btagDeepFlavB_lf", 100, 0, 1}, "Jet_btagDeepFlavB");
  auto h_Jet_btagDeepFlavC_b = df_S1_hadronFlav_b.Histo1D({"h_Jet_btagDeepFlavC_b", "h_Jet_btagDeepFlavC_b", 100, 0,1}, "Jet_btagDeepFlavC");
  auto h_Jet_btagDeepFlavC_c = df_S1_hadronFlav_c.Histo1D({"h_Jet_btagDeepFlavC_c", "h_Jet_btagDeepFLavC_c", 100, 0,1}, "Jet_btagDeepFlavC");
  auto h_Jet_btagDeepFlavC_lf = df_S1_hadronFlav_lf.Histo1D({"h_Jet_btagDeepFlavC_lf", "h_Jet_btagDeepFlavC_lf", 100, 0,1}, "Jet_btagDeepFlavC");
  auto h_Jet_btagDeepFlavLF_b = df_S1_hadronFlav_b.Histo1D({"h_Jet_btagDeepFlavLF_b", "h_Jet_btagDeepFlavLF_b", 100, 0,1}, "Jet_btagDeepFlavLF");
  auto h_Jet_btagDeepFlavLF_c = df_S1_hadronFlav_c.Histo1D({"h_Jet_btagDeepFlavLF_c", "h_Jet_btagDeepFlavLF_c", 100, 0,1}, "Jet_btagDeepFlavLF");
  auto h_Jet_btagDeepFlavLF_lf = df_S1_hadronFlav_lf.Histo1D({"h_Jet_btagDeepFlavLF_lf", "h_Jet_btagDeepFlavLF_lf", 100, 0,1}, "Jet_btagDeepFlavLF");
  auto h_CvsB_b = df_S1_hadronFlav_b.Histo1D({"h_CvsB_b", "h_CvsB_b", 100, 0,1}, "CvsB");
  auto h_CvsB_c = df_S1_hadronFlav_c.Histo1D({"h_CvsB_c", "h_CvsB_c", 100, 0,1}, "CvsB");
  auto h_CvsB_lf = df_S1_hadronFlav_lf.Histo1D({"h_CvsB_lf", "h_CvsB_lf", 100, 0,1}, "CvsB");
  auto h_CvsL_b = df_S1_hadronFlav_b.Histo1D({"h_CvsL_b", "h_CvsL_b", 100, 0,1}, "CvsL");
  auto h_CvsL_c = df_S1_hadronFlav_c.Histo1D({"h_CvsL_c", "h_CvsL_c", 100, 0,1}, "CvsL");
  auto h_CvsL_lf = df_S1_hadronFlav_lf.Histo1D({"h_CvsL_lf", "h_CvsL_lf", 100, 0,1}, "CvsL");

  auto h_n_cjets_l = df_S1_goodtau.Define("ncjets_l","Sum( goodcjets_l )").Histo1D({"h_n_cjets_l", "h_n_cjets_l", 10, 0, 10}, "ncjets_l");
  auto h_n_cjets_m = df_S1_goodtau.Define("ncjets_m","Sum( goodcjets_m )").Histo1D({"h_n_cjets_m", "h_n_cjets_m", 10, 0, 10}, "ncjets_m");
  auto h_n_cjets_t = df_S1_goodtau.Define("ncjets_t","Sum( goodcjets_t )").Histo1D({"h_n_cjets_t", "h_n_cjets_t", 10, 0, 10}, "ncjets_t");
  auto h_n_bjets_l = df_S1_goodtau.Define("nbjets_l","Sum( goodbjets_l )").Histo1D({"h_n_bjets_l", "h_n_bjets_l", 10, 0, 10}, "nbjets_l");
  auto h_n_bjets_m = df_S1_goodtau.Define("nbjets_m","Sum( goodbjets_m )").Histo1D({"h_n_bjets_m", "h_n_bjets_m", 10, 0, 10}, "nbjets_m");
  auto h_n_bjets_t = df_S1_goodtau.Define("nbjets_t","Sum( goodbjets_t )").Histo1D({"h_n_bjets_t", "h_n_bjets_t", 10, 0, 10}, "nbjets_t");
                              
  //df_S1_goodmuon.Snapshot("tree", "f.root");
  TFile f("f_flav.root", "recreate");

  plot( h_muon_pt, "h_muon_pt");
  plot( h_n_selmuon, "h_n_selmuon");
  plot( h_n_tau, "h_n_tau");
  plot( h_n_seltau, "h_n_seltau");

  plot( h_hadronFlav, "h_hadronFlav");
  plot( h_Jet_btagDeepFlavB_b, "h_Jet_btagDeepFlavB_b");
  plot( h_Jet_btagDeepFlavB_c, "h_Jet_btagDeepFlavB_c");
  plot( h_Jet_btagDeepFlavB_lf, "h_Jet_btagDeepFlavB_lf");
  plot( h_Jet_btagDeepFlavC_b, "h_Jet_btagDeepFlavC_b");
  plot( h_Jet_btagDeepFlavC_c, "h_Jet_btagDeepFlavC_c");
  plot( h_Jet_btagDeepFlavC_lf, "h_Jet_btagDeepFlavC_lf");
  plot( h_Jet_btagDeepFlavLF_b, "h_Jet_btagDeepFlavLF_b");
  plot( h_Jet_btagDeepFlavLF_c, "h_Jet_btagDeepFlavLF_c");
  plot( h_Jet_btagDeepFlavLF_lf, "h_Jet_btagDeepFlavLF_lf");
  plot( h_CvsB_b, "h_CvsB_b");
  plot( h_CvsB_c, "h_CvsB_c");
  plot( h_CvsB_lf, "h_CvsB_lf");
  plot( h_CvsL_b, "h_CvsL_b");
  plot( h_CvsL_c, "h_CvsL_c");
  plot( h_CvsL_lf, "h_CvsL_lf");


  plot( h_n_cjets_l, "h_n_cjets_l");
  plot( h_n_cjets_m, "h_n_cjets_m");
  plot( h_n_cjets_t, "h_n_cjets_t");
  plot( h_n_bjets_l, "h_n_bjets_l");
  plot( h_n_bjets_m, "h_n_bjets_m");
  plot( h_n_bjets_t, "h_n_bjets_t");

  f.Close();

  auto report = df_S1_bjets.Report();
  report->Print();

}
