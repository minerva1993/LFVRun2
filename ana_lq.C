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
}vector<int> goodjets_idx(rvec_i g){
  vector<int> out;
  for(int i = 0; i < g.size(); i++){
    if( g[i] ) out.push_back( i );
  }
  return out;
}vector<int> good_b_hadron_idx(rvec_i g){
  vector<int> out; 
  for(int i = 0; i < g.size(); i++){
    if( g[i] ) out.push_back( i );
  }
  return out;
}vector<int> good_c_hadron_idx(rvec_i g){
  vector<int> out; 
  for(int i = 0; i < g.size(); i++){
    if( g[i] ) out.push_back( i );
  }
  return out;
}vector<int> good_lf_hadron_idx(rvec_i g){
  vector<int> out; 
  for(int i = 0; i < g.size(); i++){
    if( g[i] ) out.push_back( i );
  }
  return out;
}


void ana_lq(int input_year){

  //ROOT::RDataFrame df("Events", "/xrootd/store/data/Run2018B/SingleMuon/NANOAOD/Nano14Dec2018-v1/10000/BCC1B466-EF27-9D40-A0B7-9FE64F456E13.root");
  //ROOT::RDataFrame df("Events", "signal_nano.root");
  // ROOT::RDataFrame df("Events", "/xrootd/store/user/ljw1015/LQ_Signals/LQ_2017_nano_v1.root");
  //ROOT::RDataFrame df("Events","/xrootd/store/mc/RunIISummer16NanoAODv6/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/NANOAODSIM/PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v2/30000/A7753E9B-A80C-9547-8729-17F953C5B5DE.root");
  //ROOT::RDataFrame df("Events", "/xrootd/store/user/ljw1015/LQ_Signals/LQ_2016_nano.root");
  

 // ROOT::RDataFrame df("Events", "/cms/ldap_home/ljw1015/public/LQ_Signals/LQ_2016_nano.root");
 //  ROOT::RDataFrame df("Events", "/cms/ldap_home/ljw1015/public/LQ_Signals/LQ_2017_nano.root");
  ROOT::RDataFrame df("Events", "/cms/ldap_home/ljw1015/public/LQ_Signals/LQ_2018_nano.root");




  //good muon selection
  auto df_S1_muon = df.Filter("nMuon >= 1", "Events with one lepton")
                      .Define("goodmuons","Muon_pt > 20  && abs(Muon_eta) < 2.4 && Muon_tightId && Muon_pfRelIso04_all < 0.15")
                      .Define("goodtaus","Tau_pt > 30 && abs(Tau_eta) < 2.4 &&  Tau_idMVAoldDM & 2");			
  auto df_S1_goodmuon = df_S1_muon.Filter("Sum( goodmuons ) >= 1 ","Events with at least a goood muon")
                                  .Define("goodmuons_idx",goodmuons_idx,{"goodmuons"});

  //jet tagging
  auto df_S1_goodtau = df_S1_goodmuon.Filter("Sum( goodtaus ) >= 1 ","Events with at least a goood tau")
                                     .Define("CvsB","Jet_btagDeepC/(Jet_btagDeepC+Jet_btagDeepB)")
                                     .Define("Jet_btagDeepLF","1-(Jet_btagDeepC+Jet_btagDeepB)")
                                     .Define("CvsL","Jet_btagDeepC/(Jet_btagDeepC+Jet_btagDeepLF)")
				     .Define("goodjets","Jet_pt > 20 && abs(Jet_eta) < 2.4")
				     .Define("goodjets_idx",goodjets_idx,{"goodjets"});
 // TString year = input_year;
  if (input_year == 2016){
    df_S1_goodtau = df_S1_goodtau.Define("goodbjets_l","goodjets && Jet_btagDeepB > 0.2217")
                                 .Define("goodbjets_m","goodjets && Jet_btagDeepB > 0.6321")                         
                                 .Define("goodbjets_t","goodjets && Jet_btagDeepB > 0.8953")
  				 .Define("goodcjets_l","goodjets && CvsB > 0.19 && CvsL > 0.05" )
				 .Define("goodcjets_m","goodjets && CvsB > 0.14 && CvsL > 0.155") 
				 .Define("goodcjets_t","goodjets && CvsB > 0.05 && CvsL > 0.59"); 
  }else if (input_year == 2017){
    df_S1_goodtau = df_S1_goodtau.Define("goodbjets_l","goodjets && Jet_btagDeepB > 0.1522")
                                 .Define("goodbjets_m","goodjets && Jet_btagDeepB > 0.4941")
                                 .Define("goodbjets_t","goodjets && Jet_btagDeepB > 0.8001")
				 .Define("goodcjets_l","goodjets && CvsB > 0.33 && CvsL > 0.05 ")
                                 .Define("goodcjets_m","goodjets && CvsB > 0.28 && CvsL > 0.15") 
                                 .Define("goodcjets_t","goodjets && CvsB > 0.1 && CvsL > 0.8"); 
  }else if (input_year == 2018){
    df_S1_goodtau = df_S1_goodtau.Define("goodbjets_l","goodjets && Jet_btagDeepB > 0.1241")
                                 .Define("goodbjets_m","goodjets && Jet_btagDeepB > 0.4184")
                                 .Define("goodbjets_t","goodjets && Jet_btagDeepB > 0.7527")
				 .Define("goodcjets_l","goodjets && CvsB > 0.35 && CvsL > 0.04 ")
                                 .Define("goodcjets_m","goodjets && CvsB > 0.29 && CvsL > 0.137")
                                 .Define("goodcjets_t","goodjets && CvsB > 0.1 && CvsL > 0.66");
  } 
											
  auto df_S1_hadronFlav = df_S1_goodtau.Define("good_b_hadron","goodjets && Jet_hadronFlavour == 5")
  				       .Define("good_c_hadron","goodjets  && Jet_hadronFlavour == 4")
  				       .Define("good_lf_hadron","goodjets && Jet_hadronFlavour == 0")
				       .Define("good_b_hadron_idx",good_b_hadron_idx,{"good_b_hadron"})
                                       .Define("good_c_hadron_idx",good_c_hadron_idx,{"good_c_hadron"})
                                       .Define("good_lf_hadron_idx",good_lf_hadron_idx,{"good_lf_hadron"});
                                      // .Define("n_good_b_hadron", "Sum(good_b_hadron)")
                                      // .Define("n_good_c_hadron", "Sum(good_c_hadron)")
                                      // .Define("n_good_lf_hadron", "Sum(good_lf_hadron)");
  auto df_S1_Deep_hadron = df_S1_hadronFlav.Define("hadron_Flav","Jet_hadronFlavour[goodjets]")
                                           .Define("DeepB_b", "Jet_btagDeepB[good_b_hadron]")
                                           .Define("DeepB_c", "Jet_btagDeepB[good_c_hadron]")
                                           .Define("DeepB_lf", "Jet_btagDeepB[good_lf_hadron]")
                                           .Define("CvsB_b", "CvsB[good_b_hadron]")
                                           .Define("CvsB_c", "CvsB[good_c_hadron]")
                                           .Define("CvsB_lf", "CvsB[good_lf_hadron]")
                                           .Define("CvsL_b", "CvsL[good_b_hadron]")
                                           .Define("CvsL_c", "CvsL[good_c_hadron]")
                                           .Define("CvsL_lf", "CvsL[good_lf_hadron]");

  auto df_S1_bjets = df_S1_goodtau.Filter("Sum( goodbjets_m ) >=1","Events with at least a b jet");
  auto df_S1_ttb = df_S1_bjets.Filter("(genTtbarId/10)%10 == 5","ttb+bb event");



  //calculate efficiency 

  auto df_S1_btagging = df_S1_hadronFlav.Define("b_tagged_b_l", "good_b_hadron && goodbjets_l")
                                        .Define("b_tagged_c_l", "good_c_hadron && goodbjets_l")
                                        .Define("b_tagged_lf_l", "good_lf_hadron && goodbjets_l")
                                        .Define("b_tagged_b_m", "good_b_hadron && goodbjets_m")
                                        .Define("b_tagged_c_m", "good_c_hadron && goodbjets_m")
                                        .Define("b_tagged_lf_m", "good_lf_hadron && goodbjets_m")
                                        .Define("b_tagged_b_t", "good_b_hadron && goodbjets_t")
                                        .Define("b_tagged_c_t", "good_c_hadron && goodbjets_t")
                                        .Define("b_tagged_lf_t", "good_lf_hadron && goodbjets_t");
  auto df_n_btagging = df_S1_btagging.Define("n_b_tagged_b_l", "Sum(b_tagged_b_l)")
			 	     .Define("n_b_tagged_c_l", "Sum(b_tagged_c_l)")
				     .Define("n_b_tagged_lf_l", "Sum(b_tagged_lf_l)")
				     .Define("n_b_tagged_b_m", "Sum(b_tagged_b_m)")
                                     .Define("n_b_tagged_c_m", "Sum(b_tagged_c_m)")
                                     .Define("n_b_tagged_lf_m", "Sum(b_tagged_lf_m)")
                                     .Define("n_b_tagged_b_t", "Sum(b_tagged_b_t)")
                                     .Define("n_b_tagged_c_t", "Sum(b_tagged_c_t)")
                                     .Define("n_b_tagged_lf_t", "Sum(b_tagged_lf_t)");

  auto df_S1_ctagging = df_S1_hadronFlav.Define("c_tagged_b_l", "good_b_hadron && goodcjets_l")
                                        .Define("c_tagged_c_l", "good_c_hadron && goodcjets_l")
                                        .Define("c_tagged_lf_l", "good_lf_hadron && goodcjets_l")
                                        .Define("c_tagged_b_m", "good_b_hadron && goodcjets_m")
                                        .Define("c_tagged_c_m", "good_c_hadron && goodcjets_m")
                                        .Define("c_tagged_lf_m", "good_lf_hadron && goodcjets_m")
                                        .Define("c_tagged_b_t", "good_b_hadron && goodcjets_t")
                                        .Define("c_tagged_c_t", "good_c_hadron && goodcjets_t")
                                        .Define("c_tagged_lf_t", "good_lf_hadron && goodcjets_t");
  auto df_n_ctagging = df_S1_ctagging.Define("n_c_tagged_b_l", "Sum(c_tagged_b_l)")
                                     .Define("n_c_tagged_c_l", "Sum(c_tagged_c_l)")
                                     .Define("n_c_tagged_lf_l", "Sum(c_tagged_lf_l)")
                                     .Define("n_c_tagged_b_m", "Sum(c_tagged_b_m)")
                                     .Define("n_c_tagged_c_m", "Sum(c_tagged_c_m)")
                                     .Define("n_c_tagged_lf_m", "Sum(c_tagged_lf_m)")
                                     .Define("n_c_tagged_b_t", "Sum(c_tagged_b_t)")
                                     .Define("n_c_tagged_c_t", "Sum(c_tagged_c_t)")
                                     .Define("n_c_tagged_lf_t", "Sum(c_tagged_lf_t)");
/* auto aggregator = [](float acc, float x){return acc + x;};
  auto merger = [](std::vector<double> & accumulators){ 
	auto size = accumulators.size();
	for (int i = 1; i <size; ++i){
	  accumulators[0] += accumulators[i];
	};
  double initValue = 0;
  auto result = df_n_btagging.Aggregate( aggregator, merger, n_b_tagged_b_l, initValue);
  cout << "Number of b_tagged b hadrons (loose b-tag) : " << *result/ <<endl;

  auto df_efficiency_btagging = df_n_btagging.Define("eff_b_btag_l","n_b_tagged_b_l/n_good_b_hadron")
					    .Define("eff_c_btag_l","n_b_tagged_c_l/n_good_c_hadron")
					    .Define("eff_lf_btag_l","n_b_tagged_lf_l/n_good_lf_hadron")
					    .Define("eff_b_btag_m","n_b_tagged_b_m/n_good_b_hadron")
                                            .Define("eff_c_btag_m","n_b_tagged_c_m/n_good_c_hadron")
                                            .Define("eff_lf_btag_m","n_b_tagged_lf_m/n_good_lf_hadron")
					    .Define("eff_b_btag_t","n_b_tagged_b_t/n_good_b_hadron")
                                            .Define("eff_c_btag_t","n_b_tagged_c_t/n_good_c_hadron")
                                            .Define("eff_lf_btag_t","n_b_tagged_lf_t/n_good_lf_hadron");
 auto df_S1_h_ctagging = df_S1_ctagging.Histo1D({"h_n_b_tagged_b_l", "h_n_b_tagged_b_l", 10, 0, 10}, "n_b_tagged_b_l")
                                        .Histo1D({"h_n_b_tagged_c_l", "h_n_b_tagged_c_l", 10, 0, 10}, "n_b_tagged_c_l") 
                                        .Histo1D({"h_n_b_tagged_lf_l", "h_n_b_tagged_lf_l", 10, 0, 10}, "n_b_tagged_lf_l") 
                                        .Histo1D({"h_n_b_tagged_b_m", "h_n_b_tagged_b_m", 10, 0, 10}, "n_b_tagged_b_m") 
                                        .Histo1D({"h_n_b_tagged_c_m", "h_n_b_tagged_c_m", 10, 0, 10}, "n_b_tagged_c_m") 
                                        .Histo1D({"h_n_b_tagged_lf_m", "h_n_b_tagged_lf_m", 10, 0, 10}, "n_b_tagged_lf_m") 
                                        .Histo1D({"h_n_b_tagged_b_t", "h_n_b_tagged_b_t", 10, 0, 10}, "n_b_tagged_b_t") 
                                        .Histo1D({"h_n_b_tagged_c_t", "h_n_b_tagged_c_t", 10, 0, 10}, "n_b_tagged_c_t")
                                        .Histo1D({"h_n_b_tagged_lf_t", "h_n_b_tagged_lf_t", 10, 0, 10}, "n_b_tagged_lf_t")
 					.Histo1D({"h_n_b_hadron","h_n_b_hadron", 10, 0,10}, "n_b_hadron")     
                                        .Histo1D({"h_n_c_hadron","h_n_c_hadron", 10, 0,10}, "n_c_hadron")   
                                        .Histo1D({"h_n_lf_hadron","h_n_lf_hadron", 10, 0,10}, "n_lf_hadron")  ; 
*/

  //histograms 
  auto h_muon_pt = df_S1_goodmuon.Define("muon_pt","Muon_pt[goodmuons][0]").Histo1D({"h_muon_pt", "h_muon_pt", 100, 0, 100}, "muon_pt");
  auto h_n_selmuon = df_S1_goodmuon.Define("n_selmuon","Sum(goodmuons)").Histo1D({"h_n_selmuon", "h_n_selmuon", 5, 0, 5}, "n_selmuon"); 
  auto h_n_tau = df_S1_goodmuon.Histo1D({"h_n_tau", "h_n_tau", 5, 0, 5}, "nTau");
  auto h_n_seltau = df_S1_goodmuon.Define("ntaujets", "Sum(goodtaus)").Histo1D({"h_n_seltau", "h_n_seltau", 5, 0, 5}, "ntaujets");

     //discriminator over 1 problem
  auto df_S1_Deep_good = df_S1_goodtau.Define("DeepB", "Jet_btagDeepB[goodjets]")
				      .Define("DeepC", "Jet_btagDeepC[goodjets]") 
				      .Define("DeepBC","Jet_btagDeepC[goodjets]+Jet_btagDeepB[goodjets]");
  auto h_DeepB = df_S1_Deep_good.Histo1D({"h_DeepB","h_DeepB",300, 0.9, 1.1},"DeepB");
  auto h_DeepC = df_S1_Deep_good.Histo1D({"h_DeepC","h_DeepC",300, 0.9, 1.1},"DeepC");
  auto h_DeepBC = df_S1_Deep_good.Histo1D({"h_DeepBC","h_DeepBC", 300, 0.9, 1.1},"DeepBC");
  auto h_nJets_DeepB_over1 = df_S1_Deep_good.Define("DeepB_over1","DeepB>1")
					    .Define("nJets_DeepB_over1", "Sum(DeepB_over1)")
  					    .Histo1D({"h_nJets_DeepB_over1", "h_nJets_DeepB_over1", 10, 0, 10}, "nJets_DeepB_over1");
  auto h_nJets_DeepC_over1 = df_S1_Deep_good.Define("DeepC_over1", "DeepC>1")
                                            .Define("nJets_DeepC_over1", "Sum(DeepC_over1)")
                                            .Histo1D({"h_nJets_DeepC_over1", "h_nJets_DeepC_over1", 10, 0, 10}, "nJets_DeepC_over1");
  auto h_nJets_DeepBC_over1 = df_S1_Deep_good.Define("DeepBC_over1","DeepBC>1")
                                           .Define("nJets_DeepBC_over1", "Sum(DeepBC_over1)")
  					   .Histo1D({"h_nJets_DeepBC_over1", "h_nJets_DeepBC_over1", 10, 0, 10}, "nJets_DeepBC_over1");


  auto h_hadronFlav = df_S1_Deep_hadron.Histo1D({"h_hadronFlav", "h_hadronFlav",10, 0,10}, "hadron_Flav");
  auto h_DeepB_b = df_S1_Deep_hadron.Histo1D({"h_DeepB_b", "h_DeepB_b", 100, 0, 1}, "DeepB_b");
  auto h_DeepB_c = df_S1_Deep_hadron.Histo1D({"h_DeepB_c", "h_DeepB_c", 100, 0, 1}, "DeepB_c");
  auto h_DeepB_lf = df_S1_Deep_hadron.Histo1D({"h_DeepB_lf", "h_DeepB_lf", 100, 0, 1}, "DeepB_lf");
  auto h_ctag_2D_b = df_S1_Deep_hadron.Histo2D({"h_ctag_2D_b", "h_ctag_2D_b", 100, 0, 1, 150, 0, 1.5},"CvsL_b","CvsB_b");
  auto h_ctag_2D_c = df_S1_Deep_hadron.Histo2D({"h_ctag_2D_c", "h_ctag_2D_c", 100, 0, 1, 150, 0, 1.5},"CvsL_c","CvsB_c");
  auto h_ctag_2D_lf = df_S1_Deep_hadron.Histo2D({"h_ctag_2D_lf", "h_ctag_2D_lf", 100, 0, 1, 150, 0, 1.5}, "CvsL_lf","CvsB_lf");

  auto h_n_b_tagged_b_l = df_n_btagging.Histo1D({"h_n_b_tagged_b_l", "h_n_b_tagged_b_l", 10, 0, 10}, "n_b_tagged_b_l");
  auto h_n_b_tagged_c_l = df_n_btagging.Histo1D({"h_n_b_tagged_c_l", "h_n_b_tagged_c_l", 10, 0, 10}, "n_b_tagged_c_l"); 
  auto h_n_b_tagged_lf_l = df_n_btagging.Histo1D({"h_n_b_tagged_lf_l", "h_n_b_tagged_lf_l", 10, 0, 10}, "n_b_tagged_lf_l"); 
  auto h_n_b_tagged_b_m = df_n_btagging.Histo1D({"h_n_b_tagged_b_m", "h_n_b_tagged_b_m", 10, 0, 10}, "n_b_tagged_b_m"); 
  auto h_n_b_tagged_c_m = df_n_btagging.Histo1D({"h_n_b_tagged_c_m", "h_n_b_tagged_c_m", 10, 0, 10}, "n_b_tagged_c_m"); 
  auto h_n_b_tagged_lf_m = df_n_btagging.Histo1D({"h_n_b_tagged_lf_m", "h_n_b_tagged_lf_m", 10, 0, 10}, "n_b_tagged_lf_m"); 
  auto h_n_b_tagged_b_t = df_n_btagging.Histo1D({"h_n_b_tagged_b_t", "h_n_b_tagged_b_t", 10, 0, 10}, "n_b_tagged_b_t"); 
  auto h_n_b_tagged_c_t = df_n_btagging.Histo1D({"h_n_b_tagged_c_t", "h_n_b_tagged_c_t", 10, 0, 10}, "n_b_tagged_c_t");
  auto h_n_b_tagged_lf_t = df_n_btagging.Histo1D({"h_n_b_tagged_lf_t", "h_n_b_tagged_lf_t", 10, 0, 10}, "n_b_tagged_lf_t");

  auto h_n_c_tagged_b_l = df_n_ctagging.Histo1D({"h_n_c_tagged_b_l", "h_n_c_tagged_b_l", 10, 0, 10}, "n_c_tagged_b_l");
  auto h_n_c_tagged_c_l = df_n_ctagging.Histo1D({"h_n_c_tagged_c_l", "h_n_c_tagged_c_l", 10, 0, 10}, "n_c_tagged_c_l");
  auto h_n_c_tagged_lf_l = df_n_ctagging.Histo1D({"h_n_c_tagged_lf_l", "h_n_c_tagged_lf_l", 10, 0, 10}, "n_c_tagged_lf_l");
  auto h_n_c_tagged_b_m = df_n_ctagging.Histo1D({"h_n_c_tagged_b_m", "h_n_c_tagged_b_m", 10, 0, 10}, "n_c_tagged_b_m");
  auto h_n_c_tagged_c_m = df_n_ctagging.Histo1D({"h_n_c_tagged_c_m", "h_n_c_tagged_c_m", 10, 0, 10}, "n_c_tagged_c_m");
  auto h_n_c_tagged_lf_m = df_n_ctagging.Histo1D({"h_n_c_tagged_lf_m", "h_n_c_tagged_lf_m", 10, 0, 10}, "n_c_tagged_lf_m");
  auto h_n_c_tagged_b_t = df_n_ctagging.Histo1D({"h_n_c_tagged_b_t", "h_n_c_tagged_b_t", 10, 0, 10}, "n_c_tagged_b_t");
  auto h_n_c_tagged_c_t = df_n_ctagging.Histo1D({"h_n_c_tagged_c_t", "h_n_c_tagged_c_t", 10, 0, 10}, "n_c_tagged_c_t");
  auto h_n_c_tagged_lf_t = df_n_ctagging.Histo1D({"h_n_c_tagged_lf_t", "h_n_c_tagged_lf_t", 10, 0, 10}, "n_c_tagged_lf_t");

  auto h_n_cjets_l = df_S1_goodtau.Define("ncjets_l","Sum( goodcjets_l )").Histo1D({"h_n_cjets_l", "h_n_cjets_l", 10, 0, 10}, "ncjets_l");
  auto h_n_cjets_m = df_S1_goodtau.Define("ncjets_m","Sum( goodcjets_m )").Histo1D({"h_n_cjets_m", "h_n_cjets_m", 10, 0, 10}, "ncjets_m");
  auto h_n_cjets_t = df_S1_goodtau.Define("ncjets_t","Sum( goodcjets_t )").Histo1D({"h_n_cjets_t", "h_n_cjets_t", 10, 0, 10}, "ncjets_t");
  auto h_n_bjets_l = df_S1_goodtau.Define("nbjets_l","Sum( goodbjets_l )").Histo1D({"h_n_bjets_l", "h_n_bjets_l", 10, 0, 10}, "nbjets_l");
  auto h_n_bjets_m = df_S1_goodtau.Define("nbjets_m","Sum( goodbjets_m )").Histo1D({"h_n_bjets_m", "h_n_bjets_m", 10, 0, 10}, "nbjets_m");
  auto h_n_bjets_t = df_S1_goodtau.Define("nbjets_t","Sum( goodbjets_t )").Histo1D({"h_n_bjets_t", "h_n_bjets_t", 10, 0, 10}, "nbjets_t");

                              
  //df_S1_goodmuon.Snapshot("tree", "f.root");
  string year_str = to_string(input_year);
  TString outfile = "f_DeepCSV_"+year_str+".root";
  TFile f(outfile, "recreate");

  plot( h_muon_pt, "h_muon_pt");
  plot( h_n_selmuon, "h_n_selmuon");
  plot( h_n_tau, "h_n_tau");
  plot( h_n_seltau, "h_n_seltau");

  plot( h_DeepB, "h_DeepB");
  plot( h_DeepC, "h_DeepC");
  plot( h_DeepBC, "h_DeepBC");  
  plot( h_nJets_DeepB_over1,"h_nJets_DeepB_over1");
  plot( h_nJets_DeepC_over1,"h_nJets_DeepC_over1");
  plot( h_nJets_DeepBC_over1,"h_nJets_DeepBC_over1");


  plot( h_hadronFlav, "h_hadronFlav");
  plot( h_DeepB_b, "h_DeepB_b");
  plot( h_DeepB_c, "h_DeepB_c");
  plot( h_DeepB_lf, "h_DeepB_lf"); 
  plot( h_ctag_2D_b, "h_ctag_2D_b");
  plot( h_ctag_2D_c, "h_ctag_2D_c");
  plot( h_ctag_2D_lf, "h_ctag_2D_lf");
 
  plot( h_n_b_tagged_b_l, "h_n_b_tagged_b_l");
  plot( h_n_b_tagged_c_l, "h_n_b_tagged_c_l");
  plot( h_n_b_tagged_lf_l, "h_n_b_tagged_lf_l");
  plot( h_n_b_tagged_b_m, "h_n_b_tagged_b_m");
  plot( h_n_b_tagged_c_m, "h_n_b_tagged_c_m");
  plot( h_n_b_tagged_lf_m, "h_n_b_tagged_lf_m");
  plot( h_n_b_tagged_b_t, "h_n_b_tagged_b_t");
  plot( h_n_b_tagged_c_t, "h_n_b_tagged_c_t");
  plot( h_n_b_tagged_lf_t, "h_n_b_tagged_lf_t");
  
  plot( h_n_c_tagged_b_l, "h_n_c_tagged_b_l");
  plot( h_n_c_tagged_c_l, "h_n_c_tagged_c_l");
  plot( h_n_c_tagged_lf_l, "h_n_c_tagged_lf_l");
  plot( h_n_c_tagged_b_m, "h_n_c_tagged_b_m");
  plot( h_n_c_tagged_c_m, "h_n_c_tagged_c_m");
  plot( h_n_c_tagged_lf_m, "h_n_c_tagged_lf_m");
  plot( h_n_c_tagged_b_t, "h_n_c_tagged_b_t");
  plot( h_n_c_tagged_c_t, "h_n_c_tagged_c_t");
  plot( h_n_c_tagged_lf_t, "h_n_c_tagged_lf_t");

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
