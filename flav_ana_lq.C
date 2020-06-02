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

rvec_i  good_idx(rvec_i good){
  vector<int> out;
  for(int i = 0; i < good.size(); i++){
    if( good[i] ){
			out.push_back(i);
		}
  }
  return out; 
}

int leading_idx(rvec_i good_idx){
	int out = good_idx[0];
	return out;
}
int second_idx(rvec_i good_idx){
	int out = good_idx[1];
	return out;
}

rvec_i Leading_idx(rvec_i good, rvec_i good_idx ){
	vector<int> out;
	for(int i = 0; i < good.size(); i++){
		if(i == good_idx[0]){
			out.push_back(1);
		}else {
			out.push_back(0);
		}
	}
	return out;
}

rvec_i Second_idx(rvec_i good, rvec_i good_idx ){
	vector<int> out;
	for(int i = 0; i < good.size(); i++){
		if(i == good_idx[1]){
			out.push_back(1);
		}else {
			out.push_back(0);
		}
	}
	return out;
}

rvec_i Lead_Se_idx(rvec_i Leading_idx, rvec_i Second_idx){
	vector<int> out;
	for(int i = 0; i < Leading_idx.size(); i++){
		int sum;
		sum = Leading_idx[i]+Second_idx[i];
		out.push_back(sum);
	}
	return out;
}


//exept tau jet
rvec_i ex_tau1(int Tau_traced_jet1Idx, rvec_i goodjet){
	vector<int> out;
	for(int i = 0; i < goodjet.size(); i++){
		if( i == Tau_traced_jet1Idx ) out.push_back(0);
		else if( i !=Tau_traced_jet1Idx ) out.push_back(1);
	}
	return out;
}

rvec_i good_lf_ex_tau1(rvec_i good_lf_hadron, rvec_i ex_tau1_idx){
	vector<int> out;
	for(int i = 0; i < good_lf_hadron.size(); i++){
		out.push_back( good_lf_hadron[i] * ex_tau1_idx[i] );
	}
	return out;
}

float mass_reconst2(int i_idx, rvec_f i_pt, rvec_f i_eta, rvec_f i_phi, rvec_f i_mass,
	                  int j_idx, rvec_f j_pt, rvec_f j_eta, rvec_f j_phi, rvec_f j_mass){
	float inv_mass;
	int i = i_idx; int j = j_idx;
	TLorentzVector I,J;
	I.SetPtEtaPhiM(i_pt[i], i_eta[i], i_phi[i], i_mass[i]);
	J.SetPtEtaPhiM(j_pt[j], j_eta[j], j_phi[j], j_mass[j]);
	inv_mass = (I+J).M();
	return inv_mass;
}
float mass_reconst3(int i_idx, rvec_f i_pt, rvec_f i_eta, rvec_f i_phi, rvec_f i_mass,
										int j_idx, rvec_f j_pt, rvec_f j_eta, rvec_f j_phi, rvec_f j_mass,
										int k_idx, rvec_f k_pt, rvec_f k_eta, rvec_f k_phi, rvec_f k_mass){
	float inv_mass;
	int i = i_idx; int j = j_idx; int k = k_idx;
	TLorentzVector I,J,K;
	I.SetPtEtaPhiM(i_pt[i], i_eta[i], i_phi[i], i_mass[i]);
	J.SetPtEtaPhiM(j_pt[j], j_eta[j], j_phi[j], j_mass[j]);
	K.SetPtEtaPhiM(k_pt[k], k_eta[k], k_phi[k], k_mass[k]);
	inv_mass = (I+J+K).M();
	return inv_mass;
}



