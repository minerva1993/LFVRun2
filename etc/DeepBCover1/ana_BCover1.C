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

void ana_BCover1(){ 
//  ROOT::RDataFrame df("Events","/xrootd/store/mc/RunIIAutumn18NanoAODv6/TT_noSC_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/250000/1622691F-875A-1149-802C-3BA3B98B6C6C.root");
  ROOT::RDataFrame df("Events","/xrootd/store/data/Run2018D/SingleMuon/NANOAOD/Nano25Oct2019-v1/100000/034659D3-DA7F-9C43-B604-FADBF92587EC.root"); 

  //jet tagging
  auto df_S1_goodtau = df.Define("CvsB","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
                         .Define("Jet_btagDeepFlavLF","1-(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
                         .Define("CvsL","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavLF)");

	float max1 = df_S1_goodtau.Max("CvsB").GetValue();
  cout<<"max1(CvsB) : "<<max1<<endl;

	max1 = df_S1_goodtau.Max("Jet_btagDeepFlavLF").GetValue();
  cout<<"max1(DeepLF) : "<<max1<<endl;

	max1 = df_S1_goodtau.Max("CvsL").GetValue();
	cout<<"max1(CvsL) : "<<max1<<endl;

  auto df_S1_Deep_good = df_S1_goodtau.Define("DeepB", "Jet_btagDeepFlavB")
				      .Define("DeepC", "Jet_btagDeepFlavC") 
				      .Define("DeepBC","Jet_btagDeepFlavC+Jet_btagDeepFlavB");

	max1 = df_S1_Deep_good.Max("DeepBC").GetValue();
	cout<<"max1(DeepB+DeepC) : "<<max1<<endl;

	//histograms
	auto h_CvsB_0 = df_S1_goodtau.Histo1D({"h_CvsB_0", "h_CvsB_0", 20, -0.1, 0.1},"CvsB");
	auto h_CvsB_1 = df_S1_goodtau.Histo1D({"h_CvsB_1", "h_CvsB_1", 20, 0.9, 1.1},"CvsB");
	auto h_CvsL_0 = df_S1_goodtau.Histo1D({"h_CvsL_0", "h_CvsL_0", 20, -0.1, 0.1},"CvsL");
  auto h_CvsL_1 = df_S1_goodtau.Histo1D({"h_CvsL_1", "h_CvsL_1", 20, 0.9, 1.1},"CvsL");
  
  auto h_DeepB = df_S1_Deep_good.Histo1D({"h_DeepB","h_DeepB",20, 0.9, 1.1},"DeepB");
  auto h_DeepC = df_S1_Deep_good.Histo1D({"h_DeepC","h_DeepC",20, 0.9, 1.1},"DeepC");
  auto h_DeepBC = df_S1_Deep_good.Histo1D({"h_DeepBC","h_DeepBC", 20, 0.9, 1.1},"DeepBC");

  auto h_nJets_DeepB_over1 = df_S1_Deep_good.Define("DeepB_over1","DeepB>1")
					    .Define("nJets_DeepB_over1", "Sum(DeepB_over1)")
  					    .Histo1D({"h_nJets_DeepB_over1", "h_nJets_DeepB_over1", 10, 0, 10}, "nJets_DeepB_over1");
  auto h_nJets_DeepC_over1 = df_S1_Deep_good.Define("DeepC_over1", "DeepC>1")
                                            .Define("nJets_DeepC_over1", "Sum(DeepC_over1)")
                                            .Histo1D({"h_nJets_DeepC_over1", "h_nJets_DeepC_over1", 10, 0, 10}, "nJets_DeepC_over1");
  auto h_nJets_DeepBC_over1 = df_S1_Deep_good.Define("DeepBC_over1","DeepBC>1")
                                           .Define("nJets_DeepBC_over1", "Sum(DeepBC_over1)")
  					   .Histo1D({"h_nJets_DeepBC_over1", "h_nJets_DeepBC_over1", 10, 0, 10}, "nJets_DeepBC_over1");
                              
  //df_S1_goodmuon.Snapshot("tree", "f.root");
//  TString fname = "tt_f_over1_18.root";
  TString fname = "singlemuon_f_over1_18.root";
  
  TFile f(fname, "recreate");   

	plot( h_CvsB_0, "h_CvsB_0");
	plot( h_CvsB_1, "h_CvsB_1");
	plot( h_CvsL_0, "h_CvsL_0");
	plot( h_CvsL_1, "h_CvsL_1");

  plot( h_DeepB, "h_DeepB");
  plot( h_DeepC, "h_DeepC");
  plot( h_DeepBC, "h_DeepBC");  
  plot( h_nJets_DeepB_over1,"h_nJets_DeepB_over1");
  plot( h_nJets_DeepC_over1,"h_nJets_DeepC_over1");
  plot( h_nJets_DeepBC_over1,"h_nJets_DeepBC_over1");


  f.Close();


}
