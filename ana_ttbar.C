//DeepCSV 2018 b-jet: medium, c-jet: medium

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

//good jets index
vector<int> goodjets_idx(rvec_i g){ 
  vector<int> out;
  for(int i = 0; i < g.size(); i++){
    if( g[i] ) out.push_back( i );
  }
  return out; 
}

//good jets index
vector<float> rvec_vec(rvec_f g){ 
  vector<float> out;
  for(int i = 0; i < g.size(); i++){
    if( g[i] ) out.push_back( g[i] );
  }
  return out; 
}

//categorize events (0:lf, 1:b, 2:bb, 3:c, 4:cc)
int categorize(int g){
  int out;
  if( g == 0 ) out = 0;
  else if( g == 51 || g == 52 ) out = 1;
  else if( g == 53 || g == 54 || g == 55 ) out = 2;
  else if( g == 41 || g == 42 ) out = 3;
  else if( g == 43 || g == 44 || g == 45 ) out = 4;
  return out;
}


void ana_ttbar(){

  ROOT::RDataFrame df("Events", "/xrootd/store/mc/RunIIAutumn18NanoAODv6/TT_noSC_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/250000/1622691F-875A-1149-802C-3BA3B98B6C6C.root");
  
  //good muon selection
  auto df_S1_ttbar = df.Filter("genTtbarId > 0", "ttbar events");
  auto df_S1_muon = df_S1_ttbar.Filter("nMuon >= 1", "Events with one lepton")
                                .Define("goodmuons","Muon_pt > 20  && abs(Muon_eta) < 2.4 && Muon_tightId && Muon_pfRelIso04_all < 0.15")
                                .Define("goodtaus","Tau_pt > 20 && abs(Tau_eta) < 2.3 && Tau_idMVAoldDM & 2");
  auto df_S1_goodmuon = df_S1_muon.Filter("Sum( goodmuons ) >=1 ","Events with at least a goood muon");
  auto df_S1_goodtau = df_S1_goodmuon.Filter("Sum (goodtaus ) >=1", "Events with at least a good tau")
                                     .Define("CvsB","Jet_btagDeepC/(Jet_btagDeepC+Jet_btagDeepB)")
                                     .Define("Jet_btagDeepLF", "1-(Jet_btagDeepC+Jet_btagDeepB)")
                                     .Define("CvsL", "Jet_btagDeepC/(Jet_btagDeepC+Jet_btagDeepLF)")
                                     .Define("goodcjets_m","Jet_pt > 20 && abs(Jet_eta) < 2.4 && CvsB >0.29 && CvsL > 0.137")
                                     .Define("goodcjets_t","Jet_pt > 20 && abs(Jet_eta) < 2.4 && CvsB >0.1 && CvsL > 0.66")
                                     .Define("goodbjets_m","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepB > 0.4184")
                                     .Define("goodbjets_t","Jet_pt > 20 && abs(Jet_eta) < 2.4 && Jet_btagDeepB > 0.7527")
                                     .Define("goodbjets_idx", goodjets_idx, {"goodbjets_m"})
                                     .Define("goodcjets_idx", goodjets_idx, {"goodcjets_m"})
                                     .Define("ncjets_m", "Sum( goodcjets_m )")
                                     .Define("nbjets_m", "Sum( goodbjets_m )")
                                     .Define("goodjets", "Jet_pt > 20 && abs(Jet_eta) < 2.4")
                                     .Define("goodjets_idx", goodjets_idx, {"goodjets"})     //until here no problem
                                     .Define("ngoodjets", "Sum( goodjets )");  
  auto df_S1_goodjet = df_S1_goodtau.Filter("ngoodjets >= 1", "Events with at least a goodjet")                                  
                                    .Define("goodjet_pt_rv", "Jet_pt[goodjets]")
                                    .Define("goodjet_pt", rvec_vec, {"goodjet_pt_rv"})
                                    .Define("goodjet_eta_rv", "Jet_eta[goodjets]")
                                    .Define("goodjet_eta", rvec_vec, {"goodjet_eta_rv"})
                                    .Define("goodjet_phi_rv", "Jet_phi[goodjets]")
                                    .Define("goodjet_phi", rvec_vec, {"goodjet_phi_rv"})
                                    .Define("goodjet_mass_rv", "Jet_mass[goodjets]")
                                    .Define("goodjet_mass", rvec_vec, {"goodjet_mass_rv"})
                                    .Define("bjet_pt_rv", "Jet_pt[goodbjets_m]")
                                    .Define("bjet_pt", rvec_vec, {"bjet_pt_rv"})
                                    .Define("bjet_eta_rv", "Jet_eta[goodbjets_m]")
                                    .Define("bjet_eta", rvec_vec, {"bjet_eta_rv"})
                                    .Define("bjet_phi_rv", "Jet_phi[goodbjets_m]")
                                    .Define("bjet_phi", rvec_vec, {"bjet_phi_rv"})
                                    .Define("bjet_mass_rv", "Jet_mass[goodbjets_m]")
                                    .Define("bjet_mass", rvec_vec, {"bjet_mass_rv"})
                                    .Define("cjet_pt_rv", "Jet_pt[goodcjets_m]")
                                    .Define("cjet_pt", rvec_vec, {"cjet_pt_rv"})
                                    .Define("cjet_eta_rv", "Jet_eta[goodcjets_m]")
                                    .Define("cjet_eta", rvec_vec, {"cjet_eta_rv"})
                                    .Define("cjet_phi_rv", "Jet_phi[goodcjets_m]")
                                    .Define("cjet_phi", rvec_vec, {"cjet_phi_rv"})
                                    .Define("cjet_mass_rv", "Jet_mass[goodcjets_m]")
                                    .Define("cjet_mass", rvec_vec, {"cjet_mass_rv"})
                                    .Define("genTtbarId_num", "genTtbarId%100")
                                    .Define("category", categorize, {"genTtbarId_num"});

  df_S1_goodjet.Snapshot("dnn_input", "ana_2018_cate.root", {"ncjets_m", "nbjets_m", "ngoodjets", "goodjet_pt", "goodjet_eta", "goodjet_phi", "goodjet_mass", "bjet_pt", "bjet_eta", "bjet_phi", "bjet_mass", "cjet_pt", "cjet_eta", "cjet_phi", "cjet_mass", "genTtbarId_num", "category"});
  
}

