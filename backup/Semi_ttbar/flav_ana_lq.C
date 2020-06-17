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

	//**** Top Mass Reconstruction ****//
//Top-reconstruction
//input:
//  jet_info: Jet_pt, Jet_eta, Jet_phi, Jet_mass
//  electron_info: Electron_pt, Electron_eta, Electron_phi, Electron_mass
//  muon_info: Muon_pt, Muon_eta, Muon_phi, Muon_mass
//  MET_info: MET_pt, MET_phi, MET_sumEt
//  index: Jet_idx, c_idx, tau_jet_idx, muon_idx, sel_b_idx, sel_tau_idx, sel_muon_idx
//out:
//  rvec_f (X, LQtop_mass, SMW_mass, SMtop_mass)
//
//channel_info:
//      qq:         sel_tau_idx > -1 && sel_muon_idx > -1 && Electron_pt < 0
//      electron:   sel_tau_idx > -1 && sel_muon_idx > -1 && Electron_pt > 0
//      muon:       sel_tau_idx > -1 && sel_muon_idx == -1 && Electron_pt < 0 
//      tau:        sel_tau_idx == -1 && sel_muon_idx > -1 && Electron_pt < 0


rvec_f top_reconstruction(rvec_f Jet_pt, rvec_f Jet_eta, rvec_f Jet_phi, rvec_f Jet_mass,   //Jet
        float Electron_pt, float Electron_eta, float Electron_phi, float Electron_mass,     //Electron
        rvec_f Muon_pt, rvec_f Muon_eta, rvec_f Muon_phi, rvec_f Muon_mass,                 //Muon
        float MET_pt, float MET_phi, float MET_sumEt,                                       //MET
        rvec_i jet_idx, rvec_i c_idx, rvec_i tau_jet_idx, rvec_i muon_idx,
        int sel_b_idx, int sel_tau_idx, int sel_muon_idx){
    rvec_f out;

    TLorentzVector c_jet, tau, muon, b_jet, from_SMW_1, from_SMW_2;
    float LQtop_mass, SMW_mass, SMtop_mass;
    float X_LQtop, X_SMW, X_SMtop;
    float X_min=999999999999999, X_min_LQtop_mass=-1, X_min_SMW_mass=-1, X_min_SMtop_mass=-1;
		int z = 700; float MET_pz, MET_px, MET_py; 

    if (sel_tau_idx > -1){
        tau_jet_idx.clear();
        tau_jet_idx.push_back(sel_tau_idx);
    }
    if (sel_muon_idx > -1){
        muon_idx.clear();
        muon_idx.push_back(sel_muon_idx);
    }

    //remove b_jet_idx in jet_idx & c_idx
    for (int i=0; i<jet_idx.size(); i++){
        if (jet_idx[i] == sel_b_idx) jet_idx.erase(jet_idx.begin()+i);
        if (i < c_idx.size() && c_idx[i] == sel_b_idx) c_idx.erase(c_idx.begin()+i);
    }

    b_jet.SetPtEtaPhiM(Jet_pt[sel_b_idx], Jet_eta[sel_b_idx], Jet_phi[sel_b_idx], Jet_mass[sel_b_idx]);
    from_SMW_1.SetPtEtaPhiM(Electron_pt, Electron_eta, Electron_phi, Electron_mass);

		for (int i=-700; i <= 700; i+=10){
			from_SMW_2.SetPtEtaPhiE(MET_pt,0,MET_phi,MET_pt);
			from_SMW_2.SetPz(i);
		
    for(int i=0; i<c_idx.size(); i++){
        c_jet.SetPtEtaPhiM(Jet_pt[c_idx[i]], Jet_eta[c_idx[i]], Jet_phi[c_idx[i]], Jet_mass[c_idx[i]]);
        for (int j=0; j<tau_jet_idx.size(); j++){
            if (tau_jet_idx[j] == c_idx[i]) continue;
            tau.SetPtEtaPhiM(Jet_pt[tau_jet_idx[j]], Jet_eta[tau_jet_idx[j]], Jet_phi[tau_jet_idx[j]], Jet_mass[tau_jet_idx[j]]);
            for (int k=0; k<muon_idx.size(); k++){
                muon.SetPtEtaPhiM(Muon_pt[muon_idx[k]], Muon_eta[muon_idx[k]], Muon_phi[muon_idx[k]], Muon_mass[muon_idx[k]]);
                LQtop_mass = (c_jet+tau+muon).M();
                X_LQtop = (173.0-LQtop_mass)*(173.0-LQtop_mass) / 1.41*1.41;

                for (int l=0; l<jet_idx.size(); l++){
                    if (sel_tau_idx > -1 && sel_muon_idx == -1 && Electron_pt < 0){ //muon
                        if (l == muon_idx.size()) break;
                        if (l == k) continue;
                        from_SMW_1.SetPtEtaPhiM(Muon_pt[muon_idx[l]], Muon_eta[muon_idx[l]], Muon_phi[muon_idx[l]], Muon_mass[muon_idx[l]]);
                    } else if (sel_tau_idx == -1 && sel_muon_idx > -1 && Electron_pt < 0){ //tau
                        if (l == tau_jet_idx.size()) break;
                        if (tau_jet_idx[l] == c_idx[i] or l == j) continue;
                        from_SMW_1.SetPtEtaPhiM(Jet_pt[tau_jet_idx[l]], Jet_eta[tau_jet_idx[l]], Jet_phi[tau_jet_idx[l]], Jet_mass[tau_jet_idx[l]]);
                    }
                    if (sel_tau_idx > -1 && sel_muon_idx > -1 && Electron_pt < 0){ //qq
                        if (jet_idx[l] == c_idx[i] or jet_idx[l]==tau_jet_idx[j]) continue;
                        from_SMW_1.SetPtEtaPhiM(Jet_pt[jet_idx[l]], Jet_eta[jet_idx[l]], Jet_phi[jet_idx[l]], Jet_mass[jet_idx[l]]);
                        for (int n=0; n<jet_idx.size(); n++){
                            if (jet_idx[n] == c_idx[i] or jet_idx[n] == tau_jet_idx[j] or n == l) continue;
                            from_SMW_2.SetPtEtaPhiM(Jet_pt[jet_idx[n]], Jet_eta[jet_idx[n]], Jet_phi[jet_idx[n]], Jet_mass[jet_idx[n]]);

                            SMW_mass = (from_SMW_1 + from_SMW_2).M();
                            X_SMW = (80.379-SMW_mass)*(80.379-SMW_mass) / 2.085*2.085;
                            SMtop_mass = (b_jet + from_SMW_1 + from_SMW_2).M();
                            X_SMtop = (173.0-SMtop_mass)*(173.0-SMtop_mass) / 1.41*1.41; 
                        }
                    } else{ //electron or muon or tau
                        SMW_mass = (from_SMW_1 + from_SMW_2).M();
                        X_SMW = (80.379-SMW_mass)*(80.379-SMW_mass) / 2.085*2.085;
                        SMtop_mass = (b_jet + from_SMW_1 + from_SMW_2).M();
                        X_SMtop = (173.0-SMtop_mass)*(173.0-SMtop_mass) / 1.41*1.41; 
                    }
                    if (X_LQtop + X_SMW + X_SMtop < X_min){
                        X_min = X_LQtop + X_SMW + X_SMtop;
                        X_min_LQtop_mass = LQtop_mass;
                        X_min_SMW_mass = SMW_mass;
                        X_min_SMtop_mass = SMtop_mass;
                    }
                    if (Electron_pt >= 0) break;
                }
            }
        }
			}
    }

    out.push_back(X_min);
    out.push_back(X_min_LQtop_mass);
    out.push_back(X_min_SMW_mass);
    out.push_back(X_min_SMtop_mass);
    return out;
}
void flav_ana_lq(TString input = "LQ"){
	std::vector<std::string> file = {"/cms/ldap_home/ljw1015/public/LQ_Signals/LQ_2018_nano.root"};
	if (input.Contains("LQ")){
		file = {"/cms/ldap_home/ljw1015/public/LQ_Signals/LQ_2018_nano.root",
			"/T2_KR_KISTI/store/user/jolim/LQ_2018/LQ_2018NANO/200517_045835/0000/LQ_2018_nano_1.root",
			"/T2_KR_KISTI/store/user/jolim/LQ_2018/LQ_2018NANO_A1/200523_183528/0000/LQ_2018_nano_1.root"};
	}else if (input.Contains("ttbar")){
		 file = {
		 "/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/*.root"
//		 "/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/*.root"
		 };
	}
	ROOT::RDataFrame df("Events",file);

	//S1:muon, S2:tau, S3:hadronFlavour, S4:genTtbarId, S5:b,c-tagging, S6:Filter for pt

	//**** HLT Trigger ****//
	auto df_S0_HLT = df.Filter("HLT_IsoMu24","SingleMuon Trigger");

	//**** Muon Selection (Tight muonId) ****//
	auto df_S1_goodmuon = df.Define("goodmuon","Muon_pt > 30  && abs(Muon_eta) < 2.4 && Muon_tightId && Muon_pfRelIso03_all < 0.15")
													.Define("n_goodmuon","Sum(goodmuon)")
													.Filter("n_goodmuon >= 1","Least one muon")
													.Define("goodmuon_idx",good_idx,{"goodmuon"})
													.Define("goodmuon1_idx",leading_idx,{"goodmuon_idx"});

	//**** Tau Selection ****//
	auto df_S2_goodtau = df_S1_goodmuon.Define("goodtau","Tau_pt > 30 && abs(Tau_eta) < 2.3 &&  Tau_idDecayModeNewDMs && Tau_idDeepTau2017v2p1VSmu & 1 && Tau_idDeepTau2017v2p1VSe & 1 && Tau_idDeepTau2017v2p1VSjet & 16")
																		 .Define("n_goodtau","Sum(goodtau)")
																		 .Filter("n_goodtau >= 1","Least one tau")
																		 .Define("goodtau_idx",good_idx,{"goodtau"})
																		 .Define("goodtau1_idx",leading_idx,{"goodtau_idx"});
	auto df_S2_goodtau2 = df_S2_goodtau.Filter("n_goodtau >=2","Two tau jets")
																		 .Define("goodtau2_idx",second_idx,{"goodtau_idx"});
	auto df_S2_goodtau1_mother = df_S2_goodtau.Define("goodtau1_genidx","Tau_genPartIdx[goodtau1_idx]")
																						.Filter("goodtau1_genidx > -1","gen traced tau")
																						.Define("goodtau1_motheridx","GenPart_genPartIdxMother[goodtau1_genidx]")
																						.Filter("goodtau1_motheridx > -1", "mother traced tau")
																						.Define("goodtau1_mother","abs(GenPart_pdgId[goodtau1_motheridx])");
	auto df_S2_goodtau1_gmother = df_S2_goodtau1_mother.Filter("goodtau1_mother != 6","exclude if tau's mother top") 
																										 .Define("goodtau1_gmotheridx","GenPart_genPartIdxMother[goodtau1_motheridx]")
																										 .Filter("goodtau1_gmotheridx > -1", "gmother traced")
																										 .Define("goodtau1_gmother","abs(GenPart_pdgId[goodtau1_gmotheridx])");
	auto df_S2_taumu_mother = df_S2_goodtau1_gmother.Filter("goodtau1_mother == 13","tau's mother muon")
																										.Define("goodtau1_ggmotheridx","GenPart_genPartIdxMother[goodtau1_gmotheridx]")
																										.Define("goodtau1_ggmother","abs(GenPart_pdgId[goodtau1_ggmotheridx])");
	auto df_S2_taumuW_mother = df_S2_taumu_mother.Filter("goodtau1_gmother == 24","tau muon W mother");
	auto df_S2_taumumu_mother = df_S2_taumu_mother.Filter("goodtau1_gmother == 13","tau muon muon gmother");
		

	auto df_S2_goodtau2_mother = df_S2_goodtau2.Define("goodtau2_genidx","Tau_genPartIdx[goodtau2_idx]")
																							.Filter("goodtau2_genidx > -1","gen traced tau2")
																							.Define("goodtau2_motheridx","GenPart_genPartIdxMother[goodtau2_genidx]")
																							.Filter("goodtau2_motheridx > -1", "mother traced tau")
																							.Define("goodtau2_mother","abs(GenPart_pdgId[goodtau2_motheridx])");

	auto df_S2_goodGentau = df_S1_goodmuon.Define("goodGentau","GenVisTau_pt > 30 && abs(GenVisTau_eta) < 2.3")
																				.Define("n_goodGentau","Sum(goodGentau)")
																				.Filter("n_goodGentau >= 1","GenTau Selection")
																				.Define("goodGentau_idx",good_idx,{"goodGentau"})
																				.Define("goodGentau1_idx",leading_idx,{"goodGentau_idx"});
	auto df_S2_goodGentau2 = df_S2_goodGentau.Filter("n_goodGentau >=2","Two Gentau jets")
																					 .Define("goodGentau2_idx",second_idx,{"goodGentau_idx"});
	auto df_S2_goodGentau1_mother = df_S2_goodGentau.Define("goodGentau1_motheridx","GenVisTau_genPartIdxMother[goodGentau1_idx]")
																									.Filter("goodGentau1_motheridx > -1", "mother traced Gentau")
																									.Define("goodGentau1_mother","abs(GenPart_pdgId[goodGentau1_motheridx])");
/*	auto df_S2_goodGentau1_gmother = df_S2_goodGentau1_mother.Define("goodGentau1_gmotheridx","GenPart_genPartIdxMother[goodGentau1_motheridx]")
																													 .Filter("goodGentau1_gmotheridx > -1", "gmother traced")
																													 .Define("goodGentau1_gmother","abs(GenPart_pdgId[goodGentau1_gmotheridx])");
*/	auto df_S2_goodGentau2_mother = df_S2_goodGentau2.Define("goodGentau2_motheridx","GenVisTau_genPartIdxMother[goodGentau2_idx]")
																									 .Filter("goodGentau2_motheridx > -1", "mother traced Gentau")
																									 .Define("goodGentau2_mother","abs(GenPart_pdgId[goodGentau2_motheridx])");


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

	//**** Top Mass Reconstruction ****//
	auto df_S7_base = df_S3_goodhadron.Filter("Sum(good_b_hadron) ==1", "One b jet")
																		.Filter("Sum(good_c_hadron) >= 1", "Least one c jet")
																		.Filter("Sum(goodjet) >=3","Least three jets")//separated fitler for report
																		.Define("not_ele","float(-9999.9);")
																		.Define("mu_tau2", "int(-1);")
																		.Define("goodelec","Electron_pt > 30 && abs(Electron_eta) < 2.4 && Electron_mvaFall17V2Iso_WP90")
																		.Define("n_goodelec","Sum(goodelec)")
																		.Define("goodjet_idx", good_idx, {"goodjet"})
																		.Define("good_b_hadron_idx",good_idx,{"good_b_hadron"})
																		.Define("good_b_hadron1_idx",leading_idx,{"good_b_hadron_idx"})
																		.Define("good_c_hadron_idx", good_idx, {"good_c_hadron"})
																		.Define("goodtau_jet_idx", "Tau_jetIdx[goodtau]")
																		.Define("goodtau1_jet_idx", "Tau_jetIdx[goodtau1_idx]");
	auto cate11 = df_S7_base.Filter("n_goodtau ==1 && n_goodmuon ==1","category 11");
	auto cate12 = df_S7_base.Filter("n_goodtau ==1 && n_goodmuon ==2","category 11");
	auto cate13 = df_S7_base.Filter("n_goodtau ==1 && n_goodmuon ==3","category 11");
	auto cate21 = df_S7_base.Filter("n_goodtau ==2 && n_goodmuon ==1","category 11");
	auto cate22 = df_S7_base.Filter("n_goodtau ==2 && n_goodmuon ==2","category 11");
	auto cate23 = df_S7_base.Filter("n_goodtau ==2 && n_goodmuon ==3","category 11");
	auto cateqq = df_S7_base.Filter("n_goodtau ==1 && n_goodmuon ==1 && n_goodelec == 0","category qq");

	auto df_S7_tagbase = df_S5_good_ctag.Filter("Sum(goodbjet_m) ==1", "One b-tagged jet")
																 .Filter("Sum(goodcjet_m) >= 1", "Least one c-tagged jet")
																 .Filter("Sum(goodjet) >=3","Least three jets")//separated fitler for report
																		.Define("not_ele","float(-9999.9);")
																		.Define("mu_tau2", "int(-1);")
																		.Define("goodelec","Electron_pt > 30 && abs(Electron_eta) < 2.4 && Electron_mvaFall17V2Iso_WP90")
																		.Define("n_goodelec","Sum(goodelec)")
																		.Define("goodjet_idx", good_idx, {"goodjet"})
																		.Define("goodbjet_m_idx",good_idx,{"goodbjet_m"})
																		.Define("goodbjet_m1_idx",leading_idx,{"goodbjet_m_idx"})
																		.Define("goodcjet_m_idx", good_idx, {"goodcjet_m"})
																		.Define("goodtau_jet_idx", "Tau_jetIdx[goodtau]")
																		.Define("goodtau1_jet_idx", "Tau_jetIdx[goodtau1_idx]");
	auto tagcate11 = df_S7_tagbase.Filter("n_goodtau ==1 && n_goodmuon ==1","tagcategory 11");
	auto tagcate12 = df_S7_tagbase.Filter("n_goodtau ==1 && n_goodmuon ==2","tagcategory 11");
	auto tagcate13 = df_S7_tagbase.Filter("n_goodtau ==1 && n_goodmuon ==3","tagcategory 11");
	auto tagcate21 = df_S7_tagbase.Filter("n_goodtau ==2 && n_goodmuon ==1","tagcategory 11");
	auto tagcate22 = df_S7_tagbase.Filter("n_goodtau ==2 && n_goodmuon ==2","tagcategory 11");
	auto tagcate23 = df_S7_tagbase.Filter("n_goodtau ==2 && n_goodmuon ==3","tagcategory 11");

	//tau test
	auto tau_flavour = df_S7_base.Define("tau_flavour","Jet_hadronFlavour[goodtau1_jet_idx]");
	auto tau_flavour2 = df_S7_base.Filter("n_goodtau == 2","two taus") 
																.Define("goodtau2_idx",second_idx,{"goodtau_idx"}) .Define("goodtau2_jet_idx","Tau_jetIdx[goodtau2_idx]")
																.Define("tau_flavour2","Jet_hadronFlavour[goodtau2_jet_idx]");
	auto tau_goodjet = df_S7_base.Define("tau_goodjet","goodjet[goodtau1_jet_idx]");
	auto tau_goodjet2 = df_S7_base.Filter("n_goodtau == 2","two taus")
																.Define("goodtau2_idx",second_idx,{"goodtau_idx"}) .Define("goodtau2_jet_idx","Tau_jetIdx[goodtau2_idx]")
																.Define("tau_goodjet2","goodjet[goodtau2_jet_idx]");
		//qq channel
	auto df_S7_cmutaubqq = df_S7_base.Filter("n_goodmuon == 1 && n_goodtau == 1 && n_goodelec == 0", "mu1,tau1,elec0(c_mu_tau_b_q_q)");
				//													 .Filter("Sum(goodjet)>=5", "c_muon_tau_b_q_q(five loops)");
  auto df_S7_reco_qq = df_S7_cmutaubqq.Define("reco_qq", top_reconstruction,
			{"Jet_pt", "Jet_eta", "Jet_phi", "Jet_mass", "not_ele", "not_ele", "not_ele", "not_ele",
			 "Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "MET_pt", "MET_phi", "MET_sumEt",
			 "goodjet_idx","good_c_hadron_idx", "goodtau_jet_idx", "goodmuon_idx", "good_b_hadron1_idx", "goodtau1_jet_idx", "goodmuon1_idx"})
				.Define("chi2_qq", "reco_qq[0]").Define("LQtop_qq", "reco_qq[1]").Define("SMW_qq", "reco_qq[2]").Define("SMtop_qq", "reco_qq[3]")
				.Filter("LQtop_qq > 0","LQtop_qq updated") .Filter("SMW_qq > 0","SMW_qq updated") .Filter("SMtop_qq > 0","SMtop_qq updated")
				.Filter("chi2_qq < 999999999999999","chi2_qq updated");
	auto df_S7_reco_qq_cut = df_S7_reco_qq.Filter("chi2_qq < 3000","chi2_qq < 3000");


		//tag qq channel
	auto df_S7_tagcmutaubqq = df_S7_tagbase.Filter("n_goodmuon == 1 && n_goodtau == 1 && n_goodelec == 0", "mu1,tau1,elec0(c_mu_tau_b_q_q)");
				//													 .Filter("Sum(goodjet)>=5", "c_muon_tau_b_q_q(five loops)");
  auto df_S7_tagreco_qq = df_S7_tagcmutaubqq.Define("reco_qq", top_reconstruction,
			{"Jet_pt", "Jet_eta", "Jet_phi", "Jet_mass", "not_ele", "not_ele", "not_ele", "not_ele",
			 "Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "MET_pt", "MET_phi", "MET_sumEt",
			 "goodjet_idx","goodcjet_m_idx", "goodtau_jet_idx", "goodmuon_idx", "goodbjet_m1_idx", "goodtau1_jet_idx", "goodmuon1_idx"})
				.Define("chi2_qq", "reco_qq[0]").Define("LQtop_qq", "reco_qq[1]").Define("SMW_qq", "reco_qq[2]").Define("SMtop_qq", "reco_qq[3]")
				.Filter("LQtop_qq > 0","LQtop_qq updated") .Filter("SMW_qq > 0","SMW_qq updated") .Filter("SMtop_qq > 0","SMtop_qq updated")
				.Filter("chi2_qq < 999999999999999","chi2_qq updated");
	auto df_S7_tagreco_qq_cut = df_S7_tagreco_qq.Filter("chi2_qq < 2000","chi2_qq < 2000");

		//enu channel
	auto df_S7_cmutaubenu = df_S7_base.Filter("n_goodmuon >= 1 && n_goodtau >= 1 && n_goodelec == 1", "mu1,tau1,elec1(c_mu_tau_b_e_nu)")
			.Define("goodelec_idx",good_idx, {"goodelec"})      
			.Define("goodelec1_idx",leading_idx, {"goodelec_idx"})
			.Define("goodelec1_pt", "Electron_pt[goodelec1_idx]")
			.Define("goodelec1_eta", "Electron_eta[goodelec1_idx]")
			.Define("goodelec1_phi", "Electron_phi[goodelec1_idx]")
			.Define("goodelec1_mass", "Electron_mass[goodelec1_idx]");
	auto df_S7_reco_enu = df_S7_cmutaubenu.Define("reco_enu", top_reconstruction,
			{"Jet_pt", "Jet_eta", "Jet_phi", "Jet_mass", "goodelec1_pt", "goodelec1_eta", "goodelec1_phi", "goodelec1_mass",
			 "Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "MET_pt", "MET_phi", "MET_sumEt",
			 "goodjet_idx","good_c_hadron_idx", "goodtau_jet_idx", "goodmuon_idx", "good_b_hadron1_idx", "goodtau1_jet_idx", "goodmuon1_idx"})
				.Define("chi2_enu", "reco_enu[0]").Define("LQtop_enu", "reco_enu[1]").Define("SMW_enu", "reco_enu[2]").Define("SMtop_enu", "reco_enu[3]")
				.Filter("LQtop_enu > 0","LQtop_enu updated") .Filter("SMW_enu > 0","SMW_enu updated") .Filter("SMtop_enu > 0","SMtop_enu updated")
				.Filter("chi2_enu < 999999999999999","chi2_enu updated");
	auto df_S7_reco_enu_cut = df_S7_reco_enu.Filter("chi2_enu < 3000","chi2_enu < 3000");

		//tag enu channel
	auto df_S7_tagcmutaubenu = df_S7_tagbase.Filter("n_goodmuon >= 1 && n_goodtau >= 1 && n_goodelec == 1", "mu1,tau1,elec1(c_mu_tau_b_e_nu)")
			.Define("goodelec_idx",good_idx, {"goodelec"})      
			.Define("goodelec1_idx",leading_idx, {"goodelec_idx"})
			.Define("goodelec1_pt", "Electron_pt[goodelec1_idx]")
			.Define("goodelec1_eta", "Electron_eta[goodelec1_idx]")
			.Define("goodelec1_phi", "Electron_phi[goodelec1_idx]")
			.Define("goodelec1_mass", "Electron_mass[goodelec1_idx]");
	auto df_S7_tagreco_enu = df_S7_tagcmutaubenu.Define("reco_enu", top_reconstruction,
			{"Jet_pt", "Jet_eta", "Jet_phi", "Jet_mass", "goodelec1_pt", "goodelec1_eta", "goodelec1_phi", "goodelec1_mass",
			 "Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "MET_pt", "MET_phi", "MET_sumEt",
			 "goodjet_idx","goodcjet_m_idx", "goodtau_jet_idx", "goodmuon_idx", "goodbjet_m1_idx", "goodtau1_jet_idx", "goodmuon1_idx"})
				.Define("chi2_enu", "reco_enu[0]").Define("LQtop_enu", "reco_enu[1]").Define("SMW_enu", "reco_enu[2]").Define("SMtop_enu", "reco_enu[3]")
				.Filter("LQtop_enu > 0","LQtop_enu updated") .Filter("SMW_enu > 0","SMW_enu updated") .Filter("SMtop_enu > 0","SMtop_enu updated")
				.Filter("chi2_enu < 999999999999999","chi2_enu updated");
	auto df_S7_tagreco_enu_cut = df_S7_reco_enu.Filter("chi2_enu < 2000","chi2_enu < 2000");

		//munu channel
	auto df_S7_cmutaubmunu = df_S7_base.Filter("n_goodmuon >= 2 && n_goodtau >= 1 && n_goodelec == 0", "mu2,tau1,elec0(c_mu_tau_b_mu_nu)");
  auto df_S7_reco_munu = df_S7_cmutaubmunu.Define("reco_munu", top_reconstruction,
			{"Jet_pt", "Jet_eta", "Jet_phi", "Jet_mass", "not_ele", "not_ele", "not_ele", "not_ele",
			 "Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "MET_pt", "MET_phi", "MET_sumEt",
			 "goodjet_idx","good_c_hadron_idx", "goodtau_jet_idx", "goodmuon_idx", "good_b_hadron1_idx", "goodtau1_jet_idx","mu_tau2"})
				.Define("chi2_munu", "reco_munu[0]").Define("LQtop_munu", "reco_munu[1]").Define("SMW_munu", "reco_munu[2]").Define("SMtop_munu", "reco_munu[3]")
				.Filter("LQtop_munu > 0","LQtop_munu updated") .Filter("SMW_munu > 0","SMW_munu updated") .Filter("SMtop_munu > 0","SMtop_munu updated")
				.Filter("chi2_munu < 999999999999999","chi2_munu updated");
	auto df_S7_reco_munu_cut = df_S7_reco_munu.Filter("chi2_munu < 3000","chi2_munu < 3000");

		//tag munu channel
	auto df_S7_tagcmutaubmunu = df_S7_tagbase.Filter("n_goodmuon >= 2 && n_goodtau >= 1 && n_goodelec == 0", "mu2,tau1,elec0(c_mu_tau_b_mu_nu)");
  auto df_S7_tagreco_munu = df_S7_tagcmutaubmunu.Define("reco_munu", top_reconstruction,
			{"Jet_pt", "Jet_eta", "Jet_phi", "Jet_mass", "not_ele", "not_ele", "not_ele", "not_ele",
			 "Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "MET_pt", "MET_phi", "MET_sumEt",
			 "goodjet_idx","goodcjet_m_idx", "goodtau_jet_idx", "goodmuon_idx", "goodbjet_m1_idx", "goodtau1_jet_idx","mu_tau2"})
				.Define("chi2_munu", "reco_munu[0]").Define("LQtop_munu", "reco_munu[1]").Define("SMW_munu", "reco_munu[2]").Define("SMtop_munu", "reco_munu[3]")
				.Filter("LQtop_munu > 0","LQtop_munu updated") .Filter("SMW_munu > 0","SMW_munu updated") .Filter("SMtop_munu > 0","SMtop_munu updated")
				.Filter("chi2_munu < 999999999999999","chi2_munu updated");
	auto df_S7_tagreco_munu_cut = df_S7_tagreco_munu.Filter("chi2_munu < 2000","chi2_munu < 2000");

		//taunu channel
	auto df_S7_cmutaubtaunu = df_S7_base.Filter("n_goodmuon == 1 && n_goodtau == 2 && n_goodelec == 0", "mu1,tau2,elec0(c_mu_tau_b_tau_nu)");
//																			.Filter("Sum(goodjet) >= 4","c_mu_tau_b_tau_nu(four loops)");
  auto df_S7_reco_taunu = df_S7_cmutaubtaunu.Define("reco", top_reconstruction,
			{"Jet_pt", "Jet_eta", "Jet_phi", "Jet_mass", "not_ele", "not_ele", "not_ele", "not_ele",
			 "Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "MET_pt", "MET_phi", "MET_sumEt",
			 "goodjet_idx","good_c_hadron_idx", "goodtau_jet_idx", "goodmuon_idx", "good_b_hadron1_idx", "mu_tau2","goodmuon1_idx"})
				.Define("chi2_taunu", "reco[0]").Define("LQtop_taunu", "reco[1]").Define("SMW_taunu", "reco[2]").Define("SMtop_taunu", "reco[3]")
				.Filter("LQtop_taunu > 0","LQtop_taunu updated") .Filter("SMW_taunu > 0","SMW_taunu updated") .Filter("SMtop_taunu > 0","SMtop_taunu updated")
				.Filter("chi2_taunu < 999999999999999","chi2_taunu updated");
	auto df_S7_reco_taunu_cut = df_S7_reco_taunu.Filter("chi2_taunu < 3000","chi2_taunu < 3000");

		//tag taunu channel
	auto df_S7_tagcmutaubtaunu = df_S7_tagbase.Filter("n_goodmuon == 1 && n_goodtau == 2 && n_goodelec == 0", "mu1,tau2,elec0(c_mu_tau_b_tau_nu)");
//																			.Filter("Sum(goodjet) >= 4","c_mu_tau_b_tau_nu(four loops)");
  auto df_S7_tagreco_taunu = df_S7_tagcmutaubtaunu.Define("reco", top_reconstruction,
			{"Jet_pt", "Jet_eta", "Jet_phi", "Jet_mass", "not_ele", "not_ele", "not_ele", "not_ele",
			 "Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "MET_pt", "MET_phi", "MET_sumEt",
			 "goodjet_idx","goodcjet_m_idx", "goodtau_jet_idx", "goodmuon_idx", "goodbjet_m1_idx", "mu_tau2","goodmuon1_idx"})
				.Define("chi2_taunu", "reco[0]").Define("LQtop_taunu", "reco[1]").Define("SMW_taunu", "reco[2]").Define("SMtop_taunu", "reco[3]")
				.Filter("LQtop_taunu > 0","LQtop_taunu updated") .Filter("SMW_taunu > 0","SMW_taunu updated") .Filter("SMtop_taunu > 0","SMtop_taunu updated")
				.Filter("chi2_taunu < 999999999999999","chi2_taunu updated");
	auto df_S7_tagreco_taunu_cut = df_S7_tagreco_taunu.Filter("chi2_taunu < 2000","chi2_taunu < 2000");

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
	auto h_muon_pt = df_S1_goodmuon.Define("goodmuon1_pt","Muon_pt[goodmuon1_idx]").Histo1D({"h_muon_pt", "h_muon_pt", 10, 0, 100}, "goodmuon1_pt");
	auto h_muon_eta = df_S1_goodmuon.Define("goodmuon1_eta","Muon_eta[goodmuon1_idx]").Histo1D({"h_muon_eta", "h_muon_eta", 50, -5, 5}, "goodmuon1_eta");
	auto h_n_goodmuon = df_S1_goodmuon.Histo1D({"h_n_goodmuon", "h_n_goodmuon", 5, 0, 5}, "n_goodmuon"); 

	auto h_tau_pt = df_S2_goodtau.Define("goodtau1_pt","Tau_pt[goodtau1_idx]").Histo1D({"h_tau_pt", "h_tau_pt", 10, 0, 100}, "goodtau1_pt");
	auto h_tau_eta = df_S2_goodtau.Define("goodtau1_eta","Tau_eta[goodtau1_idx]").Histo1D({"h_tau_eta", "h_tau_eta", 50, -5, 5}, "goodtau1_eta");
	auto h_n_tau = df_S1_goodmuon.Histo1D({"h_n_tau", "h_n_tau", 5, 0, 5}, "nTau");
	auto h_n_goodtau = df_S2_goodtau.Histo1D({"h_n_goodtau", "h_n_goodtau", 5, 0, 5}, "n_goodtau");
	auto h_goodtau1_mother = df_S2_goodtau1_mother.Histo1D({"h_goodtau1_mother","h_goodtau1_mother",50,0,50},"goodtau1_mother");
	auto h_goodtau1_gmother = df_S2_goodtau1_gmother.Histo1D({"h_goodtau1_gmother","h_goodtau1_gmother",50,0,50},"goodtau1_gmother");
	auto h_taumu_mother = df_S2_taumu_mother.Histo1D({"h_taumu_mother","h_taumu_mother",50,0,50},"goodtau1_gmother");
	auto h_taumuW_mother = df_S2_taumuW_mother.Histo1D({"h_taumuW_mother","b_taumuW_mother",50,0,50},"goodtau1_ggmother");
	auto h_taumumu_mother = df_S2_taumumu_mother.Histo1D({"h_taumumu_mother","b_taumumu_mother",50,0,50},"goodtau1_ggmother");
	auto h_goodtau2_mother = df_S2_goodtau2_mother.Histo1D({"h_goodtau2_mother","h_goodtau2_mother",50,0,50},"goodtau2_mother");
	auto h_n_goodGentau = df_S2_goodGentau.Histo1D({"h_n_goodGentau","h_n_goodGentau",10,0,10},"n_goodGentau");
	auto h_goodGentau1_mother = df_S2_goodGentau1_mother.Histo1D({"h_goodGentau1_mother","h_goodGentau1_mother",50,0,50},"goodGentau1_mother");
//	auto h_goodGentau1_gmother = df_S2_goodGentau1_gmother.Histo1D({"h_goodGentau1_gmother","h_goodGentau1_gmother",50,0,50},"goodGentau1_gmother");
	auto h_goodGentau2_mother = df_S2_goodGentau2_mother.Histo1D({"h_goodGentau2_mother","h_goodGentau2_mother",50,0,50},"goodGentau2_mother");


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

	auto H_n_goodmuon = df_S7_base.Histo1D({"H_n_goodmuon", "H_n_goodmuon", 5, 0, 5}, "n_goodmuon");
	auto H_n_goodtau = df_S7_base.Histo1D({"H_n_goodtau", "H_n_goodtau", 5, 0, 5}, "n_goodtau");
	auto H_n_goodelec = df_S7_base.Histo1D({"H_n_goodelec", "H_n_goodelec", 5, 0, 5}, "n_goodelec");
	//ntau/nmuon
	auto h_n_goodelec11 = cate11.Histo1D({"h_n_goodelec11","h_n_goodelec11",5,0,5},"n_goodelec");
	auto h_n_goodelec12 = cate12.Histo1D({"h_n_goodelec12","h_n_goodelec12",5,0,5},"n_goodelec");
	auto h_n_goodelec13 = cate13.Histo1D({"h_n_goodelec13","h_n_goodelec13",5,0,5},"n_goodelec");
	auto h_n_goodelec21 = cate21.Histo1D({"h_n_goodelec21","h_n_goodelec21",5,0,5},"n_goodelec");
	auto h_n_goodelec22 = cate22.Histo1D({"h_n_goodelec22","h_n_goodelec22",5,0,5},"n_goodelec");
	auto h_n_goodelec23 = cate23.Histo1D({"h_n_goodelec23","h_n_goodelec23",5,0,5},"n_goodelec");
	auto h_n_goodjetqq = cateqq.Define("n_goodjet","Sum(goodjet)") .Histo1D({"h_n_goodjet","h_n_goodjet",5,0,5},"n_goodjet");

	auto h_n_taggoodelec11 = tagcate11.Histo1D({"h_n_taggoodelec11","h_n_taggoodelec11",5,0,5},"n_goodelec");
	auto h_n_taggoodelec12 = tagcate12.Histo1D({"h_n_taggoodelec12","h_n_taggoodelec12",5,0,5},"n_goodelec");
	auto h_n_taggoodelec13 = tagcate13.Histo1D({"h_n_taggoodelec13","h_n_taggoodelec13",5,0,5},"n_goodelec");
	auto h_n_taggoodelec21 = tagcate21.Histo1D({"h_n_taggoodelec21","h_n_taggoodelec21",5,0,5},"n_goodelec");
	auto h_n_taggoodelec22 = tagcate22.Histo1D({"h_n_taggoodelec22","h_n_taggoodelec22",5,0,5},"n_goodelec");
	auto h_n_taggoodelec23 = tagcate23.Histo1D({"h_n_taggoodelec23","h_n_taggoodelec23",5,0,5},"n_goodelec");

	auto h_tau_flavour = tau_flavour.Histo1D({"h_tau_flavour","h_tau_flavour",10,0,10},"tau_flavour");
	auto h_tau_flavour2 = tau_flavour2.Histo1D({"h_tau_flavour2","h_tau_flavour2",10,0,10},"tau_flavour2");
	auto h_tau_goodjet = tau_goodjet.Histo1D({"h_tau_goodjet","h_tau_goodjet",3,0,3},"tau_goodjet");
	auto h_tau_goodjet2 = tau_goodjet2.Histo1D({"h_tau_goodjet2","h_tau_goodjet2",3,0,3},"tau_goodjet2");

	auto h_chi2_qq = df_S7_reco_qq.Histo1D({"h_chi2_qq","h_chi2_qq",30, 0, 30000}, "chi2_qq");
	auto h_LQtop_qq_invm = df_S7_reco_qq.Histo1D({"h_LQtop_qq_invm","h_LQtop_qq_invm",50, 0, 500}, "LQtop_qq");
	auto h_SMW_qq_invm = df_S7_reco_qq.Histo1D({"h_SMW_qq_invm","h_SMW_qq_invm",50, 0, 500}, "SMW_qq");
	auto h_SMtop_qq_invm = df_S7_reco_qq.Histo1D({"h_SMtop_qq_invm","h_SMtop_qq_invm",50, 0, 500}, "SMtop_qq");

	auto h_tagchi2_qq = df_S7_tagreco_qq.Histo1D({"h_tagchi2_qq","h_tagchi2_qq",30, 0, 30000}, "chi2_qq");
	auto h_tagLQtop_qq_invm = df_S7_tagreco_qq.Histo1D({"h_tagLQtop_qq_invm","h_tagLQtop_qq_invm",25, 0, 500}, "LQtop_qq");
	auto h_tagSMW_qq_invm = df_S7_tagreco_qq.Histo1D({"h_tagSMW_qq_invm","h_tagSMW_qq_invm",25, 0, 500}, "SMW_qq");
	auto h_tagSMtop_qq_invm = df_S7_tagreco_qq.Histo1D({"h_tagSMtop_qq_invm","h_tagSMtop_qq_invm",25, 0, 500}, "SMtop_qq");

	auto h_chi2_enu = df_S7_reco_enu.Histo1D({"h_chi2_enu","h_chi2_enu",30, 0, 30000}, "chi2_enu");
	auto h_LQtop_enu_invm = df_S7_reco_enu.Histo1D({"h_LQtop_enu_invm","h_LQtop_enu_invm",50, 0, 500}, "LQtop_enu");
	auto h_SMW_enu_invm = df_S7_reco_enu.Histo1D({"h_SMW_enu_invm","h_SMW_enu_invm",50, 0, 500}, "SMW_enu");
	auto h_SMtop_enu_invm = df_S7_reco_enu.Histo1D({"h_SMtop_enu_invm","h_SMtop_enu_invm",50, 0, 500}, "SMtop_enu");

	auto h_tagchi2_enu = df_S7_tagreco_enu.Histo1D({"h_tagchi2_enu","h_tagchi2_enu",30, 0, 30000}, "chi2_enu");
	auto h_tagLQtop_enu_invm = df_S7_tagreco_enu.Histo1D({"h_tagLQtop_enu_invm","h_tagLQtop_enu_invm",25, 0, 500}, "LQtop_enu");
	auto h_tagSMW_enu_invm = df_S7_tagreco_enu.Histo1D({"h_tagSMW_enu_invm","h_tagSMW_enu_invm",25, 0, 500}, "SMW_enu");
	auto h_tagSMtop_enu_invm = df_S7_tagreco_enu.Histo1D({"h_tagSMtop_enu_invm","h_tagSMtop_enu_invm",25, 0, 500}, "SMtop_enu");

	auto h_chi2_munu = df_S7_reco_munu.Histo1D({"h_chi2_munu","h_chi2_munu",30, 0, 30000}, "chi2_munu");
	auto h_LQtop_munu_invm = df_S7_reco_munu.Histo1D({"h_LQtop_munu_invm","h_LQtop_munu_invm",50, 0, 500}, "LQtop_munu");
	auto h_SMW_munu_invm = df_S7_reco_munu.Histo1D({"h_SMW_munu_invm","h_SMW_munu_invm",50, 0, 500}, "SMW_munu");
	auto h_SMtop_munu_invm = df_S7_reco_munu.Histo1D({"h_SMtop_munu_invm","h_SMtop_munu_invm",50, 0, 500}, "SMtop_munu");

	auto h_tagchi2_munu = df_S7_tagreco_munu.Histo1D({"h_tagchi2_munu","h_tagchi2_munu",30, 0, 30000}, "chi2_munu");
	auto h_tagLQtop_munu_invm = df_S7_tagreco_munu.Histo1D({"h_tagLQtop_munu_invm","h_tagLQtop_munu_invm",25, 0, 500}, "LQtop_munu");
	auto h_tagSMW_munu_invm = df_S7_tagreco_munu.Histo1D({"h_tagSMW_munu_invm","h_tagSMW_munu_invm",25, 0, 500}, "SMW_munu");
	auto h_tagSMtop_munu_invm = df_S7_tagreco_munu.Histo1D({"h_tagSMtop_munu_invm","h_tagSMtop_munu_invm",25, 0, 500}, "SMtop_munu");

	auto h_chi2_taunu = df_S7_reco_taunu.Histo1D({"h_chi2_taunu","h_chi2_taunu",30, 0, 30000}, "chi2_taunu");
	auto h_LQtop_taunu_invm = df_S7_reco_taunu.Histo1D({"h_LQtop_taunu_invm","h_LQtop_taunu_invm",50, 0, 500}, "LQtop_taunu");
	auto h_SMW_taunu_invm = df_S7_reco_taunu.Histo1D({"h_SMW_taunu_invm","h_SMW_taunu_invm",50, 0, 500}, "SMW_taunu");
	auto h_SMtop_taunu_invm = df_S7_reco_taunu.Histo1D({"h_SMtop_taunu_invm","h_SMtop_taunu_invm",50, 0, 500}, "SMtop_taunu");

	auto h_tagchi2_taunu = df_S7_tagreco_taunu.Histo1D({"h_tagchi2_taunu","h_tagchi2_taunu",30, 0, 30000}, "chi2_taunu");
	auto h_tagLQtop_taunu_invm = df_S7_tagreco_taunu.Histo1D({"h_tagLQtop_taunu_invm","h_tagLQtop_taunu_invm",25, 0, 500}, "LQtop_taunu");
	auto h_tagSMW_taunu_invm = df_S7_tagreco_taunu.Histo1D({"h_tagSMW_taunu_invm","h_tagSMW_taunu_invm",25, 0, 500}, "SMW_taunu");
	auto h_tagSMtop_taunu_invm = df_S7_tagreco_taunu.Histo1D({"h_tagSMtop_taunu_invm","h_tagSMtop_taunu_invm",25, 0, 500}, "SMtop_taunu");

	auto h_chi2_qq_cut = df_S7_reco_qq_cut.Histo1D({"h_chi2_qq_cut","h_chi2_qq_cut",30, 0, 30000}, "chi2_qq");
	auto h_LQtop_qq_invm_cut = df_S7_reco_qq_cut.Histo1D({"h_LQtop_qq_invm_cut","h_LQtop_qq_invm_cut",50, 0, 500}, "LQtop_qq");
	auto h_SMW_qq_invm_cut = df_S7_reco_qq_cut.Histo1D({"h_SMW_qq_invm_cut","h_SMW_qq_invm_cut",50, 0, 500}, "SMW_qq");
	auto h_SMtop_qq_invm_cut = df_S7_reco_qq_cut.Histo1D({"h_SMtop_qq_invm_cut","h_SMtop_qq_invm_cut",50, 0, 500}, "SMtop_qq");

	auto h_tagchi2_qq_cut = df_S7_tagreco_qq_cut.Histo1D({"h_tagchi2_qq_cut","h_tagchi2_qq_cut",30, 0, 30000}, "chi2_qq");
	auto h_tagLQtop_qq_invm_cut = df_S7_tagreco_qq_cut.Histo1D({"h_tagLQtop_qq_invm_cut","h_tagLQtop_qq_invm_cut",25, 0, 500}, "LQtop_qq");
	auto h_tagSMW_qq_invm_cut = df_S7_tagreco_qq_cut.Histo1D({"h_tagSMW_qq_invm_cut","h_tagSMW_qq_invm_cut",25, 0, 500}, "SMW_qq");
	auto h_tagSMtop_qq_invm_cut = df_S7_tagreco_qq_cut.Histo1D({"h_tagSMtop_qq_invm_cut","h_tagSMtop_qq_invm_cut",25, 0, 500}, "SMtop_qq");

	auto h_chi2_enu_cut = df_S7_reco_enu_cut.Histo1D({"h_chi2_enu_cut","h_chi2_enu_cut",30, 0, 30000}, "chi2_enu");
	auto h_LQtop_enu_invm_cut = df_S7_reco_enu_cut.Histo1D({"h_LQtop_enu_invm_cut","h_LQtop_enu_invm_cut",50, 0, 500}, "LQtop_enu");
	auto h_SMW_enu_invm_cut = df_S7_reco_enu_cut.Histo1D({"h_SMW_enu_invm_cut","h_SMW_enu_invm_cut",50, 0, 500}, "SMW_enu");
	auto h_SMtop_enu_invm_cut = df_S7_reco_enu_cut.Histo1D({"h_SMtop_enu_invm_cut","h_SMtop_enu_invm_cut",50, 0, 500}, "SMtop_enu");

	auto h_tagchi2_enu_cut = df_S7_tagreco_enu_cut.Histo1D({"h_tagchi2_enu_cut","h_tagchi2_enu_cut",30, 0, 30000}, "chi2_enu");
	auto h_tagLQtop_enu_invm_cut = df_S7_tagreco_enu_cut.Histo1D({"h_tagLQtop_enu_invm_cut","h_tagLQtop_enu_invm_cut",25, 0, 500}, "LQtop_enu");
	auto h_tagSMW_enu_invm_cut = df_S7_tagreco_enu_cut.Histo1D({"h_tagSMW_enu_invm_cut","h_tagSMW_enu_invm_cut",25, 0, 500}, "SMW_enu");
	auto h_tagSMtop_enu_invm_cut = df_S7_tagreco_enu_cut.Histo1D({"h_tagSMtop_enu_invm_cut","h_tagSMtop_enu_invm_cut",25, 0, 500}, "SMtop_enu");

	auto h_chi2_munu_cut = df_S7_reco_munu_cut.Histo1D({"h_chi2_munu_cut","h_chi2_munu_cut",30, 0, 30000}, "chi2_munu");
	auto h_LQtop_munu_invm_cut = df_S7_reco_munu_cut.Histo1D({"h_LQtop_munu_invm_cut","h_LQtop_munu_invm_cut",50, 0, 500}, "LQtop_munu");
	auto h_SMW_munu_invm_cut = df_S7_reco_munu_cut.Histo1D({"h_SMW_munu_invm_cut","h_SMW_munu_invm_cut",50, 0, 500}, "SMW_munu");
	auto h_SMtop_munu_invm_cut = df_S7_reco_munu_cut.Histo1D({"h_SMtop_munu_invm_cut","h_SMtop_munu_invm_cut",50, 0, 500}, "SMtop_munu");

	auto h_tagchi2_munu_cut = df_S7_tagreco_munu_cut.Histo1D({"h_tagchi2_munu_cut","h_tagchi2_munu_cut",30, 0, 30000}, "chi2_munu");
	auto h_tagLQtop_munu_invm_cut = df_S7_tagreco_munu_cut.Histo1D({"h_tagLQtop_munu_invm_cut","h_tagLQtop_munu_invm_cut",25, 0, 500}, "LQtop_munu");
	auto h_tagSMW_munu_invm_cut = df_S7_tagreco_munu_cut.Histo1D({"h_tagSMW_munu_invm_cut","h_tagSMW_munu_invm_cut",25, 0, 500}, "SMW_munu");
	auto h_tagSMtop_munu_invm_cut = df_S7_tagreco_munu_cut.Histo1D({"h_tagSMtop_munu_invm_cut","h_tagSMtop_munu_invm_cut",25, 0, 500}, "SMtop_munu");

	auto h_chi2_taunu_cut = df_S7_reco_taunu_cut.Histo1D({"h_chi2_taunu_cut","h_chi2_taunu_cut",30, 0, 30000}, "chi2_taunu");
	auto h_LQtop_taunu_invm_cut = df_S7_reco_taunu_cut.Histo1D({"h_LQtop_taunu_invm_cut","h_LQtop_taunu_invm_cut",50, 0, 500}, "LQtop_taunu");
	auto h_SMW_taunu_invm_cut = df_S7_reco_taunu_cut.Histo1D({"h_SMW_taunu_invm_cut","h_SMW_taunu_invm_cut",50, 0, 500}, "SMW_taunu");
	auto h_SMtop_taunu_invm_cut = df_S7_reco_taunu_cut.Histo1D({"h_SMtop_taunu_invm_cut","h_SMtop_taunu_invm_cut",50, 0, 500}, "SMtop_taunu");

	auto h_tagchi2_taunu_cut = df_S7_tagreco_taunu_cut.Histo1D({"h_tagchi2_taunu_cut","h_tagchi2_taunu_cut",30, 0, 30000}, "chi2_taunu");
	auto h_tagLQtop_taunu_invm_cut = df_S7_tagreco_taunu_cut.Histo1D({"h_tagLQtop_taunu_invm_cut","h_tagLQtop_taunu_invm_cut",25, 0, 500}, "LQtop_taunu");
	auto h_tagSMW_taunu_invm_cut = df_S7_tagreco_taunu_cut.Histo1D({"h_tagSMW_taunu_invm_cut","h_tagSMW_taunu_invm_cut",25, 0, 500}, "SMW_taunu");
	auto h_tagSMtop_taunu_invm_cut = df_S7_tagreco_taunu_cut.Histo1D({"h_tagSMtop_taunu_invm_cut","h_tagSMtop_taunu_invm_cut",25, 0, 500}, "SMtop_taunu");

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
	*/
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
		fname = "Semitt_f_DeepFlav_2018.root";
	}
	TFile f(fname, "recreate");   
	plot( h_muon_pt, "h_muon_pt");
	plot( h_muon_eta, "h_muon_eta");
	plot( h_n_goodmuon, "h_n_goodmuon");
	plot( h_tau_pt, "h_tau_pt");
	plot( h_tau_eta, "h_tau_eta");
	plot( h_n_tau, "h_n_tau");
	plot( h_n_goodtau, "h_n_goodtau");
	plot( h_goodtau1_mother, "h_goodtau1_mother");
	plot( h_goodtau1_gmother, "h_goodtau1_gmother");
	plot( h_taumu_mother, "h_taumu_mother");
	plot( h_taumuW_mother, "h_taumuW_mother");
	plot( h_taumumu_mother, "h_taumumu_mother");
	plot( h_goodtau2_mother, "h_goodtau2_mother");
	plot( h_n_goodGentau, "h_n_goodGentau");
	plot( h_goodGentau1_mother, "h_goodGentau1_mother");
