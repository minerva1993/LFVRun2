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
//  c->Print(Form("%s.pdf",name.Data()));
}

void ttbar_categorizer(const char *jet_tagging_method, const char * recommendation_year){

  //ROOT::RDataFrame df("Events", "/xrootd/store/data/Run2018B/SingleMuon/NANOAOD/Nano14Dec2018-v1/10000/BCC1B466-EF27-9D40-A0B7-9FE64F456E13.root");
  ROOT::RDataFrame df("Events","/xrootd/store/mc/RunIISummer16NanoAODv6/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/NANOAODSIM/PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v2/30000/A7753E9B-A80C-9547-8729-17F953C5B5DE.root");
  
  //good muon selection
  auto df_S1_ttbar = df.Filter("genTtbarId > 0", "ttbar events");
  auto df_S1_muon = df_S1_ttbar.Filter("nMuon >= 1", "Events with one lepton")
                                .Define("goodmuons","Muon_pt > 20  && abs(Muon_eta) < 2.4 && Muon_tightId && Muon_pfRelIso04_all < 0.15")
                                .Define("goodtaus","Tau_pt > 30 && abs(Tau_eta) < 2.4 && Tau_idMVAoldDM & 2");
  auto df_S1_goodmuon = df_S1_muon.Filter("Sum( goodmuons ) >=1 ","Events with at least a goood muon");
  auto df_S1_goodtau = df_S1_goodmuon;

  //jet tagging
  TString method = jet_tagging_method;
  TString year = recommendation_year;
  cout << method << endl;
  cout << method << " Jet Tagging with " << year << " Recommendation" << endl;
  if(method.Contains("CSV") == true){
    if(year.Contains("2016") == true){
      cout << "DeepCSV Jet Tagging with 2016 Recommendation" << endl;
      df_S1_goodtau = df_S1_goodmuon.Filter("Sum( goodtaus ) >=1 ","Events with at least a goood tau")
                                    .Define("CvsB","Jet_btagDeepC/(Jet_btagDeepC+Jet_btagDeepB)")
                                    .Define("goodcjets","Jet_pt > 20 && abs(Jet_eta) < 2.4 && CvsB < 0.19")
                                    .Define("goodbjets_l","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepB > 0.2217")
                                    .Define("goodbjets_m","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepB > 0.6321")
                                    .Define("goodbjets_t","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepB > 0.8953");
    }
    else if(year.Contains("2017") == true){
      cout << "DeepCSV Jet Tagging with 2017 Recommendation" << endl;
      df_S1_goodtau = df_S1_goodmuon.Filter("Sum( goodtaus ) >=1 ","Events with at least a goood tau")
                                    .Define("CvsB","Jet_btagDeepC/(Jet_btagDeepC+Jet_btagDeepB)")
                                    .Define("goodcjets","Jet_pt > 20 && abs(Jet_eta) < 2.4 && CvsB < 0.33")
                                    .Define("goodbjets_l","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepB > 0.1522")
                                    .Define("goodbjets_m","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepB > 0.4941")
                                    .Define("goodbjets_t","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepB > 0.8001");
    }
    else if(year.Contains("2018") == true){
      cout << "DeepCSV Jet Tagging with 2018 Recommendation" << endl;
      df_S1_goodtau = df_S1_goodmuon.Filter("Sum( goodtaus ) >=1 ","Events with at least a goood tau")
                                    .Define("CvsB","Jet_btagDeepC/(Jet_btagDeepC+Jet_btagDeepB)")
                                    .Define("goodcjets","Jet_pt > 20 && abs(Jet_eta) < 2.4 && CvsB < 0.35")
                                    .Define("goodbjets_l","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepB > 0.1241")
                                    .Define("goodbjets_m","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepB > 0.4184")
                                    .Define("goodbjets_t","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepB > 0.7527");
   }
  }

  else if(method.Contains("DeepJet") == true){
    if(year.Contains("2016") == true){
      cout << "DeepJet Jet Tagging with 2016 Recommendation" << endl;
      df_S1_goodtau = df_S1_goodmuon.Filter("Sum( goodtaus ) >=1 ","Events with at least a goood tau")
                                    .Define("CvsB","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
                                    .Define("goodcjets","Jet_pt > 20 && abs(Jet_eta) < 2.4 && CvsB < 0.4")
                                    .Define("goodbjets_l","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.0614")
                                    .Define("goodbjets_m","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.3093")
                                    .Define("goodbjets_t","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.7221");
    }
    else if(year.Contains("2017") == true){
      cout << "DeepJet Jet Tagging with 2017 Recommendation" << endl;
      df_S1_goodtau = df_S1_goodmuon.Filter("Sum( goodtaus ) >=1 ","Events with at least a goood tau")
                                    .Define("CvsB","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
                                    .Define("goodcjets","Jet_pt > 20 && abs(Jet_eta) < 2.4 && CvsB < 0.4")
                                    .Define("goodbjets_l","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.0521")
                                    .Define("goodbjets_m","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.3033")
                                    .Define("goodbjets_t","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.7489");
    }
    else if(year.Contains("2018") == true){
      cout << "DeepJet Jet Tagging with 2018 Recommendation" << endl;
      df_S1_goodtau = df_S1_goodmuon.Filter("Sum( goodtaus ) >=1 ","Events with at least a goood tau")
                                    .Define("CvsB","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
                                    .Define("goodcjets","Jet_pt > 20 && abs(Jet_eta) < 2.4 && CvsB < 0.35")
                                    .Define("goodbjets_l","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.0494")
                                    .Define("goodbjets_m","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.277")
                                    .Define("goodbjets_t","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.7264");
   } 
  }
  auto df_S1_ttbar_lf = df_S1_goodtau.Filter("genTtbarId%100 == 00", "ttbar events with only light flavour jets");
  auto df_S1_bjets = df_S1_goodtau.Filter("Sum( goodbjets_m ) >=1","Events with at least a b jet");
  auto df_S1_ttbar_b = df_S1_bjets.Filter("genTtbarId%100 == 51 || genTtbarId%100 == 52");
  auto df_S1_ttbar_bb = df_S1_bjets.Filter("genTtbarId%100 == 53 || genTtbarId%100 == 54 || genTtbarId%100 == 55");
  auto df_S1_cjets = df_S1_goodtau.Filter("Sum( goodcjets ) >= 1", "Events with at least a c jet");
  auto df_S1_ttbar_c = df_S1_cjets.Filter("genTtbarId%100 == 41 || genTtbarId%100 == 42");
  auto df_S1_ttbar_cc = df_S1_cjets.Filter("genTtbarId%100 == 43 || genTtbarId%100 == 44 || genTtbarId%100 == 45");

  //histograms 
  auto h_ttbar_nbjets = df_S1_goodtau.Define("nbjets", "Sum( goodbjets_m )").Histo1D({"h_ttbar_nbjets", "h_ttbar_nbjets", 8, 0, 8}, "nbjets");
  auto h_ttbar_ncjets = df_S1_goodtau.Define("ncjets", "Sum( goodcjets )").Histo1D({"h_ttbar_ncjets", "h_ttbar_ncjets", 6, 0, 6}, "ncjets");
  auto h_ttbar_ntaujets = df_S1_goodtau.Define("ntaujets", "Sum( goodtaus )").Histo1D({"h_ttbar_ntaujets", "h_ttbar_ntaujets", 5, 0, 5}, "ntaujets");
  
  auto h_ttbar_b_nbjets = df_S1_ttbar_b.Define("b_nbjets", "Sum( goodbjets_m )").Histo1D({"h_ttbar_b_nbjets", "h_ttbar_b_nbjets", 8, 0, 8}, "b_nbjets");
  auto h_ttbar_b_ncjets = df_S1_ttbar_b.Define("b_ncjets", "Sum( goodcjets )").Histo1D({"h_ttbar_b_ncjets", "h_ttbar_b_ncjets", 6, 0, 6}, "b_ncjets");
  auto h_ttbar_b_ntaujets = df_S1_ttbar_b.Define("b_ntaujets", "Sum( goodtaus )").Histo1D({"h_ttbar_b_ntaujets", "h_ttbar_b_ntaujets", 5, 0, 5}, "b_ntaujets");

  auto h_ttbar_bb_nbjets = df_S1_ttbar_bb.Define("bb_nbjets", "Sum( goodbjets_m )").Histo1D({"h_ttbar_bb_nbjets", "h_ttbar_bb_nbjets", 8, 0, 8}, "bb_nbjets");
  auto h_ttbar_bb_ncjets = df_S1_ttbar_bb.Define("bb_ncjets", "Sum( goodcjets )").Histo1D({"h_ttbar_bb_ncjets", "h_ttbar_bb_ncjets", 6, 0, 6}, "bb_ncjets");
  auto h_ttbar_bb_ntaujets = df_S1_ttbar_bb.Define("bb_ntaujets", "Sum( goodtaus )").Histo1D({"h_ttbar_bb_ntaujets", "h_ttbar_bb_ntaujets", 5, 0, 5}, "bb_ntaujets");

  auto h_ttbar_c_nbjets = df_S1_ttbar_c.Define("c_nbjets", "Sum( goodbjets_m )").Histo1D({"h_ttbar_c_nbjets", "h_ttbar_c_nbjets", 8, 0, 8}, "c_nbjets");
  auto h_ttbar_c_ncjets = df_S1_ttbar_c.Define("c_ncjets", "Sum( goodcjets )").Histo1D({"h_ttbar_c_ncjets", "h_ttbar_c_ncjets", 6, 0, 6}, "c_ncjets");
  auto h_ttbar_c_ntaujets = df_S1_ttbar_c.Define("c_ntaujets", "Sum( goodtaus )").Histo1D({"h_ttbar_c_ntaujets", "h_ttbar_c_ntaujets", 5, 0, 5}, "c_ntaujets");

  auto h_ttbar_cc_nbjets = df_S1_ttbar_cc.Define("cc_nbjets", "Sum( goodbjets_m )").Histo1D({"h_ttbar_cc_nbjets", "h_ttbar_cc_nbjets", 8, 0, 8}, "cc_nbjets");
  auto h_ttbar_cc_ncjets = df_S1_ttbar_cc.Define("cc_ncjets", "Sum( goodcjets )").Histo1D({"h_ttbar_cc_ncjets", "h_ttbar_cc_ncjets", 6, 0, 6}, "cc_ncjets");
  auto h_ttbar_cc_ntaujets = df_S1_ttbar_cc.Define("cc_ntaujets", "Sum( goodtaus )").Histo1D({"h_ttbar_cc_ntaujets", "h_ttbar_cc_ntaujets", 5, 0, 5}, "cc_ntaujets");

  auto h_ttbar_lf_nbjets = df_S1_ttbar_lf.Define("lf_nbjets", "Sum( goodbjets_m )").Histo1D({"h_ttbar_lf_nbjets", "h_ttbar_lf_nbjets", 8, 0, 8}, "lf_nbjets");
  auto h_ttbar_lf_ncjets = df_S1_ttbar_lf.Define("lf_ncjets", "Sum( goodcjets )").Histo1D({"h_ttbar_lf_ncjets", "h_ttbar_lf_ncjets", 6, 0, 6}, "lf_ncjets");
  auto h_ttbar_lf_ntaujets = df_S1_ttbar_lf.Define("lf_ntaujets", "Sum( goodtaus )").Histo1D({"h_ttbar_lf_ntaujets", "h_ttbar_lf_ntaujets", 5, 0, 5}, "lf_ntaujets");

  //df_S1_goodmuon.Snapshot("tree", "f.root");
  TString outfile = "ttbar_" + method + "_" + year + ".root";
  cout << outfile << endl;
  TFile f(outfile, "recreate");

	plot( h_ttbar_nbjets, "h_ttbar_nbjets");
	plot( h_ttbar_ncjets, "h_ttbar_ncjets");
	plot( h_ttbar_ntaujets, "h_ttbar_ntaujets");
	plot( h_ttbar_b_nbjets, "h_ttbar_b_nbjets");
	plot( h_ttbar_b_ncjets, "h_ttbar_b_ncjets");
	plot( h_ttbar_b_ntaujets, "h_ttbar_b_ntaujets");
	plot( h_ttbar_bb_nbjets, "h_ttbar_bb_nbjets");
	plot( h_ttbar_bb_ncjets, "h_ttbar_bb_ncjets");
	plot( h_ttbar_bb_ntaujets, "h_ttbar_bb_ntaujets");
	plot( h_ttbar_c_nbjets, "h_ttbar_c_nbjets");
	plot( h_ttbar_c_ncjets, "h_ttbar_c_ncjets");
	plot( h_ttbar_c_ntaujets, "h_ttbar_c_ntaujets");
	plot( h_ttbar_cc_nbjets, "h_ttbar_cc_nbjets");
	plot( h_ttbar_cc_ncjets, "h_ttbar_cc_ncjets");
	plot( h_ttbar_cc_ntaujets, "h_ttbar_cc_ntaujets");
	plot( h_ttbar_lf_nbjets, "h_ttbar_lf_nbjets");
	plot( h_ttbar_lf_ncjets, "h_ttbar_lf_ncjets");
	plot( h_ttbar_lf_ntaujets, "h_ttbar_lf_ntaujets");
  
  f.Close();

  auto report = df_S1_bjets.Report();
  report->Print();

}
