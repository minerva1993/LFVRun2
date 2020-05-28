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

void ttana_lq(){
  ROOT::RDataFrame df("Events","/xrootd/store/mc/RunIIAutumn18NanoAODv6/TT_noSC_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/250000/1622691F-875A-1149-802C-3BA3B98B6C6C.root");


  //gen level flavour											
  auto df_S1_hadronFlav = df//.Define("goodjets","GenJet_pt > 20 && abs(GenJet_eta) < 2.4")
								 .Define("goodjets","GenJet_pt > -20")
								 .Define("genhadron_Flav","GenJet_hadronFlavour")
								 .Define("genparton_Flav","GenJet_partonFlavour")
								 .Define("hadron_Flav","Jet_hadronFlavour")
								 .Define("good_b_hadron","goodjets && GenJet_hadronFlavour == 5")
  				       .Define("good_c_hadron","goodjets  && GenJet_hadronFlavour == 4")
  				       .Define("good_lf_hadron","goodjets && GenJet_hadronFlavour == 0");
//								 .Define("matching","GenJet_hadronFlavour != Jet_hadronFlavour");

  //gen level origin of b,c jets
	auto df_S1_TtbarId = df_S1_hadronFlav.Define("n_cjet_fromW","genTtbarId/10000")
																			 .Define("n_bjet_fromW","(genTtbarId%10000)/1000")
																			 .Define("n_bjet_fromt","(genTtbarId%1000)/100")
																			 .Define("events_addi_c","(genTtbarId%100)==41||(genTtbarId%100)==42")
																			 .Define("events_addi_cc","(genTtbarId%100)==43||(genTtbarId%100)==44||(genTtbarId%100)==45");

  auto df_S1_good_c_hadron_nfW = df_S1_hadronFlav.Filter("genTtbarId/10000==0","Events with no c-jet from W")
														.Filter("Sum(good_c_hadron)>=1", "Events with at least a good b-hadron");
	auto df_S1_good_c_hadron_ifW = df_S1_hadronFlav.Filter("genTtbarId/10000!=0","Events including c-jet from W")
		                        .Filter("Sum(good_c_hadron)>=1", "Events with at least a good b-hadron");

  auto df_S1_ttb = df.Filter("(genTtbarId/10)%10 == 5","ttb+bb event");



  //histogram
	auto h_genhadronFlav = df_S1_hadronFlav.Histo1D({"h_genhadronFlav", "h_genhadronFlav",10, 0,10}, "genhadron_Flav");
	auto h_genpartonFlav = df_S1_hadronFlav.Histo1D({"h_genpartonFlav", "h_genpartonFlav",50, -25,25}, "genparton_Flav");
	auto h_hadronFlav = df_S1_hadronFlav.Histo1D({"h_hadronFlav", "h_hadronFlav",10, 0,10}, "hadron_Flav");
	
  auto n_good_b_hadron = df_S1_hadronFlav.Define("n_good_b_hadron","Sum(good_b_hadron)");
  auto h_n_good_b_hadron = n_good_b_hadron.Histo1D({"h_n_good_b_hadron","h_n_good_b_hadron", 10, 0, 10}, "n_good_b_hadron");
  auto n_good_c_hadron = df_S1_hadronFlav.Define("n_good_c_hadron","Sum(good_c_hadron)");
  auto h_n_good_c_hadron = n_good_c_hadron.Histo1D({"h_n_good_c_hadron","h_n_good_c_hadron", 10, 0, 10}, "n_good_c_hadron");
  auto n_good_lf_hadron = df_S1_hadronFlav.Define("n_good_lf_hadron","Sum(good_lf_hadron)");
  auto h_n_good_lf_hadron = n_good_lf_hadron.Histo1D({"h_n_good_lf_hadron","h_n_good_lf_hadron", 10, 0, 10}, "n_good_lf_hadron");
  auto h_n_goodjets = df_S1_hadronFlav.Define("n_goodjets","Sum(goodjets)") .Histo1D({"h_n_goodjets","h_n_goodjets", 15, 0, 15}, "n_goodjets");
//	auto h_n_matching = df_S1_hadronFlav.Define("n_matching","Sum(matching)") .Histo1D({"h_n_matching","h_n_matching", 10, 0,10},"n_matching");

	auto h_n_cjet_fromW = df_S1_TtbarId.Histo1D({"h_n_cjet_fromW", "h_n_cjet_fromW",5, 0, 5}, "n_cjet_fromW");
	auto h_n_bjet_fromW = df_S1_TtbarId.Histo1D({"h_n_bjet_fromW", "h_n_bjet_fromW",5, 0, 5}, "n_bjet_fromW");
	auto h_n_bjet_fromt = df_S1_TtbarId.Histo1D({"h_n_bjet_fromt", "h_n_bjet_fromt",5, 0, 5}, "n_bjet_fromt");

                              
  //df_S1_goodmuon.Snapshot("tree", "f.root");
  TFile f("tt_W_f.root", "recreate");   
	plot( h_genhadronFlav, "h_genhadronFlav");
	plot( h_genpartonFlav, "h_genpartonFlav");
  plot( h_hadronFlav, "h_hadronFlav");
  plot( h_n_good_b_hadron, "h_n_good_b_hadron");
  plot( h_n_good_c_hadron, "h_n_good_c_hadron");
  plot( h_n_good_lf_hadron, "h_n_good_lf_hadron");
  plot( h_n_goodjets, "h_n_goodjets");
//  plot( h_n_matching, "h_n_matching");

  plot( h_n_cjet_fromW, "h_n_cjet_fromW");
	plot( h_n_bjet_fromW, "h_n_bjet_fromW");
	plot( h_n_bjet_fromt, "h_n_bjet_fromt");


  f.Close();


}