//	plot( h_goodGentau1_gmother, "h_goodGentau1_gmother");
	plot( h_goodGentau2_mother, "h_goodGentau2_mother");

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


	plot( H_n_goodmuon, "H_n_goodelec");
	plot( H_n_goodtau, "H_n_goodtau");
	plot( H_n_goodelec, "H_n_goodelec");

	plot( h_n_goodelec11, "h_n_goodelec11");
	plot( h_n_goodelec12, "h_n_goodelec12");
	plot( h_n_goodelec13, "h_n_goodelec13");
	plot( h_n_goodelec21, "h_n_goodelec21");
	plot( h_n_goodelec22, "h_n_goodelec22");
	plot( h_n_goodelec23, "h_n_goodelec23");
	plot( h_n_goodjetqq, "h_n_goodjetqq");

	plot( h_n_taggoodelec11, "h_n_taggoodelec11");
	plot( h_n_taggoodelec12, "h_n_taggoodelec12");
	plot( h_n_taggoodelec13, "h_n_taggoodelec13");
	plot( h_n_taggoodelec21, "h_n_taggoodelec21");
	plot( h_n_taggoodelec22, "h_n_taggoodelec22");
	plot( h_n_taggoodelec23, "h_n_taggoodelec23");

	plot( h_tau_flavour, "h_tau_flavour");
	plot( h_tau_flavour2, "h_tau_flavour2");

	plot( h_tau_goodjet, "h_tau_goodjet");
	plot( h_tau_goodjet2, "h_tau_goodjet2");

	plot( h_chi2_qq, "h_chi2_qq");
	plot( h_LQtop_qq_invm, "h_LQtop_qq_invm");
	plot( h_SMW_qq_invm, "h_SMW_qq_invm");
	plot( h_SMtop_qq_invm, "h_SMtop_qq_invm");

	plot( h_chi2_enu, "h_chi2_enu");
	plot( h_LQtop_enu_invm, "h_LQtop_enu_invm");
	plot( h_SMW_enu_invm, "h_SMW_enu_invm");
	plot( h_SMtop_enu_invm, "h_SMtop_enu_invm");

	plot( h_chi2_munu, "h_chi2_munu");
	plot( h_LQtop_munu_invm, "h_LQtop_munu_invm");
	plot( h_SMW_munu_invm, "h_SMW_munu_invm");
	plot( h_SMtop_munu_invm, "h_SMtop_munu_invm");

	plot( h_chi2_taunu, "h_chi2_taunu");
	plot( h_LQtop_taunu_invm, "h_LQtop_taunu_invm");
	plot( h_SMW_taunu_invm, "h_SMW_taunu_invm");
	plot( h_SMtop_taunu_invm, "h_SMtop_taunu_invm");

	plot( h_chi2_qq_cut, "h_chi2_qq_cut");
	plot( h_LQtop_qq_invm_cut, "h_LQtop_qq_invm_cut");
	plot( h_SMW_qq_invm_cut, "h_SMW_qq_invm_cut");
	plot( h_SMtop_qq_invm_cut, "h_SMtop_qq_invm_cut");

	plot( h_chi2_enu_cut, "h_chi2_enu_cut");
	plot( h_LQtop_enu_invm_cut, "h_LQtop_enu_invm_cut");
	plot( h_SMW_enu_invm_cut, "h_SMW_enu_invm_cut");
	plot( h_SMtop_enu_invm_cut, "h_SMtop_enu_invm_cut");

	plot( h_chi2_munu_cut, "h_chi2_munu_cut");
	plot( h_LQtop_munu_invm_cut, "h_LQtop_munu_invm_cut");
	plot( h_SMW_munu_invm_cut, "h_SMW_munu_invm_cut");
	plot( h_SMtop_munu_invm_cut, "h_SMtop_munu_invm_cut");

	plot( h_chi2_taunu_cut, "h_chi2_taunu_cut");
	plot( h_LQtop_taunu_invm_cut, "h_LQtop_taunu_invm_cut");
	plot( h_SMW_taunu_invm_cut, "h_SMW_taunu_invm_cut");
	plot( h_SMtop_taunu_invm_cut, "h_SMtop_taunu_invm_cut");

	plot( h_tagchi2_qq, "h_tagchi2_qq");
	plot( h_tagLQtop_qq_invm, "h_tagLQtop_qq_invm");
	plot( h_tagSMW_qq_invm, "h_tagSMW_qq_invm");
	plot( h_tagSMtop_qq_invm, "h_tagSMtop_qq_invm");

	plot( h_tagchi2_enu, "h_tagchi2_enu");
	plot( h_tagLQtop_enu_invm, "h_tagLQtop_enu_invm");
	plot( h_tagSMW_enu_invm, "h_tagSMW_enu_invm");
	plot( h_tagSMtop_enu_invm, "h_tagSMtop_enu_invm");

	plot( h_tagchi2_munu, "h_tagchi2_munu");
	plot( h_tagLQtop_munu_invm, "h_tagLQtop_munu_invm");
	plot( h_tagSMW_munu_invm, "h_tagSMW_munu_invm");
	plot( h_tagSMtop_munu_invm, "h_tagSMtop_munu_invm");

	plot( h_tagchi2_taunu, "h_tagchi2_taunu");
	plot( h_tagLQtop_taunu_invm, "h_tagLQtop_taunu_invm");
	plot( h_tagSMW_taunu_invm, "h_tagSMW_taunu_invm");
	plot( h_tagSMtop_taunu_invm, "h_tagSMtop_taunu_invm");

	plot( h_tagchi2_qq_cut, "h_tagchi2_qq_cut");
	plot( h_tagLQtop_qq_invm_cut, "h_tagLQtop_qq_invm_cut");
	plot( h_tagSMW_qq_invm_cut, "h_tagSMW_qq_invm_cut");
	plot( h_tagSMtop_qq_invm_cut, "h_tagSMtop_qq_invm_cut");

	plot( h_tagchi2_enu_cut, "h_tagchi2_enu_cut");
	plot( h_tagLQtop_enu_invm_cut, "h_tagLQtop_enu_invm_cut");
	plot( h_tagSMW_enu_invm_cut, "h_tagSMW_enu_invm_cut");
	plot( h_tagSMtop_enu_invm_cut, "h_tagSMtop_enu_invm_cut");

	plot( h_tagchi2_munu_cut, "h_tagchi2_munu_cut");
	plot( h_tagLQtop_munu_invm_cut, "h_tagLQtop_munu_invm_cut");
	plot( h_tagSMW_munu_invm_cut, "h_tagSMW_munu_invm_cut");
	plot( h_tagSMtop_munu_invm_cut, "h_tagSMtop_munu_invm_cut");

	plot( h_tagchi2_taunu_cut, "h_tagchi2_taunu_cut");
	plot( h_tagLQtop_taunu_invm_cut, "h_tagLQtop_taunu_invm_cut");
	plot( h_tagSMW_taunu_invm_cut, "h_tagSMW_taunu_invm_cut");
	plot( h_tagSMtop_taunu_invm_cut, "h_tagSMtop_taunu_invm_cut");
