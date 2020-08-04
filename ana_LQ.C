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
using Impl_i = vector<int, ROOT::Detail::VecOps::RAdoptAllocator<int>>;

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

//sort jet by btag
RVec<size_t> sort_btag_idx(rvec_f pt, rvec_f btag, rvec_f eta, rvec_i jetid){
  auto sort_btag = Reverse(Argsort(btag));
  RVec<size_t> idx;
  for(size_t i = 0; i < pt.size(); i++){
    if( pt[sort_btag[i]] > 30 && abs(eta[sort_btag[i]]) < 2.4 && jetid[sort_btag[i]] == 6){
      idx.push_back(sort_btag[i]);
    }
  }
  return idx;
}

//sort b jet by btag
RVec<size_t> b_sort_btag_idx(rvec_f pt, rvec_f btag, rvec_f eta, rvec_i jetid){
  auto sort_btag = Reverse(Argsort(btag));
  RVec<size_t> idx;
  for(size_t i = 0; i < pt.size(); i++){
    if( pt[sort_btag[i]] > 30 && abs(eta[sort_btag[i]]) < 2.4 && jetid[sort_btag[i]] == 6 && btag[sort_btag[i]] > 0.277){
      idx.push_back(sort_btag[i]);
    }
  }
  return idx;
}
////sort jet by CvsL, CvsB
//RVec<size_t> sort_ctag_idx(rvec_f pt, rvec_f eta, rvec_i jetid, rvec_f CvsB, rvec_f CvsL){
////  cout << "number of jets : " << pt.size() << endl;
//  rvec_f ctag = CvsL + CvsB;
// // cout << CvsL << " + " << CvsB << " = " << ctag << endl;
//  auto sort_ctag = Reverse(Argsort(ctag));
//  RVec<size_t> idx;
//  for(size_t i = 0; i < pt.size(); i++){
//    if( pt[sort_ctag[i]] > 30 && abs(eta[sort_ctag[i]]) < 2.4 && jetid[sort_ctag[i]] == 6 ){
//      idx.push_back(sort_ctag[i]);
//  //    cout << CvsL[sort_ctag[i]] << " + " << CvsB[sort_ctag[i]] << " = " << ctag[sort_ctag[i]] << endl;
//    }
//  }
//  //cout << idx << endl;
//  return idx;
//}

//sort jet by ctag
RVec<size_t> sort_ctag_idx(rvec_f pt, rvec_f ctag, rvec_f eta, rvec_i jetid){
  auto sort_ctag = Reverse(Argsort(ctag));
  RVec<size_t> idx;
  for(size_t i = 0; i < pt.size(); i++){
    if( pt[sort_ctag[i]] > 30 && abs(eta[sort_ctag[i]]) < 2.4 && jetid[sort_ctag[i]] == 6 ){
      idx.push_back(sort_ctag[i]);
    }
  }
  return idx;
}

//resize vector
RVec<size_t> resize(RVec<size_t> g){
  RVec<size_t> out;
  if(g.size() < 4){
    g.resize(4, 999);
    for(size_t i = 0; i < g.size(); i++){
      out.push_back(g[i]);
    }
  }
  else{
    for(size_t i = 0; i < g.size(); i++){
      out.push_back(g[i]);
    }
  }
  return out;
}