void flav_ana_lq(TString input = "LQ"){
	std::vector<std::string> file = {"/cms/ldap_home/ljw1015/public/LQ_Signals/LQ_2018_nano.root"};
	if (input.Contains("LQ")){
		file = {"/cms/ldap_home/ljw1015/public/LQ_Signals/LQ_2018_nano.root",
			"/T2_KR_KISTI/store/user/jolim/LQ_2018/LQ_2018NANO/200517_045835/0000/LQ_2018_nano_1.root",
			"/T2_KR_KISTI/store/user/jolim/LQ_2018/LQ_2018NANO_A1/200523_183528/0000/LQ_2018_nano_1.root"};
	}else if (input.Contains("ttbar")){
		 file = {"/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/073462AF-FDD0-AD45-970A-EB97923698F3.root",
			 "/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/0BFDE5A9-3D64-2A4D-A24E-FFB33CCE19F5.root",
			 "/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/211ACE36-21BD-CC43-864E-BDBD1CE5FE01.root"};
	}
	ROOT::RDataFrame df("Events",file);

	//S1:muon, S2:tau, S3:hadronFlavour, S4:genTtbarId, S5:b,c-tagging, S6:Filter for pt

	//**** HLT Trigger ****//
	auto df_S0_HLT = df.Filter("HLT_IsoMu24","SingleMuon Trigger");

	//**** Muon Selection (Tight muonId) ****//
	auto df_S1_goodmuon = df.Define("goodmuon","Muon_pt > 30  && abs(Muon_eta) < 2.4 && Muon_tightId && Muon_pfRelIso03_all < 0.15")
													.Define("n_goodmuon","Sum(goodmuon)")
													.Filter("n_goodmuon >= 1","Muon Selection")
													.Define("goodmuon_idx",good_idx,{"goodmuon"})
													.Define("goodmuon1_idx",leading_idx,{"goodmuon_idx"});

	//**** Tau Selection ****//
	auto df_S2_goodtau = df_S1_goodmuon.Define("goodtau","Tau_pt > 30 && abs(Tau_eta) < 2.3 &&  Tau_idMVAoldDM & 2")
																		 .Define("n_goodtau","Sum(goodtau)")
																		 .Filter("n_goodtau >= 1","Tau Selection")
																		 .Define("goodtau_idx",good_idx,{"goodtau"})
																		 .Define("goodtau1_idx",leading_idx,{"goodtau_idx"});
	auto df_S2_goodtau2 = df_S2_goodtau.Filter("n_goodtau >=2","Two tau jets")
																		 .Define("goodtau2_idx",second_idx,{"goodtau_idx"});
	auto df_S2_goodtau1_mother = df_S2_goodtau.Define("goodtau1_genidx","Tau_genPartIdx[goodtau1_idx]")
																						.Filter("goodtau1_genidx > -1","gen traced tau")
																						.Define("goodtau1_motheridx","GenPart_genPartIdxMother[goodtau1_genidx]")
																						.Filter("goodtau1_motheridx > -1", "mother traced tau")
																						.Define("goodtau1_mother","abs(GenPart_pdgId[goodtau1_motheridx])");
	auto df_S2_muon_mother = df_S2_goodtau1_mother.Filter("goodtau1_mother == 13","tau's mother muon")
																								.Define("muon_mother_idx","abs(GenPart_genPartIdxMother[goodtau1_motheridx])")
																								.Define("muon_mother","abs(GenPart_pdgId[muon_mother_idx])");
	auto df_S2_goodtau2_mother = df_S2_goodtau2.Define("goodtau2_genidx","Tau_genPartIdx[goodtau2_idx]")
																						 .Filter("goodtau2_genidx > -1","gen traced tau2")
																						 .Define("goodtau2_motheridx","GenPart_genPartIdxMother[goodtau2_genidx]")
																						 .Filter("goodtau2_motheridx > -1", "mother traced tau")
																						 .Define("goodtau2_mother","abs(GenPart_pdgId[goodtau2_motheridx])");


	//**** Gen Level Flavour ****//
	auto df_S3_goodhadron = df_S2_goodtau.Define("goodjet","Jet_pt > 30 && abs(Jet_eta) < 2.4 && Jet_jetId == 6")
													.Define("goodhadron_Flav","Jet_hadronFlavour[goodjet]")
													.Define("good_b_hadron","goodjet && Jet_hadronFlavour == 5")
													.Define("good_c_hadron","goodjet  && Jet_hadronFlavour == 4")
													.Define("good_lf_hadron","goodjet && Jet_hadronFlavour == 0");
	//**** GenTtbarId ****//
	auto df_S4_TtbarId = df_S3_goodhadron.Define("n_cjet_fromW","genTtbarId/10000")
																			 .Define("n_bjet_fromW","(genTtbarId%10000)/1000")
																			 .Define("n_bjet_fromt","(genTtbarId%1000)/100");

	//**** Jet Tagging ****//
	auto df_S5_goodtag = df_S4_TtbarId.Define("CvsB","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
																		.Define("Jet_btagDeepFlavLF","1-(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
																		.Define("CvsL","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavLF)");
	auto df_S5_good_btag = df_S5_goodtag.Define("goodbjet_l","goodjet && Jet_btagDeepFlavB > 0.0494")
																			.Define("goodbjet_m","goodjet && Jet_btagDeepFlavB > 0.2770")
																			.Define("goodbjet_t","goodjet && Jet_btagDeepFlavB > 0.7264")
																			.Filter("Sum(goodbjet_m) == 1", "Events with one medium b-tagged jet");
	auto df_S5_good_ctag = df_S5_good_btag.Define("goodcjet_l","goodjet && CvsB > 0.40 && CvsL > 0.03")
																				.Define("goodcjet_m","goodjet && CvsB > 0.29 && CvsL > 0.085")
																				.Define("goodcjet_t","goodjet && CvsB > 0.05 && CvsL > 0.48"); 

	//??Question : signal events with no c jets??//
	auto df_S5_original_hadronFlav = df_S2_goodtau.Define("original_c_hadron", "Jet_hadronFlavour == 4");
	auto n_original_c_hadron = df_S5_original_hadronFlav.Define("n_original_c_hadron","Sum(original_c_hadron)");
	auto h_n_original_c_hadron = n_original_c_hadron.Histo1D({"h_n_original_c_hadron","h_n_original_c_hadron", 10, 0, 10}, "n_original_c_hadron");


	//**** Preparation for Pt Distribution ****//
	auto df_S6_good_b_hadron = df_S4_TtbarId.Filter("Sum(good_b_hadron)>=1","Events with at least one good b hadron")
				.Define("good_b_hadron_idx",good_idx,{"good_b_hadron"})	
				.Define("good_b_hadron1_idx",leading_idx,{"good_b_hadron_idx"}); 
	auto df_S6_good_b_hadron_e1 = df_S6_good_b_hadron.Filter("Sum(good_b_hadron)==1","Events with one good b hadron");
	auto df_S6_good_b_hadron_o2 =  df_S6_good_b_hadron.Filter("Sum(good_b_hadron)>=2","Events with at least two good b hadrons")
				.Define("good_b_hadron2_idx",second_idx,{"good_b_hadron_idx"})
				.Define("Leading_good_b_hadron_idx", Leading_idx,{"good_b_hadron","good_b_hadron_idx"})
				.Define("Second_good_b_hadron_idx", Second_idx,{"good_b_hadron","good_b_hadron_idx"})
				.Define("good_b_hadron12",Lead_Se_idx,{"Leading_good_b_hadron_idx", "Second_good_b_hadron_idx"});

	auto df_S6_good_c_hadron = df_S4_TtbarId.Filter("Sum(good_c_hadron)>=1","Events with at least one good c hadron")
				.Define("good_c_hadron_idx",good_idx,{"good_c_hadron"}) 
				.Define("good_c_hadron1_idx",leading_idx,{"good_c_hadron_idx"}); 
	auto df_S6_good_lf_hadron = df_S4_TtbarId.Filter("Sum(good_lf_hadron)>=1","Events with at least one good lf hadron")
				.Define("good_lf_hadron_idx",good_idx,{"good_lf_hadron"}) 
				.Define("good_lf_hadron1_idx",leading_idx,{"good_lf_hadron_idx"});
	auto df_S6_goodjet = df_S4_TtbarId.Filter("Sum(good_b_hadron)>=1","Events with at least one good b hadron")
				.Define("goodjet_idx",good_idx,{"goodjet"}) .Define("goodjet1_idx",leading_idx,{"goodjet_idx"});

	auto df_S6_good_c_hadron_nfW = df_S6_good_c_hadron.Filter("n_cjet_fromW==0","Events with no c-jet from W");
	auto df_S6_good_c_hadron_ifW = df_S6_good_c_hadron.Filter("n_cjet_fromW!=0","Events including c-jet from W")
				.Filter("Sum(good_c_hadron)>=2", "Events with at least two good c-hadrons") .Define("good_c_hadron2_idx",second_idx,{"good_c_hadron_idx"}); 

 //**** Mass Reconstruction ****//
	auto df_S7_reco_top_toLQ_nfW = df_S6_good_c_hadron_nfW.Define("inv_mass_top_toLQ_nfW",mass_reconst3,
			{"goodmuon1_idx","Muon_pt","Muon_eta","Muon_phi","Muon_mass",
			 "good_c_hadron1_idx","Jet_pt","Jet_eta","Jet_phi","Jet_mass",
			 "goodtau1_idx","Tau_pt","Tau_eta","Tau_phi","Tau_mass"});
	auto df_S7_reco_top_toLQ_ifW = df_S6_good_c_hadron_ifW.Define("inv_mass_top_toLQ_ifW",mass_reconst3,
			{"goodmuon1_idx","Muon_pt","Muon_eta","Muon_phi","Muon_mass",
			 "good_c_hadron2_idx","Jet_pt","Jet_eta","Jet_phi","Jet_mass",
			 "goodtau1_idx","Tau_pt","Tau_eta","Tau_phi","Tau_mass"});

	auto df_S7_reco_W_tocs_ifW = df_S6_good_c_hadron_ifW
		.Define("tau_traced_jet","Tau_jetIdx>0")
		.Define("n_tau_traced_jet","Sum(tau_traced_jet)")
		.Filter("n_tau_traced_jet >= 1", "At least one tau jet(traced)")
		.Define("Tau_traced_jetIdx","Tau_jetIdx[tau_traced_jet]")
		.Define("Tau_traced_jet1Idx",leading_idx,{"Tau_traced_jetIdx"})
		.Define("ex_tau1",ex_tau1,{"Tau_traced_jet1Idx","goodjet"})
		.Define("good_lf_ex_tau1",good_lf_ex_tau1,{"good_lf_hadron","ex_tau1"})
		.Filter("Sum(good_lf_ex_tau1)>=1","At least one light jet exept tau")
		.Define("good_lf_ex_tau1_idx",good_idx,{"good_lf_ex_tau1"})
		.Define("leading_good_lf_ex_tau1_idx",leading_idx,{"good_lf_ex_tau1_idx"})
		.Define("inv_mass_W_tocs_ifW",mass_reconst2,
			{"leading_good_lf_ex_tau1_idx","Jet_pt","Jet_eta","Jet_phi","Jet_mass",
			 "good_c_hadron1_idx","Jet_pt","Jet_eta","Jet_phi","Jet_mass"});
	auto df_S7_reco_top_toWb_ifW = df_S7_reco_W_tocs_ifW
		.Filter("Sum(good_b_hadron)>=1","At least one godd b jet")
		.Define("good_b_hadron_idx",good_idx,{"good_b_hadron"})
		.Define("good_b_hadron1_idx",leading_idx,{"good_b_hadron_idx"})
		.Define("inv_mass_top_toWb_ifW",mass_reconst3,
			{"leading_good_lf_ex_tau1_idx","Jet_pt","Jet_eta","Jet_phi","Jet_mass",
			 "good_c_hadron1_idx","Jet_pt","Jet_eta","Jet_phi","Jet_mass",
			 "good_b_hadron1_idx","Jet_pt","Jet_eta","Jet_phi","Jet_mass"});

	//**** Results of tagging ****//
	auto df_S5_btagged_jet_l = df_S5_good_btag.Filter("Sum(goodbjet_l)>=1", "Events with at least one loose b-tagged jet");
	auto df_S5_btagged_jet_m = df_S5_good_btag.Filter("Sum(goodbjet_m)>=1", "Events with at least one medium b-tagged jet");
	auto df_S5_btagged_jet_t = df_S5_good_btag.Filter("Sum(goodbjet_t)>=1", "Events with at least one tight b-tagged jet");
	auto df_S5_ctagged_jet_l = df_S5_good_ctag.Filter("Sum(goodcjet_l)>=1", "Events with at least one loose c-tagged jet");
	auto df_S5_ctagged_jet_m = df_S5_good_ctag.Filter("Sum(goodcjet_m)>=1", "Events with at least one medium c-tagged jet");
	auto df_S5_ctagged_jet_t = df_S5_good_ctag.Filter("Sum(goodcjet_t)>=1", "Events with at least one tight c-tagged jet");

	//**** Discriminator Distribution ****//
	auto df_S5_Deep_hadron = df_S5_good_ctag
		.Define("DeepB_b", "Jet_btagDeepFlavB[good_b_hadron]")   .Define("DeepB_c", "Jet_btagDeepFlavB[good_c_hadron]")
		.Define("DeepB_lf", "Jet_btagDeepFlavB[good_lf_hadron]") .Define("DeepC_b", "Jet_btagDeepFlavC[good_b_hadron]")
		.Define("DeepC_c", "Jet_btagDeepFlavC[good_c_hadron]")   .Define("DeepC_lf", "Jet_btagDeepFlavC[good_lf_hadron]")
		.Define("CvsB_b", "CvsB[good_b_hadron]")   .Define("CvsB_c", "CvsB[good_c_hadron]")
		.Define("CvsB_lf", "CvsB[good_lf_hadron]") .Define("CvsL_b", "CvsL[good_b_hadron]")
		.Define("CvsL_c", "CvsL[good_c_hadron]")   .Define("CvsL_lf", "CvsL[good_lf_hadron]");

	//**** Not in use currently ****//
	auto df_S_bjet = df_S5_Deep_hadron.Filter("Sum( goodbjet_m ) >=1","Events with at least one b jet");
	auto df_S_ttb = df_S5_Deep_hadron.Filter("(genTtbarId/10)%10 == 5","ttb+bb event");


	//**** Calculate Tagging Efficiency ****// 
/*	auto df_S5_btag_eff = df_S5_good_ctag.Define("b_tagged_b_l", "good_b_hadron && goodbjet_l")
																				.Define("b_tagged_c_l", "good_c_hadron && goodbjet_l")
																				.Define("b_tagged_lf_l", "good_lf_hadron && goodbjet_l")
																				.Define("b_tagged_b_m", "good_b_hadron && goodbjet_m")
																				.Define("b_tagged_c_m", "good_c_hadron && goodbjet_m")
																				.Define("b_tagged_lf_m", "good_lf_hadron && goodbjet_m")
																				.Define("b_tagged_b_t", "good_b_hadron && goodbjet_t")
																				.Define("b_tagged_c_t", "good_c_hadron && goodbjet_t")
																				.Define("b_tagged_lf_t", "good_lf_hadron && goodbjet_t");
	auto df_S5_ctag_eff = df_S5_good_ctag.Define("c_tagged_b_l", "good_b_hadron && goodcjet_l")
																			 .Define("c_tagged_c_l", "good_c_hadron && goodcjet_l")
																			 .Define("c_tagged_lf_l", "good_lf_hadron && goodcjet_l")
																			 .Define("c_tagged_b_m", "good_b_hadron && goodcjet_m")
																			 .Define("c_tagged_c_m", "good_c_hadron && goodcjet_m")
																			 .Define("c_tagged_lf_m", "good_lf_hadron && goodcjet_m")
																			 .Define("c_tagged_b_t", "good_b_hadron && goodcjet_t")
																			 .Define("c_tagged_c_t", "good_c_hadron && goodcjet_t")
																			 .Define("c_tagged_lf_t", "good_lf_hadron && goodcjet_t");
*/

	//**** histograms ****//
	auto h_idx = df_S1_goodmuon.Define("muonidx","goodmuon1_idx").Histo1D({"h_idx","h_idx",10,-5,5},"goodmuon1_idx");

	auto h_muon_pt = df_S1_goodmuon.Define("goodmuon1_pt","Muon_pt[goodmuon1_idx]").Histo1D({"h_muon_pt", "h_muon_pt", 10, 0, 100}, "goodmuon1_pt");
	auto h_muon_eta = df_S1_goodmuon.Define("goodmuon1_eta","Muon_eta[goodmuon1_idx]").Histo1D({"h_muon_eta", "h_muon_eta", 50, -5, 5}, "goodmuon1_eta");
	auto h_n_selmuon = df_S1_goodmuon.Histo1D({"h_n_selmuon", "h_n_selmuon", 5, 0, 5}, "n_goodmuon"); 

	auto h_tau_pt = df_S2_goodtau.Define("goodtau1_pt","Tau_pt[goodtau1_idx]").Histo1D({"h_tau_pt", "h_tau_pt", 10, 0, 100}, "goodtau1_pt");
	auto h_tau_eta = df_S2_goodtau.Define("goodtau1_eta","Tau_eta[goodtau1_idx]").Histo1D({"h_tau_eta", "h_tau_eta", 50, -5, 5}, "goodtau1_eta");
	auto h_n_tau = df_S1_goodmuon.Histo1D({"h_n_tau", "h_n_tau", 5, 0, 5}, "nTau");
	auto h_n_seltau = df_S2_goodtau.Histo1D({"h_n_seltau", "h_n_seltau", 5, 0, 5}, "n_goodtau");
	auto h_goodtau1_mother = df_S2_goodtau1_mother.Histo1D({"h_goodtau1_mother","h_goodtau1_mother",100,0,100},"goodtau1_mother");
	auto h_muon_mother = df_S2_muon_mother.Histo1D({"h_muon_mother","h_muon_mother",100,0,100},"muon_mother");
	auto h_goodtau2_mother = df_S2_goodtau2_mother.Histo1D({"h_goodtau2_mother","h_goodtau2_mother",100,0,100},"goodtau2_mother");


	auto h_goodhadron_Flav = df_S3_goodhadron.Histo1D({"h_goodhadronFlav", "h_goodhadronFlav",10, 0,10}, "goodhadron_Flav");
	auto n_good_b_hadron = df_S3_goodhadron.Define("n_good_b_hadron","Sum(good_b_hadron)");
	auto h_n_good_b_hadron = n_good_b_hadron.Histo1D({"h_n_good_b_hadron","h_n_good_b_hadron", 10, 0, 10}, "n_good_b_hadron");
	auto n_good_c_hadron = df_S3_goodhadron.Define("n_good_c_hadron","Sum(good_c_hadron)");
	auto h_n_good_c_hadron = n_good_c_hadron.Histo1D({"h_n_good_c_hadron","h_n_good_c_hadron", 10, 0, 10}, "n_good_c_hadron");
	auto n_good_lf_hadron = df_S3_goodhadron.Define("n_good_lf_hadron","Sum(good_lf_hadron)");
	auto h_n_good_lf_hadron = n_good_lf_hadron.Histo1D({"h_n_good_lf_hadron","h_n_good_lf_hadron", 10, 0, 10}, "n_good_lf_hadron");
	auto h_n_goodjet= df_S3_goodhadron.Define("n_goodjet","Sum(goodjet)") .Histo1D({"h_n_goodjet","h_n_goodjet", 15, 0, 15}, "n_goodjet");

	auto h_n_cjet_fromW = df_S4_TtbarId.Histo1D({"h_n_cjet_fromW", "h_n_cjet_fromW",5, 0, 5}, "n_cjet_fromW");
	auto h_n_bjet_fromW = df_S4_TtbarId.Histo1D({"h_n_bjet_fromW", "h_n_bjet_fromW",5, 0, 5}, "n_bjet_fromW");
	auto h_n_bjet_fromt = df_S4_TtbarId.Histo1D({"h_n_bjet_fromt", "h_n_bjet_fromt",5, 0, 5}, "n_bjet_fromt");

	auto h_leading_good_b_hadron_pt = df_S6_good_b_hadron.Define("leading_good_b_hadron_pt","Jet_pt[good_b_hadron1_idx]")
				.Histo1D({"h_leading_good_b_hadron_pt","h_leading_good_b_hadron_pt",20, 0, 200}, "leading_good_b_hadron_pt");
	auto h_leading_good_b_hadron_pt_e1 = df_S6_good_b_hadron_e1.Define("leading_good_b_hadron_pt","Jet_pt[good_b_hadron1_idx]")
				.Histo1D({"h_leading_good_b_hadron_pt_e1","h_leading_good_b_hadron_pt_e1",20, 0, 200}, "leading_good_b_hadron_pt");
	auto h_leading_good_b_hadron_pt_o2 = df_S6_good_b_hadron_o2.Define("leading_good_b_hadron_pt","Jet_pt[good_b_hadron1_idx]")
				.Histo1D({"h_leading_good_b_hadron_pt_o2","h_leading_good_b_hadron_pt_o2",20, 0, 200}, "leading_good_b_hadron_pt");
	auto h_second_good_b_hadron_pt_o2 = df_S6_good_b_hadron_o2.Define("second_good_b_hadron_pt","Jet_pt[good_b_hadron2_idx]")
				.Histo1D({"h_second_good_b_hadron_pt_o2","h_second_good_b_hadron_pt_o2",20, 0, 200}, "second_good_b_hadron_pt");
	auto h_lead_se_good_b_hadron_pt_o2 = df_S6_good_b_hadron_o2.Define("lead_se_good_b_hadron_pt_o2", "Jet_pt[good_b_hadron12]")
				.Histo1D({"h_lead_se_good_b_hadron_pt_o2","h_lead_se_good_b_hadron_pt_o2",20,0,200}, "lead_se_good_b_hadron_pt_o2");
	auto h_leading_good_c_hadron_pt = df_S6_good_c_hadron.Define("leading_good_c_hadron_pt","Jet_pt[good_c_hadron1_idx]")
				.Histo1D({"h_leading_good_c_hadron_pt","h_leading_good_c_hadron_pt",20, 0, 200}, "leading_good_c_hadron_pt");
	auto h_leading_good_lf_hadron_pt = df_S6_good_lf_hadron.Define("leading_good_lf_hadron_pt","Jet_pt[good_lf_hadron1_idx]")
				.Histo1D({"h_leading_good_lf_hadron_pt","h_leading_good_lf_hadron_pt",20, 0, 200}, "leading_good_lf_hadron_pt");
	auto h_leading_goodjet_pt = df_S6_goodjet.Define("leading_goodjet_pt","Jet_pt[goodjet1_idx]")
				.Histo1D({"h_leading_goodjet_pt"," h_leading_goodjet_pt",25, 0, 250}, "leading_goodjet_pt");

	auto h_leading_good_c_hadron_nfW_pt = df_S6_good_c_hadron_nfW.Define("leading_good_c_hadron_nfW_pt", "Jet_pt[good_c_hadron1_idx]")
				.Histo1D({"h_leading_good_c_hadron_nfW_pt","h_leading_good_c_hadron_nfW_pt",20, 0, 200}, "leading_good_c_hadron_nfW_pt");
	auto h_leading_good_c_hadron_ifW_pt = df_S6_good_c_hadron_ifW.Define("leading_good_c_hadron_ifW_pt", "Jet_pt[good_c_hadron1_idx]")
				.Histo1D({"h_leading_good_c_hadron_ifW_pt","h_leading_good_c_hadron_ifW_pt",20, 0, 200}, "leading_good_c_hadron_ifW_pt");

	auto h_n_good_c_hadron_ifW = df_S6_good_c_hadron_ifW.Define("n_good_c_hadron_ifW","Sum(good_c_hadron)")
				.Histo1D({"h_n_good_c_hadron_ifW","h_n_good_c_hadron_ifW", 10, 0, 10}, "n_good_c_hadron_ifW");

	auto h_inv_mass_top_toLQ_nfW = df_S7_reco_top_toLQ_nfW.Histo1D({"h_inv_mass_top_toLQ_nfW","h_inv_mass_top_toLQ_nfW",50,0,500},"inv_mass_top_toLQ_nfW");
	auto h_inv_mass_top_toLQ_ifW = df_S7_reco_top_toLQ_ifW.Histo1D({"h_inv_mass_top_toLQ_ifW","h_inv_mass_top_toLQ_ifW",50,0,500},"inv_mass_top_toLQ_ifW");
	auto h_inv_mass_W_tocs_ifW = df_S7_reco_W_tocs_ifW.Histo1D({"h_inv_mass_W_tocs_ifW","h_inv_mass_W_tocs_ifW",50,0,500},"inv_mass_W_tocs_ifW");
	auto h_inv_mass_top_toWb_ifW = df_S7_reco_top_toWb_ifW.Histo1D({"h_inv_mass_top_toWb_ifW","h_inv_mass_top_toWb_ifW",50,0,500},"inv_mass_top_toWb_ifW");
	auto h_Tau_traced_jet1Idx = df_S7_reco_top_toWb_ifW.Histo1D({"h_Tau_traced_jet1Idx", "h_Tau_traced_jet1Idx",15,-5,10},"Tau_traced_jet1Idx");
/*
	auto h_leading_btagged_jet_l_pt = df_S1_btagged_jets_l.Define("leading_btagged_jet_l_pt","Jet_pt[goodbjet_l][0]")
				.Histo1D({"h_leading_btagged_jet_l_pt","h_leading_btagged_jet_l_pt",20, 0, 200}, "leading_btagged_jet_l_pt");
	auto h_leading_btagged_jet_m_pt = df_S1_btagged_jets_m.Define("leading_btagged_jet_m_pt","Jet_pt[goodbjet_m][0]")
				.Histo1D({"h_leading_btagged_jet_m_pt","h_leading_btagged_jet_m_pt",20, 0, 200}, "leading_btagged_jet_m_pt");
	auto h_leading_btagged_jet_t_pt = df_S1_btagged_jets_t.Define("leading_btagged_jet_t_pt","Jet_pt[goodbjet_t][0]")
				.Histo1D({"h_leading_btagged_jet_t_pt","h_leading_btagged_jet_t_pt",20, 0, 200}, "leading_btagged_jet_t_pt");
	auto h_leading_ctagged_jet_l_pt = df_S1_ctagged_jets_l.Define("leading_ctagged_jet_l_pt","Jet_pt[goodcjet_l][0]")
				.Histo1D({"h_leading_ctagged_jet_l_pt","h_leading_ctagged_jet_l_pt",20, 0, 200}, "leading_ctagged_jet_l_pt");
	auto h_leading_ctagged_jet_m_pt = df_S1_ctagged_jets_m.Define("leading_ctagged_jet_m_pt","Jet_pt[goodcjets_m][0]")
				.Histo1D({"h_leading_ctagged_jet_m_pt","h_leading_ctagged_jet_m_pt",20, 0, 200}, "leading_ctagged_jet_m_pt");
	auto h_leading_ctagged_jet_t_pt = df_S1_ctagged_jets_t.Define("leading_ctagged_jet_t_pt","Jet_pt[goodcjets_t][0]")
				.Histo1D({"h_leading_ctagged_jet_t_pt","h_leading_ctagged_jet_t_pt",20, 0, 200}, "leading_ctagged_jet_t_pt");
*/
	auto h_leading_good_b_hadron_eta = df_S6_good_b_hadron.Define("leading_good_b_hadron_eta","Jet_eta[good_b_hadron1_idx]")
				.Histo1D({"h_leading_good_b_hadron_eta","h_leading_good_b_hadron_eta",15, 0, 3}, "leading_good_b_hadron_eta");
	auto h_leading_good_c_hadron_eta = df_S6_good_c_hadron.Define("leading_good_c_hadron_eta","Jet_eta[good_c_hadron1_idx]")
				.Histo1D({"h_leading_good_c_hadron_eta","h_leading_good_c_hadron_eta",15, 0, 3}, "leading_good_c_hadron_eta");
	auto h_leading_good_lf_hadron_eta = df_S6_good_lf_hadron.Define("leading_good_lf_hadron_eta","Jet_eta[good_lf_hadron1_idx]")
				.Histo1D({"h_leading_good_lf_hadron_eta","h_leading_good_lf_hadron_eta",15, 0, 3}, "leading_good_lf_hadron_eta");
	auto h_leading_goodjet_eta = df_S6_goodjet.Define("leading_goodjet_eta","Jet_eta[goodjet1_idx]")
				.Histo1D({"h_leading_goodjet_eta"," h_leading_goodjet_eta",15, 0, 3}, "leading_goodjet_eta");


	auto h_DeepB_b = df_S5_Deep_hadron.Histo1D({"h_DeepB_b", "h_DeepB_b", 100, 0, 1}, "DeepB_b");
	auto h_DeepB_c = df_S5_Deep_hadron.Histo1D({"h_DeepB_c", "h_DeepB_c", 100, 0, 1}, "DeepB_c");
	auto h_DeepB_lf = df_S5_Deep_hadron.Histo1D({"h_DeepB_lf", "h_DeepB_lf", 100, 0, 1}, "DeepB_lf");
	auto h_DeepC_b = df_S5_Deep_hadron.Histo1D({"h_DeepC_b", "h_DeepC_b", 100, 0, 1}, "DeepC_b");
	auto h_DeepC_c = df_S5_Deep_hadron.Histo1D({"h_DeepC_c", "h_DeepC_c", 100, 0, 1}, "DeepC_c");
	auto h_DeepC_lf = df_S5_Deep_hadron.Histo1D({"h_DeepC_lf", "h_DeepC_lf", 100, 0, 1}, "DeepC_lf");
	auto h_ctag_2D_b = df_S5_Deep_hadron.Histo2D({"h_ctag_2D_b", "h_ctag_2D_b", 100, 0, 1, 150, 0, 1.5},"CvsL_b","CvsB_b");
	auto h_ctag_2D_c = df_S5_Deep_hadron.Histo2D({"h_ctag_2D_c", "h_ctag_2D_c", 100, 0, 1, 150, 0, 1.5},"CvsL_c","CvsB_c");
	auto h_ctag_2D_lf = df_S5_Deep_hadron.Histo2D({"h_ctag_2D_lf", "h_ctag_2D_lf", 100, 0, 1, 150, 0, 1.5}, "CvsL_lf","CvsB_lf");
/*
	//calculate efficiency
	auto aggregator = [](double acc, int x) { return acc+x; } ;
	auto merger = [](std::vector<double> &accumulators) {
				auto size = accumulators.size();
				for (int i = 1; i <size; ++i) {
							accumulators[0] += accumulators[i];
				}
	};
	double initValue = 0;

	auto n_btagging = df_S5_btag_eff.Define("n_b_tagged_b_l", "Sum(b_tagged_b_l)")
																	.Define("n_b_tagged_c_l", "Sum(b_tagged_c_l)")
																	.Define("n_b_tagged_lf_l", "Sum(b_tagged_lf_l)")
																	.Define("n_b_tagged_b_m", "Sum(b_tagged_b_m)")
																	.Define("n_b_tagged_c_m", "Sum(b_tagged_c_m)")
																	.Define("n_b_tagged_lf_m", "Sum(b_tagged_lf_m)")
																	.Define("n_b_tagged_b_t", "Sum(b_tagged_b_t)")
																	.Define("n_b_tagged_c_t", "Sum(b_tagged_c_t)")
																	.Define("n_b_tagged_lf_t", "Sum(b_tagged_lf_t)");

	auto n_ctagging = df_S5_ctag_eff.Define("n_c_tagged_b_l", "Sum(c_tagged_b_l)")
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

*/
	auto h_n_bjet_l = df_S5_good_btag.Define("nbjet_l","Sum( goodbjet_l )").Histo1D({"h_n_bjet_l", "h_n_bjet_l", 10, 0, 10}, "nbjet_l");
	auto h_n_bjet_m = df_S5_good_btag.Define("nbjet_m","Sum( goodbjet_m )").Histo1D({"h_n_bjet_m", "h_n_bjet_m", 10, 0, 10}, "nbjet_m");
	auto h_n_bjet_t = df_S5_good_btag.Define("nbjet_t","Sum( goodbjet_t )").Histo1D({"h_n_bjet_t", "h_n_bjet_t", 10, 0, 10}, "nbjet_t");
	auto h_n_cjet_l = df_S5_good_ctag.Define("ncjet_l","Sum( goodcjet_l )").Histo1D({"h_n_cjet_l", "h_n_cjet_l", 10, 0, 10}, "ncjet_l");
	auto h_n_cjet_m = df_S5_good_ctag.Define("ncjet_m","Sum( goodcjet_m )").Histo1D({"h_n_cjet_m", "h_n_cjet_m", 10, 0, 10}, "ncjet_m");
	auto h_n_cjet_t = df_S5_good_ctag.Define("ncjet_t","Sum( goodcjet_t )").Histo1D({"h_n_cjet_t", "h_n_cjet_t", 10, 0, 10}, "ncjet_t");



	//df_S1_goodmuon.Snapshot("tree", "f.root");
	TString fname = "default_fname";
	if (input.Contains("LQ")){
		fname = "f_DeepFlav_2018.root";
	}else if (input.Contains("ttbar")){
		fname = "tt_f_DeepFlav_2018.root";
	}
	TFile f(fname, "recreate");   


	plot( h_muon_pt, "h_muon_pt");
	plot( h_muon_eta, "h_muon_eta");
	plot( h_n_selmuon, "h_n_selmuon");
	plot( h_tau_pt, "h_tau_pt");
	plot( h_tau_eta, "h_tau_eta");
	plot( h_n_tau, "h_n_tau");
	plot( h_n_seltau, "h_n_seltau");
	plot( h_goodtau1_mother, "h_goodtau1_mother");
	plot( h_goodtau2_mother, "h_goodtau2_mother");
	plot( h_muon_mother, "h_muon_mother");

	plot( h_goodhadron_Flav, "h_goodhadron_Flav");
	plot( h_n_good_b_hadron, "h_n_good_b_hadron");
	plot( h_n_good_c_hadron, "h_n_good_c_hadron");
	plot( h_n_good_lf_hadron, "h_n_good_lf_hadron");
	plot( h_n_goodjet, "h_n_goodjet");
	plot( h_n_cjet_fromW, "h_n_cjet_fromW");
	plot( h_n_bjet_fromW, "h_n_bjet_fromW");
	plot( h_n_bjet_fromt, "h_n_bjet_fromt");

	plot( h_leading_good_b_hadron_pt, "h_leading_good_b_hadron_pt");
	plot( h_leading_good_b_hadron_pt_e1, "h_leading_good_b_hadron_pt_e1");
	plot( h_leading_good_b_hadron_pt_o2, "h_leading_good_b_hadron_pt_o2");
	plot( h_second_good_b_hadron_pt_o2, "h_second_good_b_hadron_pt_o2");
	plot( h_lead_se_good_b_hadron_pt_o2, "h_lead_se_good_b_hadron_pt_o2");
	plot( h_leading_good_c_hadron_pt, "h_leading_good_c_hadron_pt");
	plot( h_leading_good_lf_hadron_pt, "h_leading_good_lf_hadron_pt");
	plot( h_leading_goodjet_pt, "h_leading_goodjet_pt");

	plot( h_leading_good_c_hadron_nfW_pt, "h_leading_good_c_hadron_nfW_pt");
	plot( h_leading_good_c_hadron_ifW_pt, "h_leading_good_c_hadron_ifW_pt");
	plot( h_n_good_c_hadron_ifW, "h_n_good_c_hadron_ifW");
	plot( h_Tau_traced_jet1Idx, "Tau_traced_jet1Idx");
	plot( h_inv_mass_top_toLQ_nfW, "h_inv_mass_top_toLQ_nfW");
	plot( h_inv_mass_top_toLQ_ifW, "h_inv_mass_top_toLQ_ifW");
	plot( h_inv_mass_W_tocs_ifW, "h_inv_mass_W_tocs_ifW");
	plot( h_inv_mass_top_toWb_ifW, "h_inv_mass_top_toWb_ifW");
/*
	plot( h_leading_btagged_jet_l_pt, "h_leading_btagged_jet_l_pt");
	plot( h_leading_btagged_jet_m_pt, "h_leading_btagged_jet_m_pt");
	plot( h_leading_btagged_jet_t_pt, "h_leading_btagged_jet_t_pt");
	plot( h_leading_ctagged_jet_l_pt, "h_leading_ctagged_jet_l_pt");
	plot( h_leading_ctagged_jet_m_pt, "h_leading_ctagged_jet_m_pt");
	plot( h_leading_ctagged_jet_t_pt, "h_leading_ctagged_jet_t_pt");
*/
	plot( h_leading_good_b_hadron_eta, "h_leading_good_b_hadron_eta");
	plot( h_leading_good_c_hadron_eta, "h_leading_good_c_hadron_eta");
	plot( h_leading_good_lf_hadron_eta, "h_leading_good_lf_hadron_eta");
	plot( h_leading_goodjet_eta, "h_leading_goodjet_eta");

	plot( h_DeepB_b, "h_DeepB_b");
	plot( h_DeepB_c, "h_DeepB_c");
	plot( h_DeepB_lf, "h_DeepB_lf");
	plot( h_DeepC_b, "h_DeepC_b");
	plot( h_DeepC_c, "h_DeepC_c");
	plot( h_DeepC_lf, "h_DeepC_lf"); 
	plot( h_ctag_2D_b, "h_ctag_2D_b");
	plot( h_ctag_2D_c, "h_ctag_2D_c");
	plot( h_ctag_2D_lf, "h_ctag_2D_lf");

	plot( h_n_bjet_l, "h_n_bjet_l");
	plot( h_n_bjet_m, "h_n_bjet_m");
	plot( h_n_bjet_t, "h_n_bjet_t");
	plot( h_n_cjet_l, "h_n_cjet_l");
	plot( h_n_cjet_m, "h_n_cjet_m");
	plot( h_n_cjet_t, "h_n_cjet_t");

	plot( h_n_original_c_hadron, "h_n_original_c_hadron");

	f.Close();

	auto report = df_S_bjet.Report();
	report->Print();

}