/*
	plot( h_leading_btagged_jet_l_pt, "h_leading_btagged_jet_l_pt");
	plot( h_leading_btagged_jet_m_pt, "h_leading_btagged_jet_m_pt");
	plot( h_leading_btagged_jet_t_pt, "h_leading_btagged_jet_t_pt");
	plot( h_leading_ctagged_jet_l_pt, "h_leading_ctagged_jet_l_pt");
	plot( h_leading_ctagged_jet_m_pt, "h_leading_ctagged_jet_m_pt");
	plot( h_leading_ctagged_jet_t_pt, "h_leading_ctagged_jet_t_pt");

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
*/
	f.Close();

//	auto report = df_S7_base.Report();  report->Print();
  cout << "----------qq channel--------------------------" << endl;
	auto report_qq = df_S7_tagreco_qq_cut.Report();  report_qq ->Print();
  cout << "----------enu channel--------------------------" << endl;
	auto report_enu = df_S7_tagreco_enu_cut.Report();  report_enu ->Print();
  cout << "----------munu channel--------------------------" << endl;
	auto report_munu = df_S7_tagreco_munu_cut.Report();  report_munu ->Print();
  cout << "----------taunu channel--------------------------" << endl;
	auto report_taunu = df_S7_tagreco_taunu_cut.Report();  report_taunu ->Print();

}