void ana_LQ(){
  ROOT::RDataFrame df("Events", {"/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/073462AF-FDD0-AD45-970A-EB97923698F3.root",
                                 "/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/0BFDE5A9-3D64-2A4D-A24E-FFB33CCE19F5.root",
                                 "/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/211ACE36-21BD-CC43-864E-BDBD1CE5FE01.root"});
  ROOT::RDataFrame dfs("Events","/T2_KR_KISTI/store/user/jolim/LQ_2018/LQ_2018NANO_A1/200523_183528/0000/LQ_2018_nano_1.root");

  //background events selection
  auto df_S1_ttbar = df.Filter("genTtbarId > 0", "ttbar events")
                       .Define("genTtbarId_num", "genTtbarId%100")
                       .Define("category", categorize, {"genTtbarId_num"})
                       .Define("CvsB","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
                       .Define("Jet_btagDeepFlavLF", "1-(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
                       .Define("CvsL", "Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavLF)")
                       .Define("goodcjets_l","Jet_pt > 30 && abs(Jet_eta) < 2.4 && CvsB > 0.4 && CvsL > 0.03")
                       .Define("goodcjets_m","Jet_pt > 30 && abs(Jet_eta) < 2.4 && CvsB > 0.29 && CvsL > 0.085")
                       .Define("goodcjets_t","Jet_pt > 30 && abs(Jet_eta) < 2.4 && CvsB > 0.05 && CvsL > 0.48")
                       .Define("goodbjets_m","Jet_pt > 30 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.277")
                       .Define("goodbjets_t","Jet_pt > 30 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.7264")
                       .Define("goodjets", "Jet_pt > 30 && abs(Jet_eta) < 2.4 && Jet_jetId == 6");
  auto df_S1_goodjet = df_S1_ttbar.Filter("Sum( goodjets ) >= 3", "Events with at least a goodjet")
                                  .Define("ngoodjets", "Sum( goodjets )")  
                                  .Define("jet_pt", "Jet_pt[goodjets[0]]") // pt of first jet
                                  .Define("nbjets_m", "Sum( goodbjets_m )")
                                  .Define("nbjets_t", "Sum( goodbjets_t )")
                                  .Define("bjet_m_pt", "Jet_pt[goodbjets_m[0]]")
                                  .Define("bjet_t_pt", "Jet_pt[goodbjets_t[0]]")
                                  .Define("ncjets_l", "Sum( goodcjets_l )")
                                  .Define("ncjets_m", "Sum( goodcjets_m )")
                                  .Define("ncjets_t", "Sum( goodcjets_t )")
                                  .Define("cjet_l_pt", "Jet_pt[goodcjets_l[0]]")
                                  .Define("cjet_m_pt", "Jet_pt[goodcjets_m[0]]")
                                  .Define("cjet_t_pt", "Jet_pt[goodcjets_t[0]]");
  auto df_S1_bsort = df_S1_goodjet.Define("bsortjet", sort_btag_idx, {"Jet_pt", "Jet_btagDeepFlavB", "Jet_eta", "Jet_jetId"}) 
                                  .Define("bsortjet_btag1", "Jet_btagDeepFlavB[bsortjet[0]]")
                                  .Define("bsortjet_btag2", "Jet_btagDeepFlavB[bsortjet[1]]")
                                  .Define("bsortjet_btag3", "Jet_btagDeepFlavB[bsortjet[2]]")
                                  .Define("bsortjet_btag4", "Jet_btagDeepFlavB[bsortjet[3]]")
                                  .Define("bsortjet_pt1", "Jet_pt[bsortjet[0]]")
                                  .Define("bsortjet_pt2", "Jet_pt[bsortjet[1]]")
                                  .Define("bsortjet_pt3", "Jet_pt[bsortjet[2]]")
                                  .Define("bsortjet_pt4", "Jet_pt[bsortjet[3]]")
                                  .Define("bsortjet_mass1", "Jet_mass[bsortjet[0]]")
                                  .Define("bsortjet_mass2", "Jet_mass[bsortjet[1]]")
                                  .Define("bsortjet_mass3", "Jet_mass[bsortjet[2]]")
                                  .Define("bsortjet_mass4", "Jet_mass[bsortjet[3]]");
  auto df_S1_ctag = df_S1_bsort.Define("ctag_idx", sort_ctag_idx, {"Jet_pt", "Jet_btagDeepFlavC", "Jet_eta", "Jet_jetId"})
                               .Define("ctag", resize , {"ctag_idx"})
                               .Define("ctag_pt1", "Jet_pt[ctag[0]]")
                               .Define("ctag_pt2", "Jet_pt[ctag[1]]")
                               .Define("ctag_pt3", "Jet_pt[ctag[2]]")
                               .Define("ctag_pt4", "Jet_pt[ctag[3]]")
                               .Define("ctag_mass1", "Jet_mass[ctag[0]]")
                               .Define("ctag_mass2", "Jet_mass[ctag[1]]")
                               .Define("ctag_mass3", "Jet_mass[ctag[2]]")
                               .Define("ctag_mass4", "Jet_mass[ctag[3]]")
                               .Define("ctag_eta1", "Jet_eta[ctag[0]]")
                               .Define("ctag_eta2", "Jet_eta[ctag[1]]")
                               .Define("ctag_eta3", "Jet_eta[ctag[2]]")
                               .Define("ctag_eta4", "Jet_eta[ctag[3]]")
                               .Define("ctag_phi1", "Jet_phi[ctag[0]]")
                               .Define("ctag_phi2", "Jet_phi[ctag[1]]")
                               .Define("ctag_phi3", "Jet_phi[ctag[2]]")
                               .Define("ctag_phi4", "Jet_phi[ctag[3]]")
                               .Define("ctag_CvsL1", "CvsL[ctag[0]]")
                               .Define("ctag_CvsL2", "CvsL[ctag[1]]")
                               .Define("ctag_CvsL3", "CvsL[ctag[2]]")
                               .Define("ctag_CvsL4", "CvsL[ctag[3]]")
                               .Define("ctag_CvsB1", "CvsB[ctag[0]]")
                               .Define("ctag_CvsB2", "CvsB[ctag[1]]")
                               .Define("ctag_CvsB3", "CvsB[ctag[2]]")
                               .Define("ctag_CvsB4", "CvsB[ctag[3]]")
                               .Define("ctag_Ctag1", "Jet_btagDeepFlavC[ctag[0]]")
                               .Define("ctag_Ctag2", "Jet_btagDeepFlavC[ctag[1]]")
                               .Define("ctag_Ctag3", "Jet_btagDeepFlavC[ctag[2]]")
                               .Define("ctag_Ctag4", "Jet_btagDeepFlavC[ctag[3]]");
  auto df_S1_cvsl = df_S1_ctag.Define("cvsl_idx", sort_ctag_idx, {"Jet_pt", "CvsL", "Jet_eta", "Jet_jetId"})
                              .Define("cvsl", resize , {"cvsl_idx"})
                              .Define("cvsl_pt1", "Jet_pt[cvsl[0]]")
                              .Define("cvsl_pt2", "Jet_pt[cvsl[1]]")
                              .Define("cvsl_pt3", "Jet_pt[cvsl[2]]")
                              .Define("cvsl_pt4", "Jet_pt[cvsl[3]]")
                              .Define("cvsl_mass1", "Jet_mass[cvsl[0]]")
                              .Define("cvsl_mass2", "Jet_mass[cvsl[1]]")
                              .Define("cvsl_mass3", "Jet_mass[cvsl[2]]")
                              .Define("cvsl_mass4", "Jet_mass[cvsl[3]]")
                              .Define("cvsl_eta1", "Jet_eta[cvsl[0]]")
                              .Define("cvsl_eta2", "Jet_eta[cvsl[1]]")
                              .Define("cvsl_eta3", "Jet_eta[cvsl[2]]")
                              .Define("cvsl_eta4", "Jet_eta[cvsl[3]]")
                              .Define("cvsl_phi1", "Jet_phi[cvsl[0]]")
                              .Define("cvsl_phi2", "Jet_phi[cvsl[1]]")
                              .Define("cvsl_phi3", "Jet_phi[cvsl[2]]")
                              .Define("cvsl_phi4", "Jet_phi[cvsl[3]]")
                              .Define("cvsl_CvsL1", "CvsL[cvsl[0]]")
                              .Define("cvsl_CvsL2", "CvsL[cvsl[1]]")
                              .Define("cvsl_CvsL3", "CvsL[cvsl[2]]")
                              .Define("cvsl_CvsL4", "CvsL[cvsl[3]]")
                              .Define("cvsl_CvsB1", "CvsB[cvsl[0]]")
                              .Define("cvsl_CvsB2", "CvsB[cvsl[1]]")
                              .Define("cvsl_CvsB3", "CvsB[cvsl[2]]")
                              .Define("cvsl_CvsB4", "CvsB[cvsl[3]]")
                              .Define("cvsl_Ctag1", "Jet_btagDeepFlavC[cvsl[0]]")
                              .Define("cvsl_Ctag2", "Jet_btagDeepFlavC[cvsl[1]]")
                              .Define("cvsl_Ctag3", "Jet_btagDeepFlavC[cvsl[2]]")
                              .Define("cvsl_Ctag4", "Jet_btagDeepFlavC[cvsl[3]]");
  auto df_S1_cvsb = df_S1_cvsl.Define("cvsb_idx", sort_ctag_idx, {"Jet_pt", "CvsB", "Jet_eta", "Jet_jetId"})
                              .Define("cvsb", resize , {"cvsb_idx"})
                              .Define("cvsb_pt1", "Jet_pt[cvsb[0]]")
                              .Define("cvsb_pt2", "Jet_pt[cvsb[1]]")
                              .Define("cvsb_pt3", "Jet_pt[cvsb[2]]")
                              .Define("cvsb_pt4", "Jet_pt[cvsb[3]]")
                              .Define("cvsb_mass1", "Jet_mass[cvsb[0]]")
                              .Define("cvsb_mass2", "Jet_mass[cvsb[1]]")
                              .Define("cvsb_mass3", "Jet_mass[cvsb[2]]")
                              .Define("cvsb_mass4", "Jet_mass[cvsb[3]]")
                              .Define("cvsb_eta1", "Jet_eta[cvsb[0]]")
                              .Define("cvsb_eta2", "Jet_eta[cvsb[1]]")
                              .Define("cvsb_eta3", "Jet_eta[cvsb[2]]")
                              .Define("cvsb_eta4", "Jet_eta[cvsb[3]]")
                              .Define("cvsb_phi1", "Jet_phi[cvsb[0]]")
                              .Define("cvsb_phi2", "Jet_phi[cvsb[1]]")
                              .Define("cvsb_phi3", "Jet_phi[cvsb[2]]")
                              .Define("cvsb_phi4", "Jet_phi[cvsb[3]]")
                              .Define("cvsb_CvsL1", "CvsL[cvsb[0]]")
                              .Define("cvsb_CvsL2", "CvsL[cvsb[1]]")
                              .Define("cvsb_CvsL3", "CvsL[cvsb[2]]")
                              .Define("cvsb_CvsL4", "CvsL[cvsb[3]]")
                              .Define("cvsb_CvsB1", "CvsB[cvsb[0]]")
                              .Define("cvsb_CvsB2", "CvsB[cvsb[1]]")
                              .Define("cvsb_CvsB3", "CvsB[cvsb[2]]")
                              .Define("cvsb_CvsB4", "CvsB[cvsb[3]]")
                              .Define("cvsb_Ctag1", "Jet_btagDeepFlavC[cvsb[0]]")
                              .Define("cvsb_Ctag2", "Jet_btagDeepFlavC[cvsb[1]]")
                              .Define("cvsb_Ctag3", "Jet_btagDeepFlavC[cvsb[2]]")
                              .Define("cvsb_Ctag4", "Jet_btagDeepFlavC[cvsb[3]]");
//  auto df_S1_sortbjet = df_S1_goodjet.Define("sortbjet_idx", b_sort_btag_idx, {"Jet_pt", "Jet_btagDeepFlavB", "Jet_eta", "Jet_jetId"})
//                                       .Define("sortbjet", resize, {"sortbjet_idx"})
//                                       .Define("sortbjet_pt1", "Jet_pt[sortbjet[0]]")
//                                       .Define("sortbjet_pt2", "Jet_pt[sortbjet[1]]")
//                                       .Define("sortbjet_pt3", "Jet_pt[sortbjet[2]]")
//                                       .Define("sortbjet_pt4", "Jet_pt[sortbjet[3]]")
//                                       .Define("sortbjet_mass1", "Jet_mass[sortbjet[0]]")
//                                       .Define("sortbjet_mass2", "Jet_mass[sortbjet[1]]")
//                                       .Define("sortbjet_mass3", "Jet_mass[sortbjet[2]]")
//                                       .Define("sortbjet_mass4", "Jet_mass[sortbjet[3]]")
//                                       .Define("sortbjet_btag1", "Jet_btagDeepFlavB[sortbjet[0]]")
//                                       .Define("sortbjet_btag2", "Jet_btagDeepFlavB[sortbjet[1]]")
//                                       .Define("sortbjet_btag3", "Jet_btagDeepFlavB[sortbjet[2]]")
//                                       .Define("sortbjet_btag4", "Jet_btagDeepFlavB[sortbjet[3]]");
  auto df_S1_HLT = df_S1_cvsb.Filter("HLT_IsoMu24", "HLT Trigger IsoMu24")
                              .Define("goodmuons","Muon_pt > 30  && abs(Muon_eta) < 2.4 && Muon_tightId && Muon_pfRelIso03_all < 0.15")
                              .Define("goodtaus","Tau_pt > 30 && abs(Tau_eta) < 2.3 && Tau_idMVAoldDM & 2");
  auto df_S1_goodmuon = df_S1_HLT.Filter("Sum( goodmuons ) >=1 ","Events with at least a goood muon")
                                 .Define("ntaujets", "Sum( goodtaus )");
  auto df_S1_goodtau = df_S1_goodmuon.Filter("Sum ( goodtaus ) >=1", "Events with at least a good tau");
  auto df_S1_lf = df_S1_goodtau.Filter("category == 0", "ttbar events with only light flavour jets");
  auto df_S1_b = df_S1_goodtau.Filter("category == 1");
  auto df_S1_bb = df_S1_goodtau.Filter("category == 2");
  auto df_S1_c = df_S1_goodtau.Filter("category == 3");
  auto df_S1_cc = df_S1_goodtau.Filter("category == 4");

  //signal events selection
  auto dfs_S2_signal = dfs.Define("CvsB","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
                          .Define("Jet_btagDeepFlavLF", "1-(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
                          .Define("CvsL", "Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavLF)")
                          .Define("goodcjets_l","Jet_pt > 30 && abs(Jet_eta) < 2.4 && CvsB > 0.4 && CvsL > 0.03")
                          .Define("goodcjets_m","Jet_pt > 30 && abs(Jet_eta) < 2.4 && CvsB > 0.29 && CvsL > 0.085")
                          .Define("goodcjets_t","Jet_pt > 30 && abs(Jet_eta) < 2.4 && CvsB > 0.05 && CvsL > 0.48")
                          .Define("goodbjets_m","Jet_pt > 30 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.277")
                          .Define("goodbjets_t","Jet_pt > 30 && abs(Jet_eta) < 2.4 && Jet_btagDeepFlavB > 0.7264")
                          .Define("goodjets", "Jet_pt > 30 && abs(Jet_eta) < 2.4 && Jet_jetId == 6");
  auto dfs_S2_goodjet = dfs_S2_signal.Filter("Sum( goodjets ) >= 3", "Events with at least a goodjet")
                                     .Define("ngoodjets", "Sum( goodjets )")  
                                     .Define("jet_pt", "Jet_pt[goodjets[0]]") // pt of first jet
                                     .Define("nbjets_m", "Sum( goodbjets_m )")
                                     .Define("nbjets_t", "Sum( goodbjets_t )")
                                     .Define("bjet_m_pt", "Jet_pt[goodbjets_m[0]]")
                                     .Define("bjet_t_pt", "Jet_pt[goodbjets_t[0]]")
                                     .Define("ncjets_l", "Sum( goodcjets_l )")
                                     .Define("ncjets_m", "Sum( goodcjets_m )")
                                     .Define("ncjets_t", "Sum( goodcjets_t )")
                                     .Define("cjet_l_pt", "Jet_pt[goodcjets_l[0]]")
                                     .Define("cjet_m_pt", "Jet_pt[goodcjets_m[0]]")
                                     .Define("cjet_t_pt", "Jet_pt[goodcjets_t[0]]");
  auto dfs_S2_bsort = dfs_S2_goodjet.Define("bsortjet", sort_btag_idx, {"Jet_pt", "Jet_btagDeepFlavB", "Jet_eta", "Jet_jetId"}) 
                                  .Define("bsortjet_btag1", "Jet_btagDeepFlavB[bsortjet[0]]")
                                  .Define("bsortjet_btag2", "Jet_btagDeepFlavB[bsortjet[1]]")
                                  .Define("bsortjet_btag3", "Jet_btagDeepFlavB[bsortjet[2]]")
                                  .Define("bsortjet_btag4", "Jet_btagDeepFlavB[bsortjet[3]]")
                                  .Define("bsortjet_pt1", "Jet_pt[bsortjet[0]]")
                                  .Define("bsortjet_pt2", "Jet_pt[bsortjet[1]]")
                                  .Define("bsortjet_pt3", "Jet_pt[bsortjet[2]]")
                                  .Define("bsortjet_pt4", "Jet_pt[bsortjet[3]]")
                                  .Define("bsortjet_mass1", "Jet_mass[bsortjet[0]]")
                                  .Define("bsortjet_mass2", "Jet_mass[bsortjet[1]]")
                                  .Define("bsortjet_mass3", "Jet_mass[bsortjet[2]]")
                                  .Define("bsortjet_mass4", "Jet_mass[bsortjet[3]]");
  auto dfs_S2_ctag = dfs_S2_bsort.Define("ctag_idx", sort_ctag_idx, {"Jet_pt", "Jet_btagDeepFlavC", "Jet_eta", "Jet_jetId"})
                                 .Define("ctag", resize , {"ctag_idx"})
                                 .Define("ctag_pt1", "Jet_pt[ctag[0]]")
                                 .Define("ctag_pt2", "Jet_pt[ctag[1]]")
                                 .Define("ctag_pt3", "Jet_pt[ctag[2]]")
                                 .Define("ctag_pt4", "Jet_pt[ctag[3]]")
                                 .Define("ctag_mass1", "Jet_mass[ctag[0]]")
                                 .Define("ctag_mass2", "Jet_mass[ctag[1]]")
                                 .Define("ctag_mass3", "Jet_mass[ctag[2]]")
                                 .Define("ctag_mass4", "Jet_mass[ctag[3]]")
                                 .Define("ctag_eta1", "Jet_eta[ctag[0]]")
                                 .Define("ctag_eta2", "Jet_eta[ctag[1]]")
                                 .Define("ctag_eta3", "Jet_eta[ctag[2]]")
                                 .Define("ctag_eta4", "Jet_eta[ctag[3]]")
                                 .Define("ctag_phi1", "Jet_phi[ctag[0]]")
                                 .Define("ctag_phi2", "Jet_phi[ctag[1]]")
                                 .Define("ctag_phi3", "Jet_phi[ctag[2]]")
                                 .Define("ctag_phi4", "Jet_phi[ctag[3]]")
                                 .Define("ctag_CvsL1", "CvsL[ctag[0]]")
                                 .Define("ctag_CvsL2", "CvsL[ctag[1]]")
                                 .Define("ctag_CvsL3", "CvsL[ctag[2]]")
                                 .Define("ctag_CvsL4", "CvsL[ctag[3]]")
                                 .Define("ctag_CvsB1", "CvsB[ctag[0]]")
                                 .Define("ctag_CvsB2", "CvsB[ctag[1]]")
                                 .Define("ctag_CvsB3", "CvsB[ctag[2]]")
                                 .Define("ctag_CvsB4", "CvsB[ctag[3]]")
                                 .Define("ctag_Ctag1", "Jet_btagDeepFlavC[ctag[0]]")
                                 .Define("ctag_Ctag2", "Jet_btagDeepFlavC[ctag[1]]")
                                 .Define("ctag_Ctag3", "Jet_btagDeepFlavC[ctag[2]]")
                                 .Define("ctag_Ctag4", "Jet_btagDeepFlavC[ctag[3]]");
  auto dfs_S2_cvsl = dfs_S2_ctag.Define("cvsl_idx", sort_ctag_idx, {"Jet_pt", "CvsL", "Jet_eta", "Jet_jetId"})
                                .Define("cvsl", resize , {"cvsl_idx"})
                                .Define("cvsl_pt1", "Jet_pt[cvsl[0]]")
                                .Define("cvsl_pt2", "Jet_pt[cvsl[1]]")
                                .Define("cvsl_pt3", "Jet_pt[cvsl[2]]")
                                .Define("cvsl_pt4", "Jet_pt[cvsl[3]]")
                                .Define("cvsl_mass1", "Jet_mass[cvsl[0]]")
                                .Define("cvsl_mass2", "Jet_mass[cvsl[1]]")
                                .Define("cvsl_mass3", "Jet_mass[cvsl[2]]")
                                .Define("cvsl_mass4", "Jet_mass[cvsl[3]]")
                                .Define("cvsl_eta1", "Jet_eta[cvsl[0]]")
                                .Define("cvsl_eta2", "Jet_eta[cvsl[1]]")
                                .Define("cvsl_eta3", "Jet_eta[cvsl[2]]")
                                .Define("cvsl_eta4", "Jet_eta[cvsl[3]]")
                                .Define("cvsl_phi1", "Jet_phi[cvsl[0]]")
                                .Define("cvsl_phi2", "Jet_phi[cvsl[1]]")
                                .Define("cvsl_phi3", "Jet_phi[cvsl[2]]")
                                .Define("cvsl_phi4", "Jet_phi[cvsl[3]]")
                                .Define("cvsl_CvsL1", "CvsL[cvsl[0]]")
                                .Define("cvsl_CvsL2", "CvsL[cvsl[1]]")
                                .Define("cvsl_CvsL3", "CvsL[cvsl[2]]")
                                .Define("cvsl_CvsL4", "CvsL[cvsl[3]]")
                                .Define("cvsl_CvsB1", "CvsB[cvsl[0]]")
                                .Define("cvsl_CvsB2", "CvsB[cvsl[1]]")
                                .Define("cvsl_CvsB3", "CvsB[cvsl[2]]")
                                .Define("cvsl_CvsB4", "CvsB[cvsl[3]]")
                                .Define("cvsl_Ctag1", "Jet_btagDeepFlavC[cvsl[0]]")
                                .Define("cvsl_Ctag2", "Jet_btagDeepFlavC[cvsl[1]]")
                                .Define("cvsl_Ctag3", "Jet_btagDeepFlavC[cvsl[2]]")
                                .Define("cvsl_Ctag4", "Jet_btagDeepFlavC[cvsl[3]]");
  auto dfs_S2_cvsb = dfs_S2_cvsl.Define("cvsb_idx", sort_ctag_idx, {"Jet_pt", "CvsB", "Jet_eta", "Jet_jetId"})
                                .Define("cvsb", resize , {"cvsb_idx"})
                                .Define("cvsb_pt1", "Jet_pt[cvsb[0]]")
                                .Define("cvsb_pt2", "Jet_pt[cvsb[1]]")
                                .Define("cvsb_pt3", "Jet_pt[cvsb[2]]")
                                .Define("cvsb_pt4", "Jet_pt[cvsb[3]]")
                                .Define("cvsb_mass1", "Jet_mass[cvsb[0]]")
                                .Define("cvsb_mass2", "Jet_mass[cvsb[1]]")
                                .Define("cvsb_mass3", "Jet_mass[cvsb[2]]")
                                .Define("cvsb_mass4", "Jet_mass[cvsb[3]]")
                                .Define("cvsb_eta1", "Jet_eta[cvsb[0]]")
                                .Define("cvsb_eta2", "Jet_eta[cvsb[1]]")
                                .Define("cvsb_eta3", "Jet_eta[cvsb[2]]")
                                .Define("cvsb_eta4", "Jet_eta[cvsb[3]]")
                                .Define("cvsb_phi1", "Jet_phi[cvsb[0]]")
                                .Define("cvsb_phi2", "Jet_phi[cvsb[1]]")
                                .Define("cvsb_phi3", "Jet_phi[cvsb[2]]")
                                .Define("cvsb_phi4", "Jet_phi[cvsb[3]]")
                                .Define("cvsb_CvsL1", "CvsL[cvsb[0]]")
                                .Define("cvsb_CvsL2", "CvsL[cvsb[1]]")
                                .Define("cvsb_CvsL3", "CvsL[cvsb[2]]")
                                .Define("cvsb_CvsL4", "CvsL[cvsb[3]]")
                                .Define("cvsb_CvsB1", "CvsB[cvsb[0]]")
                                .Define("cvsb_CvsB2", "CvsB[cvsb[1]]")
                                .Define("cvsb_CvsB3", "CvsB[cvsb[2]]")
                                .Define("cvsb_CvsB4", "CvsB[cvsb[3]]")
                                .Define("cvsb_Ctag1", "Jet_btagDeepFlavC[cvsb[0]]")
                                .Define("cvsb_Ctag2", "Jet_btagDeepFlavC[cvsb[1]]")
                                .Define("cvsb_Ctag3", "Jet_btagDeepFlavC[cvsb[2]]")
                                .Define("cvsb_Ctag4", "Jet_btagDeepFlavC[cvsb[3]]");
//  auto df_S1_sortbjet = df_S1_goodjet.Define("sortbjet_idx", b_sort_btag_idx, {"Jet_pt", "Jet_btagDeepFlavB", "Jet_eta", "Jet_jetId"})
  auto dfs_S2_HLT = dfs_S2_cvsb.Filter("HLT_IsoMu24", "HLT Trigger IsoMu24")
                                .Define("goodmuons","Muon_pt > 30  && abs(Muon_eta) < 2.4 && Muon_tightId && Muon_pfRelIso03_all < 0.15")
                                .Define("goodtaus","Tau_pt > 30 && abs(Tau_eta) < 2.3 && Tau_idMVAoldDM & 2");
  auto dfs_S2_goodmuon = dfs_S2_HLT.Filter("Sum( goodmuons ) >=1 ","Events with at least a goood muon")
                                   .Define("ntaujets", "Sum( goodtaus )");
  auto dfs_S2_goodtau = dfs_S2_goodmuon.Filter("Sum ( goodtaus ) >=1", "Events with at least a good tau");

  
  //histograms 
  auto h_lf_nbjets_m = df_S1_lf.Histo1D({"h_lf_nbjets_m", "h_lf_nbjets_m", 6, 0, 6}, "nbjets_m");
  auto h_lf_nbjets_t = df_S1_lf.Histo1D({"h_lf_nbjets_t", "h_lf_nbjets_t", 6, 0, 6}, "nbjets_t");
  auto h_lf_ncjets_l = df_S1_lf.Histo1D({"h_lf_ncjets_l", "h_lf_ncjets_l", 10, 0, 10}, "ncjets_l");
  auto h_lf_ncjets_m = df_S1_lf.Histo1D({"h_lf_ncjets_m", "h_lf_ncjets_m", 8, 0, 8}, "ncjets_m");
  auto h_lf_ncjets_t = df_S1_lf.Histo1D({"h_lf_ncjets_t", "h_lf_ncjets_t", 6, 0, 6}, "ncjets_t");
  auto h_lf_ntaujets = df_S1_goodmuon.Filter("category == 0").Histo1D({"h_lf_ntaujets", "h_lf_ntaujets", 5, 0, 5}, "ntaujets");
  auto h_lf_jet_pt = df_S1_lf.Histo1D({"h_lf_jet_pt", "h_lf_jet_pt", 12, 0, 240}, "jet_pt");
  auto h_lf_bjet_m_pt = df_S1_lf.Filter("nbjets_m >= 1").Histo1D({"h_lf_bjet_m_pt", "h_lf_bjet_m_pt", 12, 0, 240}, "bjet_m_pt");
  auto h_lf_bjet_t_pt = df_S1_lf.Filter("nbjets_t >= 1").Histo1D({"h_lf_bjet_t_pt", "h_lf_bjet_t_pt", 12, 0, 240}, "bjet_t_pt");
  auto h_lf_cjet_l_pt = df_S1_lf.Filter("ncjets_l >= 1").Histo1D({"h_lf_cjet_l_pt", "h_lf_cjet_l_pt", 12, 0, 240}, "cjet_l_pt");
  auto h_lf_cjet_m_pt = df_S1_lf.Filter("ncjets_m >= 1").Histo1D({"h_lf_cjet_m_pt", "h_lf_cjet_m_pt", 12, 0, 240}, "cjet_m_pt");
  auto h_lf_cjet_t_pt = df_S1_lf.Filter("ncjets_t >= 1").Histo1D({"h_lf_cjet_t_pt", "h_lf_cjet_t_pt", 12, 0, 240}, "cjet_t_pt");
  auto h_lf_bsortjet_btag1 = df_S1_lf.Histo1D({"h_lf_bsortjet_btag1", "h_lf_bsortjet_btag1", 44, 0, 1.1}, "bsortjet_btag1");
  auto h_lf_bsortjet_btag2 = df_S1_lf.Histo1D({"h_lf_bsortjet_btag2", "h_lf_bsortjet_btag2", 44, 0, 1.1}, "bsortjet_btag2");
  auto h_lf_bsortjet_btag3 = df_S1_lf.Histo1D({"h_lf_bsortjet_btag3", "h_lf_bsortjet_btag3", 44, 0, 1.1}, "bsortjet_btag3");
  auto h_lf_bsortjet_btag4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_bsortjet_btag4", "h_lf_bsortjet_btag4", 44, 0, 1.1}, "bsortjet_btag4");
  auto h_lf_bsortjet_pt1 = df_S1_lf.Histo1D({"h_lf_bsortjet_pt1", "h_lf_bsortjet_pt1", 10, 0, 200}, "bsortjet_pt1");
  auto h_lf_bsortjet_pt2 = df_S1_lf.Histo1D({"h_lf_bsortjet_pt2", "h_lf_bsortjet_pt2", 10, 0, 200}, "bsortjet_pt2");
  auto h_lf_bsortjet_pt3 = df_S1_lf.Histo1D({"h_lf_bsortjet_pt3", "h_lf_bsortjet_pt3", 10, 0, 200}, "bsortjet_pt3");
  auto h_lf_bsortjet_pt4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_bsortjet_pt4", "h_lf_bsortjet_pt4", 10, 0, 200}, "bsortjet_pt4");
  auto h_lf_bsortjet_mass1 = df_S1_lf.Histo1D({"h_lf_bsortjet_mass1", "h_lf_bsortjet_mass1", 25, 0, 25}, "bsortjet_mass1");
  auto h_lf_bsortjet_mass2 = df_S1_lf.Histo1D({"h_lf_bsortjet_mass2", "h_lf_bsortjet_mass2", 25, 0, 25}, "bsortjet_mass2");
  auto h_lf_bsortjet_mass3 = df_S1_lf.Histo1D({"h_lf_bsortjet_mass3", "h_lf_bsortjet_mass3", 25, 0, 25}, "bsortjet_mass3");
  auto h_lf_bsortjet_mass4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_bsortjet_mass4", "h_lf_bsortjet_mass4", 25, 0, 25}, "bsortjet_mass4");
  auto h_lf_ctag_pt1 = df_S1_lf.Histo1D({"h_lf_ctag_pt1", "h_lf_ctag_pt1", 10, 0, 200}, "ctag_pt1");
  auto h_lf_ctag_pt2 = df_S1_lf.Histo1D({"h_lf_ctag_pt2", "h_lf_ctag_pt2", 10, 0, 200}, "ctag_pt2");
  auto h_lf_ctag_pt3 = df_S1_lf.Histo1D({"h_lf_ctag_pt3", "h_lf_ctag_pt3", 10, 0, 200}, "ctag_pt3");
  auto h_lf_ctag_pt4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_ctag_pt4", "h_lf_ctag_pt4", 10, 0, 200}, "ctag_pt4");
  auto h_lf_ctag_mass1 = df_S1_lf.Histo1D({"h_lf_ctag_mass1", "h_lf_ctag_mass1", 25, 0, 25}, "ctag_mass1");
  auto h_lf_ctag_mass2 = df_S1_lf.Histo1D({"h_lf_ctag_mass2", "h_lf_ctag_mass2", 25, 0, 25}, "ctag_mass2");
  auto h_lf_ctag_mass3 = df_S1_lf.Histo1D({"h_lf_ctag_mass3", "h_lf_ctag_mass3", 25, 0, 25}, "ctag_mass3");
  auto h_lf_ctag_mass4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_ctag_mass4", "h_lf_ctag_mass4", 25, 0, 25}, "ctag_mass4");
  auto h_lf_ctag_eta1 = df_S1_lf.Histo1D({"h_lf_ctag_eta1", "h_lf_ctag_eta1", 45, -3, 3}, "ctag_eta1");
  auto h_lf_ctag_eta2 = df_S1_lf.Histo1D({"h_lf_ctag_eta2", "h_lf_ctag_eta2", 45, -3, 3}, "ctag_eta2");
  auto h_lf_ctag_eta3 = df_S1_lf.Histo1D({"h_lf_ctag_eta3", "h_lf_ctag_eta3", 45, -3, 3}, "ctag_eta3");
  auto h_lf_ctag_eta4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_ctag_eta4", "h_lf_ctag_eta4", 45, -3, 3}, "ctag_eta4");
  auto h_lf_ctag_phi1 = df_S1_lf.Histo1D({"h_lf_ctag_phi1", "h_lf_ctag_phi1", 100, -4, 4}, "ctag_phi1");
  auto h_lf_ctag_phi2 = df_S1_lf.Histo1D({"h_lf_ctag_phi2", "h_lf_ctag_phi2", 100, -4, 4}, "ctag_phi2");
  auto h_lf_ctag_phi3 = df_S1_lf.Histo1D({"h_lf_ctag_phi3", "h_lf_ctag_phi3", 100, -4, 4}, "ctag_phi3");
  auto h_lf_ctag_phi4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_ctag_phi4", "h_lf_ctag_phi4", 100, -4, 4}, "ctag_phi4");
  auto h_lf_ctag_CvsL1 = df_S1_lf.Histo1D({"h_lf_ctag_CvsL1", "h_lf_ctag_CvsL1", 44, 0, 1.2}, "ctag_CvsL1");
  auto h_lf_ctag_CvsL2 = df_S1_lf.Histo1D({"h_lf_ctag_CvsL2", "h_lf_ctag_CvsL2", 44, 0, 1.2}, "ctag_CvsL2");
  auto h_lf_ctag_CvsL3 = df_S1_lf.Histo1D({"h_lf_ctag_CvsL3", "h_lf_ctag_CvsL3", 44, 0, 1.2}, "ctag_CvsL3");
  auto h_lf_ctag_CvsL4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_ctag_CvsL4", "h_lf_ctag_CvsL4", 44, 0, 1.2}, "ctag_CvsL4");
  auto h_lf_ctag_CvsB1 = df_S1_lf.Histo1D({"h_lf_ctag_CvsB1", "h_lf_ctag_CvsB1", 44, 0, 1.2}, "ctag_CvsB1");
  auto h_lf_ctag_CvsB2 = df_S1_lf.Histo1D({"h_lf_ctag_CvsB2", "h_lf_ctag_CvsB2", 44, 0, 1.2}, "ctag_CvsB2");
  auto h_lf_ctag_CvsB3 = df_S1_lf.Histo1D({"h_lf_ctag_CvsB3", "h_lf_ctag_CvsB3", 44, 0, 1.2}, "ctag_CvsB3");
  auto h_lf_ctag_CvsB4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_ctag_CvsB4", "h_lf_ctag_CvsB4", 44, 0, 1.2}, "ctag_CvsB4");
  auto h_lf_ctag_Ctag1 = df_S1_lf.Histo1D({"h_lf_ctag_Ctag1", "h_lf_ctag_Ctag1", 44, 0, 1.2}, "ctag_Ctag1");
  auto h_lf_ctag_Ctag2 = df_S1_lf.Histo1D({"h_lf_ctag_Ctag2", "h_lf_ctag_Ctag2", 44, 0, 1.2}, "ctag_Ctag2");
  auto h_lf_ctag_Ctag3 = df_S1_lf.Histo1D({"h_lf_ctag_Ctag3", "h_lf_ctag_Ctag3", 44, 0, 1.2}, "ctag_Ctag3");
  auto h_lf_ctag_Ctag4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_ctag_Ctag4", "h_lf_ctag_Ctag4", 44, 0, 1.2}, "ctag_Ctag4");
  auto h_lf_cvsl_pt1 = df_S1_lf.Histo1D({"h_lf_cvsl_pt1", "h_lf_cvsl_pt1", 10, 0, 200}, "cvsl_pt1");
  auto h_lf_cvsl_pt2 = df_S1_lf.Histo1D({"h_lf_cvsl_pt2", "h_lf_cvsl_pt2", 10, 0, 200}, "cvsl_pt2");
  auto h_lf_cvsl_pt3 = df_S1_lf.Histo1D({"h_lf_cvsl_pt3", "h_lf_cvsl_pt3", 10, 0, 200}, "cvsl_pt3");
  auto h_lf_cvsl_pt4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_cvsl_pt4", "h_lf_cvsl_pt4", 10, 0, 200}, "cvsl_pt4");
  auto h_lf_cvsl_mass1 = df_S1_lf.Histo1D({"h_lf_cvsl_mass1", "h_lf_cvsl_mass1", 25, 0, 25}, "cvsl_mass1");
  auto h_lf_cvsl_mass2 = df_S1_lf.Histo1D({"h_lf_cvsl_mass2", "h_lf_cvsl_mass2", 25, 0, 25}, "cvsl_mass2");
  auto h_lf_cvsl_mass3 = df_S1_lf.Histo1D({"h_lf_cvsl_mass3", "h_lf_cvsl_mass3", 25, 0, 25}, "cvsl_mass3");
  auto h_lf_cvsl_mass4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_cvsl_mass4", "h_lf_cvsl_mass4", 25, 0, 25}, "cvsl_mass4");
  auto h_lf_cvsl_eta1 = df_S1_lf.Histo1D({"h_lf_cvsl_eta1", "h_lf_cvsl_eta1", 45, -3, 3}, "cvsl_eta1");
  auto h_lf_cvsl_eta2 = df_S1_lf.Histo1D({"h_lf_cvsl_eta2", "h_lf_cvsl_eta2", 45, -3, 3}, "cvsl_eta2");
  auto h_lf_cvsl_eta3 = df_S1_lf.Histo1D({"h_lf_cvsl_eta3", "h_lf_cvsl_eta3", 45, -3, 3}, "cvsl_eta3");
  auto h_lf_cvsl_eta4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_cvsl_eta4", "h_lf_cvsl_eta4", 45, -3, 3}, "cvsl_eta4");
  auto h_lf_cvsl_phi1 = df_S1_lf.Histo1D({"h_lf_cvsl_phi1", "h_lf_cvsl_phi1", 100, -4, 4}, "cvsl_phi1");
  auto h_lf_cvsl_phi2 = df_S1_lf.Histo1D({"h_lf_cvsl_phi2", "h_lf_cvsl_phi2", 100, -4, 4}, "cvsl_phi2");
  auto h_lf_cvsl_phi3 = df_S1_lf.Histo1D({"h_lf_cvsl_phi3", "h_lf_cvsl_phi3", 100, -4, 4}, "cvsl_phi3");
  auto h_lf_cvsl_phi4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_cvsl_phi4", "h_lf_cvsl_phi4", 100, -4, 4}, "cvsl_phi4");
  auto h_lf_cvsl_CvsL1 = df_S1_lf.Histo1D({"h_lf_cvsl_CvsL1", "h_lf_cvsl_CvsL1", 44, 0, 1.2}, "cvsl_CvsL1");
  auto h_lf_cvsl_CvsL2 = df_S1_lf.Histo1D({"h_lf_cvsl_CvsL2", "h_lf_cvsl_CvsL2", 44, 0, 1.2}, "cvsl_CvsL2");
  auto h_lf_cvsl_CvsL3 = df_S1_lf.Histo1D({"h_lf_cvsl_CvsL3", "h_lf_cvsl_CvsL3", 44, 0, 1.2}, "cvsl_CvsL3");
  auto h_lf_cvsl_CvsL4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_cvsl_CvsL4", "h_lf_cvsl_CvsL4", 44, 0, 1.2}, "cvsl_CvsL4");
  auto h_lf_cvsl_CvsB1 = df_S1_lf.Histo1D({"h_lf_cvsl_CvsB1", "h_lf_cvsl_CvsB1", 44, 0, 1.2}, "cvsl_CvsB1");
  auto h_lf_cvsl_CvsB2 = df_S1_lf.Histo1D({"h_lf_cvsl_CvsB2", "h_lf_cvsl_CvsB2", 44, 0, 1.2}, "cvsl_CvsB2");
  auto h_lf_cvsl_CvsB3 = df_S1_lf.Histo1D({"h_lf_cvsl_CvsB3", "h_lf_cvsl_CvsB3", 44, 0, 1.2}, "cvsl_CvsB3");
  auto h_lf_cvsl_CvsB4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_cvsl_CvsB4", "h_lf_cvsl_CvsB4", 44, 0, 1.2}, "cvsl_CvsB4");
  auto h_lf_cvsl_Ctag1 = df_S1_lf.Histo1D({"h_lf_cvsl_Ctag1", "h_lf_cvsl_Ctag1", 44, 0, 1.2}, "cvsl_Ctag1");
  auto h_lf_cvsl_Ctag2 = df_S1_lf.Histo1D({"h_lf_cvsl_Ctag2", "h_lf_cvsl_Ctag2", 44, 0, 1.2}, "cvsl_Ctag2");
  auto h_lf_cvsl_Ctag3 = df_S1_lf.Histo1D({"h_lf_cvsl_Ctag3", "h_lf_cvsl_Ctag3", 44, 0, 1.2}, "cvsl_Ctag3");
  auto h_lf_cvsl_Ctag4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_cvsl_Ctag4", "h_lf_cvsl_Ctag4", 44, 0, 1.2}, "cvsl_Ctag4");
  auto h_lf_cvsb_pt1 = df_S1_lf.Histo1D({"h_lf_cvsb_pt1", "h_lf_cvsb_pt1", 10, 0, 200}, "cvsb_pt1");
  auto h_lf_cvsb_pt2 = df_S1_lf.Histo1D({"h_lf_cvsb_pt2", "h_lf_cvsb_pt2", 10, 0, 200}, "cvsb_pt2");
  auto h_lf_cvsb_pt3 = df_S1_lf.Histo1D({"h_lf_cvsb_pt3", "h_lf_cvsb_pt3", 10, 0, 200}, "cvsb_pt3");
  auto h_lf_cvsb_pt4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_cvsb_pt4", "h_lf_cvsb_pt4", 10, 0, 200}, "cvsb_pt4");
  auto h_lf_cvsb_mass1 = df_S1_lf.Histo1D({"h_lf_cvsb_mass1", "h_lf_cvsb_mass1", 25, 0, 25}, "cvsb_mass1");
  auto h_lf_cvsb_mass2 = df_S1_lf.Histo1D({"h_lf_cvsb_mass2", "h_lf_cvsb_mass2", 25, 0, 25}, "cvsb_mass2");
  auto h_lf_cvsb_mass3 = df_S1_lf.Histo1D({"h_lf_cvsb_mass3", "h_lf_cvsb_mass3", 25, 0, 25}, "cvsb_mass3");
  auto h_lf_cvsb_mass4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_cvsb_mass4", "h_lf_cvsb_mass4", 25, 0, 25}, "cvsb_mass4");
  auto h_lf_cvsb_eta1 = df_S1_lf.Histo1D({"h_lf_cvsb_eta1", "h_lf_cvsb_eta1", 45, -3, 3}, "cvsb_eta1");
  auto h_lf_cvsb_eta2 = df_S1_lf.Histo1D({"h_lf_cvsb_eta2", "h_lf_cvsb_eta2", 45, -3, 3}, "cvsb_eta2");
  auto h_lf_cvsb_eta3 = df_S1_lf.Histo1D({"h_lf_cvsb_eta3", "h_lf_cvsb_eta3", 45, -3, 3}, "cvsb_eta3");
  auto h_lf_cvsb_eta4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_cvsb_eta4", "h_lf_cvsb_eta4", 45, -3, 3}, "cvsb_eta4");
  auto h_lf_cvsb_phi1 = df_S1_lf.Histo1D({"h_lf_cvsb_phi1", "h_lf_cvsb_phi1", 100, -4, 4}, "cvsb_phi1");
  auto h_lf_cvsb_phi2 = df_S1_lf.Histo1D({"h_lf_cvsb_phi2", "h_lf_cvsb_phi2", 100, -4, 4}, "cvsb_phi2");
  auto h_lf_cvsb_phi3 = df_S1_lf.Histo1D({"h_lf_cvsb_phi3", "h_lf_cvsb_phi3", 100, -4, 4}, "cvsb_phi3");
  auto h_lf_cvsb_phi4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_cvsb_phi4", "h_lf_cvsb_phi4", 100, -4, 4}, "cvsb_phi4");
  auto h_lf_cvsb_CvsL1 = df_S1_lf.Histo1D({"h_lf_cvsb_CvsL1", "h_lf_cvsb_CvsL1", 44, 0, 1.2}, "cvsb_CvsL1");
  auto h_lf_cvsb_CvsL2 = df_S1_lf.Histo1D({"h_lf_cvsb_CvsL2", "h_lf_cvsb_CvsL2", 44, 0, 1.2}, "cvsb_CvsL2");
  auto h_lf_cvsb_CvsL3 = df_S1_lf.Histo1D({"h_lf_cvsb_CvsL3", "h_lf_cvsb_CvsL3", 44, 0, 1.2}, "cvsb_CvsL3");
  auto h_lf_cvsb_CvsL4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_cvsb_CvsL4", "h_lf_cvsb_CvsL4", 44, 0, 1.2}, "cvsb_CvsL4");
  auto h_lf_cvsb_CvsB1 = df_S1_lf.Histo1D({"h_lf_cvsb_CvsB1", "h_lf_cvsb_CvsB1", 44, 0, 1.2}, "cvsb_CvsB1");
  auto h_lf_cvsb_CvsB2 = df_S1_lf.Histo1D({"h_lf_cvsb_CvsB2", "h_lf_cvsb_CvsB2", 44, 0, 1.2}, "cvsb_CvsB2");
  auto h_lf_cvsb_CvsB3 = df_S1_lf.Histo1D({"h_lf_cvsb_CvsB3", "h_lf_cvsb_CvsB3", 44, 0, 1.2}, "cvsb_CvsB3");
  auto h_lf_cvsb_CvsB4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_cvsb_CvsB4", "h_lf_cvsb_CvsB4", 44, 0, 1.2}, "cvsb_CvsB4");
  auto h_lf_cvsb_Ctag1 = df_S1_lf.Histo1D({"h_lf_cvsb_Ctag1", "h_lf_cvsb_Ctag1", 44, 0, 1.2}, "cvsb_Ctag1");
  auto h_lf_cvsb_Ctag2 = df_S1_lf.Histo1D({"h_lf_cvsb_Ctag2", "h_lf_cvsb_Ctag2", 44, 0, 1.2}, "cvsb_Ctag2");
  auto h_lf_cvsb_Ctag3 = df_S1_lf.Histo1D({"h_lf_cvsb_Ctag3", "h_lf_cvsb_Ctag3", 44, 0, 1.2}, "cvsb_Ctag3");
  auto h_lf_cvsb_Ctag4 = df_S1_lf.Filter("ngoodjets >= 4").Histo1D({"h_lf_cvsb_Ctag4", "h_lf_cvsb_Ctag4", 44, 0, 1.2}, "cvsb_Ctag4");

  auto h_b_nbjets_m = df_S1_b.Histo1D({"h_b_nbjets_m", "h_b_nbjets_m", 6, 0, 6}, "nbjets_m");
  auto h_b_nbjets_t = df_S1_b.Histo1D({"h_b_nbjets_t", "h_b_nbjets_t", 6, 0, 6}, "nbjets_t");
  auto h_b_ncjets_l = df_S1_b.Histo1D({"h_b_ncjets_l", "h_b_ncjets_l", 10, 0, 10}, "ncjets_l");
  auto h_b_ncjets_m = df_S1_b.Histo1D({"h_b_ncjets_m", "h_b_ncjets_m", 8, 0, 8}, "ncjets_m");
  auto h_b_ncjets_t = df_S1_b.Histo1D({"h_b_ncjets_t", "h_b_ncjets_t", 6, 0, 6}, "ncjets_t");
  auto h_b_ntaujets = df_S1_goodmuon.Filter("category == 1").Histo1D({"h_b_ntaujets", "h_b_ntaujets", 5, 0, 5}, "ntaujets");
  auto h_b_jet_pt = df_S1_b.Histo1D({"h_b_jet_pt", "h_b_jet_pt", 12, 0, 240}, "jet_pt");
  auto h_b_bjet_m_pt = df_S1_b.Filter("nbjets_m >= 1").Histo1D({"h_b_bjet_m_pt", "h_b_bjet_m_pt", 12, 0, 240}, "bjet_m_pt");
  auto h_b_bjet_t_pt = df_S1_b.Filter("nbjets_t >= 1").Histo1D({"h_b_bjet_t_pt", "h_b_bjet_t_pt", 12, 0, 240}, "bjet_t_pt");
  auto h_b_cjet_l_pt = df_S1_b.Filter("ncjets_l >= 1").Histo1D({"h_b_cjet_l_pt", "h_b_cjet_l_pt", 12, 0, 240}, "cjet_l_pt");
  auto h_b_cjet_m_pt = df_S1_b.Filter("ncjets_m >= 1").Histo1D({"h_b_cjet_m_pt", "h_b_cjet_m_pt", 12, 0, 240}, "cjet_m_pt");
  auto h_b_cjet_t_pt = df_S1_b.Filter("ncjets_t >= 1").Histo1D({"h_b_cjet_t_pt", "h_b_cjet_t_pt", 12, 0, 240}, "cjet_t_pt");
  auto h_b_bsortjet_btag1 = df_S1_b.Histo1D({"h_b_bsortjet_btag1", "h_b_bsortjet_btag1", 44, 0, 1.1}, "bsortjet_btag1");
  auto h_b_bsortjet_btag2 = df_S1_b.Histo1D({"h_b_bsortjet_btag2", "h_b_bsortjet_btag2", 44, 0, 1.1}, "bsortjet_btag2");
  auto h_b_bsortjet_btag3 = df_S1_b.Histo1D({"h_b_bsortjet_btag3", "h_b_bsortjet_btag3", 44, 0, 1.1}, "bsortjet_btag3");
  auto h_b_bsortjet_btag4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_bsortjet_btag4", "h_b_bsortjet_btag4", 44, 0, 1.1}, "bsortjet_btag4");
  auto h_b_bsortjet_pt1 = df_S1_b.Histo1D({"h_b_bsortjet_pt1", "h_b_bsortjet_pt1", 10, 0, 200}, "bsortjet_pt1");
  auto h_b_bsortjet_pt2 = df_S1_b.Histo1D({"h_b_bsortjet_pt2", "h_b_bsortjet_pt2", 10, 0, 200}, "bsortjet_pt2");
  auto h_b_bsortjet_pt3 = df_S1_b.Histo1D({"h_b_bsortjet_pt3", "h_b_bsortjet_pt3", 10, 0, 200}, "bsortjet_pt3");
  auto h_b_bsortjet_pt4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_bsortjet_pt4", "h_b_bsortjet_pt4", 10, 0, 200}, "bsortjet_pt4");
  auto h_b_bsortjet_mass1 = df_S1_b.Histo1D({"h_b_bsortjet_mass1", "h_b_bsortjet_mass1", 25, 0, 25}, "bsortjet_mass1");
  auto h_b_bsortjet_mass2 = df_S1_b.Histo1D({"h_b_bsortjet_mass2", "h_b_bsortjet_mass2", 25, 0, 25}, "bsortjet_mass2");
  auto h_b_bsortjet_mass3 = df_S1_b.Histo1D({"h_b_bsortjet_mass3", "h_b_bsortjet_mass3", 25, 0, 25}, "bsortjet_mass3");
  auto h_b_bsortjet_mass4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_bsortjet_mass4", "h_b_bsortjet_mass4", 25, 0, 25}, "bsortjet_mass4");
  auto h_b_ctag_pt1 = df_S1_b.Histo1D({"h_b_ctag_pt1", "h_b_ctag_pt1", 10, 0, 200}, "ctag_pt1");
  auto h_b_ctag_pt2 = df_S1_b.Histo1D({"h_b_ctag_pt2", "h_b_ctag_pt2", 10, 0, 200}, "ctag_pt2");
  auto h_b_ctag_pt3 = df_S1_b.Histo1D({"h_b_ctag_pt3", "h_b_ctag_pt3", 10, 0, 200}, "ctag_pt3");
  auto h_b_ctag_pt4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_ctag_pt4", "h_b_ctag_pt4", 10, 0, 200}, "ctag_pt4");
  auto h_b_ctag_mass1 = df_S1_b.Histo1D({"h_b_ctag_mass1", "h_b_ctag_mass1", 25, 0, 25}, "ctag_mass1");
  auto h_b_ctag_mass2 = df_S1_b.Histo1D({"h_b_ctag_mass2", "h_b_ctag_mass2", 25, 0, 25}, "ctag_mass2");
  auto h_b_ctag_mass3 = df_S1_b.Histo1D({"h_b_ctag_mass3", "h_b_ctag_mass3", 25, 0, 25}, "ctag_mass3");
  auto h_b_ctag_mass4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_ctag_mass4", "h_b_ctag_mass4", 25, 0, 25}, "ctag_mass4");
  auto h_b_ctag_eta1 = df_S1_b.Histo1D({"h_b_ctag_eta1", "h_b_ctag_eta1", 45, -3, 3}, "ctag_eta1");
  auto h_b_ctag_eta2 = df_S1_b.Histo1D({"h_b_ctag_eta2", "h_b_ctag_eta2", 45, -3, 3}, "ctag_eta2");
  auto h_b_ctag_eta3 = df_S1_b.Histo1D({"h_b_ctag_eta3", "h_b_ctag_eta3", 45, -3, 3}, "ctag_eta3");
  auto h_b_ctag_eta4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_ctag_eta4", "h_b_ctag_eta4", 45, -3, 3}, "ctag_eta4");
  auto h_b_ctag_phi1 = df_S1_b.Histo1D({"h_b_ctag_phi1", "h_b_ctag_phi1", 100, -4, 4}, "ctag_phi1");
  auto h_b_ctag_phi2 = df_S1_b.Histo1D({"h_b_ctag_phi2", "h_b_ctag_phi2", 100, -4, 4}, "ctag_phi2");
  auto h_b_ctag_phi3 = df_S1_b.Histo1D({"h_b_ctag_phi3", "h_b_ctag_phi3", 100, -4, 4}, "ctag_phi3");
  auto h_b_ctag_phi4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_ctag_phi4", "h_b_ctag_phi4", 100, -4, 4}, "ctag_phi4");
  auto h_b_ctag_CvsL1 = df_S1_b.Histo1D({"h_b_ctag_CvsL1", "h_b_ctag_CvsL1", 44, 0, 1.2}, "ctag_CvsL1");
  auto h_b_ctag_CvsL2 = df_S1_b.Histo1D({"h_b_ctag_CvsL2", "h_b_ctag_CvsL2", 44, 0, 1.2}, "ctag_CvsL2");
  auto h_b_ctag_CvsL3 = df_S1_b.Histo1D({"h_b_ctag_CvsL3", "h_b_ctag_CvsL3", 44, 0, 1.2}, "ctag_CvsL3");
  auto h_b_ctag_CvsL4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_ctag_CvsL4", "h_b_ctag_CvsL4", 44, 0, 1.2}, "ctag_CvsL4");
  auto h_b_ctag_CvsB1 = df_S1_b.Histo1D({"h_b_ctag_CvsB1", "h_b_ctag_CvsB1", 44, 0, 1.2}, "ctag_CvsB1");
  auto h_b_ctag_CvsB2 = df_S1_b.Histo1D({"h_b_ctag_CvsB2", "h_b_ctag_CvsB2", 44, 0, 1.2}, "ctag_CvsB2");
  auto h_b_ctag_CvsB3 = df_S1_b.Histo1D({"h_b_ctag_CvsB3", "h_b_ctag_CvsB3", 44, 0, 1.2}, "ctag_CvsB3");
  auto h_b_ctag_CvsB4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_ctag_CvsB4", "h_b_ctag_CvsB4", 44, 0, 1.2}, "ctag_CvsB4");
  auto h_b_ctag_Ctag1 = df_S1_b.Histo1D({"h_b_ctag_Ctag1", "h_b_ctag_Ctag1", 44, 0, 1.2}, "ctag_Ctag1");
  auto h_b_ctag_Ctag2 = df_S1_b.Histo1D({"h_b_ctag_Ctag2", "h_b_ctag_Ctag2", 44, 0, 1.2}, "ctag_Ctag2");
  auto h_b_ctag_Ctag3 = df_S1_b.Histo1D({"h_b_ctag_Ctag3", "h_b_ctag_Ctag3", 44, 0, 1.2}, "ctag_Ctag3");
  auto h_b_ctag_Ctag4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_ctag_Ctag4", "h_b_ctag_Ctag4", 44, 0, 1.2}, "ctag_Ctag4");
  auto h_b_cvsl_pt1 = df_S1_b.Histo1D({"h_b_cvsl_pt1", "h_b_cvsl_pt1", 10, 0, 200}, "cvsl_pt1");
  auto h_b_cvsl_pt2 = df_S1_b.Histo1D({"h_b_cvsl_pt2", "h_b_cvsl_pt2", 10, 0, 200}, "cvsl_pt2");
  auto h_b_cvsl_pt3 = df_S1_b.Histo1D({"h_b_cvsl_pt3", "h_b_cvsl_pt3", 10, 0, 200}, "cvsl_pt3");
  auto h_b_cvsl_pt4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_cvsl_pt4", "h_b_cvsl_pt4", 10, 0, 200}, "cvsl_pt4");
  auto h_b_cvsl_mass1 = df_S1_b.Histo1D({"h_b_cvsl_mass1", "h_b_cvsl_mass1", 25, 0, 25}, "cvsl_mass1");
  auto h_b_cvsl_mass2 = df_S1_b.Histo1D({"h_b_cvsl_mass2", "h_b_cvsl_mass2", 25, 0, 25}, "cvsl_mass2");
  auto h_b_cvsl_mass3 = df_S1_b.Histo1D({"h_b_cvsl_mass3", "h_b_cvsl_mass3", 25, 0, 25}, "cvsl_mass3");
  auto h_b_cvsl_mass4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_cvsl_mass4", "h_b_cvsl_mass4", 25, 0, 25}, "cvsl_mass4");
  auto h_b_cvsl_eta1 = df_S1_b.Histo1D({"h_b_cvsl_eta1", "h_b_cvsl_eta1", 45, -3, 3}, "cvsl_eta1");
  auto h_b_cvsl_eta2 = df_S1_b.Histo1D({"h_b_cvsl_eta2", "h_b_cvsl_eta2", 45, -3, 3}, "cvsl_eta2");
  auto h_b_cvsl_eta3 = df_S1_b.Histo1D({"h_b_cvsl_eta3", "h_b_cvsl_eta3", 45, -3, 3}, "cvsl_eta3");
  auto h_b_cvsl_eta4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_cvsl_eta4", "h_b_cvsl_eta4", 45, -3, 3}, "cvsl_eta4");
  auto h_b_cvsl_phi1 = df_S1_b.Histo1D({"h_b_cvsl_phi1", "h_b_cvsl_phi1", 100, -4, 4}, "cvsl_phi1");
  auto h_b_cvsl_phi2 = df_S1_b.Histo1D({"h_b_cvsl_phi2", "h_b_cvsl_phi2", 100, -4, 4}, "cvsl_phi2");
  auto h_b_cvsl_phi3 = df_S1_b.Histo1D({"h_b_cvsl_phi3", "h_b_cvsl_phi3", 100, -4, 4}, "cvsl_phi3");
  auto h_b_cvsl_phi4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_cvsl_phi4", "h_b_cvsl_phi4", 100, -4, 4}, "cvsl_phi4");
  auto h_b_cvsl_CvsL1 = df_S1_b.Histo1D({"h_b_cvsl_CvsL1", "h_b_cvsl_CvsL1", 44, 0, 1.2}, "cvsl_CvsL1");
  auto h_b_cvsl_CvsL2 = df_S1_b.Histo1D({"h_b_cvsl_CvsL2", "h_b_cvsl_CvsL2", 44, 0, 1.2}, "cvsl_CvsL2");
  auto h_b_cvsl_CvsL3 = df_S1_b.Histo1D({"h_b_cvsl_CvsL3", "h_b_cvsl_CvsL3", 44, 0, 1.2}, "cvsl_CvsL3");
  auto h_b_cvsl_CvsL4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_cvsl_CvsL4", "h_b_cvsl_CvsL4", 44, 0, 1.2}, "cvsl_CvsL4");
  auto h_b_cvsl_CvsB1 = df_S1_b.Histo1D({"h_b_cvsl_CvsB1", "h_b_cvsl_CvsB1", 44, 0, 1.2}, "cvsl_CvsB1");
  auto h_b_cvsl_CvsB2 = df_S1_b.Histo1D({"h_b_cvsl_CvsB2", "h_b_cvsl_CvsB2", 44, 0, 1.2}, "cvsl_CvsB2");
  auto h_b_cvsl_CvsB3 = df_S1_b.Histo1D({"h_b_cvsl_CvsB3", "h_b_cvsl_CvsB3", 44, 0, 1.2}, "cvsl_CvsB3");
  auto h_b_cvsl_CvsB4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_cvsl_CvsB4", "h_b_cvsl_CvsB4", 44, 0, 1.2}, "cvsl_CvsB4");
  auto h_b_cvsl_Ctag1 = df_S1_b.Histo1D({"h_b_cvsl_Ctag1", "h_b_cvsl_Ctag1", 44, 0, 1.2}, "cvsl_Ctag1");
  auto h_b_cvsl_Ctag2 = df_S1_b.Histo1D({"h_b_cvsl_Ctag2", "h_b_cvsl_Ctag2", 44, 0, 1.2}, "cvsl_Ctag2");
  auto h_b_cvsl_Ctag3 = df_S1_b.Histo1D({"h_b_cvsl_Ctag3", "h_b_cvsl_Ctag3", 44, 0, 1.2}, "cvsl_Ctag3");
  auto h_b_cvsl_Ctag4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_cvsl_Ctag4", "h_b_cvsl_Ctag4", 44, 0, 1.2}, "cvsl_Ctag4");
  auto h_b_cvsb_pt1 = df_S1_b.Histo1D({"h_b_cvsb_pt1", "h_b_cvsb_pt1", 10, 0, 200}, "cvsb_pt1");
  auto h_b_cvsb_pt2 = df_S1_b.Histo1D({"h_b_cvsb_pt2", "h_b_cvsb_pt2", 10, 0, 200}, "cvsb_pt2");
  auto h_b_cvsb_pt3 = df_S1_b.Histo1D({"h_b_cvsb_pt3", "h_b_cvsb_pt3", 10, 0, 200}, "cvsb_pt3");
  auto h_b_cvsb_pt4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_cvsb_pt4", "h_b_cvsb_pt4", 10, 0, 200}, "cvsb_pt4");
  auto h_b_cvsb_mass1 = df_S1_b.Histo1D({"h_b_cvsb_mass1", "h_b_cvsb_mass1", 25, 0, 25}, "cvsb_mass1");
  auto h_b_cvsb_mass2 = df_S1_b.Histo1D({"h_b_cvsb_mass2", "h_b_cvsb_mass2", 25, 0, 25}, "cvsb_mass2");
  auto h_b_cvsb_mass3 = df_S1_b.Histo1D({"h_b_cvsb_mass3", "h_b_cvsb_mass3", 25, 0, 25}, "cvsb_mass3");
  auto h_b_cvsb_mass4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_cvsb_mass4", "h_b_cvsb_mass4", 25, 0, 25}, "cvsb_mass4");
  auto h_b_cvsb_eta1 = df_S1_b.Histo1D({"h_b_cvsb_eta1", "h_b_cvsb_eta1", 45, -3, 3}, "cvsb_eta1");
  auto h_b_cvsb_eta2 = df_S1_b.Histo1D({"h_b_cvsb_eta2", "h_b_cvsb_eta2", 45, -3, 3}, "cvsb_eta2");
  auto h_b_cvsb_eta3 = df_S1_b.Histo1D({"h_b_cvsb_eta3", "h_b_cvsb_eta3", 45, -3, 3}, "cvsb_eta3");
  auto h_b_cvsb_eta4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_cvsb_eta4", "h_b_cvsb_eta4", 45, -3, 3}, "cvsb_eta4");
  auto h_b_cvsb_phi1 = df_S1_b.Histo1D({"h_b_cvsb_phi1", "h_b_cvsb_phi1", 100, -4, 4}, "cvsb_phi1");
  auto h_b_cvsb_phi2 = df_S1_b.Histo1D({"h_b_cvsb_phi2", "h_b_cvsb_phi2", 100, -4, 4}, "cvsb_phi2");
  auto h_b_cvsb_phi3 = df_S1_b.Histo1D({"h_b_cvsb_phi3", "h_b_cvsb_phi3", 100, -4, 4}, "cvsb_phi3");
  auto h_b_cvsb_phi4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_cvsb_phi4", "h_b_cvsb_phi4", 100, -4, 4}, "cvsb_phi4");
  auto h_b_cvsb_CvsL1 = df_S1_b.Histo1D({"h_b_cvsb_CvsL1", "h_b_cvsb_CvsL1", 44, 0, 1.2}, "cvsb_CvsL1");
  auto h_b_cvsb_CvsL2 = df_S1_b.Histo1D({"h_b_cvsb_CvsL2", "h_b_cvsb_CvsL2", 44, 0, 1.2}, "cvsb_CvsL2");
  auto h_b_cvsb_CvsL3 = df_S1_b.Histo1D({"h_b_cvsb_CvsL3", "h_b_cvsb_CvsL3", 44, 0, 1.2}, "cvsb_CvsL3");
  auto h_b_cvsb_CvsL4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_cvsb_CvsL4", "h_b_cvsb_CvsL4", 44, 0, 1.2}, "cvsb_CvsL4");
  auto h_b_cvsb_CvsB1 = df_S1_b.Histo1D({"h_b_cvsb_CvsB1", "h_b_cvsb_CvsB1", 44, 0, 1.2}, "cvsb_CvsB1");
  auto h_b_cvsb_CvsB2 = df_S1_b.Histo1D({"h_b_cvsb_CvsB2", "h_b_cvsb_CvsB2", 44, 0, 1.2}, "cvsb_CvsB2");
  auto h_b_cvsb_CvsB3 = df_S1_b.Histo1D({"h_b_cvsb_CvsB3", "h_b_cvsb_CvsB3", 44, 0, 1.2}, "cvsb_CvsB3");
  auto h_b_cvsb_CvsB4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_cvsb_CvsB4", "h_b_cvsb_CvsB4", 44, 0, 1.2}, "cvsb_CvsB4");
  auto h_b_cvsb_Ctag1 = df_S1_b.Histo1D({"h_b_cvsb_Ctag1", "h_b_cvsb_Ctag1", 44, 0, 1.2}, "cvsb_Ctag1");
  auto h_b_cvsb_Ctag2 = df_S1_b.Histo1D({"h_b_cvsb_Ctag2", "h_b_cvsb_Ctag2", 44, 0, 1.2}, "cvsb_Ctag2");
  auto h_b_cvsb_Ctag3 = df_S1_b.Histo1D({"h_b_cvsb_Ctag3", "h_b_cvsb_Ctag3", 44, 0, 1.2}, "cvsb_Ctag3");
  auto h_b_cvsb_Ctag4 = df_S1_b.Filter("ngoodjets >= 4").Histo1D({"h_b_cvsb_Ctag4", "h_b_cvsb_Ctag4", 44, 0, 1.2}, "cvsb_Ctag4");

  auto h_bb_nbjets_m = df_S1_bb.Histo1D({"h_bb_nbjets_m", "h_bb_nbjets_m", 6, 0, 6}, "nbjets_m");
  auto h_bb_nbjets_t = df_S1_bb.Histo1D({"h_bb_nbjets_t", "h_bb_nbjets_t", 6, 0, 6}, "nbjets_t");
  auto h_bb_ncjets_l = df_S1_b.Histo1D({"h_bb_ncjets_l", "h_bb_ncjets_l", 10, 0, 10}, "ncjets_l");
  auto h_bb_ncjets_m = df_S1_b.Histo1D({"h_bb_ncjets_m", "h_bb_ncjets_m", 8, 0, 8}, "ncjets_m");
  auto h_bb_ncjets_t = df_S1_b.Histo1D({"h_bb_ncjets_t", "h_bb_ncjets_t", 6, 0, 6}, "ncjets_t");
  auto h_bb_ntaujets = df_S1_goodmuon.Filter("category == 2").Histo1D({"h_bb_ntaujets", "h_bb_ntaujets", 5, 0, 5}, "ntaujets");
  auto h_bb_jet_pt = df_S1_bb.Histo1D({"h_bb_jet_pt", "h_bb_jet_pt", 12, 0, 240}, "jet_pt");
  auto h_bb_bjet_m_pt = df_S1_bb.Filter("nbjets_m >= 1").Histo1D({"h_bb_bjet_m_pt", "h_bb_bjet_m_pt", 12, 0, 240}, "bjet_m_pt");
  auto h_bb_bjet_t_pt = df_S1_bb.Filter("nbjets_t >= 1").Histo1D({"h_bb_bjet_t_pt", "h_bb_bjet_t_pt", 12, 0, 240}, "bjet_t_pt");
  auto h_bb_cjet_l_pt = df_S1_bb.Filter("ncjets_l >= 1").Histo1D({"h_bb_cjet_l_pt", "h_bb_cjet_l_pt", 12, 0, 240}, "cjet_l_pt");
  auto h_bb_cjet_m_pt = df_S1_bb.Filter("ncjets_m >= 1").Histo1D({"h_bb_cjet_m_pt", "h_bb_cjet_m_pt", 12, 0, 240}, "cjet_m_pt");
  auto h_bb_cjet_t_pt = df_S1_bb.Filter("ncjets_t >= 1").Histo1D({"h_bb_cjet_t_pt", "h_bb_cjet_t_pt", 12, 0, 240}, "cjet_t_pt");
  auto h_bb_bsortjet_btag1 = df_S1_bb.Histo1D({"h_bb_bsortjet_btag1", "h_bb_bsortjet_btag1", 44, 0, 1.1}, "bsortjet_btag1");
  auto h_bb_bsortjet_btag2 = df_S1_bb.Histo1D({"h_bb_bsortjet_btag2", "h_bb_bsortjet_btag2", 44, 0, 1.1}, "bsortjet_btag2");
  auto h_bb_bsortjet_btag3 = df_S1_bb.Histo1D({"h_bb_bsortjet_btag3", "h_bb_bsortjet_btag3", 44, 0, 1.1}, "bsortjet_btag3");
  auto h_bb_bsortjet_btag4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_bsortjet_btag4", "h_bb_bsortjet_btag4", 44, 0, 1.1}, "bsortjet_btag4");
  auto h_bb_bsortjet_pt1 = df_S1_bb.Histo1D({"h_bb_bsortjet_pt1", "h_bb_bsortjet_pt1", 10, 0, 200}, "bsortjet_pt1");
  auto h_bb_bsortjet_pt2 = df_S1_bb.Histo1D({"h_bb_bsortjet_pt2", "h_bb_bsortjet_pt2", 10, 0, 200}, "bsortjet_pt2");
  auto h_bb_bsortjet_pt3 = df_S1_bb.Histo1D({"h_bb_bsortjet_pt3", "h_bb_bsortjet_pt3", 10, 0, 200}, "bsortjet_pt3");
  auto h_bb_bsortjet_pt4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_bsortjet_pt4", "h_bb_bsortjet_pt4", 10, 0, 200}, "bsortjet_pt4");
  auto h_bb_bsortjet_mass1 = df_S1_bb.Histo1D({"h_bb_bsortjet_mass1", "h_bb_bsortjet_mass1", 25, 0, 25}, "bsortjet_mass1");
  auto h_bb_bsortjet_mass2 = df_S1_bb.Histo1D({"h_bb_bsortjet_mass2", "h_bb_bsortjet_mass2", 25, 0, 25}, "bsortjet_mass2");
  auto h_bb_bsortjet_mass3 = df_S1_bb.Histo1D({"h_bb_bsortjet_mass3", "h_bb_bsortjet_mass3", 25, 0, 25}, "bsortjet_mass3");
  auto h_bb_bsortjet_mass4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_bsortjet_mass4", "h_bb_bsortjet_mass4", 25, 0, 25}, "bsortjet_mass4");
  auto h_bb_ctag_pt1 = df_S1_bb.Histo1D({"h_bb_ctag_pt1", "h_bb_ctag_pt1", 10, 0, 200}, "ctag_pt1");
  auto h_bb_ctag_pt2 = df_S1_bb.Histo1D({"h_bb_ctag_pt2", "h_bb_ctag_pt2", 10, 0, 200}, "ctag_pt2");
  auto h_bb_ctag_pt3 = df_S1_bb.Histo1D({"h_bb_ctag_pt3", "h_bb_ctag_pt3", 10, 0, 200}, "ctag_pt3");
  auto h_bb_ctag_pt4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_ctag_pt4", "h_bb_ctag_pt4", 10, 0, 200}, "ctag_pt4");
  auto h_bb_ctag_mass1 = df_S1_bb.Histo1D({"h_bb_ctag_mass1", "h_bb_ctag_mass1", 25, 0, 25}, "ctag_mass1");
  auto h_bb_ctag_mass2 = df_S1_bb.Histo1D({"h_bb_ctag_mass2", "h_bb_ctag_mass2", 25, 0, 25}, "ctag_mass2");
  auto h_bb_ctag_mass3 = df_S1_bb.Histo1D({"h_bb_ctag_mass3", "h_bb_ctag_mass3", 25, 0, 25}, "ctag_mass3");
  auto h_bb_ctag_mass4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_ctag_mass4", "h_bb_ctag_mass4", 25, 0, 25}, "ctag_mass4");
  auto h_bb_ctag_eta1 = df_S1_bb.Histo1D({"h_bb_ctag_eta1", "h_bb_ctag_eta1", 45, -3, 3}, "ctag_eta1");
  auto h_bb_ctag_eta2 = df_S1_bb.Histo1D({"h_bb_ctag_eta2", "h_bb_ctag_eta2", 45, -3, 3}, "ctag_eta2");
  auto h_bb_ctag_eta3 = df_S1_bb.Histo1D({"h_bb_ctag_eta3", "h_bb_ctag_eta3", 45, -3, 3}, "ctag_eta3");
  auto h_bb_ctag_eta4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_ctag_eta4", "h_bb_ctag_eta4", 45, -3, 3}, "ctag_eta4");
  auto h_bb_ctag_phi1 = df_S1_bb.Histo1D({"h_bb_ctag_phi1", "h_bb_ctag_phi1", 100, -4, 4}, "ctag_phi1");
  auto h_bb_ctag_phi2 = df_S1_bb.Histo1D({"h_bb_ctag_phi2", "h_bb_ctag_phi2", 100, -4, 4}, "ctag_phi2");
  auto h_bb_ctag_phi3 = df_S1_bb.Histo1D({"h_bb_ctag_phi3", "h_bb_ctag_phi3", 100, -4, 4}, "ctag_phi3");
  auto h_bb_ctag_phi4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_ctag_phi4", "h_bb_ctag_phi4", 100, -4, 4}, "ctag_phi4");
  auto h_bb_ctag_CvsL1 = df_S1_bb.Histo1D({"h_bb_ctag_CvsL1", "h_bb_ctag_CvsL1", 44, 0, 1.2}, "ctag_CvsL1");
  auto h_bb_ctag_CvsL2 = df_S1_bb.Histo1D({"h_bb_ctag_CvsL2", "h_bb_ctag_CvsL2", 44, 0, 1.2}, "ctag_CvsL2");
  auto h_bb_ctag_CvsL3 = df_S1_bb.Histo1D({"h_bb_ctag_CvsL3", "h_bb_ctag_CvsL3", 44, 0, 1.2}, "ctag_CvsL3");
  auto h_bb_ctag_CvsL4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_ctag_CvsL4", "h_bb_ctag_CvsL4", 44, 0, 1.2}, "ctag_CvsL4");
  auto h_bb_ctag_CvsB1 = df_S1_bb.Histo1D({"h_bb_ctag_CvsB1", "h_bb_ctag_CvsB1", 44, 0, 1.2}, "ctag_CvsB1");
  auto h_bb_ctag_CvsB2 = df_S1_bb.Histo1D({"h_bb_ctag_CvsB2", "h_bb_ctag_CvsB2", 44, 0, 1.2}, "ctag_CvsB2");
  auto h_bb_ctag_CvsB3 = df_S1_bb.Histo1D({"h_bb_ctag_CvsB3", "h_bb_ctag_CvsB3", 44, 0, 1.2}, "ctag_CvsB3");
  auto h_bb_ctag_CvsB4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_ctag_CvsB4", "h_bb_ctag_CvsB4", 44, 0, 1.2}, "ctag_CvsB4");
  auto h_bb_ctag_Ctag1 = df_S1_bb.Histo1D({"h_bb_ctag_Ctag1", "h_bb_ctag_Ctag1", 44, 0, 1.2}, "ctag_Ctag1");
  auto h_bb_ctag_Ctag2 = df_S1_bb.Histo1D({"h_bb_ctag_Ctag2", "h_bb_ctag_Ctag2", 44, 0, 1.2}, "ctag_Ctag2");
  auto h_bb_ctag_Ctag3 = df_S1_bb.Histo1D({"h_bb_ctag_Ctag3", "h_bb_ctag_Ctag3", 44, 0, 1.2}, "ctag_Ctag3");
  auto h_bb_ctag_Ctag4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_ctag_Ctag4", "h_bb_ctag_Ctag4", 44, 0, 1.2}, "ctag_Ctag4");
  auto h_bb_cvsl_pt1 = df_S1_bb.Histo1D({"h_bb_cvsl_pt1", "h_bb_cvsl_pt1", 10, 0, 200}, "cvsl_pt1");
  auto h_bb_cvsl_pt2 = df_S1_bb.Histo1D({"h_bb_cvsl_pt2", "h_bb_cvsl_pt2", 10, 0, 200}, "cvsl_pt2");
  auto h_bb_cvsl_pt3 = df_S1_bb.Histo1D({"h_bb_cvsl_pt3", "h_bb_cvsl_pt3", 10, 0, 200}, "cvsl_pt3");
  auto h_bb_cvsl_pt4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_cvsl_pt4", "h_bb_cvsl_pt4", 10, 0, 200}, "cvsl_pt4");
  auto h_bb_cvsl_mass1 = df_S1_bb.Histo1D({"h_bb_cvsl_mass1", "h_bb_cvsl_mass1", 25, 0, 25}, "cvsl_mass1");
  auto h_bb_cvsl_mass2 = df_S1_bb.Histo1D({"h_bb_cvsl_mass2", "h_bb_cvsl_mass2", 25, 0, 25}, "cvsl_mass2");
  auto h_bb_cvsl_mass3 = df_S1_bb.Histo1D({"h_bb_cvsl_mass3", "h_bb_cvsl_mass3", 25, 0, 25}, "cvsl_mass3");
  auto h_bb_cvsl_mass4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_cvsl_mass4", "h_bb_cvsl_mass4", 25, 0, 25}, "cvsl_mass4");
  auto h_bb_cvsl_eta1 = df_S1_bb.Histo1D({"h_bb_cvsl_eta1", "h_bb_cvsl_eta1", 45, -3, 3}, "cvsl_eta1");
  auto h_bb_cvsl_eta2 = df_S1_bb.Histo1D({"h_bb_cvsl_eta2", "h_bb_cvsl_eta2", 45, -3, 3}, "cvsl_eta2");
  auto h_bb_cvsl_eta3 = df_S1_bb.Histo1D({"h_bb_cvsl_eta3", "h_bb_cvsl_eta3", 45, -3, 3}, "cvsl_eta3");
  auto h_bb_cvsl_eta4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_cvsl_eta4", "h_bb_cvsl_eta4", 45, -3, 3}, "cvsl_eta4");
  auto h_bb_cvsl_phi1 = df_S1_bb.Histo1D({"h_bb_cvsl_phi1", "h_bb_cvsl_phi1", 100, -4, 4}, "cvsl_phi1");
  auto h_bb_cvsl_phi2 = df_S1_bb.Histo1D({"h_bb_cvsl_phi2", "h_bb_cvsl_phi2", 100, -4, 4}, "cvsl_phi2");
  auto h_bb_cvsl_phi3 = df_S1_bb.Histo1D({"h_bb_cvsl_phi3", "h_bb_cvsl_phi3", 100, -4, 4}, "cvsl_phi3");
  auto h_bb_cvsl_phi4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_cvsl_phi4", "h_bb_cvsl_phi4", 100, -4, 4}, "cvsl_phi4");
  auto h_bb_cvsl_CvsL1 = df_S1_bb.Histo1D({"h_bb_cvsl_CvsL1", "h_bb_cvsl_CvsL1", 44, 0, 1.2}, "cvsl_CvsL1");
  auto h_bb_cvsl_CvsL2 = df_S1_bb.Histo1D({"h_bb_cvsl_CvsL2", "h_bb_cvsl_CvsL2", 44, 0, 1.2}, "cvsl_CvsL2");
  auto h_bb_cvsl_CvsL3 = df_S1_bb.Histo1D({"h_bb_cvsl_CvsL3", "h_bb_cvsl_CvsL3", 44, 0, 1.2}, "cvsl_CvsL3");
  auto h_bb_cvsl_CvsL4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_cvsl_CvsL4", "h_bb_cvsl_CvsL4", 44, 0, 1.2}, "cvsl_CvsL4");
  auto h_bb_cvsl_CvsB1 = df_S1_bb.Histo1D({"h_bb_cvsl_CvsB1", "h_bb_cvsl_CvsB1", 44, 0, 1.2}, "cvsl_CvsB1");
  auto h_bb_cvsl_CvsB2 = df_S1_bb.Histo1D({"h_bb_cvsl_CvsB2", "h_bb_cvsl_CvsB2", 44, 0, 1.2}, "cvsl_CvsB2");
  auto h_bb_cvsl_CvsB3 = df_S1_bb.Histo1D({"h_bb_cvsl_CvsB3", "h_bb_cvsl_CvsB3", 44, 0, 1.2}, "cvsl_CvsB3");
  auto h_bb_cvsl_CvsB4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_cvsl_CvsB4", "h_bb_cvsl_CvsB4", 44, 0, 1.2}, "cvsl_CvsB4");
  auto h_bb_cvsl_Ctag1 = df_S1_bb.Histo1D({"h_bb_cvsl_Ctag1", "h_bb_cvsl_Ctag1", 44, 0, 1.2}, "cvsl_Ctag1");
  auto h_bb_cvsl_Ctag2 = df_S1_bb.Histo1D({"h_bb_cvsl_Ctag2", "h_bb_cvsl_Ctag2", 44, 0, 1.2}, "cvsl_Ctag2");
  auto h_bb_cvsl_Ctag3 = df_S1_bb.Histo1D({"h_bb_cvsl_Ctag3", "h_bb_cvsl_Ctag3", 44, 0, 1.2}, "cvsl_Ctag3");
  auto h_bb_cvsl_Ctag4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_cvsl_Ctag4", "h_bb_cvsl_Ctag4", 44, 0, 1.2}, "cvsl_Ctag4");
  auto h_bb_cvsb_pt1 = df_S1_bb.Histo1D({"h_bb_cvsb_pt1", "h_bb_cvsb_pt1", 10, 0, 200}, "cvsb_pt1");
  auto h_bb_cvsb_pt2 = df_S1_bb.Histo1D({"h_bb_cvsb_pt2", "h_bb_cvsb_pt2", 10, 0, 200}, "cvsb_pt2");
  auto h_bb_cvsb_pt3 = df_S1_bb.Histo1D({"h_bb_cvsb_pt3", "h_bb_cvsb_pt3", 10, 0, 200}, "cvsb_pt3");
  auto h_bb_cvsb_pt4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_cvsb_pt4", "h_bb_cvsb_pt4", 10, 0, 200}, "cvsb_pt4");
  auto h_bb_cvsb_mass1 = df_S1_bb.Histo1D({"h_bb_cvsb_mass1", "h_bb_cvsb_mass1", 25, 0, 25}, "cvsb_mass1");
  auto h_bb_cvsb_mass2 = df_S1_bb.Histo1D({"h_bb_cvsb_mass2", "h_bb_cvsb_mass2", 25, 0, 25}, "cvsb_mass2");
  auto h_bb_cvsb_mass3 = df_S1_bb.Histo1D({"h_bb_cvsb_mass3", "h_bb_cvsb_mass3", 25, 0, 25}, "cvsb_mass3");
  auto h_bb_cvsb_mass4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_cvsb_mass4", "h_bb_cvsb_mass4", 25, 0, 25}, "cvsb_mass4");
  auto h_bb_cvsb_eta1 = df_S1_bb.Histo1D({"h_bb_cvsb_eta1", "h_bb_cvsb_eta1", 45, -3, 3}, "cvsb_eta1");
  auto h_bb_cvsb_eta2 = df_S1_bb.Histo1D({"h_bb_cvsb_eta2", "h_bb_cvsb_eta2", 45, -3, 3}, "cvsb_eta2");
  auto h_bb_cvsb_eta3 = df_S1_bb.Histo1D({"h_bb_cvsb_eta3", "h_bb_cvsb_eta3", 45, -3, 3}, "cvsb_eta3");
  auto h_bb_cvsb_eta4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_cvsb_eta4", "h_bb_cvsb_eta4", 45, -3, 3}, "cvsb_eta4");
  auto h_bb_cvsb_phi1 = df_S1_bb.Histo1D({"h_bb_cvsb_phi1", "h_bb_cvsb_phi1", 100, -4, 4}, "cvsb_phi1");
  auto h_bb_cvsb_phi2 = df_S1_bb.Histo1D({"h_bb_cvsb_phi2", "h_bb_cvsb_phi2", 100, -4, 4}, "cvsb_phi2");
  auto h_bb_cvsb_phi3 = df_S1_bb.Histo1D({"h_bb_cvsb_phi3", "h_bb_cvsb_phi3", 100, -4, 4}, "cvsb_phi3");
  auto h_bb_cvsb_phi4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_cvsb_phi4", "h_bb_cvsb_phi4", 100, -4, 4}, "cvsb_phi4");
  auto h_bb_cvsb_CvsL1 = df_S1_bb.Histo1D({"h_bb_cvsb_CvsL1", "h_bb_cvsb_CvsL1", 44, 0, 1.2}, "cvsb_CvsL1");
  auto h_bb_cvsb_CvsL2 = df_S1_bb.Histo1D({"h_bb_cvsb_CvsL2", "h_bb_cvsb_CvsL2", 44, 0, 1.2}, "cvsb_CvsL2");
  auto h_bb_cvsb_CvsL3 = df_S1_bb.Histo1D({"h_bb_cvsb_CvsL3", "h_bb_cvsb_CvsL3", 44, 0, 1.2}, "cvsb_CvsL3");
  auto h_bb_cvsb_CvsL4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_cvsb_CvsL4", "h_bb_cvsb_CvsL4", 44, 0, 1.2}, "cvsb_CvsL4");
  auto h_bb_cvsb_CvsB1 = df_S1_bb.Histo1D({"h_bb_cvsb_CvsB1", "h_bb_cvsb_CvsB1", 44, 0, 1.2}, "cvsb_CvsB1");
  auto h_bb_cvsb_CvsB2 = df_S1_bb.Histo1D({"h_bb_cvsb_CvsB2", "h_bb_cvsb_CvsB2", 44, 0, 1.2}, "cvsb_CvsB2");
  auto h_bb_cvsb_CvsB3 = df_S1_bb.Histo1D({"h_bb_cvsb_CvsB3", "h_bb_cvsb_CvsB3", 44, 0, 1.2}, "cvsb_CvsB3");
  auto h_bb_cvsb_CvsB4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_cvsb_CvsB4", "h_bb_cvsb_CvsB4", 44, 0, 1.2}, "cvsb_CvsB4");
  auto h_bb_cvsb_Ctag1 = df_S1_bb.Histo1D({"h_bb_cvsb_Ctag1", "h_bb_cvsb_Ctag1", 44, 0, 1.2}, "cvsb_Ctag1");
  auto h_bb_cvsb_Ctag2 = df_S1_bb.Histo1D({"h_bb_cvsb_Ctag2", "h_bb_cvsb_Ctag2", 44, 0, 1.2}, "cvsb_Ctag2");
  auto h_bb_cvsb_Ctag3 = df_S1_bb.Histo1D({"h_bb_cvsb_Ctag3", "h_bb_cvsb_Ctag3", 44, 0, 1.2}, "cvsb_Ctag3");
  auto h_bb_cvsb_Ctag4 = df_S1_bb.Filter("ngoodjets >= 4").Histo1D({"h_bb_cvsb_Ctag4", "h_bb_cvsb_Ctag4", 44, 0, 1.2}, "cvsb_Ctag4");

  auto h_c_nbjets_m = df_S1_c.Histo1D({"h_c_nbjets_m", "h_c_nbjets_m", 6, 0, 6}, "nbjets_m");
  auto h_c_nbjets_t = df_S1_c.Histo1D({"h_c_nbjets_t", "h_c_nbjets_t", 6, 0, 6}, "nbjets_t");
  auto h_c_ncjets_l = df_S1_c.Histo1D({"h_c_ncjets_l", "h_c_ncjets_l", 10, 0, 10}, "ncjets_l");
  auto h_c_ncjets_m = df_S1_c.Histo1D({"h_c_ncjets_m", "h_c_ncjets_m", 8, 0, 8}, "ncjets_m");
  auto h_c_ncjets_t = df_S1_c.Histo1D({"h_c_ncjets_t", "h_c_ncjets_t", 6, 0, 6}, "ncjets_t");
  auto h_c_ntaujets = df_S1_goodmuon.Filter("category == 3").Histo1D({"h_c_ntaujets", "h_c_ntaujets", 5, 0, 5}, "ntaujets");
  auto h_c_jet_pt = df_S1_c.Histo1D({"h_c_jet_pt", "h_c_jet_pt", 12, 0, 240}, "jet_pt");
  auto h_c_bjet_m_pt = df_S1_c.Filter("nbjets_m >= 1").Histo1D({"h_c_bjet_m_pt", "h_c_bjet_m_pt", 12, 0, 240}, "bjet_m_pt");
  auto h_c_bjet_t_pt = df_S1_c.Filter("nbjets_t >= 1").Histo1D({"h_c_bjet_t_pt", "h_c_bjet_t_pt", 12, 0, 240}, "bjet_t_pt");
  auto h_c_cjet_l_pt = df_S1_c.Filter("ncjets_l >= 1").Histo1D({"h_c_cjet_l_pt", "h_c_cjet_l_pt", 12, 0, 240}, "cjet_l_pt");
  auto h_c_cjet_m_pt = df_S1_c.Filter("ncjets_m >= 1").Histo1D({"h_c_cjet_m_pt", "h_c_cjet_m_pt", 12, 0, 240}, "cjet_m_pt");
  auto h_c_cjet_t_pt = df_S1_c.Filter("ncjets_t >= 1").Histo1D({"h_c_cjet_t_pt", "h_c_cjet_t_pt", 12, 0, 240}, "cjet_t_pt");
  auto h_c_bsortjet_btag1 = df_S1_c.Histo1D({"h_c_bsortjet_btag1", "h_c_bsortjet_btag1", 44, 0, 1.1}, "bsortjet_btag1");
  auto h_c_bsortjet_btag2 = df_S1_c.Histo1D({"h_c_bsortjet_btag2", "h_c_bsortjet_btag2", 44, 0, 1.1}, "bsortjet_btag2");
  auto h_c_bsortjet_btag3 = df_S1_c.Histo1D({"h_c_bsortjet_btag3", "h_c_bsortjet_btag3", 44, 0, 1.1}, "bsortjet_btag3");
  auto h_c_bsortjet_btag4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_bsortjet_btag4", "h_c_bsortjet_btag4", 44, 0, 1.1}, "bsortjet_btag4");
  auto h_c_bsortjet_pt1 = df_S1_c.Histo1D({"h_c_bsortjet_pt1", "h_c_bsortjet_pt1", 10, 0, 200}, "bsortjet_pt1");
  auto h_c_bsortjet_pt2 = df_S1_c.Histo1D({"h_c_bsortjet_pt2", "h_c_bsortjet_pt2", 10, 0, 200}, "bsortjet_pt2");
  auto h_c_bsortjet_pt3 = df_S1_c.Histo1D({"h_c_bsortjet_pt3", "h_c_bsortjet_pt3", 10, 0, 200}, "bsortjet_pt3");
  auto h_c_bsortjet_pt4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_bsortjet_pt4", "h_c_bsortjet_pt4", 10, 0, 200}, "bsortjet_pt4");
  auto h_c_bsortjet_mass1 = df_S1_c.Histo1D({"h_c_bsortjet_mass1", "h_c_bsortjet_mass1", 25, 0, 25}, "bsortjet_mass1");
  auto h_c_bsortjet_mass2 = df_S1_c.Histo1D({"h_c_bsortjet_mass2", "h_c_bsortjet_mass2", 25, 0, 25}, "bsortjet_mass2");
  auto h_c_bsortjet_mass3 = df_S1_c.Histo1D({"h_c_bsortjet_mass3", "h_c_bsortjet_mass3", 25, 0, 25}, "bsortjet_mass3");
  auto h_c_bsortjet_mass4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_bsortjet_mass4", "h_c_bsortjet_mass4", 25, 0, 25}, "bsortjet_mass4");
  auto h_c_ctag_pt1 = df_S1_c.Histo1D({"h_c_ctag_pt1", "h_c_ctag_pt1", 10, 0, 200}, "ctag_pt1");
  auto h_c_ctag_pt2 = df_S1_c.Histo1D({"h_c_ctag_pt2", "h_c_ctag_pt2", 10, 0, 200}, "ctag_pt2");
  auto h_c_ctag_pt3 = df_S1_c.Histo1D({"h_c_ctag_pt3", "h_c_ctag_pt3", 10, 0, 200}, "ctag_pt3");
  auto h_c_ctag_pt4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_ctag_pt4", "h_c_ctag_pt4", 10, 0, 200}, "ctag_pt4");
  auto h_c_ctag_mass1 = df_S1_c.Histo1D({"h_c_ctag_mass1", "h_c_ctag_mass1", 25, 0, 25}, "ctag_mass1");
  auto h_c_ctag_mass2 = df_S1_c.Histo1D({"h_c_ctag_mass2", "h_c_ctag_mass2", 25, 0, 25}, "ctag_mass2");
  auto h_c_ctag_mass3 = df_S1_c.Histo1D({"h_c_ctag_mass3", "h_c_ctag_mass3", 25, 0, 25}, "ctag_mass3");
  auto h_c_ctag_mass4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_ctag_mass4", "h_c_ctag_mass4", 25, 0, 25}, "ctag_mass4");
  auto h_c_ctag_eta1 = df_S1_c.Histo1D({"h_c_ctag_eta1", "h_c_ctag_eta1", 45, -3, 3}, "ctag_eta1");
  auto h_c_ctag_eta2 = df_S1_c.Histo1D({"h_c_ctag_eta2", "h_c_ctag_eta2", 45, -3, 3}, "ctag_eta2");
  auto h_c_ctag_eta3 = df_S1_c.Histo1D({"h_c_ctag_eta3", "h_c_ctag_eta3", 45, -3, 3}, "ctag_eta3");
  auto h_c_ctag_eta4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_ctag_eta4", "h_c_ctag_eta4", 45, -3, 3}, "ctag_eta4");
  auto h_c_ctag_phi1 = df_S1_c.Histo1D({"h_c_ctag_phi1", "h_c_ctag_phi1", 100, -4, 4}, "ctag_phi1");
  auto h_c_ctag_phi2 = df_S1_c.Histo1D({"h_c_ctag_phi2", "h_c_ctag_phi2", 100, -4, 4}, "ctag_phi2");
  auto h_c_ctag_phi3 = df_S1_c.Histo1D({"h_c_ctag_phi3", "h_c_ctag_phi3", 100, -4, 4}, "ctag_phi3");
  auto h_c_ctag_phi4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_ctag_phi4", "h_c_ctag_phi4", 100, -4, 4}, "ctag_phi4");
  auto h_c_ctag_CvsL1 = df_S1_c.Histo1D({"h_c_ctag_CvsL1", "h_c_ctag_CvsL1", 44, 0, 1.2}, "ctag_CvsL1");
  auto h_c_ctag_CvsL2 = df_S1_c.Histo1D({"h_c_ctag_CvsL2", "h_c_ctag_CvsL2", 44, 0, 1.2}, "ctag_CvsL2");
  auto h_c_ctag_CvsL3 = df_S1_c.Histo1D({"h_c_ctag_CvsL3", "h_c_ctag_CvsL3", 44, 0, 1.2}, "ctag_CvsL3");
  auto h_c_ctag_CvsL4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_ctag_CvsL4", "h_c_ctag_CvsL4", 44, 0, 1.2}, "ctag_CvsL4");
  auto h_c_ctag_CvsB1 = df_S1_c.Histo1D({"h_c_ctag_CvsB1", "h_c_ctag_CvsB1", 44, 0, 1.2}, "ctag_CvsB1");
  auto h_c_ctag_CvsB2 = df_S1_c.Histo1D({"h_c_ctag_CvsB2", "h_c_ctag_CvsB2", 44, 0, 1.2}, "ctag_CvsB2");
  auto h_c_ctag_CvsB3 = df_S1_c.Histo1D({"h_c_ctag_CvsB3", "h_c_ctag_CvsB3", 44, 0, 1.2}, "ctag_CvsB3");
  auto h_c_ctag_CvsB4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_ctag_CvsB4", "h_c_ctag_CvsB4", 44, 0, 1.2}, "ctag_CvsB4");
  auto h_c_ctag_Ctag1 = df_S1_c.Histo1D({"h_c_ctag_Ctag1", "h_c_ctag_Ctag1", 44, 0, 1.2}, "ctag_Ctag1");
  auto h_c_ctag_Ctag2 = df_S1_c.Histo1D({"h_c_ctag_Ctag2", "h_c_ctag_Ctag2", 44, 0, 1.2}, "ctag_Ctag2");
  auto h_c_ctag_Ctag3 = df_S1_c.Histo1D({"h_c_ctag_Ctag3", "h_c_ctag_Ctag3", 44, 0, 1.2}, "ctag_Ctag3");
  auto h_c_ctag_Ctag4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_ctag_Ctag4", "h_c_ctag_Ctag4", 44, 0, 1.2}, "ctag_Ctag4");
  auto h_c_cvsl_pt1 = df_S1_c.Histo1D({"h_c_cvsl_pt1", "h_c_cvsl_pt1", 10, 0, 200}, "cvsl_pt1");
  auto h_c_cvsl_pt2 = df_S1_c.Histo1D({"h_c_cvsl_pt2", "h_c_cvsl_pt2", 10, 0, 200}, "cvsl_pt2");
  auto h_c_cvsl_pt3 = df_S1_c.Histo1D({"h_c_cvsl_pt3", "h_c_cvsl_pt3", 10, 0, 200}, "cvsl_pt3");
  auto h_c_cvsl_pt4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_cvsl_pt4", "h_c_cvsl_pt4", 10, 0, 200}, "cvsl_pt4");
  auto h_c_cvsl_mass1 = df_S1_c.Histo1D({"h_c_cvsl_mass1", "h_c_cvsl_mass1", 25, 0, 25}, "cvsl_mass1");
  auto h_c_cvsl_mass2 = df_S1_c.Histo1D({"h_c_cvsl_mass2", "h_c_cvsl_mass2", 25, 0, 25}, "cvsl_mass2");
  auto h_c_cvsl_mass3 = df_S1_c.Histo1D({"h_c_cvsl_mass3", "h_c_cvsl_mass3", 25, 0, 25}, "cvsl_mass3");
  auto h_c_cvsl_mass4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_cvsl_mass4", "h_c_cvsl_mass4", 25, 0, 25}, "cvsl_mass4");
  auto h_c_cvsl_eta1 = df_S1_c.Histo1D({"h_c_cvsl_eta1", "h_c_cvsl_eta1", 45, -3, 3}, "cvsl_eta1");
  auto h_c_cvsl_eta2 = df_S1_c.Histo1D({"h_c_cvsl_eta2", "h_c_cvsl_eta2", 45, -3, 3}, "cvsl_eta2");
  auto h_c_cvsl_eta3 = df_S1_c.Histo1D({"h_c_cvsl_eta3", "h_c_cvsl_eta3", 45, -3, 3}, "cvsl_eta3");
  auto h_c_cvsl_eta4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_cvsl_eta4", "h_c_cvsl_eta4", 45, -3, 3}, "cvsl_eta4");
  auto h_c_cvsl_phi1 = df_S1_c.Histo1D({"h_c_cvsl_phi1", "h_c_cvsl_phi1", 100, -4, 4}, "cvsl_phi1");
  auto h_c_cvsl_phi2 = df_S1_c.Histo1D({"h_c_cvsl_phi2", "h_c_cvsl_phi2", 100, -4, 4}, "cvsl_phi2");
  auto h_c_cvsl_phi3 = df_S1_c.Histo1D({"h_c_cvsl_phi3", "h_c_cvsl_phi3", 100, -4, 4}, "cvsl_phi3");
  auto h_c_cvsl_phi4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_cvsl_phi4", "h_c_cvsl_phi4", 100, -4, 4}, "cvsl_phi4");
  auto h_c_cvsl_CvsL1 = df_S1_c.Histo1D({"h_c_cvsl_CvsL1", "h_c_cvsl_CvsL1", 44, 0, 1.2}, "cvsl_CvsL1");
  auto h_c_cvsl_CvsL2 = df_S1_c.Histo1D({"h_c_cvsl_CvsL2", "h_c_cvsl_CvsL2", 44, 0, 1.2}, "cvsl_CvsL2");
  auto h_c_cvsl_CvsL3 = df_S1_c.Histo1D({"h_c_cvsl_CvsL3", "h_c_cvsl_CvsL3", 44, 0, 1.2}, "cvsl_CvsL3");
  auto h_c_cvsl_CvsL4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_cvsl_CvsL4", "h_c_cvsl_CvsL4", 44, 0, 1.2}, "cvsl_CvsL4");
  auto h_c_cvsl_CvsB1 = df_S1_c.Histo1D({"h_c_cvsl_CvsB1", "h_c_cvsl_CvsB1", 44, 0, 1.2}, "cvsl_CvsB1");
  auto h_c_cvsl_CvsB2 = df_S1_c.Histo1D({"h_c_cvsl_CvsB2", "h_c_cvsl_CvsB2", 44, 0, 1.2}, "cvsl_CvsB2");
  auto h_c_cvsl_CvsB3 = df_S1_c.Histo1D({"h_c_cvsl_CvsB3", "h_c_cvsl_CvsB3", 44, 0, 1.2}, "cvsl_CvsB3");
  auto h_c_cvsl_CvsB4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_cvsl_CvsB4", "h_c_cvsl_CvsB4", 44, 0, 1.2}, "cvsl_CvsB4");
  auto h_c_cvsl_Ctag1 = df_S1_c.Histo1D({"h_c_cvsl_Ctag1", "h_c_cvsl_Ctag1", 44, 0, 1.2}, "cvsl_Ctag1");
  auto h_c_cvsl_Ctag2 = df_S1_c.Histo1D({"h_c_cvsl_Ctag2", "h_c_cvsl_Ctag2", 44, 0, 1.2}, "cvsl_Ctag2");
  auto h_c_cvsl_Ctag3 = df_S1_c.Histo1D({"h_c_cvsl_Ctag3", "h_c_cvsl_Ctag3", 44, 0, 1.2}, "cvsl_Ctag3");
  auto h_c_cvsl_Ctag4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_cvsl_Ctag4", "h_c_cvsl_Ctag4", 44, 0, 1.2}, "cvsl_Ctag4");
  auto h_c_cvsb_pt1 = df_S1_c.Histo1D({"h_c_cvsb_pt1", "h_c_cvsb_pt1", 10, 0, 200}, "cvsb_pt1");
  auto h_c_cvsb_pt2 = df_S1_c.Histo1D({"h_c_cvsb_pt2", "h_c_cvsb_pt2", 10, 0, 200}, "cvsb_pt2");
  auto h_c_cvsb_pt3 = df_S1_c.Histo1D({"h_c_cvsb_pt3", "h_c_cvsb_pt3", 10, 0, 200}, "cvsb_pt3");
  auto h_c_cvsb_pt4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_cvsb_pt4", "h_c_cvsb_pt4", 10, 0, 200}, "cvsb_pt4");
  auto h_c_cvsb_mass1 = df_S1_c.Histo1D({"h_c_cvsb_mass1", "h_c_cvsb_mass1", 25, 0, 25}, "cvsb_mass1");
  auto h_c_cvsb_mass2 = df_S1_c.Histo1D({"h_c_cvsb_mass2", "h_c_cvsb_mass2", 25, 0, 25}, "cvsb_mass2");
  auto h_c_cvsb_mass3 = df_S1_c.Histo1D({"h_c_cvsb_mass3", "h_c_cvsb_mass3", 25, 0, 25}, "cvsb_mass3");
  auto h_c_cvsb_mass4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_cvsb_mass4", "h_c_cvsb_mass4", 25, 0, 25}, "cvsb_mass4");
  auto h_c_cvsb_eta1 = df_S1_c.Histo1D({"h_c_cvsb_eta1", "h_c_cvsb_eta1", 45, -3, 3}, "cvsb_eta1");
  auto h_c_cvsb_eta2 = df_S1_c.Histo1D({"h_c_cvsb_eta2", "h_c_cvsb_eta2", 45, -3, 3}, "cvsb_eta2");
  auto h_c_cvsb_eta3 = df_S1_c.Histo1D({"h_c_cvsb_eta3", "h_c_cvsb_eta3", 45, -3, 3}, "cvsb_eta3");
  auto h_c_cvsb_eta4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_cvsb_eta4", "h_c_cvsb_eta4", 45, -3, 3}, "cvsb_eta4");
  auto h_c_cvsb_phi1 = df_S1_c.Histo1D({"h_c_cvsb_phi1", "h_c_cvsb_phi1", 100, -4, 4}, "cvsb_phi1");
  auto h_c_cvsb_phi2 = df_S1_c.Histo1D({"h_c_cvsb_phi2", "h_c_cvsb_phi2", 100, -4, 4}, "cvsb_phi2");
  auto h_c_cvsb_phi3 = df_S1_c.Histo1D({"h_c_cvsb_phi3", "h_c_cvsb_phi3", 100, -4, 4}, "cvsb_phi3");
  auto h_c_cvsb_phi4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_cvsb_phi4", "h_c_cvsb_phi4", 100, -4, 4}, "cvsb_phi4");
  auto h_c_cvsb_CvsL1 = df_S1_c.Histo1D({"h_c_cvsb_CvsL1", "h_c_cvsb_CvsL1", 44, 0, 1.2}, "cvsb_CvsL1");
  auto h_c_cvsb_CvsL2 = df_S1_c.Histo1D({"h_c_cvsb_CvsL2", "h_c_cvsb_CvsL2", 44, 0, 1.2}, "cvsb_CvsL2");
  auto h_c_cvsb_CvsL3 = df_S1_c.Histo1D({"h_c_cvsb_CvsL3", "h_c_cvsb_CvsL3", 44, 0, 1.2}, "cvsb_CvsL3");
  auto h_c_cvsb_CvsL4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_cvsb_CvsL4", "h_c_cvsb_CvsL4", 44, 0, 1.2}, "cvsb_CvsL4");
  auto h_c_cvsb_CvsB1 = df_S1_c.Histo1D({"h_c_cvsb_CvsB1", "h_c_cvsb_CvsB1", 44, 0, 1.2}, "cvsb_CvsB1");
  auto h_c_cvsb_CvsB2 = df_S1_c.Histo1D({"h_c_cvsb_CvsB2", "h_c_cvsb_CvsB2", 44, 0, 1.2}, "cvsb_CvsB2");
  auto h_c_cvsb_CvsB3 = df_S1_c.Histo1D({"h_c_cvsb_CvsB3", "h_c_cvsb_CvsB3", 44, 0, 1.2}, "cvsb_CvsB3");
  auto h_c_cvsb_CvsB4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_cvsb_CvsB4", "h_c_cvsb_CvsB4", 44, 0, 1.2}, "cvsb_CvsB4");
  auto h_c_cvsb_Ctag1 = df_S1_c.Histo1D({"h_c_cvsb_Ctag1", "h_c_cvsb_Ctag1", 44, 0, 1.2}, "cvsb_Ctag1");
  auto h_c_cvsb_Ctag2 = df_S1_c.Histo1D({"h_c_cvsb_Ctag2", "h_c_cvsb_Ctag2", 44, 0, 1.2}, "cvsb_Ctag2");
  auto h_c_cvsb_Ctag3 = df_S1_c.Histo1D({"h_c_cvsb_Ctag3", "h_c_cvsb_Ctag3", 44, 0, 1.2}, "cvsb_Ctag3");
  auto h_c_cvsb_Ctag4 = df_S1_c.Filter("ngoodjets >= 4").Histo1D({"h_c_cvsb_Ctag4", "h_c_cvsb_Ctag4", 44, 0, 1.2}, "cvsb_Ctag4");

  auto h_cc_nbjets_m = df_S1_cc.Histo1D({"h_cc_nbjets_m", "h_cc_nbjets_m", 6, 0, 6}, "nbjets_m");
  auto h_cc_nbjets_t = df_S1_cc.Histo1D({"h_cc_nbjets_t", "h_cc_nbjets_t", 6, 0, 6}, "nbjets_t");
  auto h_cc_ncjets_l = df_S1_cc.Histo1D({"h_cc_ncjets_l", "h_cc_ncjets_l", 10, 0, 10}, "ncjets_l");
  auto h_cc_ncjets_m = df_S1_cc.Histo1D({"h_cc_ncjets_m", "h_cc_ncjets_m", 8, 0, 8}, "ncjets_m");
  auto h_cc_ncjets_t = df_S1_cc.Histo1D({"h_cc_ncjets_t", "h_cc_ncjets_t", 6, 0, 6}, "ncjets_t");
  auto h_cc_ntaujets = df_S1_goodmuon.Filter("category == 4").Histo1D({"h_cc_ntaujets", "h_cc_ntaujets", 5, 0, 5}, "ntaujets");
  auto h_cc_jet_pt = df_S1_cc.Histo1D({"h_cc_jet_pt", "h_cc_jet_pt", 12, 0, 240}, "jet_pt");
  auto h_cc_bjet_m_pt = df_S1_cc.Filter("nbjets_m >= 1").Histo1D({"h_cc_bjet_m_pt", "h_cc_bjet_m_pt", 12, 0, 240}, "bjet_m_pt");
  auto h_cc_bjet_t_pt = df_S1_cc.Filter("nbjets_t >= 1").Histo1D({"h_cc_bjet_t_pt", "h_cc_bjet_t_pt", 12, 0, 240}, "bjet_t_pt");
  auto h_cc_cjet_l_pt = df_S1_cc.Filter("ncjets_l >= 1").Histo1D({"h_cc_cjet_l_pt", "h_cc_cjet_l_pt", 12, 0, 240}, "cjet_l_pt");
  auto h_cc_cjet_m_pt = df_S1_cc.Filter("ncjets_m >= 1").Histo1D({"h_cc_cjet_m_pt", "h_cc_cjet_m_pt", 12, 0, 240}, "cjet_m_pt");
  auto h_cc_cjet_t_pt = df_S1_cc.Filter("ncjets_t >= 1").Histo1D({"h_cc_cjet_t_pt", "h_cc_cjet_t_pt", 12, 0, 240}, "cjet_t_pt");
  auto h_cc_bsortjet_btag1 = df_S1_cc.Histo1D({"h_cc_bsortjet_btag1", "h_cc_bsortjet_btag1", 44, 0, 1.1}, "bsortjet_btag1");
  auto h_cc_bsortjet_btag2 = df_S1_cc.Histo1D({"h_cc_bsortjet_btag2", "h_cc_bsortjet_btag2", 44, 0, 1.1}, "bsortjet_btag2");
  auto h_cc_bsortjet_btag3 = df_S1_cc.Histo1D({"h_cc_bsortjet_btag3", "h_cc_bsortjet_btag3", 44, 0, 1.1}, "bsortjet_btag3");
  auto h_cc_bsortjet_btag4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_bsortjet_btag4", "h_cc_bsortjet_btag4", 44, 0, 1.1}, "bsortjet_btag4");
  auto h_cc_bsortjet_pt1 = df_S1_cc.Histo1D({"h_cc_bsortjet_pt1", "h_cc_bsortjet_pt1", 10, 0, 200}, "bsortjet_pt1");
  auto h_cc_bsortjet_pt2 = df_S1_cc.Histo1D({"h_cc_bsortjet_pt2", "h_cc_bsortjet_pt2", 10, 0, 200}, "bsortjet_pt2");
  auto h_cc_bsortjet_pt3 = df_S1_cc.Histo1D({"h_cc_bsortjet_pt3", "h_cc_bsortjet_pt3", 10, 0, 200}, "bsortjet_pt3");
  auto h_cc_bsortjet_pt4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_bsortjet_pt4", "h_cc_bsortjet_pt4", 10, 0, 200}, "bsortjet_pt4");
  auto h_cc_bsortjet_mass1 = df_S1_cc.Histo1D({"h_cc_bsortjet_mass1", "h_cc_bsortjet_mass1", 25, 0, 25}, "bsortjet_mass1");
  auto h_cc_bsortjet_mass2 = df_S1_cc.Histo1D({"h_cc_bsortjet_mass2", "h_cc_bsortjet_mass2", 25, 0, 25}, "bsortjet_mass2");
  auto h_cc_bsortjet_mass3 = df_S1_cc.Histo1D({"h_cc_bsortjet_mass3", "h_cc_bsortjet_mass3", 25, 0, 25}, "bsortjet_mass3");
  auto h_cc_bsortjet_mass4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_bsortjet_mass4", "h_cc_bsortjet_mass4", 25, 0, 25}, "bsortjet_mass4");
  auto h_cc_ctag_pt1 = df_S1_cc.Histo1D({"h_cc_ctag_pt1", "h_cc_ctag_pt1", 10, 0, 200}, "ctag_pt1");
  auto h_cc_ctag_pt2 = df_S1_cc.Histo1D({"h_cc_ctag_pt2", "h_cc_ctag_pt2", 10, 0, 200}, "ctag_pt2");
  auto h_cc_ctag_pt3 = df_S1_cc.Histo1D({"h_cc_ctag_pt3", "h_cc_ctag_pt3", 10, 0, 200}, "ctag_pt3");
  auto h_cc_ctag_pt4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_ctag_pt4", "h_cc_ctag_pt4", 10, 0, 200}, "ctag_pt4");
  auto h_cc_ctag_mass1 = df_S1_cc.Histo1D({"h_cc_ctag_mass1", "h_cc_ctag_mass1", 25, 0, 25}, "ctag_mass1");
  auto h_cc_ctag_mass2 = df_S1_cc.Histo1D({"h_cc_ctag_mass2", "h_cc_ctag_mass2", 25, 0, 25}, "ctag_mass2");
  auto h_cc_ctag_mass3 = df_S1_cc.Histo1D({"h_cc_ctag_mass3", "h_cc_ctag_mass3", 25, 0, 25}, "ctag_mass3");
  auto h_cc_ctag_mass4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_ctag_mass4", "h_cc_ctag_mass4", 25, 0, 25}, "ctag_mass4");
  auto h_cc_ctag_eta1 = df_S1_cc.Histo1D({"h_cc_ctag_eta1", "h_cc_ctag_eta1", 45, -3, 3}, "ctag_eta1");
  auto h_cc_ctag_eta2 = df_S1_cc.Histo1D({"h_cc_ctag_eta2", "h_cc_ctag_eta2", 45, -3, 3}, "ctag_eta2");
  auto h_cc_ctag_eta3 = df_S1_cc.Histo1D({"h_cc_ctag_eta3", "h_cc_ctag_eta3", 45, -3, 3}, "ctag_eta3");
  auto h_cc_ctag_eta4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_ctag_eta4", "h_cc_ctag_eta4", 45, -3, 3}, "ctag_eta4");
  auto h_cc_ctag_phi1 = df_S1_cc.Histo1D({"h_cc_ctag_phi1", "h_cc_ctag_phi1", 100, -4, 4}, "ctag_phi1");
  auto h_cc_ctag_phi2 = df_S1_cc.Histo1D({"h_cc_ctag_phi2", "h_cc_ctag_phi2", 100, -4, 4}, "ctag_phi2");
  auto h_cc_ctag_phi3 = df_S1_cc.Histo1D({"h_cc_ctag_phi3", "h_cc_ctag_phi3", 100, -4, 4}, "ctag_phi3");
  auto h_cc_ctag_phi4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_ctag_phi4", "h_cc_ctag_phi4", 100, -4, 4}, "ctag_phi4");
  auto h_cc_ctag_CvsL1 = df_S1_cc.Histo1D({"h_cc_ctag_CvsL1", "h_cc_ctag_CvsL1", 44, 0, 1.2}, "ctag_CvsL1");
  auto h_cc_ctag_CvsL2 = df_S1_cc.Histo1D({"h_cc_ctag_CvsL2", "h_cc_ctag_CvsL2", 44, 0, 1.2}, "ctag_CvsL2");
  auto h_cc_ctag_CvsL3 = df_S1_cc.Histo1D({"h_cc_ctag_CvsL3", "h_cc_ctag_CvsL3", 44, 0, 1.2}, "ctag_CvsL3");
  auto h_cc_ctag_CvsL4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_ctag_CvsL4", "h_cc_ctag_CvsL4", 44, 0, 1.2}, "ctag_CvsL4");
  auto h_cc_ctag_CvsB1 = df_S1_cc.Histo1D({"h_cc_ctag_CvsB1", "h_cc_ctag_CvsB1", 44, 0, 1.2}, "ctag_CvsB1");
  auto h_cc_ctag_CvsB2 = df_S1_cc.Histo1D({"h_cc_ctag_CvsB2", "h_cc_ctag_CvsB2", 44, 0, 1.2}, "ctag_CvsB2");
  auto h_cc_ctag_CvsB3 = df_S1_cc.Histo1D({"h_cc_ctag_CvsB3", "h_cc_ctag_CvsB3", 44, 0, 1.2}, "ctag_CvsB3");
  auto h_cc_ctag_CvsB4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_ctag_CvsB4", "h_cc_ctag_CvsB4", 44, 0, 1.2}, "ctag_CvsB4");
  auto h_cc_ctag_Ctag1 = df_S1_cc.Histo1D({"h_cc_ctag_Ctag1", "h_cc_ctag_Ctag1", 44, 0, 1.2}, "ctag_Ctag1");
  auto h_cc_ctag_Ctag2 = df_S1_cc.Histo1D({"h_cc_ctag_Ctag2", "h_cc_ctag_Ctag2", 44, 0, 1.2}, "ctag_Ctag2");
  auto h_cc_ctag_Ctag3 = df_S1_cc.Histo1D({"h_cc_ctag_Ctag3", "h_cc_ctag_Ctag3", 44, 0, 1.2}, "ctag_Ctag3");
  auto h_cc_ctag_Ctag4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_ctag_Ctag4", "h_cc_ctag_Ctag4", 44, 0, 1.2}, "ctag_Ctag4");
  auto h_cc_cvsl_pt1 = df_S1_cc.Histo1D({"h_cc_cvsl_pt1", "h_cc_cvsl_pt1", 10, 0, 200}, "cvsl_pt1");
  auto h_cc_cvsl_pt2 = df_S1_cc.Histo1D({"h_cc_cvsl_pt2", "h_cc_cvsl_pt2", 10, 0, 200}, "cvsl_pt2");
  auto h_cc_cvsl_pt3 = df_S1_cc.Histo1D({"h_cc_cvsl_pt3", "h_cc_cvsl_pt3", 10, 0, 200}, "cvsl_pt3");
  auto h_cc_cvsl_pt4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_cvsl_pt4", "h_cc_cvsl_pt4", 10, 0, 200}, "cvsl_pt4");
  auto h_cc_cvsl_mass1 = df_S1_cc.Histo1D({"h_cc_cvsl_mass1", "h_cc_cvsl_mass1", 25, 0, 25}, "cvsl_mass1");
  auto h_cc_cvsl_mass2 = df_S1_cc.Histo1D({"h_cc_cvsl_mass2", "h_cc_cvsl_mass2", 25, 0, 25}, "cvsl_mass2");
  auto h_cc_cvsl_mass3 = df_S1_cc.Histo1D({"h_cc_cvsl_mass3", "h_cc_cvsl_mass3", 25, 0, 25}, "cvsl_mass3");
  auto h_cc_cvsl_mass4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_cvsl_mass4", "h_cc_cvsl_mass4", 25, 0, 25}, "cvsl_mass4");
  auto h_cc_cvsl_eta1 = df_S1_cc.Histo1D({"h_cc_cvsl_eta1", "h_cc_cvsl_eta1", 45, -3, 3}, "cvsl_eta1");
  auto h_cc_cvsl_eta2 = df_S1_cc.Histo1D({"h_cc_cvsl_eta2", "h_cc_cvsl_eta2", 45, -3, 3}, "cvsl_eta2");
  auto h_cc_cvsl_eta3 = df_S1_cc.Histo1D({"h_cc_cvsl_eta3", "h_cc_cvsl_eta3", 45, -3, 3}, "cvsl_eta3");
  auto h_cc_cvsl_eta4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_cvsl_eta4", "h_cc_cvsl_eta4", 45, -3, 3}, "cvsl_eta4");
  auto h_cc_cvsl_phi1 = df_S1_cc.Histo1D({"h_cc_cvsl_phi1", "h_cc_cvsl_phi1", 100, -4, 4}, "cvsl_phi1");
  auto h_cc_cvsl_phi2 = df_S1_cc.Histo1D({"h_cc_cvsl_phi2", "h_cc_cvsl_phi2", 100, -4, 4}, "cvsl_phi2");
  auto h_cc_cvsl_phi3 = df_S1_cc.Histo1D({"h_cc_cvsl_phi3", "h_cc_cvsl_phi3", 100, -4, 4}, "cvsl_phi3");
  auto h_cc_cvsl_phi4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_cvsl_phi4", "h_cc_cvsl_phi4", 100, -4, 4}, "cvsl_phi4");
  auto h_cc_cvsl_CvsL1 = df_S1_cc.Histo1D({"h_cc_cvsl_CvsL1", "h_cc_cvsl_CvsL1", 44, 0, 1.2}, "cvsl_CvsL1");
  auto h_cc_cvsl_CvsL2 = df_S1_cc.Histo1D({"h_cc_cvsl_CvsL2", "h_cc_cvsl_CvsL2", 44, 0, 1.2}, "cvsl_CvsL2");
  auto h_cc_cvsl_CvsL3 = df_S1_cc.Histo1D({"h_cc_cvsl_CvsL3", "h_cc_cvsl_CvsL3", 44, 0, 1.2}, "cvsl_CvsL3");
  auto h_cc_cvsl_CvsL4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_cvsl_CvsL4", "h_cc_cvsl_CvsL4", 44, 0, 1.2}, "cvsl_CvsL4");
  auto h_cc_cvsl_CvsB1 = df_S1_cc.Histo1D({"h_cc_cvsl_CvsB1", "h_cc_cvsl_CvsB1", 44, 0, 1.2}, "cvsl_CvsB1");
  auto h_cc_cvsl_CvsB2 = df_S1_cc.Histo1D({"h_cc_cvsl_CvsB2", "h_cc_cvsl_CvsB2", 44, 0, 1.2}, "cvsl_CvsB2");
  auto h_cc_cvsl_CvsB3 = df_S1_cc.Histo1D({"h_cc_cvsl_CvsB3", "h_cc_cvsl_CvsB3", 44, 0, 1.2}, "cvsl_CvsB3");
  auto h_cc_cvsl_CvsB4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_cvsl_CvsB4", "h_cc_cvsl_CvsB4", 44, 0, 1.2}, "cvsl_CvsB4");
  auto h_cc_cvsl_Ctag1 = df_S1_cc.Histo1D({"h_cc_cvsl_Ctag1", "h_cc_cvsl_Ctag1", 44, 0, 1.2}, "cvsl_Ctag1");
  auto h_cc_cvsl_Ctag2 = df_S1_cc.Histo1D({"h_cc_cvsl_Ctag2", "h_cc_cvsl_Ctag2", 44, 0, 1.2}, "cvsl_Ctag2");
  auto h_cc_cvsl_Ctag3 = df_S1_cc.Histo1D({"h_cc_cvsl_Ctag3", "h_cc_cvsl_Ctag3", 44, 0, 1.2}, "cvsl_Ctag3");
  auto h_cc_cvsl_Ctag4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_cvsl_Ctag4", "h_cc_cvsl_Ctag4", 44, 0, 1.2}, "cvsl_Ctag4");
  auto h_cc_cvsb_pt1 = df_S1_cc.Histo1D({"h_cc_cvsb_pt1", "h_cc_cvsb_pt1", 10, 0, 200}, "cvsb_pt1");
  auto h_cc_cvsb_pt2 = df_S1_cc.Histo1D({"h_cc_cvsb_pt2", "h_cc_cvsb_pt2", 10, 0, 200}, "cvsb_pt2");
  auto h_cc_cvsb_pt3 = df_S1_cc.Histo1D({"h_cc_cvsb_pt3", "h_cc_cvsb_pt3", 10, 0, 200}, "cvsb_pt3");
  auto h_cc_cvsb_pt4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_cvsb_pt4", "h_cc_cvsb_pt4", 10, 0, 200}, "cvsb_pt4");
  auto h_cc_cvsb_mass1 = df_S1_cc.Histo1D({"h_cc_cvsb_mass1", "h_cc_cvsb_mass1", 25, 0, 25}, "cvsb_mass1");
  auto h_cc_cvsb_mass2 = df_S1_cc.Histo1D({"h_cc_cvsb_mass2", "h_cc_cvsb_mass2", 25, 0, 25}, "cvsb_mass2");
  auto h_cc_cvsb_mass3 = df_S1_cc.Histo1D({"h_cc_cvsb_mass3", "h_cc_cvsb_mass3", 25, 0, 25}, "cvsb_mass3");
  auto h_cc_cvsb_mass4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_cvsb_mass4", "h_cc_cvsb_mass4", 25, 0, 25}, "cvsb_mass4");
  auto h_cc_cvsb_eta1 = df_S1_cc.Histo1D({"h_cc_cvsb_eta1", "h_cc_cvsb_eta1", 45, -3, 3}, "cvsb_eta1");
  auto h_cc_cvsb_eta2 = df_S1_cc.Histo1D({"h_cc_cvsb_eta2", "h_cc_cvsb_eta2", 45, -3, 3}, "cvsb_eta2");
  auto h_cc_cvsb_eta3 = df_S1_cc.Histo1D({"h_cc_cvsb_eta3", "h_cc_cvsb_eta3", 45, -3, 3}, "cvsb_eta3");
  auto h_cc_cvsb_eta4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_cvsb_eta4", "h_cc_cvsb_eta4", 45, -3, 3}, "cvsb_eta4");
  auto h_cc_cvsb_phi1 = df_S1_cc.Histo1D({"h_cc_cvsb_phi1", "h_cc_cvsb_phi1", 100, -4, 4}, "cvsb_phi1");
  auto h_cc_cvsb_phi2 = df_S1_cc.Histo1D({"h_cc_cvsb_phi2", "h_cc_cvsb_phi2", 100, -4, 4}, "cvsb_phi2");
  auto h_cc_cvsb_phi3 = df_S1_cc.Histo1D({"h_cc_cvsb_phi3", "h_cc_cvsb_phi3", 100, -4, 4}, "cvsb_phi3");
  auto h_cc_cvsb_phi4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_cvsb_phi4", "h_cc_cvsb_phi4", 100, -4, 4}, "cvsb_phi4");
  auto h_cc_cvsb_CvsL1 = df_S1_cc.Histo1D({"h_cc_cvsb_CvsL1", "h_cc_cvsb_CvsL1", 44, 0, 1.2}, "cvsb_CvsL1");
  auto h_cc_cvsb_CvsL2 = df_S1_cc.Histo1D({"h_cc_cvsb_CvsL2", "h_cc_cvsb_CvsL2", 44, 0, 1.2}, "cvsb_CvsL2");
  auto h_cc_cvsb_CvsL3 = df_S1_cc.Histo1D({"h_cc_cvsb_CvsL3", "h_cc_cvsb_CvsL3", 44, 0, 1.2}, "cvsb_CvsL3");
  auto h_cc_cvsb_CvsL4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_cvsb_CvsL4", "h_cc_cvsb_CvsL4", 44, 0, 1.2}, "cvsb_CvsL4");
  auto h_cc_cvsb_CvsB1 = df_S1_cc.Histo1D({"h_cc_cvsb_CvsB1", "h_cc_cvsb_CvsB1", 44, 0, 1.2}, "cvsb_CvsB1");
  auto h_cc_cvsb_CvsB2 = df_S1_cc.Histo1D({"h_cc_cvsb_CvsB2", "h_cc_cvsb_CvsB2", 44, 0, 1.2}, "cvsb_CvsB2");
  auto h_cc_cvsb_CvsB3 = df_S1_cc.Histo1D({"h_cc_cvsb_CvsB3", "h_cc_cvsb_CvsB3", 44, 0, 1.2}, "cvsb_CvsB3");
  auto h_cc_cvsb_CvsB4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_cvsb_CvsB4", "h_cc_cvsb_CvsB4", 44, 0, 1.2}, "cvsb_CvsB4");
  auto h_cc_cvsb_Ctag1 = df_S1_cc.Histo1D({"h_cc_cvsb_Ctag1", "h_cc_cvsb_Ctag1", 44, 0, 1.2}, "cvsb_Ctag1");
  auto h_cc_cvsb_Ctag2 = df_S1_cc.Histo1D({"h_cc_cvsb_Ctag2", "h_cc_cvsb_Ctag2", 44, 0, 1.2}, "cvsb_Ctag2");
  auto h_cc_cvsb_Ctag3 = df_S1_cc.Histo1D({"h_cc_cvsb_Ctag3", "h_cc_cvsb_Ctag3", 44, 0, 1.2}, "cvsb_Ctag3");
  auto h_cc_cvsb_Ctag4 = df_S1_cc.Filter("ngoodjets >= 4").Histo1D({"h_cc_cvsb_Ctag4", "h_cc_cvsb_Ctag4", 44, 0, 1.2}, "cvsb_Ctag4");

  auto h_sig_nbjets_m = dfs_S2_goodtau.Histo1D({"h_sig_nbjets_m", "h_sig_nbjets_m", 6, 0, 6}, "nbjets_m");
  auto h_sig_nbjets_t = dfs_S2_goodtau.Histo1D({"h_sig_nbjets_t", "h_sig_nbjets_t", 6, 0, 6}, "nbjets_t");
  auto h_sig_ncjets_l = dfs_S2_goodtau.Histo1D({"h_sig_ncjets_l", "h_sig_ncjets_l", 10, 0, 10}, "ncjets_l");
  auto h_sig_ncjets_m = dfs_S2_goodtau.Histo1D({"h_sig_ncjets_m", "h_sig_ncjets_m", 8, 0, 8}, "ncjets_m");
  auto h_sig_ncjets_t = dfs_S2_goodtau.Histo1D({"h_sig_ncjets_t", "h_sig_ncjets_t", 6, 0, 6}, "ncjets_t");
  auto h_sig_ntaujets = dfs_S2_goodmuon.Histo1D({"h_sig_ntaujets", "h_sig_ntaujets", 5, 0, 5}, "ntaujets");
  auto h_sig_jet_pt = dfs_S2_goodtau.Histo1D({"h_sig_jet_pt", "h_sig_jet_pt", 12, 0, 240}, "jet_pt");
  auto h_sig_bjet_m_pt = dfs_S2_goodtau.Filter("nbjets_m >= 1").Histo1D({"h_sig_bjet_m_pt", "h_sig_bjet_m_pt", 12, 0, 240}, "bjet_m_pt");
  auto h_sig_bjet_t_pt = dfs_S2_goodtau.Filter("nbjets_t >= 1").Histo1D({"h_sig_bjet_t_pt", "h_sig_bjet_t_pt", 12, 0, 240}, "bjet_t_pt");
  auto h_sig_cjet_l_pt = dfs_S2_goodtau.Filter("ncjets_l >= 1").Histo1D({"h_sig_cjet_l_pt", "h_sig_cjet_l_pt", 12, 0, 240}, "cjet_l_pt");
  auto h_sig_cjet_m_pt = dfs_S2_goodtau.Filter("ncjets_m >= 1").Histo1D({"h_sig_cjet_m_pt", "h_sig_cjet_m_pt", 12, 0, 240}, "cjet_m_pt");
  auto h_sig_cjet_t_pt = dfs_S2_goodtau.Filter("ncjets_t >= 1").Histo1D({"h_sig_cjet_t_pt", "h_sig_cjet_t_pt", 12, 0, 240}, "cjet_t_pt");
  auto h_sig_bsortjet_btag1 = dfs_S2_goodtau.Histo1D({"h_sig_bsortjet_btag1", "h_sig_bsortjet_btag1", 44, 0, 1.1}, "bsortjet_btag1");
  auto h_sig_bsortjet_btag2 = dfs_S2_goodtau.Histo1D({"h_sig_bsortjet_btag2", "h_sig_bsortjet_btag2", 44, 0, 1.1}, "bsortjet_btag2");
  auto h_sig_bsortjet_btag3 = dfs_S2_goodtau.Histo1D({"h_sig_bsortjet_btag3", "h_sig_bsortjet_btag3", 44, 0, 1.1}, "bsortjet_btag3");
  auto h_sig_bsortjet_btag4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_bsortjet_btag4", "h_sig_bsortjet_btag4", 44, 0, 1.1}, "bsortjet_btag4");
  auto h_sig_bsortjet_pt1 = dfs_S2_goodtau.Histo1D({"h_sig_bsortjet_pt1", "h_sig_bsortjet_pt1", 10, 0, 200}, "bsortjet_pt1");
  auto h_sig_bsortjet_pt2 = dfs_S2_goodtau.Histo1D({"h_sig_bsortjet_pt2", "h_sig_bsortjet_pt2", 10, 0, 200}, "bsortjet_pt2");
  auto h_sig_bsortjet_pt3 = dfs_S2_goodtau.Histo1D({"h_sig_bsortjet_pt3", "h_sig_bsortjet_pt3", 10, 0, 200}, "bsortjet_pt3");
  auto h_sig_bsortjet_pt4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_bsortjet_pt4", "h_sig_bsortjet_pt4", 10, 0, 200}, "bsortjet_pt4");
  auto h_sig_bsortjet_mass1 = dfs_S2_goodtau.Histo1D({"h_sig_bsortjet_mass1", "h_sig_bsortjet_mass1", 25, 0, 25}, "bsortjet_mass1");
  auto h_sig_bsortjet_mass2 = dfs_S2_goodtau.Histo1D({"h_sig_bsortjet_mass2", "h_sig_bsortjet_mass2", 25, 0, 25}, "bsortjet_mass2");
  auto h_sig_bsortjet_mass3 = dfs_S2_goodtau.Histo1D({"h_sig_bsortjet_mass3", "h_sig_bsortjet_mass3", 25, 0, 25}, "bsortjet_mass3");
  auto h_sig_bsortjet_mass4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_bsortjet_mass4", "h_sig_bsortjet_mass4", 25, 0, 25}, "bsortjet_mass4");
  auto h_sig_ctag_pt1 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_pt1", "h_sig_ctag_pt1", 10, 0, 200}, "ctag_pt1");
  auto h_sig_ctag_pt2 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_pt2", "h_sig_ctag_pt2", 10, 0, 200}, "ctag_pt2");
  auto h_sig_ctag_pt3 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_pt3", "h_sig_ctag_pt3", 10, 0, 200}, "ctag_pt3");
  auto h_sig_ctag_pt4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_ctag_pt4", "h_sig_ctag_pt4", 10, 0, 200}, "ctag_pt4");
  auto h_sig_ctag_mass1 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_mass1", "h_sig_ctag_mass1", 25, 0, 25}, "ctag_mass1");
  auto h_sig_ctag_mass2 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_mass2", "h_sig_ctag_mass2", 25, 0, 25}, "ctag_mass2");
  auto h_sig_ctag_mass3 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_mass3", "h_sig_ctag_mass3", 25, 0, 25}, "ctag_mass3");
  auto h_sig_ctag_mass4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_ctag_mass4", "h_sig_ctag_mass4", 25, 0, 25}, "ctag_mass4");
  auto h_sig_ctag_eta1 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_eta1", "h_sig_ctag_eta1", 45, -3, 3}, "ctag_eta1");
  auto h_sig_ctag_eta2 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_eta2", "h_sig_ctag_eta2", 45, -3, 3}, "ctag_eta2");
  auto h_sig_ctag_eta3 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_eta3", "h_sig_ctag_eta3", 45, -3, 3}, "ctag_eta3");
  auto h_sig_ctag_eta4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_ctag_eta4", "h_sig_ctag_eta4", 45, -3, 3}, "ctag_eta4");
  auto h_sig_ctag_phi1 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_phi1", "h_sig_ctag_phi1", 100, -4, 4}, "ctag_phi1");
  auto h_sig_ctag_phi2 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_phi2", "h_sig_ctag_phi2", 100, -4, 4}, "ctag_phi2");
  auto h_sig_ctag_phi3 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_phi3", "h_sig_ctag_phi3", 100, -4, 4}, "ctag_phi3");
  auto h_sig_ctag_phi4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_ctag_phi4", "h_sig_ctag_phi4", 100, -4, 4}, "ctag_phi4");
  auto h_sig_ctag_CvsL1 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_CvsL1", "h_sig_ctag_CvsL1", 44, 0, 1.2}, "ctag_CvsL1");
  auto h_sig_ctag_CvsL2 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_CvsL2", "h_sig_ctag_CvsL2", 44, 0, 1.2}, "ctag_CvsL2");
  auto h_sig_ctag_CvsL3 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_CvsL3", "h_sig_ctag_CvsL3", 44, 0, 1.2}, "ctag_CvsL3");
  auto h_sig_ctag_CvsL4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_ctag_CvsL4", "h_sig_ctag_CvsL4", 44, 0, 1.2}, "ctag_CvsL4");
  auto h_sig_ctag_CvsB1 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_CvsB1", "h_sig_ctag_CvsB1", 44, 0, 1.2}, "ctag_CvsB1");
  auto h_sig_ctag_CvsB2 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_CvsB2", "h_sig_ctag_CvsB2", 44, 0, 1.2}, "ctag_CvsB2");
  auto h_sig_ctag_CvsB3 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_CvsB3", "h_sig_ctag_CvsB3", 44, 0, 1.2}, "ctag_CvsB3");
  auto h_sig_ctag_CvsB4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_ctag_CvsB4", "h_sig_ctag_CvsB4", 44, 0, 1.2}, "ctag_CvsB4");
  auto h_sig_ctag_Ctag1 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_Ctag1", "h_sig_ctag_Ctag1", 44, 0, 1.2}, "ctag_Ctag1");
  auto h_sig_ctag_Ctag2 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_Ctag2", "h_sig_ctag_Ctag2", 44, 0, 1.2}, "ctag_Ctag2");
  auto h_sig_ctag_Ctag3 = dfs_S2_goodtau.Histo1D({"h_sig_ctag_Ctag3", "h_sig_ctag_Ctag3", 44, 0, 1.2}, "ctag_Ctag3");
  auto h_sig_ctag_Ctag4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_ctag_Ctag4", "h_sig_ctag_Ctag4", 44, 0, 1.2}, "ctag_Ctag4");
  auto h_sig_cvsl_pt1 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_pt1", "h_sig_cvsl_pt1", 10, 0, 200}, "cvsl_pt1");
  auto h_sig_cvsl_pt2 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_pt2", "h_sig_cvsl_pt2", 10, 0, 200}, "cvsl_pt2");
  auto h_sig_cvsl_pt3 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_pt3", "h_sig_cvsl_pt3", 10, 0, 200}, "cvsl_pt3");
  auto h_sig_cvsl_pt4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_cvsl_pt4", "h_sig_cvsl_pt4", 10, 0, 200}, "cvsl_pt4");
  auto h_sig_cvsl_mass1 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_mass1", "h_sig_cvsl_mass1", 25, 0, 25}, "cvsl_mass1");
  auto h_sig_cvsl_mass2 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_mass2", "h_sig_cvsl_mass2", 25, 0, 25}, "cvsl_mass2");
  auto h_sig_cvsl_mass3 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_mass3", "h_sig_cvsl_mass3", 25, 0, 25}, "cvsl_mass3");
  auto h_sig_cvsl_mass4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_cvsl_mass4", "h_sig_cvsl_mass4", 25, 0, 25}, "cvsl_mass4");
  auto h_sig_cvsl_eta1 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_eta1", "h_sig_cvsl_eta1", 45, -3, 3}, "cvsl_eta1");
  auto h_sig_cvsl_eta2 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_eta2", "h_sig_cvsl_eta2", 45, -3, 3}, "cvsl_eta2");
  auto h_sig_cvsl_eta3 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_eta3", "h_sig_cvsl_eta3", 45, -3, 3}, "cvsl_eta3");
  auto h_sig_cvsl_eta4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_cvsl_eta4", "h_sig_cvsl_eta4", 45, -3, 3}, "cvsl_eta4");
  auto h_sig_cvsl_phi1 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_phi1", "h_sig_cvsl_phi1", 100, -4, 4}, "cvsl_phi1");
  auto h_sig_cvsl_phi2 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_phi2", "h_sig_cvsl_phi2", 100, -4, 4}, "cvsl_phi2");
  auto h_sig_cvsl_phi3 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_phi3", "h_sig_cvsl_phi3", 100, -4, 4}, "cvsl_phi3");
  auto h_sig_cvsl_phi4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_cvsl_phi4", "h_sig_cvsl_phi4", 100, -4, 4}, "cvsl_phi4");
  auto h_sig_cvsl_CvsL1 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_CvsL1", "h_sig_cvsl_CvsL1", 44, 0, 1.2}, "cvsl_CvsL1");
  auto h_sig_cvsl_CvsL2 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_CvsL2", "h_sig_cvsl_CvsL2", 44, 0, 1.2}, "cvsl_CvsL2");
  auto h_sig_cvsl_CvsL3 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_CvsL3", "h_sig_cvsl_CvsL3", 44, 0, 1.2}, "cvsl_CvsL3");
  auto h_sig_cvsl_CvsL4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_cvsl_CvsL4", "h_sig_cvsl_CvsL4", 44, 0, 1.2}, "cvsl_CvsL4");
  auto h_sig_cvsl_CvsB1 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_CvsB1", "h_sig_cvsl_CvsB1", 44, 0, 1.2}, "cvsl_CvsB1");
  auto h_sig_cvsl_CvsB2 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_CvsB2", "h_sig_cvsl_CvsB2", 44, 0, 1.2}, "cvsl_CvsB2");
  auto h_sig_cvsl_CvsB3 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_CvsB3", "h_sig_cvsl_CvsB3", 44, 0, 1.2}, "cvsl_CvsB3");
  auto h_sig_cvsl_CvsB4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_cvsl_CvsB4", "h_sig_cvsl_CvsB4", 44, 0, 1.2}, "cvsl_CvsB4");
  auto h_sig_cvsl_Ctag1 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_Ctag1", "h_sig_cvsl_Ctag1", 44, 0, 1.2}, "cvsl_Ctag1");
  auto h_sig_cvsl_Ctag2 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_Ctag2", "h_sig_cvsl_Ctag2", 44, 0, 1.2}, "cvsl_Ctag2");
  auto h_sig_cvsl_Ctag3 = dfs_S2_goodtau.Histo1D({"h_sig_cvsl_Ctag3", "h_sig_cvsl_Ctag3", 44, 0, 1.2}, "cvsl_Ctag3");
  auto h_sig_cvsl_Ctag4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_cvsl_Ctag4", "h_sig_cvsl_Ctag4", 44, 0, 1.2}, "cvsl_Ctag4");
  auto h_sig_cvsb_pt1 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_pt1", "h_sig_cvsb_pt1", 10, 0, 200}, "cvsb_pt1");
  auto h_sig_cvsb_pt2 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_pt2", "h_sig_cvsb_pt2", 10, 0, 200}, "cvsb_pt2");
  auto h_sig_cvsb_pt3 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_pt3", "h_sig_cvsb_pt3", 10, 0, 200}, "cvsb_pt3");
  auto h_sig_cvsb_pt4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_cvsb_pt4", "h_sig_cvsb_pt4", 10, 0, 200}, "cvsb_pt4");
  auto h_sig_cvsb_mass1 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_mass1", "h_sig_cvsb_mass1", 25, 0, 25}, "cvsb_mass1");
  auto h_sig_cvsb_mass2 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_mass2", "h_sig_cvsb_mass2", 25, 0, 25}, "cvsb_mass2");
  auto h_sig_cvsb_mass3 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_mass3", "h_sig_cvsb_mass3", 25, 0, 25}, "cvsb_mass3");
  auto h_sig_cvsb_mass4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_cvsb_mass4", "h_sig_cvsb_mass4", 25, 0, 25}, "cvsb_mass4");
  auto h_sig_cvsb_eta1 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_eta1", "h_sig_cvsb_eta1", 45, -3, 3}, "cvsb_eta1");
  auto h_sig_cvsb_eta2 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_eta2", "h_sig_cvsb_eta2", 45, -3, 3}, "cvsb_eta2");
  auto h_sig_cvsb_eta3 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_eta3", "h_sig_cvsb_eta3", 45, -3, 3}, "cvsb_eta3");
  auto h_sig_cvsb_eta4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_cvsb_eta4", "h_sig_cvsb_eta4", 45, -3, 3}, "cvsb_eta4");
  auto h_sig_cvsb_phi1 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_phi1", "h_sig_cvsb_phi1", 100, -4, 4}, "cvsb_phi1");
  auto h_sig_cvsb_phi2 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_phi2", "h_sig_cvsb_phi2", 100, -4, 4}, "cvsb_phi2");
  auto h_sig_cvsb_phi3 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_phi3", "h_sig_cvsb_phi3", 100, -4, 4}, "cvsb_phi3");
  auto h_sig_cvsb_phi4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_cvsb_phi4", "h_sig_cvsb_phi4", 100, -4, 4}, "cvsb_phi4");
  auto h_sig_cvsb_CvsL1 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_CvsL1", "h_sig_cvsb_CvsL1", 44, 0, 1.2}, "cvsb_CvsL1");
  auto h_sig_cvsb_CvsL2 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_CvsL2", "h_sig_cvsb_CvsL2", 44, 0, 1.2}, "cvsb_CvsL2");
  auto h_sig_cvsb_CvsL3 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_CvsL3", "h_sig_cvsb_CvsL3", 44, 0, 1.2}, "cvsb_CvsL3");
  auto h_sig_cvsb_CvsL4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_cvsb_CvsL4", "h_sig_cvsb_CvsL4", 44, 0, 1.2}, "cvsb_CvsL4");
  auto h_sig_cvsb_CvsB1 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_CvsB1", "h_sig_cvsb_CvsB1", 44, 0, 1.2}, "cvsb_CvsB1");
  auto h_sig_cvsb_CvsB2 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_CvsB2", "h_sig_cvsb_CvsB2", 44, 0, 1.2}, "cvsb_CvsB2");
  auto h_sig_cvsb_CvsB3 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_CvsB3", "h_sig_cvsb_CvsB3", 44, 0, 1.2}, "cvsb_CvsB3");
  auto h_sig_cvsb_CvsB4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_cvsb_CvsB4", "h_sig_cvsb_CvsB4", 44, 0, 1.2}, "cvsb_CvsB4");
  auto h_sig_cvsb_Ctag1 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_Ctag1", "h_sig_cvsb_Ctag1", 44, 0, 1.2}, "cvsb_Ctag1");
  auto h_sig_cvsb_Ctag2 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_Ctag2", "h_sig_cvsb_Ctag2", 44, 0, 1.2}, "cvsb_Ctag2");
  auto h_sig_cvsb_Ctag3 = dfs_S2_goodtau.Histo1D({"h_sig_cvsb_Ctag3", "h_sig_cvsb_Ctag3", 44, 0, 1.2}, "cvsb_Ctag3");
  auto h_sig_cvsb_Ctag4 = dfs_S2_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_sig_cvsb_Ctag4", "h_sig_cvsb_Ctag4", 44, 0, 1.2}, "cvsb_Ctag4");

  auto h_ttbar_ctag_pt1 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_pt1", "h_ttbar_ctag_pt1", 10, 0, 200}, "ctag_pt1");
  auto h_ttbar_ctag_pt2 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_pt2", "h_ttbar_ctag_pt2", 10, 0, 200}, "ctag_pt2");
  auto h_ttbar_ctag_pt3 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_pt3", "h_ttbar_ctag_pt3", 10, 0, 200}, "ctag_pt3");
  auto h_ttbar_ctag_pt4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_ctag_pt4", "h_ttbar_ctag_pt4", 10, 0, 200}, "ctag_pt4");
  auto h_ttbar_ctag_mass1 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_mass1", "h_ttbar_ctag_mass1", 25, 0, 25}, "ctag_mass1");
  auto h_ttbar_ctag_mass2 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_mass2", "h_ttbar_ctag_mass2", 25, 0, 25}, "ctag_mass2");
  auto h_ttbar_ctag_mass3 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_mass3", "h_ttbar_ctag_mass3", 25, 0, 25}, "ctag_mass3");
  auto h_ttbar_ctag_mass4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_ctag_mass4", "h_ttbar_ctag_mass4", 25, 0, 25}, "ctag_mass4");
  auto h_ttbar_ctag_eta1 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_eta1", "h_ttbar_ctag_eta1", 45, -3, 3}, "ctag_eta1");
  auto h_ttbar_ctag_eta2 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_eta2", "h_ttbar_ctag_eta2", 45, -3, 3}, "ctag_eta2");
  auto h_ttbar_ctag_eta3 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_eta3", "h_ttbar_ctag_eta3", 45, -3, 3}, "ctag_eta3");
  auto h_ttbar_ctag_eta4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_ctag_eta4", "h_ttbar_ctag_eta4", 45, -3, 3}, "ctag_eta4");
  auto h_ttbar_ctag_phi1 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_phi1", "h_ttbar_ctag_phi1", 100, -4, 4}, "ctag_phi1");
  auto h_ttbar_ctag_phi2 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_phi2", "h_ttbar_ctag_phi2", 100, -4, 4}, "ctag_phi2");
  auto h_ttbar_ctag_phi3 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_phi3", "h_ttbar_ctag_phi3", 100, -4, 4}, "ctag_phi3");
  auto h_ttbar_ctag_phi4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_ctag_phi4", "h_ttbar_ctag_phi4", 100, -4, 4}, "ctag_phi4");
  auto h_ttbar_ctag_CvsL1 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_CvsL1", "h_ttbar_ctag_CvsL1", 44, 0, 1.2}, "ctag_CvsL1");
  auto h_ttbar_ctag_CvsL2 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_CvsL2", "h_ttbar_ctag_CvsL2", 44, 0, 1.2}, "ctag_CvsL2");
  auto h_ttbar_ctag_CvsL3 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_CvsL3", "h_ttbar_ctag_CvsL3", 44, 0, 1.2}, "ctag_CvsL3");
  auto h_ttbar_ctag_CvsL4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_ctag_CvsL4", "h_ttbar_ctag_CvsL4", 44, 0, 1.2}, "ctag_CvsL4");
  auto h_ttbar_ctag_CvsB1 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_CvsB1", "h_ttbar_ctag_CvsB1", 44, 0, 1.2}, "ctag_CvsB1");
  auto h_ttbar_ctag_CvsB2 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_CvsB2", "h_ttbar_ctag_CvsB2", 44, 0, 1.2}, "ctag_CvsB2");
  auto h_ttbar_ctag_CvsB3 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_CvsB3", "h_ttbar_ctag_CvsB3", 44, 0, 1.2}, "ctag_CvsB3");
  auto h_ttbar_ctag_CvsB4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_ctag_CvsB4", "h_ttbar_ctag_CvsB4", 44, 0, 1.2}, "ctag_CvsB4");
  auto h_ttbar_ctag_Ctag1 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_Ctag1", "h_ttbar_ctag_Ctag1", 44, 0, 1.2}, "ctag_Ctag1");
  auto h_ttbar_ctag_Ctag2 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_Ctag2", "h_ttbar_ctag_Ctag2", 44, 0, 1.2}, "ctag_Ctag2");
  auto h_ttbar_ctag_Ctag3 = df_S1_goodtau.Histo1D({"h_ttbar_ctag_Ctag3", "h_ttbar_ctag_Ctag3", 44, 0, 1.2}, "ctag_Ctag3");
  auto h_ttbar_ctag_Ctag4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_ctag_Ctag4", "h_ttbar_ctag_Ctag4", 44, 0, 1.2}, "ctag_Ctag4");
  auto h_ttbar_cvsl_pt1 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_pt1", "h_ttbar_cvsl_pt1", 10, 0, 200}, "cvsl_pt1");
  auto h_ttbar_cvsl_pt2 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_pt2", "h_ttbar_cvsl_pt2", 10, 0, 200}, "cvsl_pt2");
  auto h_ttbar_cvsl_pt3 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_pt3", "h_ttbar_cvsl_pt3", 10, 0, 200}, "cvsl_pt3");
  auto h_ttbar_cvsl_pt4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_cvsl_pt4", "h_ttbar_cvsl_pt4", 10, 0, 200}, "cvsl_pt4");
  auto h_ttbar_cvsl_mass1 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_mass1", "h_ttbar_cvsl_mass1", 25, 0, 25}, "cvsl_mass1");
  auto h_ttbar_cvsl_mass2 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_mass2", "h_ttbar_cvsl_mass2", 25, 0, 25}, "cvsl_mass2");
  auto h_ttbar_cvsl_mass3 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_mass3", "h_ttbar_cvsl_mass3", 25, 0, 25}, "cvsl_mass3");
  auto h_ttbar_cvsl_mass4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_cvsl_mass4", "h_ttbar_cvsl_mass4", 25, 0, 25}, "cvsl_mass4");
  auto h_ttbar_cvsl_eta1 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_eta1", "h_ttbar_cvsl_eta1", 45, -3, 3}, "cvsl_eta1");
  auto h_ttbar_cvsl_eta2 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_eta2", "h_ttbar_cvsl_eta2", 45, -3, 3}, "cvsl_eta2");
  auto h_ttbar_cvsl_eta3 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_eta3", "h_ttbar_cvsl_eta3", 45, -3, 3}, "cvsl_eta3");
  auto h_ttbar_cvsl_eta4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_cvsl_eta4", "h_ttbar_cvsl_eta4", 45, -3, 3}, "cvsl_eta4");
  auto h_ttbar_cvsl_phi1 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_phi1", "h_ttbar_cvsl_phi1", 100, -4, 4}, "cvsl_phi1");
  auto h_ttbar_cvsl_phi2 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_phi2", "h_ttbar_cvsl_phi2", 100, -4, 4}, "cvsl_phi2");
  auto h_ttbar_cvsl_phi3 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_phi3", "h_ttbar_cvsl_phi3", 100, -4, 4}, "cvsl_phi3");
  auto h_ttbar_cvsl_phi4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_cvsl_phi4", "h_ttbar_cvsl_phi4", 100, -4, 4}, "cvsl_phi4");
  auto h_ttbar_cvsl_CvsL1 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_CvsL1", "h_ttbar_cvsl_CvsL1", 44, 0, 1.2}, "cvsl_CvsL1");
  auto h_ttbar_cvsl_CvsL2 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_CvsL2", "h_ttbar_cvsl_CvsL2", 44, 0, 1.2}, "cvsl_CvsL2");
  auto h_ttbar_cvsl_CvsL3 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_CvsL3", "h_ttbar_cvsl_CvsL3", 44, 0, 1.2}, "cvsl_CvsL3");
  auto h_ttbar_cvsl_CvsL4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_cvsl_CvsL4", "h_ttbar_cvsl_CvsL4", 44, 0, 1.2}, "cvsl_CvsL4");
  auto h_ttbar_cvsl_CvsB1 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_CvsB1", "h_ttbar_cvsl_CvsB1", 44, 0, 1.2}, "cvsl_CvsB1");
  auto h_ttbar_cvsl_CvsB2 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_CvsB2", "h_ttbar_cvsl_CvsB2", 44, 0, 1.2}, "cvsl_CvsB2");
  auto h_ttbar_cvsl_CvsB3 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_CvsB3", "h_ttbar_cvsl_CvsB3", 44, 0, 1.2}, "cvsl_CvsB3");
  auto h_ttbar_cvsl_CvsB4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_cvsl_CvsB4", "h_ttbar_cvsl_CvsB4", 44, 0, 1.2}, "cvsl_CvsB4");
  auto h_ttbar_cvsl_Ctag1 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_Ctag1", "h_ttbar_cvsl_Ctag1", 44, 0, 1.2}, "cvsl_Ctag1");
  auto h_ttbar_cvsl_Ctag2 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_Ctag2", "h_ttbar_cvsl_Ctag2", 44, 0, 1.2}, "cvsl_Ctag2");
  auto h_ttbar_cvsl_Ctag3 = df_S1_goodtau.Histo1D({"h_ttbar_cvsl_Ctag3", "h_ttbar_cvsl_Ctag3", 44, 0, 1.2}, "cvsl_Ctag3");
  auto h_ttbar_cvsl_Ctag4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_cvsl_Ctag4", "h_ttbar_cvsl_Ctag4", 44, 0, 1.2}, "cvsl_Ctag4");
  auto h_ttbar_cvsb_pt1 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_pt1", "h_ttbar_cvsb_pt1", 10, 0, 200}, "cvsb_pt1");
  auto h_ttbar_cvsb_pt2 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_pt2", "h_ttbar_cvsb_pt2", 10, 0, 200}, "cvsb_pt2");
  auto h_ttbar_cvsb_pt3 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_pt3", "h_ttbar_cvsb_pt3", 10, 0, 200}, "cvsb_pt3");
  auto h_ttbar_cvsb_pt4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_cvsb_pt4", "h_ttbar_cvsb_pt4", 10, 0, 200}, "cvsb_pt4");
  auto h_ttbar_cvsb_mass1 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_mass1", "h_ttbar_cvsb_mass1", 25, 0, 25}, "cvsb_mass1");
  auto h_ttbar_cvsb_mass2 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_mass2", "h_ttbar_cvsb_mass2", 25, 0, 25}, "cvsb_mass2");
  auto h_ttbar_cvsb_mass3 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_mass3", "h_ttbar_cvsb_mass3", 25, 0, 25}, "cvsb_mass3");
  auto h_ttbar_cvsb_mass4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_cvsb_mass4", "h_ttbar_cvsb_mass4", 25, 0, 25}, "cvsb_mass4");
  auto h_ttbar_cvsb_eta1 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_eta1", "h_ttbar_cvsb_eta1", 45, -3, 3}, "cvsb_eta1");
  auto h_ttbar_cvsb_eta2 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_eta2", "h_ttbar_cvsb_eta2", 45, -3, 3}, "cvsb_eta2");
  auto h_ttbar_cvsb_eta3 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_eta3", "h_ttbar_cvsb_eta3", 45, -3, 3}, "cvsb_eta3");
  auto h_ttbar_cvsb_eta4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_cvsb_eta4", "h_ttbar_cvsb_eta4", 45, -3, 3}, "cvsb_eta4");
  auto h_ttbar_cvsb_phi1 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_phi1", "h_ttbar_cvsb_phi1", 100, -4, 4}, "cvsb_phi1");
  auto h_ttbar_cvsb_phi2 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_phi2", "h_ttbar_cvsb_phi2", 100, -4, 4}, "cvsb_phi2");
  auto h_ttbar_cvsb_phi3 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_phi3", "h_ttbar_cvsb_phi3", 100, -4, 4}, "cvsb_phi3");
  auto h_ttbar_cvsb_phi4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_cvsb_phi4", "h_ttbar_cvsb_phi4", 100, -4, 4}, "cvsb_phi4");
  auto h_ttbar_cvsb_CvsL1 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_CvsL1", "h_ttbar_cvsb_CvsL1", 44, 0, 1.2}, "cvsb_CvsL1");
  auto h_ttbar_cvsb_CvsL2 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_CvsL2", "h_ttbar_cvsb_CvsL2", 44, 0, 1.2}, "cvsb_CvsL2");
  auto h_ttbar_cvsb_CvsL3 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_CvsL3", "h_ttbar_cvsb_CvsL3", 44, 0, 1.2}, "cvsb_CvsL3");
  auto h_ttbar_cvsb_CvsL4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_cvsb_CvsL4", "h_ttbar_cvsb_CvsL4", 44, 0, 1.2}, "cvsb_CvsL4");
  auto h_ttbar_cvsb_CvsB1 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_CvsB1", "h_ttbar_cvsb_CvsB1", 44, 0, 1.2}, "cvsb_CvsB1");
  auto h_ttbar_cvsb_CvsB2 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_CvsB2", "h_ttbar_cvsb_CvsB2", 44, 0, 1.2}, "cvsb_CvsB2");
  auto h_ttbar_cvsb_CvsB3 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_CvsB3", "h_ttbar_cvsb_CvsB3", 44, 0, 1.2}, "cvsb_CvsB3");
  auto h_ttbar_cvsb_CvsB4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_cvsb_CvsB4", "h_ttbar_cvsb_CvsB4", 44, 0, 1.2}, "cvsb_CvsB4");
  auto h_ttbar_cvsb_Ctag1 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_Ctag1", "h_ttbar_cvsb_Ctag1", 44, 0, 1.2}, "cvsb_Ctag1");
  auto h_ttbar_cvsb_Ctag2 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_Ctag2", "h_ttbar_cvsb_Ctag2", 44, 0, 1.2}, "cvsb_Ctag2");
  auto h_ttbar_cvsb_Ctag3 = df_S1_goodtau.Histo1D({"h_ttbar_cvsb_Ctag3", "h_ttbar_cvsb_Ctag3", 44, 0, 1.2}, "cvsb_Ctag3");
  auto h_ttbar_cvsb_Ctag4 = df_S1_goodtau.Filter("ngoodjets >= 4").Histo1D({"h_ttbar_cvsb_Ctag4", "h_ttbar_cvsb_Ctag4", 44, 0, 1.2}, "cvsb_Ctag4");
  //save as root file
  ROOT::RDF::RSnapshotOptions opts;
  opts.fMode = "UPDATE";
  //df_S1_goodtau.Snapshot("Event_category", "LQ_new.root", {"category"});
  //df_S1_lf.Snapshot("ttbar_lf", "LQ_new.root", {"sortbjet"});
  //df_S1_lf.Snapshot("ttbar_lf", "LQ_new_3.root", {"nbjets_m", "nbjets_t", "ncjets_l", "ncjets_m", "ncjets_t", "ntaujets", "ngoodjets", "jet_pt", "bjet_m_pt", "bjet_t_pt", "cjet_l_pt", "cjet_m_pt", "cjet_t_pt", "sortbjet_pt1", "sortbjet_pt2", "sortbjet_pt3", "sortbjet_pt4", "category"}, opts);
//  df_S1_lf.Snapshot("ttbar_lf", "LQ_new.root", {"nbjets_m", "nbjets_t", "ncjets_l", "ncjets_m", "ncjets_t", "ntaujets", "ngoodjets", "jet_pt", "bjet_m_pt", "bjet_t_pt", "cjet_l_pt", "cjet_m_pt", "cjet_t_pt", "sortbjet_pt1", "sortbjet_pt2", "sortbjet_pt3", "sortbjet_pt4", "sortbjet_mass1", "sortbjet_mass2", "sortbjet_mass3", "sortbjet_mass4", "category"}, opts);
//
//  df_S1_b.Snapshot("ttbar_b", "LQ_new.root", {"nbjets_m", "nbjets_t", "ncjets_l", "ncjets_m", "ncjets_t", "ntaujets", "ngoodjets", "jet_pt", "bjet_m_pt", "bjet_t_pt", "cjet_l_pt", "cjet_m_pt", "cjet_t_pt", "sortbjet_pt1", "sortbjet_pt2", "sortbjet_pt3", "sortbjet_pt4", "sortbjet_mass1", "sortbjet_mass2", "sortbjet_mass3", "sortbjet_mass4", "category"}, opts);
//
//  df_S1_bb.Snapshot("ttbar_bb", "LQ_new.root", {"nbjets_m", "nbjets_t", "ncjets_l", "ncjets_m", "ncjets_t", "ntaujets", "ngoodjets", "jet_pt", "bjet_m_pt", "bjet_t_pt", "cjet_l_pt", "cjet_m_pt", "cjet_t_pt", "sortbjet_pt1", "sortbjet_pt2", "sortbjet_pt3", "sortbjet_pt4", "sortbjet_mass1", "sortbjet_mass2", "sortbjet_mass3", "sortbjet_mass4", "category"}, opts);
//
//  df_S1_c.Snapshot("ttbar_c", "LQ_new.root", {"nbjets_m", "nbjets_t", "ncjets_l", "ncjets_m", "ncjets_t", "ntaujets", "ngoodjets", "jet_pt", "bjet_m_pt", "bjet_t_pt", "cjet_l_pt", "cjet_m_pt", "cjet_t_pt", "sortbjet_pt1", "sortbjet_pt2", "sortbjet_pt3", "sortbjet_pt4", "sortbjet_mass1", "sortbjet_mass2", "sortbjet_mass3", "sortbjet_mass4", "category"}, opts);
//
//  df_S1_cc.Snapshot("ttbar_cc", "LQ_new.root", {"nbjets_m", "nbjets_t", "ncjets_l", "ncjets_m", "ncjets_t", "ntaujets", "ngoodjets", "jet_pt", "bjet_m_pt", "bjet_t_pt", "cjet_l_pt", "cjet_m_pt", "cjet_t_pt", "sortbjet_pt1", "sortbjet_pt2", "sortbjet_pt3", "sortbjet_pt4", "sortbjet_mass1", "sortbjet_mass2", "sortbjet_mass3", "sortbjet_mass4", "category"}, opts);
//
//  dfs_S2_goodtau.Snapshot("ttbar_signal", "LQ_new.root", {"nbjets_m", "nbjets_t", "ncjets_l", "ncjets_m", "ncjets_t", "ntaujets", "ngoodjets", "jet_pt", "bjet_m_pt", "bjet_t_pt", "cjet_l_pt", "cjet_m_pt", "cjet_t_pt", "sortbjet_pt1", "sortbjet_pt2", "sortbjet_pt3", "sortbjet_pt4", "sortbjet_mass1", "sortbjet_mass2", "sortbjet_mass3", "sortbjet_mass4"}, opts);
//
//  df_S1_lf.Snapshot("ttbar_lf", "LQ_new.root", {"nbjets_m"}, opts);
//
//  df_S1_b.Snapshot("ttbar_b", "LQ_new.root", {"nbjets_m"}, opts);
//
//  df_S1_bb.Snapshot("ttbar_bb", "LQ_new.root", {"nbjets_m"}, opts);
//
//  df_S1_c.Snapshot("ttbar_c", "LQ_new.root", {"nbjets_m"}, opts);
////
//  df_S1_cc.Snapshot("ttbar_cc", "LQ_new.root", {"nbjets_m"}, opts);
////
//  dfs_S2_goodtau.Snapshot("ttbar_signal", "LQ_new.root", {"nbjets_m"}, opts);
//
  TFile f("LQ_new.root", "UPDATE");
    
//    plot(h_lf_nbjets_m, "h_lf_nbjets_m");
//	plot(h_lf_nbjets_t, "h_lf_nbjets_t");
//	plot(h_lf_ncjets_l, "h_lf_ncjets_l");
//	plot(h_lf_ncjets_m, "h_lf_ncjets_m");
//	plot(h_lf_ncjets_t, "h_lf_ncjets_t");
//	plot(h_lf_ntaujets, "h_lf_ntaujets");
//	plot(h_lf_jet_pt, "h_lf_jet_pt");
//	plot(h_lf_bjet_m_pt , "h_lf_bjet_m_pt");
//	plot(h_lf_bjet_t_pt , "h_lf_bjet_t_pt");
//	plot(h_lf_cjet_l_pt , "h_lf_cjet_l_pt");
//	plot(h_lf_cjet_m_pt , "h_lf_cjet_m_pt");
//	plot(h_lf_cjet_t_pt , "h_lf_cjet_t_pt");
//    plot(h_lf_sortbjet_pt1, "h_lf_sortbjet_pt1");
//    plot(h_lf_sortbjet_pt2, "h_lf_sortbjet_pt2");
//    plot(h_lf_sortbjet_pt3, "h_lf_sortbjet_pt3");
//    plot(h_lf_sortbjet_pt4, "h_lf_sortbjet_pt4");
//    plot(h_lf_sortbjet_mass1, "h_lf_sortbjet_mass1");
//    plot(h_lf_sortbjet_mass2, "h_lf_sortbjet_mass2");
//    plot(h_lf_sortbjet_mass3, "h_lf_sortbjet_mass3");
//    plot(h_lf_sortbjet_mass4, "h_lf_sortbjet_mass4");
//
//	plot(h_b_nbjets_m, "h_b_nbjets_m");
//	plot(h_b_nbjets_t, "h_b_nbjets_t");
//	plot(h_b_ncjets_l, "h_b_ncjets_l");
//	plot(h_b_ncjets_m, "h_b_ncjets_m");
//	plot(h_b_ncjets_t, "h_b_ncjets_t");
//	plot(h_b_ntaujets, "h_b_ntaujets");
//	plot(h_b_jet_pt, "h_b_jet_pt");
//	plot(h_b_bjet_m_pt, "h_b_bjet_m_pt");
//	plot(h_b_bjet_t_pt, "h_b_bjet_t_pt");
//	plot(h_b_cjet_l_pt, "h_b_cjet_l_pt");
//	plot(h_b_cjet_m_pt, "h_b_cjet_m_pt");
//	plot(h_b_cjet_t_pt, "h_b_cjet_t_pt");
//    plot(h_b_sortbjet_pt1, "h_b_sortbjet_pt1");
//    plot(h_b_sortbjet_pt2, "h_b_sortbjet_pt2");
//    plot(h_b_sortbjet_pt3, "h_b_sortbjet_pt3");
//    plot(h_b_sortbjet_pt4, "h_b_sortbjet_pt4");
//    plot(h_b_sortbjet_mass1, "h_b_sortbjet_mass1");
//    plot(h_b_sortbjet_mass2, "h_b_sortbjet_mass2");
//    plot(h_b_sortbjet_mass3, "h_b_sortbjet_mass3");
//    plot(h_b_sortbjet_mass4, "h_b_sortbjet_mass4");
//
//	plot(h_bb_nbjets_m, "h_bb_nbjets_m");
//	plot(h_bb_nbjets_t, "h_bb_nbjets_t");
//	plot(h_bb_ncjets_l, "h_bb_ncjets_l");
//	plot(h_bb_ncjets_m, "h_bb_ncjets_m");
//	plot(h_bb_ncjets_t, "h_bb_ncjets_t");
//	plot(h_bb_ntaujets, "h_bb_ntaujets");
//	plot(h_bb_jet_pt, "h_bb_jet_pt");
//	plot(h_bb_bjet_m_pt, "h_bb_bjet_m_pt");
//	plot(h_bb_bjet_t_pt, "h_bb_bjet_t_pt");
//	plot(h_bb_cjet_l_pt, "h_bb_cjet_l_pt");
//	plot(h_bb_cjet_m_pt, "h_bb_cjet_m_pt");
//	plot(h_bb_cjet_t_pt, "h_bb_cjet_t_pt");
//    plot(h_bb_sortbjet_pt1, "h_bb_sortbjet_pt1");
//    plot(h_bb_sortbjet_pt2, "h_bb_sortbjet_pt2");
//    plot(h_bb_sortbjet_pt3, "h_bb_sortbjet_pt3");
//    plot(h_bb_sortbjet_pt4, "h_bb_sortbjet_pt4");
//    plot(h_bb_sortbjet_mass1, "h_bb_sortbjet_mass1");
//    plot(h_bb_sortbjet_mass2, "h_bb_sortbjet_mass2");
//    plot(h_bb_sortbjet_mass3, "h_bb_sortbjet_mass3");
//    plot(h_bb_sortbjet_mass4, "h_bb_sortbjet_mass4");
//
//	plot(h_c_nbjets_m, "h_c_nbjets_m");
//	plot(h_c_nbjets_t, "h_c_nbjets_t");
//	plot(h_c_ncjets_l, "h_c_ncjets_l");
//	plot(h_c_ncjets_m, "h_c_ncjets_m");
//	plot(h_c_ncjets_t, "h_c_ncjets_t");
//	plot(h_c_ntaujets, "h_c_ntaujets");
//	plot(h_c_jet_pt, "h_c_jet_pt");
//	plot(h_c_bjet_m_pt, "h_c_bjet_m_pt");
//	plot(h_c_bjet_t_pt, "h_c_bjet_t_pt");
//	plot(h_c_cjet_l_pt, "h_c_cjet_l_pt");
//	plot(h_c_cjet_m_pt, "h_c_cjet_m_pt");
//	plot(h_c_cjet_t_pt, "h_c_cjet_t_pt");
//    plot(h_c_sortbjet_pt1, "h_c_sortbjet_pt1");
//    plot(h_c_sortbjet_pt2, "h_c_sortbjet_pt2");
//    plot(h_c_sortbjet_pt3, "h_c_sortbjet_pt3");
//    plot(h_c_sortbjet_pt4, "h_c_sortbjet_pt4");
//    plot(h_c_sortbjet_mass1, "h_c_sortbjet_mass1");
//    plot(h_c_sortbjet_mass2, "h_c_sortbjet_mass2");
//    plot(h_c_sortbjet_mass3, "h_c_sortbjet_mass3");
//    plot(h_c_sortbjet_mass4, "h_c_sortbjet_mass4");
//
//	plot(h_cc_nbjets_m, "h_cc_nbjets_m");
//	plot(h_cc_nbjets_t, "h_cc_nbjets_t");
//	plot(h_cc_ncjets_l, "h_cc_ncjets_l");
//	plot(h_cc_ncjets_m, "h_cc_ncjets_m");
//	plot(h_cc_ncjets_t, "h_cc_ncjets_t");
//	plot(h_cc_ntaujets, "h_cc_ntaujets");
//	plot(h_cc_jet_pt, "h_cc_jet_pt");
//	plot(h_cc_bjet_m_pt, "h_cc_bjet_m_pt");
//	plot(h_cc_bjet_t_pt, "h_cc_bjet_t_pt");
//	plot(h_cc_cjet_l_pt, "h_cc_cjet_l_pt");
//	plot(h_cc_cjet_m_pt, "h_cc_cjet_m_pt");
//	plot(h_cc_cjet_t_pt, "h_cc_cjet_t_pt");
//    plot(h_cc_sortbjet_pt1, "h_cc_sortbjet_pt1");
//    plot(h_cc_sortbjet_pt2, "h_cc_sortbjet_pt2");
//    plot(h_cc_sortbjet_pt3, "h_cc_sortbjet_pt3");
//    plot(h_cc_sortbjet_pt4, "h_cc_sortbjet_pt4");
//    plot(h_cc_sortbjet_mass1, "h_cc_sortbjet_mass1");
//    plot(h_cc_sortbjet_mass2, "h_cc_sortbjet_mass2");
//    plot(h_cc_sortbjet_mass3, "h_cc_sortbjet_mass3");
//    plot(h_cc_sortbjet_mass4, "h_cc_sortbjet_mass4");
//
//	plot(h_sig_nbjets_m, "h_sig_nbjets_m");
//	plot(h_sig_nbjets_t, "h_sig_nbjets_t");
//	plot(h_sig_ncjets_l, "h_sig_ncjets_l");
//	plot(h_sig_ncjets_m, "h_sig_ncjets_m");
//	plot(h_sig_ncjets_t, "h_sig_ncjets_t");
//	plot(h_sig_ntaujets, "h_sig_ntaujets");
//	plot(h_sig_jet_pt, "h_sig_jet_pt");
//	plot(h_sig_bjet_m_pt, "h_sig_bjet_m_pt");
//	plot(h_sig_bjet_t_pt, "h_sig_bjet_t_pt");
//	plot(h_sig_cjet_l_pt, "h_sig_cjet_l_pt");
//	plot(h_sig_cjet_m_pt, "h_sig_cjet_m_pt");
//	plot(h_sig_cjet_t_pt, "h_sig_cjet_t_pt");  
//    plot(h_sig_sortbjet_pt1, "h_sig_sortbjet_pt1");
//    plot(h_sig_sortbjet_pt2, "h_sig_sortbjet_pt2");
//    plot(h_sig_sortbjet_pt3, "h_sig_sortbjet_pt3");
//    plot(h_sig_sortbjet_pt4, "h_sig_sortbjet_pt4");
//    plot(h_sig_sortbjet_mass1, "h_sig_sortbjet_mass1");
//    plot(h_sig_sortbjet_mass2, "h_sig_sortbjet_mass2");
//    plot(h_sig_sortbjet_mass3, "h_sig_sortbjet_mass3");
//    plot(h_sig_sortbjet_mass4, "h_sig_sortbjet_mass4");

//    plot(h_lf_sortbjet_btag1, "h_lf_sortbjet_btag1");
//    plot(h_lf_sortbjet_btag2, "h_lf_sortbjet_btag2");
//    plot(h_lf_sortbjet_btag3, "h_lf_sortbjet_btag3");
//    plot(h_lf_sortbjet_btag4, "h_lf_sortbjet_btag4");
//
//    plot(h_b_sortbjet_btag1, "h_b_sortbjet_btag1");
//    plot(h_b_sortbjet_btag2, "h_b_sortbjet_btag2");
//    plot(h_b_sortbjet_btag3, "h_b_sortbjet_btag3");
//    plot(h_b_sortbjet_btag4, "h_b_sortbjet_btag4");
//
//    plot(h_bb_sortbjet_btag1, "h_bb_sortbjet_btag1");
//    plot(h_bb_sortbjet_btag2, "h_bb_sortbjet_btag2");
//    plot(h_bb_sortbjet_btag3, "h_bb_sortbjet_btag3");
//    plot(h_bb_sortbjet_btag4, "h_bb_sortbjet_btag4");
//
//    plot(h_c_sortbjet_btag1, "h_c_sortbjet_btag1");
//    plot(h_c_sortbjet_btag2, "h_c_sortbjet_btag2");
//    plot(h_c_sortbjet_btag3, "h_c_sortbjet_btag3");
//    plot(h_c_sortbjet_btag4, "h_c_sortbjet_btag4");
//
//    plot(h_cc_sortbjet_btag1, "h_cc_sortbjet_btag1");
//    plot(h_cc_sortbjet_btag2, "h_cc_sortbjet_btag2");
//    plot(h_cc_sortbjet_btag3, "h_cc_sortbjet_btag3");
//    plot(h_cc_sortbjet_btag4, "h_cc_sortbjet_btag4");

//    plot(h_lf_bsortjet_pt1, "h_lf_bsortjet_pt1");
//    plot(h_lf_bsortjet_pt2, "h_lf_bsortjet_pt2");
//    plot(h_lf_bsortjet_pt3, "h_lf_bsortjet_pt3");
//    plot(h_lf_bsortjet_pt4, "h_lf_bsortjet_pt4");
//    plot(h_lf_bsortjet_mass1, "h_lf_bsortjet_mass1");
//    plot(h_lf_bsortjet_mass2, "h_lf_bsortjet_mass2");
//    plot(h_lf_bsortjet_mass3, "h_lf_bsortjet_mass3");
//    plot(h_lf_bsortjet_mass4, "h_lf_bsortjet_mass4");
//    plot(h_lf_bsortjet_btag1, "h_lf_bsortjet_btag1");
//    plot(h_lf_bsortjet_btag2, "h_lf_bsortjet_btag2");
//    plot(h_lf_bsortjet_btag3, "h_lf_bsortjet_btag3");
//    plot(h_lf_bsortjet_btag4, "h_lf_bsortjet_btag4");
//
//    plot(h_b_bsortjet_pt1, "h_b_bsortjet_pt1");
//    plot(h_b_bsortjet_pt2, "h_b_bsortjet_pt2");
//    plot(h_b_bsortjet_pt3, "h_b_bsortjet_pt3");
//    plot(h_b_bsortjet_pt4, "h_b_bsortjet_pt4");
//    plot(h_b_bsortjet_mass1, "h_b_bsortjet_mass1");
//    plot(h_b_bsortjet_mass2, "h_b_bsortjet_mass2");
//    plot(h_b_bsortjet_mass3, "h_b_bsortjet_mass3");
//    plot(h_b_bsortjet_mass4, "h_b_bsortjet_mass4");
//    plot(h_b_bsortjet_btag1, "h_b_bsortjet_btag1");
//    plot(h_b_bsortjet_btag2, "h_b_bsortjet_btag2");
//    plot(h_b_bsortjet_btag3, "h_b_bsortjet_btag3");
//    plot(h_b_bsortjet_btag4, "h_b_bsortjet_btag4");
//
//    plot(h_bb_bsortjet_pt1, "h_bb_bsortjet_pt1");
//    plot(h_bb_bsortjet_pt2, "h_bb_bsortjet_pt2");
//    plot(h_bb_bsortjet_pt3, "h_bb_bsortjet_pt3");
//    plot(h_bb_bsortjet_pt4, "h_bb_bsortjet_pt4");
//    plot(h_bb_bsortjet_mass1, "h_bb_bsortjet_mass1");
//    plot(h_bb_bsortjet_mass2, "h_bb_bsortjet_mass2");
//    plot(h_bb_bsortjet_mass3, "h_bb_bsortjet_mass3");
//    plot(h_bb_bsortjet_mass4, "h_bb_bsortjet_mass4");
//    plot(h_bb_bsortjet_btag1, "h_bb_bsortjet_btag1");
//    plot(h_bb_bsortjet_btag2, "h_bb_bsortjet_btag2");
//    plot(h_bb_bsortjet_btag3, "h_bb_bsortjet_btag3");
//    plot(h_bb_bsortjet_btag4, "h_bb_bsortjet_btag4");
//
//    plot(h_c_bsortjet_pt1, "h_c_bsortjet_pt1");
//    plot(h_c_bsortjet_pt2, "h_c_bsortjet_pt2");
//    plot(h_c_bsortjet_pt3, "h_c_bsortjet_pt3");
//    plot(h_c_bsortjet_pt4, "h_c_bsortjet_pt4");
//    plot(h_c_bsortjet_mass1, "h_c_bsortjet_mass1");
//    plot(h_c_bsortjet_mass2, "h_c_bsortjet_mass2");
//    plot(h_c_bsortjet_mass3, "h_c_bsortjet_mass3");
//    plot(h_c_bsortjet_mass4, "h_c_bsortjet_mass4");
//    plot(h_c_bsortjet_btag1, "h_c_bsortjet_btag1");
//    plot(h_c_bsortjet_btag2, "h_c_bsortjet_btag2");
//    plot(h_c_bsortjet_btag3, "h_c_bsortjet_btag3");
//    plot(h_c_bsortjet_btag4, "h_c_bsortjet_btag4");
//
//    plot(h_cc_bsortjet_pt1, "h_cc_bsortjet_pt1");
//    plot(h_cc_bsortjet_pt2, "h_cc_bsortjet_pt2");
//    plot(h_cc_bsortjet_pt3, "h_cc_bsortjet_pt3");
//    plot(h_cc_bsortjet_pt4, "h_cc_bsortjet_pt4");
//    plot(h_cc_bsortjet_mass1, "h_cc_bsortjet_mass1");
//    plot(h_cc_bsortjet_mass2, "h_cc_bsortjet_mass2");
//    plot(h_cc_bsortjet_mass3, "h_cc_bsortjet_mass3");
//    plot(h_cc_bsortjet_mass4, "h_cc_bsortjet_mass4");
//    plot(h_cc_bsortjet_btag1, "h_cc_bsortjet_btag1");
//    plot(h_cc_bsortjet_btag2, "h_cc_bsortjet_btag2");
//    plot(h_cc_bsortjet_btag3, "h_cc_bsortjet_btag3");
//    plot(h_cc_bsortjet_btag4, "h_cc_bsortjet_btag4");
//
//    plot(h_sig_bsortjet_pt1, "h_sig_bsortjet_pt1");
//    plot(h_sig_bsortjet_pt2, "h_sig_bsortjet_pt2");
//    plot(h_sig_bsortjet_pt3, "h_sig_bsortjet_pt3");
//    plot(h_sig_bsortjet_pt4, "h_sig_bsortjet_pt4");
//    plot(h_sig_bsortjet_mass1, "h_sig_bsortjet_mass1");
//    plot(h_sig_bsortjet_mass2, "h_sig_bsortjet_mass2");
//    plot(h_sig_bsortjet_mass3, "h_sig_bsortjet_mass3");
//    plot(h_sig_bsortjet_mass4, "h_sig_bsortjet_mass4");
//    plot(h_sig_bsortjet_btag1, "h_sig_bsortjet_btag1");
//    plot(h_sig_bsortjet_btag2, "h_sig_bsortjet_btag2");
//    plot(h_sig_bsortjet_btag3, "h_sig_bsortjet_btag3");
//    plot(h_sig_bsortjet_btag4, "h_sig_bsortjet_btag4");

//    plot(h_lf_ctag_pt1, "h_lf_ctag_pt1");
//    plot(h_lf_ctag_pt2, "h_lf_ctag_pt2");
//    plot(h_lf_ctag_pt3, "h_lf_ctag_pt3");
//    plot(h_lf_ctag_pt4, "h_lf_ctag_pt4");
//    plot(h_lf_ctag_mass1, "h_lf_ctag_mass1");
//    plot(h_lf_ctag_mass2, "h_lf_ctag_mass2");
//    plot(h_lf_ctag_mass3, "h_lf_ctag_mass3");
//    plot(h_lf_ctag_mass4, "h_lf_ctag_mass4");
//    plot(h_lf_ctag_eta1, "h_lf_ctag_eta1");
//    plot(h_lf_ctag_eta2, "h_lf_ctag_eta2");
//    plot(h_lf_ctag_eta3, "h_lf_ctag_eta3");
//    plot(h_lf_ctag_eta4, "h_lf_ctag_eta4");
//    plot(h_lf_ctag_phi1, "h_lf_ctag_phi1");
//    plot(h_lf_ctag_phi2, "h_lf_ctag_phi2");
//    plot(h_lf_ctag_phi3, "h_lf_ctag_phi3");
//    plot(h_lf_ctag_phi4, "h_lf_ctag_phi4");
//    plot(h_lf_ctag_CvsL1, "h_lf_ctag_CvsL1");
//    plot(h_lf_ctag_CvsL2, "h_lf_ctag_CvsL2");
//    plot(h_lf_ctag_CvsL3, "h_lf_ctag_CvsL3");
//    plot(h_lf_ctag_CvsL4, "h_lf_ctag_CvsL4");
//    plot(h_lf_ctag_CvsB1, "h_lf_ctag_CvsB1");
//    plot(h_lf_ctag_CvsB2, "h_lf_ctag_CvsB2");
//    plot(h_lf_ctag_CvsB3, "h_lf_ctag_CvsB3");
//    plot(h_lf_ctag_CvsB4, "h_lf_ctag_CvsB4");
//    plot(h_lf_ctag_Ctag1, "h_lf_ctag_Ctag1");
//    plot(h_lf_ctag_Ctag2, "h_lf_ctag_Ctag2");
//    plot(h_lf_ctag_Ctag3, "h_lf_ctag_Ctag3");
//    plot(h_lf_ctag_Ctag4, "h_lf_ctag_Ctag4");
//    plot(h_lf_cvsl_pt1, "h_lf_cvsl_pt1");
//    plot(h_lf_cvsl_pt2, "h_lf_cvsl_pt2");
//    plot(h_lf_cvsl_pt3, "h_lf_cvsl_pt3");
//    plot(h_lf_cvsl_pt4, "h_lf_cvsl_pt4");
//    plot(h_lf_cvsl_mass1, "h_lf_cvsl_mass1");
//    plot(h_lf_cvsl_mass2, "h_lf_cvsl_mass2");
//    plot(h_lf_cvsl_mass3, "h_lf_cvsl_mass3");
//    plot(h_lf_cvsl_mass4, "h_lf_cvsl_mass4");
//    plot(h_lf_cvsl_eta1, "h_lf_cvsl_eta1");
//    plot(h_lf_cvsl_eta2, "h_lf_cvsl_eta2");
//    plot(h_lf_cvsl_eta3, "h_lf_cvsl_eta3");
//    plot(h_lf_cvsl_eta4, "h_lf_cvsl_eta4");
//    plot(h_lf_cvsl_phi1, "h_lf_cvsl_phi1");
//    plot(h_lf_cvsl_phi2, "h_lf_cvsl_phi2");
//    plot(h_lf_cvsl_phi3, "h_lf_cvsl_phi3");
//    plot(h_lf_cvsl_phi4, "h_lf_cvsl_phi4");
//    plot(h_lf_cvsl_CvsL1, "h_lf_cvsl_CvsL1");
//    plot(h_lf_cvsl_CvsL2, "h_lf_cvsl_CvsL2");
//    plot(h_lf_cvsl_CvsL3, "h_lf_cvsl_CvsL3");
//    plot(h_lf_cvsl_CvsL4, "h_lf_cvsl_CvsL4");
//    plot(h_lf_cvsl_CvsB1, "h_lf_cvsl_CvsB1");
//    plot(h_lf_cvsl_CvsB2, "h_lf_cvsl_CvsB2");
//    plot(h_lf_cvsl_CvsB3, "h_lf_cvsl_CvsB3");
//    plot(h_lf_cvsl_CvsB4, "h_lf_cvsl_CvsB4");
//    plot(h_lf_cvsl_Ctag1, "h_lf_cvsl_Ctag1");
//    plot(h_lf_cvsl_Ctag2, "h_lf_cvsl_Ctag2");
//    plot(h_lf_cvsl_Ctag3, "h_lf_cvsl_Ctag3");
//    plot(h_lf_cvsl_Ctag4, "h_lf_cvsl_Ctag4");
//    plot(h_lf_cvsb_pt1, "h_lf_cvsb_pt1");
//    plot(h_lf_cvsb_pt2, "h_lf_cvsb_pt2");
//    plot(h_lf_cvsb_pt3, "h_lf_cvsb_pt3");
//    plot(h_lf_cvsb_pt4, "h_lf_cvsb_pt4");
//    plot(h_lf_cvsb_mass1, "h_lf_cvsb_mass1");
//    plot(h_lf_cvsb_mass2, "h_lf_cvsb_mass2");
//    plot(h_lf_cvsb_mass3, "h_lf_cvsb_mass3");
//    plot(h_lf_cvsb_mass4, "h_lf_cvsb_mass4");
//    plot(h_lf_cvsb_eta1, "h_lf_cvsb_eta1");
//    plot(h_lf_cvsb_eta2, "h_lf_cvsb_eta2");
//    plot(h_lf_cvsb_eta3, "h_lf_cvsb_eta3");
//    plot(h_lf_cvsb_eta4, "h_lf_cvsb_eta4");
//    plot(h_lf_cvsb_phi1, "h_lf_cvsb_phi1");
//    plot(h_lf_cvsb_phi2, "h_lf_cvsb_phi2");
//    plot(h_lf_cvsb_phi3, "h_lf_cvsb_phi3");
//    plot(h_lf_cvsb_phi4, "h_lf_cvsb_phi4");
//    plot(h_lf_cvsb_CvsL1, "h_lf_cvsb_CvsL1");
//    plot(h_lf_cvsb_CvsL2, "h_lf_cvsb_CvsL2");
//    plot(h_lf_cvsb_CvsL3, "h_lf_cvsb_CvsL3");
//    plot(h_lf_cvsb_CvsL4, "h_lf_cvsb_CvsL4");
//    plot(h_lf_cvsb_CvsB1, "h_lf_cvsb_CvsB1");
//    plot(h_lf_cvsb_CvsB2, "h_lf_cvsb_CvsB2");
//    plot(h_lf_cvsb_CvsB3, "h_lf_cvsb_CvsB3");
//    plot(h_lf_cvsb_CvsB4, "h_lf_cvsb_CvsB4");
//    plot(h_lf_cvsb_Ctag1, "h_lf_cvsb_Ctag1");
//    plot(h_lf_cvsb_Ctag2, "h_lf_cvsb_Ctag2");
//    plot(h_lf_cvsb_Ctag3, "h_lf_cvsb_Ctag3");
//    plot(h_lf_cvsb_Ctag4, "h_lf_cvsb_Ctag4");
//
//    plot(h_b_ctag_pt1, "h_b_ctag_pt1");
//    plot(h_b_ctag_pt2, "h_b_ctag_pt2");
//    plot(h_b_ctag_pt3, "h_b_ctag_pt3");
//    plot(h_b_ctag_pt4, "h_b_ctag_pt4");
//    plot(h_b_ctag_mass1, "h_b_ctag_mass1");
//    plot(h_b_ctag_mass2, "h_b_ctag_mass2");
//    plot(h_b_ctag_mass3, "h_b_ctag_mass3");
//    plot(h_b_ctag_mass4, "h_b_ctag_mass4");
//    plot(h_b_ctag_eta1, "h_b_ctag_eta1");
//    plot(h_b_ctag_eta2, "h_b_ctag_eta2");
//    plot(h_b_ctag_eta3, "h_b_ctag_eta3");
//    plot(h_b_ctag_eta4, "h_b_ctag_eta4");
//    plot(h_b_ctag_phi1, "h_b_ctag_phi1");
//    plot(h_b_ctag_phi2, "h_b_ctag_phi2");
//    plot(h_b_ctag_phi3, "h_b_ctag_phi3");
//    plot(h_b_ctag_phi4, "h_b_ctag_phi4");
//    plot(h_b_ctag_CvsL1, "h_b_ctag_CvsL1");
//    plot(h_b_ctag_CvsL2, "h_b_ctag_CvsL2");
//    plot(h_b_ctag_CvsL3, "h_b_ctag_CvsL3");
//    plot(h_b_ctag_CvsL4, "h_b_ctag_CvsL4");
//    plot(h_b_ctag_CvsB1, "h_b_ctag_CvsB1");
//    plot(h_b_ctag_CvsB2, "h_b_ctag_CvsB2");
//    plot(h_b_ctag_CvsB3, "h_b_ctag_CvsB3");
//    plot(h_b_ctag_CvsB4, "h_b_ctag_CvsB4");
//    plot(h_b_ctag_Ctag1, "h_b_ctag_Ctag1");
//    plot(h_b_ctag_Ctag2, "h_b_ctag_Ctag2");
//    plot(h_b_ctag_Ctag3, "h_b_ctag_Ctag3");
//    plot(h_b_ctag_Ctag4, "h_b_ctag_Ctag4");
//    plot(h_b_cvsl_pt1, "h_b_cvsl_pt1");
//    plot(h_b_cvsl_pt2, "h_b_cvsl_pt2");
//    plot(h_b_cvsl_pt3, "h_b_cvsl_pt3");
//    plot(h_b_cvsl_pt4, "h_b_cvsl_pt4");
//    plot(h_b_cvsl_mass1, "h_b_cvsl_mass1");
//    plot(h_b_cvsl_mass2, "h_b_cvsl_mass2");
//    plot(h_b_cvsl_mass3, "h_b_cvsl_mass3");
//    plot(h_b_cvsl_mass4, "h_b_cvsl_mass4");
//    plot(h_b_cvsl_eta1, "h_b_cvsl_eta1");
//    plot(h_b_cvsl_eta2, "h_b_cvsl_eta2");
//    plot(h_b_cvsl_eta3, "h_b_cvsl_eta3");
//    plot(h_b_cvsl_eta4, "h_b_cvsl_eta4");
//    plot(h_b_cvsl_phi1, "h_b_cvsl_phi1");
//    plot(h_b_cvsl_phi2, "h_b_cvsl_phi2");
//    plot(h_b_cvsl_phi3, "h_b_cvsl_phi3");
//    plot(h_b_cvsl_phi4, "h_b_cvsl_phi4");
//    plot(h_b_cvsl_CvsL1, "h_b_cvsl_CvsL1");
//    plot(h_b_cvsl_CvsL2, "h_b_cvsl_CvsL2");
//    plot(h_b_cvsl_CvsL3, "h_b_cvsl_CvsL3");
//    plot(h_b_cvsl_CvsL4, "h_b_cvsl_CvsL4");
//    plot(h_b_cvsl_CvsB1, "h_b_cvsl_CvsB1");
//    plot(h_b_cvsl_CvsB2, "h_b_cvsl_CvsB2");
//    plot(h_b_cvsl_CvsB3, "h_b_cvsl_CvsB3");
//    plot(h_b_cvsl_CvsB4, "h_b_cvsl_CvsB4");
//    plot(h_b_cvsl_Ctag1, "h_b_cvsl_Ctag1");
//    plot(h_b_cvsl_Ctag2, "h_b_cvsl_Ctag2");
//    plot(h_b_cvsl_Ctag3, "h_b_cvsl_Ctag3");
//    plot(h_b_cvsl_Ctag4, "h_b_cvsl_Ctag4");
//    plot(h_b_cvsb_pt1, "h_b_cvsb_pt1");
//    plot(h_b_cvsb_pt2, "h_b_cvsb_pt2");
//    plot(h_b_cvsb_pt3, "h_b_cvsb_pt3");
//    plot(h_b_cvsb_pt4, "h_b_cvsb_pt4");
//    plot(h_b_cvsb_mass1, "h_b_cvsb_mass1");
//    plot(h_b_cvsb_mass2, "h_b_cvsb_mass2");
//    plot(h_b_cvsb_mass3, "h_b_cvsb_mass3");
//    plot(h_b_cvsb_mass4, "h_b_cvsb_mass4");
//    plot(h_b_cvsb_eta1, "h_b_cvsb_eta1");
//    plot(h_b_cvsb_eta2, "h_b_cvsb_eta2");
//    plot(h_b_cvsb_eta3, "h_b_cvsb_eta3");
//    plot(h_b_cvsb_eta4, "h_b_cvsb_eta4");
//    plot(h_b_cvsb_phi1, "h_b_cvsb_phi1");
//    plot(h_b_cvsb_phi2, "h_b_cvsb_phi2");
//    plot(h_b_cvsb_phi3, "h_b_cvsb_phi3");
//    plot(h_b_cvsb_phi4, "h_b_cvsb_phi4");
//    plot(h_b_cvsb_CvsL1, "h_b_cvsb_CvsL1");
//    plot(h_b_cvsb_CvsL2, "h_b_cvsb_CvsL2");
//    plot(h_b_cvsb_CvsL3, "h_b_cvsb_CvsL3");
//    plot(h_b_cvsb_CvsL4, "h_b_cvsb_CvsL4");
//    plot(h_b_cvsb_CvsB1, "h_b_cvsb_CvsB1");
//    plot(h_b_cvsb_CvsB2, "h_b_cvsb_CvsB2");
//    plot(h_b_cvsb_CvsB3, "h_b_cvsb_CvsB3");
//    plot(h_b_cvsb_CvsB4, "h_b_cvsb_CvsB4");
//    plot(h_b_cvsb_Ctag1, "h_b_cvsb_Ctag1");
//    plot(h_b_cvsb_Ctag2, "h_b_cvsb_Ctag2");
//    plot(h_b_cvsb_Ctag3, "h_b_cvsb_Ctag3");
//    plot(h_b_cvsb_Ctag4, "h_b_cvsb_Ctag4");
//
//    plot(h_bb_ctag_pt1, "h_bb_ctag_pt1");
//    plot(h_bb_ctag_pt2, "h_bb_ctag_pt2");
//    plot(h_bb_ctag_pt3, "h_bb_ctag_pt3");
//    plot(h_bb_ctag_pt4, "h_bb_ctag_pt4");
//    plot(h_bb_ctag_mass1, "h_bb_ctag_mass1");
//    plot(h_bb_ctag_mass2, "h_bb_ctag_mass2");
//    plot(h_bb_ctag_mass3, "h_bb_ctag_mass3");
//    plot(h_bb_ctag_mass4, "h_bb_ctag_mass4");
//    plot(h_bb_ctag_eta1, "h_bb_ctag_eta1");
//    plot(h_bb_ctag_eta2, "h_bb_ctag_eta2");
//    plot(h_bb_ctag_eta3, "h_bb_ctag_eta3");
//    plot(h_bb_ctag_eta4, "h_bb_ctag_eta4");
//    plot(h_bb_ctag_phi1, "h_bb_ctag_phi1");
//    plot(h_bb_ctag_phi2, "h_bb_ctag_phi2");
//    plot(h_bb_ctag_phi3, "h_bb_ctag_phi3");
//    plot(h_bb_ctag_phi4, "h_bb_ctag_phi4");
//    plot(h_bb_ctag_CvsL1, "h_bb_ctag_CvsL1");
//    plot(h_bb_ctag_CvsL2, "h_bb_ctag_CvsL2");
//    plot(h_bb_ctag_CvsL3, "h_bb_ctag_CvsL3");
//    plot(h_bb_ctag_CvsL4, "h_bb_ctag_CvsL4");
//    plot(h_bb_ctag_CvsB1, "h_bb_ctag_CvsB1");
//    plot(h_bb_ctag_CvsB2, "h_bb_ctag_CvsB2");
//    plot(h_bb_ctag_CvsB3, "h_bb_ctag_CvsB3");
//    plot(h_bb_ctag_CvsB4, "h_bb_ctag_CvsB4");
//    plot(h_bb_ctag_Ctag1, "h_bb_ctag_Ctag1");
//    plot(h_bb_ctag_Ctag2, "h_bb_ctag_Ctag2");
//    plot(h_bb_ctag_Ctag3, "h_bb_ctag_Ctag3");
//    plot(h_bb_ctag_Ctag4, "h_bb_ctag_Ctag4");
//    plot(h_bb_cvsl_pt1, "h_bb_cvsl_pt1");
//    plot(h_bb_cvsl_pt2, "h_bb_cvsl_pt2");
//    plot(h_bb_cvsl_pt3, "h_bb_cvsl_pt3");
//    plot(h_bb_cvsl_pt4, "h_bb_cvsl_pt4");
//    plot(h_bb_cvsl_mass1, "h_bb_cvsl_mass1");
//    plot(h_bb_cvsl_mass2, "h_bb_cvsl_mass2");
//    plot(h_bb_cvsl_mass3, "h_bb_cvsl_mass3");
//    plot(h_bb_cvsl_mass4, "h_bb_cvsl_mass4");
//    plot(h_bb_cvsl_eta1, "h_bb_cvsl_eta1");
//    plot(h_bb_cvsl_eta2, "h_bb_cvsl_eta2");
//    plot(h_bb_cvsl_eta3, "h_bb_cvsl_eta3");
//    plot(h_bb_cvsl_eta4, "h_bb_cvsl_eta4");
//    plot(h_bb_cvsl_phi1, "h_bb_cvsl_phi1");
//    plot(h_bb_cvsl_phi2, "h_bb_cvsl_phi2");
//    plot(h_bb_cvsl_phi3, "h_bb_cvsl_phi3");
//    plot(h_bb_cvsl_phi4, "h_bb_cvsl_phi4");
//    plot(h_bb_cvsl_CvsL1, "h_bb_cvsl_CvsL1");
//    plot(h_bb_cvsl_CvsL2, "h_bb_cvsl_CvsL2");
//    plot(h_bb_cvsl_CvsL3, "h_bb_cvsl_CvsL3");
//    plot(h_bb_cvsl_CvsL4, "h_bb_cvsl_CvsL4");
//    plot(h_bb_cvsl_CvsB1, "h_bb_cvsl_CvsB1");
//    plot(h_bb_cvsl_CvsB2, "h_bb_cvsl_CvsB2");
//    plot(h_bb_cvsl_CvsB3, "h_bb_cvsl_CvsB3");
//    plot(h_bb_cvsl_CvsB4, "h_bb_cvsl_CvsB4");
//    plot(h_bb_cvsl_Ctag1, "h_bb_cvsl_Ctag1");
//    plot(h_bb_cvsl_Ctag2, "h_bb_cvsl_Ctag2");
//    plot(h_bb_cvsl_Ctag3, "h_bb_cvsl_Ctag3");
//    plot(h_bb_cvsl_Ctag4, "h_bb_cvsl_Ctag4");
//    plot(h_bb_cvsb_pt1, "h_bb_cvsb_pt1");
//    plot(h_bb_cvsb_pt2, "h_bb_cvsb_pt2");
//    plot(h_bb_cvsb_pt3, "h_bb_cvsb_pt3");
//    plot(h_bb_cvsb_pt4, "h_bb_cvsb_pt4");
//    plot(h_bb_cvsb_mass1, "h_bb_cvsb_mass1");
//    plot(h_bb_cvsb_mass2, "h_bb_cvsb_mass2");
//    plot(h_bb_cvsb_mass3, "h_bb_cvsb_mass3");
//    plot(h_bb_cvsb_mass4, "h_bb_cvsb_mass4");
//    plot(h_bb_cvsb_eta1, "h_bb_cvsb_eta1");
//    plot(h_bb_cvsb_eta2, "h_bb_cvsb_eta2");
//    plot(h_bb_cvsb_eta3, "h_bb_cvsb_eta3");
//    plot(h_bb_cvsb_eta4, "h_bb_cvsb_eta4");
//    plot(h_bb_cvsb_phi1, "h_bb_cvsb_phi1");
//    plot(h_bb_cvsb_phi2, "h_bb_cvsb_phi2");
//    plot(h_bb_cvsb_phi3, "h_bb_cvsb_phi3");
//    plot(h_bb_cvsb_phi4, "h_bb_cvsb_phi4");
//    plot(h_bb_cvsb_CvsL1, "h_bb_cvsb_CvsL1");
//    plot(h_bb_cvsb_CvsL2, "h_bb_cvsb_CvsL2");
//    plot(h_bb_cvsb_CvsL3, "h_bb_cvsb_CvsL3");
//    plot(h_bb_cvsb_CvsL4, "h_bb_cvsb_CvsL4");
//    plot(h_bb_cvsb_CvsB1, "h_bb_cvsb_CvsB1");
//    plot(h_bb_cvsb_CvsB2, "h_bb_cvsb_CvsB2");
//    plot(h_bb_cvsb_CvsB3, "h_bb_cvsb_CvsB3");
//    plot(h_bb_cvsb_CvsB4, "h_bb_cvsb_CvsB4");
//    plot(h_bb_cvsb_Ctag1, "h_bb_cvsb_Ctag1");
//    plot(h_bb_cvsb_Ctag2, "h_bb_cvsb_Ctag2");
//    plot(h_bb_cvsb_Ctag3, "h_bb_cvsb_Ctag3");
//    plot(h_bb_cvsb_Ctag4, "h_bb_cvsb_Ctag4");
//
//    plot(h_c_ctag_pt1, "h_c_ctag_pt1");
//    plot(h_c_ctag_pt2, "h_c_ctag_pt2");
//    plot(h_c_ctag_pt3, "h_c_ctag_pt3");
//    plot(h_c_ctag_pt4, "h_c_ctag_pt4");
//    plot(h_c_ctag_mass1, "h_c_ctag_mass1");
//    plot(h_c_ctag_mass2, "h_c_ctag_mass2");
//    plot(h_c_ctag_mass3, "h_c_ctag_mass3");
//    plot(h_c_ctag_mass4, "h_c_ctag_mass4");
//    plot(h_c_ctag_eta1, "h_c_ctag_eta1");
//    plot(h_c_ctag_eta2, "h_c_ctag_eta2");
//    plot(h_c_ctag_eta3, "h_c_ctag_eta3");
//    plot(h_c_ctag_eta4, "h_c_ctag_eta4");
//    plot(h_c_ctag_phi1, "h_c_ctag_phi1");
//    plot(h_c_ctag_phi2, "h_c_ctag_phi2");
//    plot(h_c_ctag_phi3, "h_c_ctag_phi3");
//    plot(h_c_ctag_phi4, "h_c_ctag_phi4");
//    plot(h_c_ctag_CvsL1, "h_c_ctag_CvsL1");
//    plot(h_c_ctag_CvsL2, "h_c_ctag_CvsL2");
//    plot(h_c_ctag_CvsL3, "h_c_ctag_CvsL3");
//    plot(h_c_ctag_CvsL4, "h_c_ctag_CvsL4");
//    plot(h_c_ctag_CvsB1, "h_c_ctag_CvsB1");
//    plot(h_c_ctag_CvsB2, "h_c_ctag_CvsB2");
//    plot(h_c_ctag_CvsB3, "h_c_ctag_CvsB3");
//    plot(h_c_ctag_CvsB4, "h_c_ctag_CvsB4");
//    plot(h_c_ctag_Ctag1, "h_c_ctag_Ctag1");
//    plot(h_c_ctag_Ctag2, "h_c_ctag_Ctag2");
//    plot(h_c_ctag_Ctag3, "h_c_ctag_Ctag3");
//    plot(h_c_ctag_Ctag4, "h_c_ctag_Ctag4");
//    plot(h_c_cvsl_pt1, "h_c_cvsl_pt1");
//    plot(h_c_cvsl_pt2, "h_c_cvsl_pt2");
//    plot(h_c_cvsl_pt3, "h_c_cvsl_pt3");
//    plot(h_c_cvsl_pt4, "h_c_cvsl_pt4");
//    plot(h_c_cvsl_mass1, "h_c_cvsl_mass1");
//    plot(h_c_cvsl_mass2, "h_c_cvsl_mass2");
//    plot(h_c_cvsl_mass3, "h_c_cvsl_mass3");
//    plot(h_c_cvsl_mass4, "h_c_cvsl_mass4");
//    plot(h_c_cvsl_eta1, "h_c_cvsl_eta1");
//    plot(h_c_cvsl_eta2, "h_c_cvsl_eta2");
//    plot(h_c_cvsl_eta3, "h_c_cvsl_eta3");
//    plot(h_c_cvsl_eta4, "h_c_cvsl_eta4");
//    plot(h_c_cvsl_phi1, "h_c_cvsl_phi1");
//    plot(h_c_cvsl_phi2, "h_c_cvsl_phi2");
//    plot(h_c_cvsl_phi3, "h_c_cvsl_phi3");
//    plot(h_c_cvsl_phi4, "h_c_cvsl_phi4");
//    plot(h_c_cvsl_CvsL1, "h_c_cvsl_CvsL1");
//    plot(h_c_cvsl_CvsL2, "h_c_cvsl_CvsL2");
//    plot(h_c_cvsl_CvsL3, "h_c_cvsl_CvsL3");
//    plot(h_c_cvsl_CvsL4, "h_c_cvsl_CvsL4");
//    plot(h_c_cvsl_CvsB1, "h_c_cvsl_CvsB1");
//    plot(h_c_cvsl_CvsB2, "h_c_cvsl_CvsB2");
//    plot(h_c_cvsl_CvsB3, "h_c_cvsl_CvsB3");
//    plot(h_c_cvsl_CvsB4, "h_c_cvsl_CvsB4");
//    plot(h_c_cvsl_Ctag1, "h_c_cvsl_Ctag1");
//    plot(h_c_cvsl_Ctag2, "h_c_cvsl_Ctag2");
//    plot(h_c_cvsl_Ctag3, "h_c_cvsl_Ctag3");
//    plot(h_c_cvsl_Ctag4, "h_c_cvsl_Ctag4");
//    plot(h_c_cvsb_pt1, "h_c_cvsb_pt1");
//    plot(h_c_cvsb_pt2, "h_c_cvsb_pt2");
//    plot(h_c_cvsb_pt3, "h_c_cvsb_pt3");
//    plot(h_c_cvsb_pt4, "h_c_cvsb_pt4");
//    plot(h_c_cvsb_mass1, "h_c_cvsb_mass1");
//    plot(h_c_cvsb_mass2, "h_c_cvsb_mass2");
//    plot(h_c_cvsb_mass3, "h_c_cvsb_mass3");
//    plot(h_c_cvsb_mass4, "h_c_cvsb_mass4");
//    plot(h_c_cvsb_eta1, "h_c_cvsb_eta1");
//    plot(h_c_cvsb_eta2, "h_c_cvsb_eta2");
//    plot(h_c_cvsb_eta3, "h_c_cvsb_eta3");
//    plot(h_c_cvsb_eta4, "h_c_cvsb_eta4");
//    plot(h_c_cvsb_phi1, "h_c_cvsb_phi1");
//    plot(h_c_cvsb_phi2, "h_c_cvsb_phi2");
//    plot(h_c_cvsb_phi3, "h_c_cvsb_phi3");
//    plot(h_c_cvsb_phi4, "h_c_cvsb_phi4");
//    plot(h_c_cvsb_CvsL1, "h_c_cvsb_CvsL1");
//    plot(h_c_cvsb_CvsL2, "h_c_cvsb_CvsL2");
//    plot(h_c_cvsb_CvsL3, "h_c_cvsb_CvsL3");
//    plot(h_c_cvsb_CvsL4, "h_c_cvsb_CvsL4");
//    plot(h_c_cvsb_CvsB1, "h_c_cvsb_CvsB1");
//    plot(h_c_cvsb_CvsB2, "h_c_cvsb_CvsB2");
//    plot(h_c_cvsb_CvsB3, "h_c_cvsb_CvsB3");
//    plot(h_c_cvsb_CvsB4, "h_c_cvsb_CvsB4");
//    plot(h_c_cvsb_Ctag1, "h_c_cvsb_Ctag1");
//    plot(h_c_cvsb_Ctag2, "h_c_cvsb_Ctag2");
//    plot(h_c_cvsb_Ctag3, "h_c_cvsb_Ctag3");
//    plot(h_c_cvsb_Ctag4, "h_c_cvsb_Ctag4");
//
//    plot(h_cc_ctag_pt1, "h_cc_ctag_pt1");
//    plot(h_cc_ctag_pt2, "h_cc_ctag_pt2");
//    plot(h_cc_ctag_pt3, "h_cc_ctag_pt3");
//    plot(h_cc_ctag_pt4, "h_cc_ctag_pt4");
//    plot(h_cc_ctag_mass1, "h_cc_ctag_mass1");
//    plot(h_cc_ctag_mass2, "h_cc_ctag_mass2");
//    plot(h_cc_ctag_mass3, "h_cc_ctag_mass3");
//    plot(h_cc_ctag_mass4, "h_cc_ctag_mass4");
//    plot(h_cc_ctag_eta1, "h_cc_ctag_eta1");
//    plot(h_cc_ctag_eta2, "h_cc_ctag_eta2");
//    plot(h_cc_ctag_eta3, "h_cc_ctag_eta3");
//    plot(h_cc_ctag_eta4, "h_cc_ctag_eta4");
//    plot(h_cc_ctag_phi1, "h_cc_ctag_phi1");
//    plot(h_cc_ctag_phi2, "h_cc_ctag_phi2");
//    plot(h_cc_ctag_phi3, "h_cc_ctag_phi3");
//    plot(h_cc_ctag_phi4, "h_cc_ctag_phi4");
//    plot(h_cc_ctag_CvsL1, "h_cc_ctag_CvsL1");
//    plot(h_cc_ctag_CvsL2, "h_cc_ctag_CvsL2");
//    plot(h_cc_ctag_CvsL3, "h_cc_ctag_CvsL3");
//    plot(h_cc_ctag_CvsL4, "h_cc_ctag_CvsL4");
//    plot(h_cc_ctag_CvsB1, "h_cc_ctag_CvsB1");
//    plot(h_cc_ctag_CvsB2, "h_cc_ctag_CvsB2");
//    plot(h_cc_ctag_CvsB3, "h_cc_ctag_CvsB3");
//    plot(h_cc_ctag_CvsB4, "h_cc_ctag_CvsB4");
//    plot(h_cc_ctag_Ctag1, "h_cc_ctag_Ctag1");
//    plot(h_cc_ctag_Ctag2, "h_cc_ctag_Ctag2");
//    plot(h_cc_ctag_Ctag3, "h_cc_ctag_Ctag3");
//    plot(h_cc_ctag_Ctag4, "h_cc_ctag_Ctag4");
//    plot(h_cc_cvsl_pt1, "h_cc_cvsl_pt1");
//    plot(h_cc_cvsl_pt2, "h_cc_cvsl_pt2");
//    plot(h_cc_cvsl_pt3, "h_cc_cvsl_pt3");
//    plot(h_cc_cvsl_pt4, "h_cc_cvsl_pt4");
//    plot(h_cc_cvsl_mass1, "h_cc_cvsl_mass1");
//    plot(h_cc_cvsl_mass2, "h_cc_cvsl_mass2");
//    plot(h_cc_cvsl_mass3, "h_cc_cvsl_mass3");
//    plot(h_cc_cvsl_mass4, "h_cc_cvsl_mass4");
//    plot(h_cc_cvsl_eta1, "h_cc_cvsl_eta1");
//    plot(h_cc_cvsl_eta2, "h_cc_cvsl_eta2");
//    plot(h_cc_cvsl_eta3, "h_cc_cvsl_eta3");
//    plot(h_cc_cvsl_eta4, "h_cc_cvsl_eta4");
//    plot(h_cc_cvsl_phi1, "h_cc_cvsl_phi1");
//    plot(h_cc_cvsl_phi2, "h_cc_cvsl_phi2");
//    plot(h_cc_cvsl_phi3, "h_cc_cvsl_phi3");
//    plot(h_cc_cvsl_phi4, "h_cc_cvsl_phi4");
//    plot(h_cc_cvsl_CvsL1, "h_cc_cvsl_CvsL1");
//    plot(h_cc_cvsl_CvsL2, "h_cc_cvsl_CvsL2");
//    plot(h_cc_cvsl_CvsL3, "h_cc_cvsl_CvsL3");
//    plot(h_cc_cvsl_CvsL4, "h_cc_cvsl_CvsL4");
//    plot(h_cc_cvsl_CvsB1, "h_cc_cvsl_CvsB1");
//    plot(h_cc_cvsl_CvsB2, "h_cc_cvsl_CvsB2");
//    plot(h_cc_cvsl_CvsB3, "h_cc_cvsl_CvsB3");
//    plot(h_cc_cvsl_CvsB4, "h_cc_cvsl_CvsB4");
//    plot(h_cc_cvsl_Ctag1, "h_cc_cvsl_Ctag1");
//    plot(h_cc_cvsl_Ctag2, "h_cc_cvsl_Ctag2");
//    plot(h_cc_cvsl_Ctag3, "h_cc_cvsl_Ctag3");
//    plot(h_cc_cvsl_Ctag4, "h_cc_cvsl_Ctag4");
//    plot(h_cc_cvsb_pt1, "h_cc_cvsb_pt1");
//    plot(h_cc_cvsb_pt2, "h_cc_cvsb_pt2");
//    plot(h_cc_cvsb_pt3, "h_cc_cvsb_pt3");
//    plot(h_cc_cvsb_pt4, "h_cc_cvsb_pt4");
//    plot(h_cc_cvsb_mass1, "h_cc_cvsb_mass1");
//    plot(h_cc_cvsb_mass2, "h_cc_cvsb_mass2");
//    plot(h_cc_cvsb_mass3, "h_cc_cvsb_mass3");
//    plot(h_cc_cvsb_mass4, "h_cc_cvsb_mass4");
//    plot(h_cc_cvsb_eta1, "h_cc_cvsb_eta1");
//    plot(h_cc_cvsb_eta2, "h_cc_cvsb_eta2");
//    plot(h_cc_cvsb_eta3, "h_cc_cvsb_eta3");
//    plot(h_cc_cvsb_eta4, "h_cc_cvsb_eta4");
//    plot(h_cc_cvsb_phi1, "h_cc_cvsb_phi1");
//    plot(h_cc_cvsb_phi2, "h_cc_cvsb_phi2");
//    plot(h_cc_cvsb_phi3, "h_cc_cvsb_phi3");
//    plot(h_cc_cvsb_phi4, "h_cc_cvsb_phi4");
//    plot(h_cc_cvsb_CvsL1, "h_cc_cvsb_CvsL1");
//    plot(h_cc_cvsb_CvsL2, "h_cc_cvsb_CvsL2");
//    plot(h_cc_cvsb_CvsL3, "h_cc_cvsb_CvsL3");
//    plot(h_cc_cvsb_CvsL4, "h_cc_cvsb_CvsL4");
//    plot(h_cc_cvsb_CvsB1, "h_cc_cvsb_CvsB1");
//    plot(h_cc_cvsb_CvsB2, "h_cc_cvsb_CvsB2");
//    plot(h_cc_cvsb_CvsB3, "h_cc_cvsb_CvsB3");
//    plot(h_cc_cvsb_CvsB4, "h_cc_cvsb_CvsB4");
//    plot(h_cc_cvsb_Ctag1, "h_cc_cvsb_Ctag1");
//    plot(h_cc_cvsb_Ctag2, "h_cc_cvsb_Ctag2");
//    plot(h_cc_cvsb_Ctag3, "h_cc_cvsb_Ctag3");
//    plot(h_cc_cvsb_Ctag4, "h_cc_cvsb_Ctag4");
//
//    plot(h_sig_ctag_pt1, "h_sig_ctag_pt1");
//    plot(h_sig_ctag_pt2, "h_sig_ctag_pt2");
//    plot(h_sig_ctag_pt3, "h_sig_ctag_pt3");
//    plot(h_sig_ctag_pt4, "h_sig_ctag_pt4");
//    plot(h_sig_ctag_mass1, "h_sig_ctag_mass1");
//    plot(h_sig_ctag_mass2, "h_sig_ctag_mass2");
//    plot(h_sig_ctag_mass3, "h_sig_ctag_mass3");
//    plot(h_sig_ctag_mass4, "h_sig_ctag_mass4");
//    plot(h_sig_ctag_eta1, "h_sig_ctag_eta1");
//    plot(h_sig_ctag_eta2, "h_sig_ctag_eta2");
//    plot(h_sig_ctag_eta3, "h_sig_ctag_eta3");
//    plot(h_sig_ctag_eta4, "h_sig_ctag_eta4");
//    plot(h_sig_ctag_phi1, "h_sig_ctag_phi1");
//    plot(h_sig_ctag_phi2, "h_sig_ctag_phi2");
//    plot(h_sig_ctag_phi3, "h_sig_ctag_phi3");
//    plot(h_sig_ctag_phi4, "h_sig_ctag_phi4");
//    plot(h_sig_ctag_CvsL1, "h_sig_ctag_CvsL1");
//    plot(h_sig_ctag_CvsL2, "h_sig_ctag_CvsL2");
//    plot(h_sig_ctag_CvsL3, "h_sig_ctag_CvsL3");
//    plot(h_sig_ctag_CvsL4, "h_sig_ctag_CvsL4");
//    plot(h_sig_ctag_CvsB1, "h_sig_ctag_CvsB1");
//    plot(h_sig_ctag_CvsB2, "h_sig_ctag_CvsB2");
//    plot(h_sig_ctag_CvsB3, "h_sig_ctag_CvsB3");
//    plot(h_sig_ctag_CvsB4, "h_sig_ctag_CvsB4");
//    plot(h_sig_ctag_Ctag1, "h_sig_ctag_Ctag1");
//    plot(h_sig_ctag_Ctag2, "h_sig_ctag_Ctag2");
//    plot(h_sig_ctag_Ctag3, "h_sig_ctag_Ctag3");
//    plot(h_sig_ctag_Ctag4, "h_sig_ctag_Ctag4");
//    plot(h_sig_cvsl_pt1, "h_sig_cvsl_pt1");
//    plot(h_sig_cvsl_pt2, "h_sig_cvsl_pt2");
//    plot(h_sig_cvsl_pt3, "h_sig_cvsl_pt3");
//    plot(h_sig_cvsl_pt4, "h_sig_cvsl_pt4");
//    plot(h_sig_cvsl_mass1, "h_sig_cvsl_mass1");
//    plot(h_sig_cvsl_mass2, "h_sig_cvsl_mass2");
//    plot(h_sig_cvsl_mass3, "h_sig_cvsl_mass3");
//    plot(h_sig_cvsl_mass4, "h_sig_cvsl_mass4");
//    plot(h_sig_cvsl_eta1, "h_sig_cvsl_eta1");
//    plot(h_sig_cvsl_eta2, "h_sig_cvsl_eta2");
//    plot(h_sig_cvsl_eta3, "h_sig_cvsl_eta3");
//    plot(h_sig_cvsl_eta4, "h_sig_cvsl_eta4");
//    plot(h_sig_cvsl_phi1, "h_sig_cvsl_phi1");
//    plot(h_sig_cvsl_phi2, "h_sig_cvsl_phi2");
//    plot(h_sig_cvsl_phi3, "h_sig_cvsl_phi3");
//    plot(h_sig_cvsl_phi4, "h_sig_cvsl_phi4");
//    plot(h_sig_cvsl_CvsL1, "h_sig_cvsl_CvsL1");
//    plot(h_sig_cvsl_CvsL2, "h_sig_cvsl_CvsL2");
//    plot(h_sig_cvsl_CvsL3, "h_sig_cvsl_CvsL3");
//    plot(h_sig_cvsl_CvsL4, "h_sig_cvsl_CvsL4");
//    plot(h_sig_cvsl_CvsB1, "h_sig_cvsl_CvsB1");
//    plot(h_sig_cvsl_CvsB2, "h_sig_cvsl_CvsB2");
//    plot(h_sig_cvsl_CvsB3, "h_sig_cvsl_CvsB3");
//    plot(h_sig_cvsl_CvsB4, "h_sig_cvsl_CvsB4");
//    plot(h_sig_cvsl_Ctag1, "h_sig_cvsl_Ctag1");
//    plot(h_sig_cvsl_Ctag2, "h_sig_cvsl_Ctag2");
//    plot(h_sig_cvsl_Ctag3, "h_sig_cvsl_Ctag3");
//    plot(h_sig_cvsl_Ctag4, "h_sig_cvsl_Ctag4");
//    plot(h_sig_cvsb_pt1, "h_sig_cvsb_pt1");
//    plot(h_sig_cvsb_pt2, "h_sig_cvsb_pt2");
//    plot(h_sig_cvsb_pt3, "h_sig_cvsb_pt3");
//    plot(h_sig_cvsb_pt4, "h_sig_cvsb_pt4");
//    plot(h_sig_cvsb_mass1, "h_sig_cvsb_mass1");
//    plot(h_sig_cvsb_mass2, "h_sig_cvsb_mass2");
//    plot(h_sig_cvsb_mass3, "h_sig_cvsb_mass3");
//    plot(h_sig_cvsb_mass4, "h_sig_cvsb_mass4");
//    plot(h_sig_cvsb_eta1, "h_sig_cvsb_eta1");
//    plot(h_sig_cvsb_eta2, "h_sig_cvsb_eta2");
//    plot(h_sig_cvsb_eta3, "h_sig_cvsb_eta3");
//    plot(h_sig_cvsb_eta4, "h_sig_cvsb_eta4");
//    plot(h_sig_cvsb_phi1, "h_sig_cvsb_phi1");
//    plot(h_sig_cvsb_phi2, "h_sig_cvsb_phi2");
//    plot(h_sig_cvsb_phi3, "h_sig_cvsb_phi3");
//    plot(h_sig_cvsb_phi4, "h_sig_cvsb_phi4");
//    plot(h_sig_cvsb_CvsL1, "h_sig_cvsb_CvsL1");
//    plot(h_sig_cvsb_CvsL2, "h_sig_cvsb_CvsL2");
//    plot(h_sig_cvsb_CvsL3, "h_sig_cvsb_CvsL3");
//    plot(h_sig_cvsb_CvsL4, "h_sig_cvsb_CvsL4");
//    plot(h_sig_cvsb_CvsB1, "h_sig_cvsb_CvsB1");
//    plot(h_sig_cvsb_CvsB2, "h_sig_cvsb_CvsB2");
//    plot(h_sig_cvsb_CvsB3, "h_sig_cvsb_CvsB3");
//    plot(h_sig_cvsb_CvsB4, "h_sig_cvsb_CvsB4");
//    plot(h_sig_cvsb_Ctag1, "h_sig_cvsb_Ctag1");
//    plot(h_sig_cvsb_Ctag2, "h_sig_cvsb_Ctag2");
//    plot(h_sig_cvsb_Ctag3, "h_sig_cvsb_Ctag3");
//    plot(h_sig_cvsb_Ctag4, "h_sig_cvsb_Ctag4");

    plot(h_ttbar_ctag_pt1, "h_ttbar_ctag_pt1");
    plot(h_ttbar_ctag_pt2, "h_ttbar_ctag_pt2");
    plot(h_ttbar_ctag_pt3, "h_ttbar_ctag_pt3");
    plot(h_ttbar_ctag_pt4, "h_ttbar_ctag_pt4");
    plot(h_ttbar_ctag_mass1, "h_ttbar_ctag_mass1");
    plot(h_ttbar_ctag_mass2, "h_ttbar_ctag_mass2");
    plot(h_ttbar_ctag_mass3, "h_ttbar_ctag_mass3");
    plot(h_ttbar_ctag_mass4, "h_ttbar_ctag_mass4");
    plot(h_ttbar_ctag_eta1, "h_ttbar_ctag_eta1");
    plot(h_ttbar_ctag_eta2, "h_ttbar_ctag_eta2");
    plot(h_ttbar_ctag_eta3, "h_ttbar_ctag_eta3");
    plot(h_ttbar_ctag_eta4, "h_ttbar_ctag_eta4");
    plot(h_ttbar_ctag_phi1, "h_ttbar_ctag_phi1");
    plot(h_ttbar_ctag_phi2, "h_ttbar_ctag_phi2");
    plot(h_ttbar_ctag_phi3, "h_ttbar_ctag_phi3");
    plot(h_ttbar_ctag_phi4, "h_ttbar_ctag_phi4");
    plot(h_ttbar_ctag_CvsL1, "h_ttbar_ctag_CvsL1");
    plot(h_ttbar_ctag_CvsL2, "h_ttbar_ctag_CvsL2");
    plot(h_ttbar_ctag_CvsL3, "h_ttbar_ctag_CvsL3");
    plot(h_ttbar_ctag_CvsL4, "h_ttbar_ctag_CvsL4");
    plot(h_ttbar_ctag_CvsB1, "h_ttbar_ctag_CvsB1");
    plot(h_ttbar_ctag_CvsB2, "h_ttbar_ctag_CvsB2");
    plot(h_ttbar_ctag_CvsB3, "h_ttbar_ctag_CvsB3");
    plot(h_ttbar_ctag_CvsB4, "h_ttbar_ctag_CvsB4");
    plot(h_ttbar_ctag_Ctag1, "h_ttbar_ctag_Ctag1");
    plot(h_ttbar_ctag_Ctag2, "h_ttbar_ctag_Ctag2");
    plot(h_ttbar_ctag_Ctag3, "h_ttbar_ctag_Ctag3");
    plot(h_ttbar_ctag_Ctag4, "h_ttbar_ctag_Ctag4");
    plot(h_ttbar_cvsl_pt1, "h_ttbar_cvsl_pt1");
    plot(h_ttbar_cvsl_pt2, "h_ttbar_cvsl_pt2");
    plot(h_ttbar_cvsl_pt3, "h_ttbar_cvsl_pt3");
    plot(h_ttbar_cvsl_pt4, "h_ttbar_cvsl_pt4");
    plot(h_ttbar_cvsl_mass1, "h_ttbar_cvsl_mass1");
    plot(h_ttbar_cvsl_mass2, "h_ttbar_cvsl_mass2");
    plot(h_ttbar_cvsl_mass3, "h_ttbar_cvsl_mass3");
    plot(h_ttbar_cvsl_mass4, "h_ttbar_cvsl_mass4");
    plot(h_ttbar_cvsl_eta1, "h_ttbar_cvsl_eta1");
    plot(h_ttbar_cvsl_eta2, "h_ttbar_cvsl_eta2");
    plot(h_ttbar_cvsl_eta3, "h_ttbar_cvsl_eta3");
    plot(h_ttbar_cvsl_eta4, "h_ttbar_cvsl_eta4");
    plot(h_ttbar_cvsl_phi1, "h_ttbar_cvsl_phi1");
    plot(h_ttbar_cvsl_phi2, "h_ttbar_cvsl_phi2");
    plot(h_ttbar_cvsl_phi3, "h_ttbar_cvsl_phi3");
    plot(h_ttbar_cvsl_phi4, "h_ttbar_cvsl_phi4");
    plot(h_ttbar_cvsl_CvsL1, "h_ttbar_cvsl_CvsL1");
    plot(h_ttbar_cvsl_CvsL2, "h_ttbar_cvsl_CvsL2");
    plot(h_ttbar_cvsl_CvsL3, "h_ttbar_cvsl_CvsL3");
    plot(h_ttbar_cvsl_CvsL4, "h_ttbar_cvsl_CvsL4");
    plot(h_ttbar_cvsl_CvsB1, "h_ttbar_cvsl_CvsB1");
    plot(h_ttbar_cvsl_CvsB2, "h_ttbar_cvsl_CvsB2");
    plot(h_ttbar_cvsl_CvsB3, "h_ttbar_cvsl_CvsB3");
    plot(h_ttbar_cvsl_CvsB4, "h_ttbar_cvsl_CvsB4");
    plot(h_ttbar_cvsl_Ctag1, "h_ttbar_cvsl_Ctag1");
    plot(h_ttbar_cvsl_Ctag2, "h_ttbar_cvsl_Ctag2");
    plot(h_ttbar_cvsl_Ctag3, "h_ttbar_cvsl_Ctag3");
    plot(h_ttbar_cvsl_Ctag4, "h_ttbar_cvsl_Ctag4");
    plot(h_ttbar_cvsb_pt1, "h_ttbar_cvsb_pt1");
    plot(h_ttbar_cvsb_pt2, "h_ttbar_cvsb_pt2");
    plot(h_ttbar_cvsb_pt3, "h_ttbar_cvsb_pt3");
    plot(h_ttbar_cvsb_pt4, "h_ttbar_cvsb_pt4");
    plot(h_ttbar_cvsb_mass1, "h_ttbar_cvsb_mass1");
    plot(h_ttbar_cvsb_mass2, "h_ttbar_cvsb_mass2");
    plot(h_ttbar_cvsb_mass3, "h_ttbar_cvsb_mass3");
    plot(h_ttbar_cvsb_mass4, "h_ttbar_cvsb_mass4");
    plot(h_ttbar_cvsb_eta1, "h_ttbar_cvsb_eta1");
    plot(h_ttbar_cvsb_eta2, "h_ttbar_cvsb_eta2");
    plot(h_ttbar_cvsb_eta3, "h_ttbar_cvsb_eta3");
    plot(h_ttbar_cvsb_eta4, "h_ttbar_cvsb_eta4");
    plot(h_ttbar_cvsb_phi1, "h_ttbar_cvsb_phi1");
    plot(h_ttbar_cvsb_phi2, "h_ttbar_cvsb_phi2");
    plot(h_ttbar_cvsb_phi3, "h_ttbar_cvsb_phi3");
    plot(h_ttbar_cvsb_phi4, "h_ttbar_cvsb_phi4");
    plot(h_ttbar_cvsb_CvsL1, "h_ttbar_cvsb_CvsL1");
    plot(h_ttbar_cvsb_CvsL2, "h_ttbar_cvsb_CvsL2");
    plot(h_ttbar_cvsb_CvsL3, "h_ttbar_cvsb_CvsL3");
    plot(h_ttbar_cvsb_CvsL4, "h_ttbar_cvsb_CvsL4");
    plot(h_ttbar_cvsb_CvsB1, "h_ttbar_cvsb_CvsB1");
    plot(h_ttbar_cvsb_CvsB2, "h_ttbar_cvsb_CvsB2");
    plot(h_ttbar_cvsb_CvsB3, "h_ttbar_cvsb_CvsB3");
    plot(h_ttbar_cvsb_CvsB4, "h_ttbar_cvsb_CvsB4");
    plot(h_ttbar_cvsb_Ctag1, "h_ttbar_cvsb_Ctag1");
    plot(h_ttbar_cvsb_Ctag2, "h_ttbar_cvsb_Ctag2");
    plot(h_ttbar_cvsb_Ctag3, "h_ttbar_cvsb_Ctag3");
    plot(h_ttbar_cvsb_Ctag4, "h_ttbar_cvsb_Ctag4");
  f.Close();
  //auto report = df_S1_bjets.Report();
  //report->Print();

  //dfs_S2_sortbjet.Snapshot("sort","sort.root", {"sortjet", "ngoodbjets_m", "sort_btag_11", "sort_btag_22", "sort_pt_11", "sort_pt_22"});
  //dfs_S2_goodtau.Snapshot("sort","sort.root", {"sortjet", "ngoodbjets_m", "sort_btag_11", "sort_btag_22", "sort_pt_11", "sort_pt_22"});
}

