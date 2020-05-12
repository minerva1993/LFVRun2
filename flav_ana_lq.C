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
/*
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
*/

void flav_ana_lq(int input_year){

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
                      .Define("goodtaus","Tau_pt > 30 && abs(Tau_eta) < 2.3 &&  Tau_idMVAoldDM & 2");			
  auto df_S1_goodmuon = df_S1_muon.Filter("Sum( goodmuons ) >= 1 ","Events with at least a goood muon");
//                                  .Define("goodmuons_idx",goodmuons_idx,{"goodmuons"});

  //jet tagging
  auto df_S1_goodtau = df_S1_goodmuon.Filter("Sum( goodtaus ) >= 1 ","Events with at least a goood tau")
                                     .Define("CvsB","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
                                     .Define("Jet_btagDeepFlavLF","1-(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
                                     .Define("CvsL","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavLF)")
				     .Define("goodjets","Jet_pt > 20 && abs(Jet_eta) < 2.4");
//				     .Define("goodjets_idx",goodjets_idx,{"goodjets"});
  if (input_year == 2016){
    df_S1_goodtau = df_S1_goodtau.Define("goodbjets_l","goodjets && Jet_btagDeepFlavB > 0.0614")
                                 .Define("goodbjets_m","goodjets && Jet_btagDeepFlavB > 0.3093")                         
                                 .Define("goodbjets_t","goodjets && Jet_btagDeepFlavB > 0.7221")
  				 .Define("goodcjets_l","goodjets && CvsB > 0.40 && CvsL > 0.03" )
				 .Define("goodcjets_m","goodjets && CvsB > 0.29 && CvsL > 0.085") 
				 .Define("goodcjets_t","goodjets && CvsB > 0.05 && CvsL > 0.48"); 
  }else if (input_year == 2017){
    df_S1_goodtau = df_S1_goodtau.Define("goodbjets_l","goodjets && Jet_btagDeepFlavB > 0.0521")
                                 .Define("goodbjets_m","goodjets && Jet_btagDeepFlavB > 0.3033")
                                 .Define("goodbjets_t","goodjets && Jet_btagDeepFlavB > 0.7489")
				 .Define("goodcjets_l","goodjets && CvsB > 0.345 && CvsL > 0.04")
                                 .Define("goodcjets_m","goodjets && CvsB > 0.29 && CvsL > 0.144") 
                                 .Define("goodcjets_t","goodjets && CvsB > 0.1 && CvsL > 0.73"); 
  }else if (input_year == 2018){
    df_S1_goodtau = df_S1_goodtau.Define("goodbjets_l","goodjets && Jet_btagDeepFlavB > 0.0494")
                                 .Define("goodbjets_m","goodjets && Jet_btagDeepFlavB > 0.2770")
                                 .Define("goodbjets_t","goodjets && Jet_btagDeepFlavB > 0.7264")
				 .Define("goodcjets_l","goodjets && CvsB > 0.40 && CvsL > 0.03")
                                 .Define("goodcjets_m","goodjets && CvsB > 0.29 && CvsL > 0.085")
                                 .Define("goodcjets_t","goodjets && CvsB > 0.05 && CvsL > 0.48");
  } 
											
  auto df_S1_hadronFlav = df_S1_goodtau.Define("hadron_Flav","Jet_hadronFlavour[goodjets]")
				       .Define("good_b_hadron","goodjets && Jet_hadronFlavour == 5")
  				       .Define("good_c_hadron","goodjets  && Jet_hadronFlavour == 4")
  				       .Define("good_lf_hadron","goodjets && Jet_hadronFlavour == 0");
//			  	         .Define("good_b_hadron_idx",good_b_hadron_idx,{"good_b_hadron"})
//                                       .Define("good_c_hadron_idx",good_c_hadron_idx,{"good_c_hadron"})
//                                       .Define("good_lf_hadron_idx",good_lf_hadron_idx,{"good_lf_hadron"});
  
  //Question : signal events with no c jets?
  auto df_S1_original_hadronFlav = df_S1_goodtau.Define("original_c_hadron", "Jet_hadronFlavour == 4");
  auto n_original_c_hadron = df_S1_original_hadronFlav.Define("n_original_c_hadron","Sum(original_c_hadron)");
  auto h_n_original_c_hadron = n_original_c_hadron.Histo1D({"h_n_original_c_hadron","h_n_original_c_hadron", 10, 0, 10}, "n_original_c_hadron");


 
  auto df_S1_good_b_hadron = df_S1_hadronFlav.Filter("Sum(good_b_hadron)>=1","Events with at least a good b-hadron");
  auto df_S1_good_c_hadron = df_S1_hadronFlav.Filter("Sum(good_c_hadron)>=1","Events with at least a good c-hadron");
  auto df_S1_good_lf_hadron = df_S1_hadronFlav.Filter("Sum(good_lf_hadron)>=1","Events with at least a good lf-hadron");
  auto df_S1_goodjet = df_S1_goodtau.Filter("Sum(goodjets)>=1", "Events with at least a good jet");

  auto df_S1_Deep_hadron = df_S1_hadronFlav.Define("DeepB_b", "Jet_btagDeepFlavB[good_b_hadron]")
                                           .Define("DeepB_c", "Jet_btagDeepFlavB[good_c_hadron]")
                                           .Define("DeepB_lf", "Jet_btagDeepFlavB[good_lf_hadron]")
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

  auto df_S1_ctagging = df_S1_hadronFlav.Define("c_tagged_b_l", "good_b_hadron && goodcjets_l")
                                        .Define("c_tagged_c_l", "good_c_hadron && goodcjets_l")
                                        .Define("c_tagged_lf_l", "good_lf_hadron && goodcjets_l")
                                        .Define("c_tagged_b_m", "good_b_hadron && goodcjets_m")
                                        .Define("c_tagged_c_m", "good_c_hadron && goodcjets_m")
                                        .Define("c_tagged_lf_m", "good_lf_hadron && goodcjets_m")
                                        .Define("c_tagged_b_t", "good_b_hadron && goodcjets_t")
                                        .Define("c_tagged_c_t", "good_c_hadron && goodcjets_t")
                                        .Define("c_tagged_lf_t", "good_lf_hadron && goodcjets_t");


  //histograms 
  auto h_muon_pt = df_S1_goodmuon.Define("muon_pt","Muon_pt[goodmuons][0]").Histo1D({"h_muon_pt", "h_muon_pt", 100, 0, 100}, "muon_pt");
  auto h_n_selmuon = df_S1_goodmuon.Define("n_selmuon","Sum(goodmuons)").Histo1D({"h_n_selmuon", "h_n_selmuon", 5, 0, 5}, "n_selmuon"); 
  auto h_n_tau = df_S1_goodmuon.Histo1D({"h_n_tau", "h_n_tau", 5, 0, 5}, "nTau");
  auto h_n_seltau = df_S1_goodmuon.Define("ntaujets", "Sum(goodtaus)").Histo1D({"h_n_seltau", "h_n_seltau", 5, 0, 5}, "ntaujets");

      //discriminator over 1 problem
  auto df_S1_Deep_good = df_S1_goodtau.Define("DeepB", "Jet_btagDeepFlavB[goodjets]")
				      .Define("DeepC", "Jet_btagDeepFlavC[goodjets]") 
				      .Define("DeepBC","Jet_btagDeepFlavC[goodjets]+Jet_btagDeepFlavB[goodjets]");
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
  auto n_good_b_hadron = df_S1_hadronFlav.Define("n_good_b_hadron","Sum(good_b_hadron)");
  auto h_n_good_b_hadron = n_good_b_hadron.Histo1D({"h_n_good_b_hadron","h_n_good_b_hadron", 10, 0, 10}, "n_good_b_hadron");
  auto n_good_c_hadron = df_S1_hadronFlav.Define("n_good_c_hadron","Sum(good_c_hadron)");
  auto h_n_good_c_hadron = n_good_c_hadron.Histo1D({"h_n_good_c_hadron","h_n_good_c_hadron", 10, 0, 10}, "n_good_c_hadron");
  auto n_good_lf_hadron = df_S1_hadronFlav.Define("n_good_lf_hadron","Sum(good_lf_hadron)");
  auto h_n_good_lf_hadron = n_good_lf_hadron.Histo1D({"h_n_good_lf_hadron","h_n_good_lf_hadron", 10, 0, 10}, "n_good_lf_hadron");
  auto h_n_goodjets= df_S1_goodtau.Define("n_goodjets","Sum(goodjets)") .Histo1D({"h_n_goodjets","h_n_goodjets", 15, 0, 15}, "n_goodjets");

  auto h_leading_good_b_hadron_pt = df_S1_good_b_hadron.Define("leading_good_b_hadron_pt","Jet_pt[good_b_hadron][0]")
				  .Histo1D({"h_leading_good_b_hadron_pt","leading_good_b_hadron_pt",10, 0, 200}, "leading_good_b_hadron_pt");
  auto h_leading_good_c_hadron_pt = df_S1_good_c_hadron.Define("leading_good_c_hadron_pt","Jet_pt[good_c_hadron][0]")
                                  .Histo1D({"h_leading_good_c_hadron_pt","leading_good_c_hadron_pt",10, 0, 200}, "leading_good_c_hadron_pt");
  auto h_leading_good_lf_hadron_pt = df_S1_good_lf_hadron.Define("leading_good_lf_hadron_pt","Jet_pt[good_lf_hadron][0]")
                                  .Histo1D({"h_leading_good_lf_hadron_pt","leading_good_lf_hadron_pt",10, 0, 200}, "leading_good_lf_hadron_pt");
  auto h_leading_goodjet_pt = df_S1_goodjet.Define("leading_goodjet_pt","Jet_pt[goodjets][0]")
				  .Histo1D({"h_leading_goodjet_pt"," h_leading_goodjet_pt",10, 0, 250}, "leading_goodjet_pt");

  auto h_DeepB_b = df_S1_Deep_hadron.Histo1D({"h_DeepB_b", "h_DeepB_b", 100, 0, 1}, "DeepB_b");
  auto h_DeepB_c = df_S1_Deep_hadron.Histo1D({"h_DeepB_c", "h_DeepB_c", 100, 0, 1}, "DeepB_c");
  auto h_DeepB_lf = df_S1_Deep_hadron.Histo1D({"h_DeepB_lf", "h_DeepB_lf", 100, 0, 1}, "DeepB_lf");
  auto h_ctag_2D_b = df_S1_Deep_hadron.Histo2D({"h_ctag_2D_b", "h_ctag_2D_b", 100, 0, 1, 150, 0, 1.5},"CvsL_b","CvsB_b");
  auto h_ctag_2D_c = df_S1_Deep_hadron.Histo2D({"h_ctag_2D_c", "h_ctag_2D_c", 100, 0, 1, 150, 0, 1.5},"CvsL_c","CvsB_c");
  auto h_ctag_2D_lf = df_S1_Deep_hadron.Histo2D({"h_ctag_2D_lf", "h_ctag_2D_lf", 100, 0, 1, 150, 0, 1.5}, "CvsL_lf","CvsB_lf");

  //calculate efficiency
  auto aggregator = [](double acc, int x) { return acc+x; } ;
  auto merger = [](std::vector<double> &accumulators) {
        auto size = accumulators.size();
        for (int i = 1; i <size; ++i) {
                accumulators[0] += accumulators[i];
        }
  };
  double initValue = 0;

  auto n_btagging = df_S1_btagging.Define("n_b_tagged_b_l", "Sum(b_tagged_b_l)")
                                  .Define("n_b_tagged_c_l", "Sum(b_tagged_c_l)")
                                  .Define("n_b_tagged_lf_l", "Sum(b_tagged_lf_l)")
                                  .Define("n_b_tagged_b_m", "Sum(b_tagged_b_m)")
                                  .Define("n_b_tagged_c_m", "Sum(b_tagged_c_m)")
                                  .Define("n_b_tagged_lf_m", "Sum(b_tagged_lf_m)")
                                  .Define("n_b_tagged_b_t", "Sum(b_tagged_b_t)")
                                  .Define("n_b_tagged_c_t", "Sum(b_tagged_c_t)")
                                  .Define("n_b_tagged_lf_t", "Sum(b_tagged_lf_t)");

  auto n_ctagging = df_S1_ctagging.Define("n_c_tagged_b_l", "Sum(c_tagged_b_l)")
                                  .Define("n_c_tagged_c_l", "Sum(c_tagged_c_l)")
                                  .Define("n_c_tagged_lf_l", "Sum(c_tagged_lf_l)")
                                  .Define("n_c_tagged_b_m", "Sum(c_tagged_b_m)")
                                  .Define("n_c_tagged_c_m", "Sum(c_tagged_c_m)")
                                  .Define("n_c_tagged_lf_m", "Sum(c_tagged_lf_m)")
                                  .Define("n_c_tagged_b_t", "Sum(c_tagged_b_t)")
                                  .Define("n_c_tagged_c_t", "Sum(c_tagged_c_t)")
                                  .Define("n_c_tagged_lf_t", "Sum(c_tagged_lf_t)");
  auto total_n_good_b_hadron = n_good_b_hadron.Aggregate( aggregator, merger, "n_good_b_hadron", initValue);
  auto total_n_good_c_hadron = n_good_c_hadron.Aggregate( aggregator, merger, "n_good_c_hadron", initValue);
  auto total_n_good_lf_hadron = n_good_lf_hadron.Aggregate( aggregator, merger, "n_good_lf_hadron", initValue);

  auto total_n_b_tagged_b_l = n_btagging.Aggregate( aggregator, merger, "n_b_tagged_b_l", initValue);
  auto total_n_b_tagged_c_l = n_btagging.Aggregate( aggregator, merger, "n_b_tagged_c_l", initValue);
  auto total_n_b_tagged_lf_l = n_btagging.Aggregate( aggregator, merger, "n_b_tagged_lf_l", initValue);
  auto total_n_b_tagged_b_m = n_btagging.Aggregate( aggregator, merger, "n_b_tagged_b_m", initValue);
  auto total_n_b_tagged_c_m = n_btagging.Aggregate( aggregator, merger, "n_b_tagged_c_m", initValue);
  auto total_n_b_tagged_lf_m = n_btagging.Aggregate( aggregator, merger, "n_b_tagged_lf_m", initValue);
  auto total_n_b_tagged_b_t = n_btagging.Aggregate( aggregator, merger, "n_b_tagged_b_t", initValue);
  auto total_n_b_tagged_c_t = n_btagging.Aggregate( aggregator, merger, "n_b_tagged_c_t", initValue);
  auto total_n_b_tagged_lf_t = n_btagging.Aggregate( aggregator, merger, "n_b_tagged_lf_t", initValue);

  auto total_n_c_tagged_b_l = n_ctagging.Aggregate( aggregator, merger, "n_c_tagged_b_l", initValue);
  auto total_n_c_tagged_c_l = n_ctagging.Aggregate( aggregator, merger, "n_c_tagged_c_l", initValue);
  auto total_n_c_tagged_lf_l = n_ctagging.Aggregate( aggregator, merger, "n_c_tagged_lf_l", initValue);
  auto total_n_c_tagged_b_m = n_ctagging.Aggregate( aggregator, merger, "n_c_tagged_b_m", initValue);
  auto total_n_c_tagged_c_m = n_ctagging.Aggregate( aggregator, merger, "n_c_tagged_c_m", initValue);
  auto total_n_c_tagged_lf_m = n_ctagging.Aggregate( aggregator, merger, "n_c_tagged_lf_m", initValue);
  auto total_n_c_tagged_b_t = n_ctagging.Aggregate( aggregator, merger, "n_c_tagged_b_t", initValue);
  auto total_n_c_tagged_c_t = n_ctagging.Aggregate( aggregator, merger, "n_c_tagged_c_t", initValue);
  auto total_n_c_tagged_lf_t = n_ctagging.Aggregate( aggregator, merger, "n_c_tagged_lf_t", initValue);

  std::cout << "total_n_b_tagged_b_l : " << *total_n_b_tagged_b_l
        << "    --  eff = " << round(*total_n_b_tagged_b_l / *total_n_good_b_hadron * 10000)/100<< "%" << std::endl;
  std::cout << "total_n_b_tagged_c_l : " << *total_n_b_tagged_c_l
        << "    --  eff = " << round(*total_n_b_tagged_c_l / *total_n_good_c_hadron * 10000)/100 << "%" << std::endl;
  std::cout << "total_n_b_tagged_lf_l : " << *total_n_b_tagged_lf_l
        << "    --  eff = " << round(*total_n_b_tagged_lf_l / *total_n_good_lf_hadron * 10000)/100 << "%"<< std::endl;
  std::cout << "total_n_b_tagged_b_m : " << *total_n_b_tagged_b_m
        << "    --  eff = " << round(*total_n_b_tagged_b_m / *total_n_good_b_hadron * 10000)/100 << "%"<< std::endl;
  std::cout << "total_n_b_tagged_c_m : " << *total_n_b_tagged_c_m
        << "    --  eff = " << round(*total_n_b_tagged_c_m / *total_n_good_c_hadron * 10000)/100 << "%"<< std::endl;
  std::cout << "total_n_b_tagged_lf_m : " << *total_n_b_tagged_lf_m
        << "    --  eff = " << round(*total_n_b_tagged_lf_m / *total_n_good_lf_hadron * 10000)/100 << "%"<< std::endl;
  std::cout << "total_n_b_tagged_b_t : " << *total_n_b_tagged_b_t
        << "    --  eff = " << round(*total_n_b_tagged_b_t / *total_n_good_b_hadron * 10000)/100 << "%"<< std::endl;
  std::cout << "total_n_b_tagged_c_t : " << *total_n_b_tagged_c_t
        << "    --  eff = " << round(*total_n_b_tagged_c_t / *total_n_good_c_hadron * 10000)/100 << "%"<< std::endl;
  std::cout << "total_n_b_tagged_lf_t : " << *total_n_b_tagged_lf_t
        << "    --  eff = " << round(*total_n_b_tagged_lf_t / *total_n_good_lf_hadron * 10000)/100 << "%"<< std::endl;

  std::cout << "total_n_c_tagged_b_l : " << *total_n_c_tagged_b_l
        << "    --  eff = " << round(*total_n_c_tagged_b_l / *total_n_good_b_hadron * 10000)/100 << "%" << std::endl;
  std::cout << "total_n_c_tagged_c_l : " << *total_n_c_tagged_c_l
        << "    --  eff = " << round(*total_n_c_tagged_c_l / *total_n_good_c_hadron * 10000)/100 << "%" << std::endl;
  std::cout << "total_n_c_tagged_lf_l : " << *total_n_c_tagged_lf_l
        << "    --  eff = " << round(*total_n_c_tagged_lf_l / *total_n_good_lf_hadron * 10000)/100 << "%"<< std::endl;
  std::cout << "total_n_c_tagged_b_m : " << *total_n_c_tagged_b_m
        << "    --  eff = " << round(*total_n_c_tagged_b_m / *total_n_good_b_hadron * 10000)/100 << "%"<< std::endl;
  std::cout << "total_n_c_tagged_c_m : " << *total_n_c_tagged_c_m
        << "    --  eff = " << round(*total_n_c_tagged_c_m / *total_n_good_c_hadron * 10000)/100 << "%"<< std::endl;
  std::cout << "total_n_c_tagged_lf_m : " << *total_n_c_tagged_lf_m
        << "    --  eff = " << round(*total_n_c_tagged_lf_m / *total_n_good_lf_hadron * 10000)/100 << "%"<< std::endl;
  std::cout << "total_n_c_tagged_b_t : " << *total_n_c_tagged_b_t
        << "    --  eff = " << round(*total_n_c_tagged_b_t / *total_n_good_b_hadron * 10000)/100 << "%"<< std::endl;
  std::cout << "total_n_c_tagged_c_t : " << *total_n_c_tagged_c_t
        << "    --  eff = " << round(*total_n_c_tagged_c_t / *total_n_good_c_hadron * 10000)/100 << "%"<< std::endl;
  std::cout << "total_n_c_tagged_lf_t : " << *total_n_c_tagged_lf_t
        << "    --  eff = " << round(*total_n_c_tagged_lf_t / *total_n_good_lf_hadron * 10000)/100 << "%"<< std::endl;

  auto h_n_bjets_l = df_S1_goodtau.Define("nbjets_l","Sum( goodbjets_l )").Histo1D({"h_n_bjets_l", "h_n_bjets_l", 10, 0, 10}, "nbjets_l");
  auto h_n_bjets_m = df_S1_goodtau.Define("nbjets_m","Sum( goodbjets_m )").Histo1D({"h_n_bjets_m", "h_n_bjets_m", 10, 0, 10}, "nbjets_m");
  auto h_n_bjets_t = df_S1_goodtau.Define("nbjets_t","Sum( goodbjets_t )").Histo1D({"h_n_bjets_t", "h_n_bjets_t", 10, 0, 10}, "nbjets_t");
  auto h_n_cjets_l = df_S1_goodtau.Define("ncjets_l","Sum( goodcjets_l )").Histo1D({"h_n_cjets_l", "h_n_cjets_l", 10, 0, 10}, "ncjets_l");
  auto h_n_cjets_m = df_S1_goodtau.Define("ncjets_m","Sum( goodcjets_m )").Histo1D({"h_n_cjets_m", "h_n_cjets_m", 10, 0, 10}, "ncjets_m");
  auto h_n_cjets_t = df_S1_goodtau.Define("ncjets_t","Sum( goodcjets_t )").Histo1D({"h_n_cjets_t", "h_n_cjets_t", 10, 0, 10}, "ncjets_t");

                              
  //df_S1_goodmuon.Snapshot("tree", "f.root");
  string year_str = to_string(input_year);
  TString outfile = "f_DeepFlav_"+year_str+".root";
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
  plot( h_n_good_b_hadron, "h_n_good_b_hadron");
  plot( h_n_good_c_hadron, "h_n_good_c_hadron");
  plot( h_n_good_lf_hadron, "h_n_good_lf_hadron");
  plot( h_n_goodjets, "h_n_goodjets");
  plot( h_leading_good_b_hadron_pt, "h_leading_good_b_hadron_pt");
  plot( h_leading_good_c_hadron_pt, "h_leading_good_c_hadron_pt");
  plot( h_leading_good_lf_hadron_pt, "h_leading_good_lf_hadron_pt");
  plot( h_leading_goodjet_pt, "h_leading_goodjet_pt");
  plot( h_DeepB_b, "h_DeepB_b");
  plot( h_DeepB_c, "h_DeepB_c");
  plot( h_DeepB_lf, "h_DeepB_lf"); 
  plot( h_ctag_2D_b, "h_ctag_2D_b");
  plot( h_ctag_2D_c, "h_ctag_2D_c");
  plot( h_ctag_2D_lf, "h_ctag_2D_lf");
 
  plot( h_n_bjets_l, "h_n_bjets_l");
  plot( h_n_bjets_m, "h_n_bjets_m");
  plot( h_n_bjets_t, "h_n_bjets_t");
  plot( h_n_cjets_l, "h_n_cjets_l");
  plot( h_n_cjets_m, "h_n_cjets_m");
  plot( h_n_cjets_t, "h_n_cjets_t");

  plot( h_n_original_c_hadron, "h_n_original_c_hadron");

  f.Close();

  auto report = df_S1_bjets.Report();
  report->Print();

}
