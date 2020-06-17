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


	//**** Top Mass Reconstruction ****//
//Top-reconstruction
//input:
//  jet_info: Jet_pt, Jet_eta, Jet_phi, Jet_mass
//  electron_info: Electron_pt, Electron_eta, Electron_phi, Electron_mass
//  muon_info: Muon_pt, Muon_eta, Muon_phi, Muon_mass
//  MET_info: MET_pt, MET_phi
//  index: Jet_idx, c_idx, tau_jet_idx, muon_idx, sel_b_idx, sel_tau_idx, sel_muon_idx
//out:
//  rvec_f (X, LQtop_mass, SMW_mass, SMtop_mass)
//
//scenario_info:
//      qq:         sel_tau_idx > -1 && sel_muon_idx > -1 && Electron_pt < 0
//      electron:   sel_tau_idx > -1 && sel_muon_idx > -1 && Electron_pt > 0
//      muon:       sel_tau_idx > -1 && sel_muon_idx == -1 && Electron_pt < 0 
//      tau:        sel_tau_idx == -1 && sel_muon_idx > -1 && Electron_pt < 0


rvec_f top_reconstruction(rvec_f Jet_pt, rvec_f Jet_eta, rvec_f Jet_phi, rvec_f Jet_mass,   //Jet
        float Electron_pt, float Electron_eta, float Electron_phi, float Electron_mass,     //Electron
        rvec_f Muon_pt, rvec_f Muon_eta, rvec_f Muon_phi, rvec_f Muon_mass,                 //Muon
        float MET_pt, float MET_phi, int pz_range,                                          //MET
        rvec_i jet_idx, rvec_i c_idx, rvec_i tau_jet_idx, rvec_i muon_idx,
        int sel_b_idx, int sel_tau_idx, int sel_muon_idx){
    rvec_f out;

    TLorentzVector c_jet, tau, muon, b_jet, from_SMW_1, from_SMW_2;
    float LQtop_mass, SMW_mass, SMtop_mass;
    float X_LQtop, X_SMW, X_SMtop;
    float X_min=999999999999999, X_min_LQtop_mass=-1, X_min_SMW_mass=-1, X_min_SMtop_mass=-1;

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

		for (int i = -pz_range; i <= pz_range; i+=10){
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
	}else if (input.Contains("semitt")){
		file = {"/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/*.root"};
	}else if (input.Contains("ditt")){
		file = {"/xrootd/store/mc/RunIIAutumn18NanoAODv6/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/230000/*.root"};
	}
	ROOT::RDataFrame df("Events",file);

	//S0:trigger, S1:muon, S2:tau, S3:jet, S4:base, S5:top mass reco

	//**** HLT Trigger ****//
	auto df_S0_HLT = df.Filter("HLT_IsoMu24","SingleMuon Trigger");

	//**** Muon Selection (Tight muonId) ****//
	auto df_S1_goodmuon = df.Define("goodmuon","Muon_pt > 30  && abs(Muon_eta) < 2.4 && Muon_tightId && Muon_pfRelIso03_all < 0.15")
													.Define("n_goodmuon","Sum(goodmuon)")
													.Filter("n_goodmuon >= 1","Least one muon")
													.Define("goodmuon_idx",good_idx,{"goodmuon"})
													.Define("goodmuon1_idx",leading_idx,{"goodmuon_idx"});

	//**** Tau Selection ****//
	auto df_S2_goodtau = df_S1_goodmuon.Define("goodtau","Tau_pt > 30 && abs(Tau_eta) < 2.3 &&  Tau_idDecayModeNewDMs && Tau_idDeepTau2017v2p1VSmu & 1 && Tau_idDeepTau2017v2p1VSe & 1 && Tau_idDeepTau2017v2p1VSjet & 32")
																		 .Define("n_goodtau","Sum(goodtau)")
																		 .Filter("n_goodtau >= 1","Least one tau")
																		 .Define("goodtau_idx",good_idx,{"goodtau"})
																		 .Define("goodtau1_idx",leading_idx,{"goodtau_idx"});
	auto df_S2_goodtau2 = df_S2_goodtau.Filter("n_goodtau >=2","Two tau jets")
																		 .Define("goodtau2_idx",second_idx,{"goodtau_idx"});
	//**** Jet Tagging ****//
	auto df_S3_goodjet = df_S2_goodtau.Define("goodjet","Jet_pt > 30 && abs(Jet_eta) < 2.4 && Jet_jetId == 6")
																		.Define("CvsB","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
																		.Define("Jet_btagDeepFlavLF","1-(Jet_btagDeepFlavC+Jet_btagDeepFlavB)")
																		.Define("CvsL","Jet_btagDeepFlavC/(Jet_btagDeepFlavC+Jet_btagDeepFlavLF)")
																		.Define("goodbjet_m","goodjet && Jet_btagDeepFlavB > 0.2770")
																		.Define("goodcjet_m","goodjet && CvsB > 0.29 && CvsL > 0.085");


	//**** Baseline Selection ****//
	auto df_S4_base = df_S3_goodjet.Filter("Sum(goodjet) >=3","Least three jets")
																 .Filter("Sum(goodbjet_m) ==1", "One b-tagged jet")
																 .Filter("Sum(goodcjet_m) >= 1", "Least one c-tagged jet")
																 .Define("not_ele", "float(-9999.9);")
																 .Define("mu_tau2", "int(-1);")
																 .Define("pz_range", "int(700);")
																 .Define("goodelec","Electron_pt > 30 && abs(Electron_eta) < 2.4 && Electron_mvaFall17V2Iso_WP90")
																 .Define("n_goodelec","Sum(goodelec)")
																 .Define("goodjet_idx", good_idx, {"goodjet"})
																 .Define("goodbjet_m_idx",good_idx,{"goodbjet_m"})
																 .Define("goodbjet_m1_idx",leading_idx,{"goodbjet_m_idx"})
																 .Define("goodcjet_m_idx", good_idx, {"goodcjet_m"})
																 .Define("goodtau_jet_idx", "Tau_jetIdx[goodtau]")
																 .Define("goodtau1_jet_idx", "Tau_jetIdx[goodtau1_idx]");
	auto cate11 = df_S4_base.Filter("n_goodtau ==1 && n_goodmuon ==1","category 11");
	auto cate12 = df_S4_base.Filter("n_goodtau ==1 && n_goodmuon ==2","category 11");
	auto cate13 = df_S4_base.Filter("n_goodtau ==1 && n_goodmuon ==3","category 11");
	auto cate21 = df_S4_base.Filter("n_goodtau ==2 && n_goodmuon ==1","category 11");
	auto cate22 = df_S4_base.Filter("n_goodtau ==2 && n_goodmuon ==2","category 11");
	auto cate23 = df_S4_base.Filter("n_goodtau ==2 && n_goodmuon ==3","category 11");


	//**** Top Mass Reconstruction****//

		// qq scenario
	auto df_S5_qq = df_S4_base.Filter("n_goodmuon == 1 && n_goodtau == 1 && n_goodelec == 0", "mu1,tau1,elec0");
//																	 .Filter("Sum(goodjet)>=5", "c_muon_tau_b_q_q(five loops)");
  auto df_S5_reco_qq = df_S5_qq.Define("reco_qq", top_reconstruction,
			{"Jet_pt", "Jet_eta", "Jet_phi", "Jet_mass", "not_ele", "not_ele", "not_ele", "not_ele",
			 "Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "MET_pt", "MET_phi", "pz_range",
			 "goodjet_idx","goodcjet_m_idx", "goodtau_jet_idx", "goodmuon_idx", "goodbjet_m1_idx", "goodtau1_jet_idx", "goodmuon1_idx"})
				.Define("chi2_qq", "reco_qq[0]").Define("LQtop_qq", "reco_qq[1]").Define("SMW_qq", "reco_qq[2]").Define("SMtop_qq", "reco_qq[3]")
				.Filter("LQtop_qq > 0 && SMW_qq > 0 && SMtop_qq > 0","Six object designation completed")
				.Filter("chi2_qq < 999999999999999","chi2_qq updated");
	auto df_S5_reco_qq_cut1 = df_S5_reco_qq.Filter("chi2_qq < 8000","chi2_qq < 8000")
																				 .Filter("chi2_qq < 5000","chi2_qq < 5000")
																				 .Filter("chi2_qq < 3000","chi2_qq < 3000");
	auto df_S5_reco_qq_cut2 = df_S5_reco_qq.Filter("chi2_qq < 5000","chi2_qq < 5000");
	auto df_S5_reco_qq_cut3 = df_S5_reco_qq.Filter("chi2_qq < 8000","chi2_qq < 8000");


		// enu scenario
	auto df_S5_enu = df_S4_base.Filter("n_goodmuon >= 1 && n_goodtau >= 1 && n_goodelec == 1", "mu>=1,tau>=1,elec1")
			.Define("goodelec_idx",good_idx, {"goodelec"})      
			.Define("goodelec1_idx",leading_idx, {"goodelec_idx"})
			.Define("goodelec1_pt", "Electron_pt[goodelec1_idx]")
			.Define("goodelec1_eta", "Electron_eta[goodelec1_idx]")
			.Define("goodelec1_phi", "Electron_phi[goodelec1_idx]")
			.Define("goodelec1_mass", "Electron_mass[goodelec1_idx]");
	auto df_S5_reco_enu = df_S5_enu.Define("reco_enu", top_reconstruction,
			{"Jet_pt", "Jet_eta", "Jet_phi", "Jet_mass", "goodelec1_pt", "goodelec1_eta", "goodelec1_phi", "goodelec1_mass",
			 "Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "MET_pt", "MET_phi", "pz_range",
			 "goodjet_idx","goodcjet_m_idx", "goodtau_jet_idx", "goodmuon_idx", "goodbjet_m1_idx", "goodtau1_jet_idx", "goodmuon1_idx"})
				.Define("chi2_enu", "reco_enu[0]").Define("LQtop_enu", "reco_enu[1]").Define("SMW_enu", "reco_enu[2]").Define("SMtop_enu", "reco_enu[3]")
				.Filter("LQtop_enu > 0 && SMW_enu > 0 && SMtop_enu > 0","Six object designation completed")
				.Filter("chi2_enu < 999999999999999","chi2_enu updated");
	auto df_S5_reco_enu_cut1 = df_S5_reco_enu.Filter("chi2_enu < 8000","chi2_enu < 8000")
																					 .Filter("chi2_enu < 5000","chi2_enu < 5000")
																					 .Filter("chi2_enu < 3000","chi2_enu < 3000");
	auto df_S5_reco_enu_cut2 = df_S5_reco_enu.Filter("chi2_enu < 5000","chi2_enu < 5000");
	auto df_S5_reco_enu_cut3 = df_S5_reco_enu.Filter("chi2_enu < 8000","chi2_enu < 8000");

		// munu scenario
	auto df_S5_munu = df_S4_base.Filter("n_goodmuon >= 2 && n_goodtau >= 1 && n_goodelec == 0", "mu>=2,tau>=1,elec0");
  auto df_S5_reco_munu = df_S5_munu.Define("reco_munu", top_reconstruction,
			{"Jet_pt", "Jet_eta", "Jet_phi", "Jet_mass", "not_ele", "not_ele", "not_ele", "not_ele",
			 "Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "MET_pt", "MET_phi", "pz_range",
			 "goodjet_idx","goodcjet_m_idx", "goodtau_jet_idx", "goodmuon_idx", "goodbjet_m1_idx", "goodtau1_jet_idx","mu_tau2"})
				.Define("chi2_munu", "reco_munu[0]").Define("LQtop_munu", "reco_munu[1]").Define("SMW_munu", "reco_munu[2]").Define("SMtop_munu", "reco_munu[3]")
				.Filter("LQtop_munu > 0 && SMW_munu > 0 && SMtop_munu > 0","Six object designation completed")
				.Filter("chi2_munu < 999999999999999","chi2_munu updated");
	auto df_S5_reco_munu_cut1 = df_S5_reco_munu.Filter("chi2_munu < 8000","chi2_munu < 8000")
																						 .Filter("chi2_munu < 5000","chi2_munu < 5000")
																						 .Filter("chi2_munu < 3000","chi2_munu < 3000");
	auto df_S5_reco_munu_cut2 = df_S5_reco_munu.Filter("chi2_munu < 5000","chi2_munu < 5000");
	auto df_S5_reco_munu_cut3 = df_S5_reco_munu.Filter("chi2_munu < 8000","chi2_munu < 8000");

		// taunu scenario
	auto df_S5_taunu = df_S4_base.Filter("n_goodmuon == 1 && n_goodtau == 2 && n_goodelec == 0", "mu1,tau2,elec0");
//																			.Filter("Sum(goodjet) >= 4","c_mu_tau_b_tau_nu(four loops)");
  auto df_S5_reco_taunu = df_S5_taunu.Define("reco", top_reconstruction,
			{"Jet_pt", "Jet_eta", "Jet_phi", "Jet_mass", "not_ele", "not_ele", "not_ele", "not_ele",
			 "Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "MET_pt", "MET_phi", "pz_range",
			 "goodjet_idx","goodcjet_m_idx", "goodtau_jet_idx", "goodmuon_idx", "goodbjet_m1_idx", "mu_tau2","goodmuon1_idx"})
				.Define("chi2_taunu", "reco[0]").Define("LQtop_taunu", "reco[1]").Define("SMW_taunu", "reco[2]").Define("SMtop_taunu", "reco[3]")
				.Filter("LQtop_taunu > 0 && SMW_taunu > 0 && SMtop_taunu > 0","Six object designation completed")
				.Filter("chi2_taunu < 999999999999999","chi2_taunu updated");
	auto df_S5_reco_taunu_cut1 = df_S5_reco_taunu.Filter("chi2_taunu < 8000","chi2_taunu < 8000")
																							 .Filter("chi2_taunu < 5000","chi2_taunu < 5000")
																							 .Filter("chi2_taunu < 3000","chi2_taunu < 3000");
	auto df_S5_reco_taunu_cut2 = df_S5_reco_taunu.Filter("chi2_taunu < 5000","chi2_taunu < 5000");
	auto df_S5_reco_taunu_cut3 = df_S5_reco_taunu.Filter("chi2_taunu < 8000","chi2_taunu < 8000");



	//**** Histograms ****//
	auto h_muon_pt = df_S1_goodmuon.Define("goodmuon1_pt","Muon_pt[goodmuon1_idx]").Histo1D({"h_muon_pt", "h_muon_pt", 10, 0, 100}, "goodmuon1_pt");
	auto h_muon_eta = df_S1_goodmuon.Define("goodmuon1_eta","Muon_eta[goodmuon1_idx]").Histo1D({"h_muon_eta", "h_muon_eta", 50, -5, 5}, "goodmuon1_eta");
	auto h_n_goodmuon = df_S1_goodmuon.Histo1D({"h_n_goodmuon", "h_n_goodmuon", 5, 0, 5}, "n_goodmuon"); 

	auto h_tau_pt = df_S2_goodtau.Define("goodtau1_pt","Tau_pt[goodtau1_idx]").Histo1D({"h_tau_pt", "h_tau_pt", 10, 0, 100}, "goodtau1_pt");
	auto h_tau_eta = df_S2_goodtau.Define("goodtau1_eta","Tau_eta[goodtau1_idx]").Histo1D({"h_tau_eta", "h_tau_eta", 50, -5, 5}, "goodtau1_eta");
	auto h_n_tau = df_S1_goodmuon.Histo1D({"h_n_tau", "h_n_tau", 5, 0, 5}, "nTau");
	auto h_n_goodtau = df_S2_goodtau.Histo1D({"h_n_goodtau", "h_n_goodtau", 5, 0, 5}, "n_goodtau");

	auto h_n_goodjet= df_S3_goodjet.Define("n_goodjet","Sum(goodjet)") .Histo1D({"h_n_goodjet","h_n_goodjet", 15, 0, 15}, "n_goodjet");

	auto H_n_goodmuon = df_S4_base.Histo1D({"H_n_goodmuon", "H_n_goodmuon", 5, 0, 5}, "n_goodmuon");
	auto H_n_goodtau = df_S4_base.Histo1D({"H_n_goodtau", "H_n_goodtau", 5, 0, 5}, "n_goodtau");
	auto H_n_goodelec = df_S4_base.Histo1D({"H_n_goodelec", "H_n_goodelec", 5, 0, 5}, "n_goodelec");

	auto h_n_goodelec11 = cate11.Histo1D({"h_n_goodelec11","h_n_goodelec11",5,0,5},"n_goodelec");
	auto h_n_goodelec12 = cate12.Histo1D({"h_n_goodelec12","h_n_goodelec12",5,0,5},"n_goodelec");
	auto h_n_goodelec13 = cate13.Histo1D({"h_n_goodelec13","h_n_goodelec13",5,0,5},"n_goodelec");
	auto h_n_goodelec21 = cate21.Histo1D({"h_n_goodelec21","h_n_goodelec21",5,0,5},"n_goodelec");
	auto h_n_goodelec22 = cate22.Histo1D({"h_n_goodelec22","h_n_goodelec22",5,0,5},"n_goodelec");
	auto h_n_goodelec23 = cate23.Histo1D({"h_n_goodelec23","h_n_goodelec23",5,0,5},"n_goodelec");

	auto h_chi2_qq = df_S5_reco_qq.Histo1D({"h_chi2_qq","h_chi2_qq",30, 0, 30000}, "chi2_qq");
	auto h_LQtop_qq_invm = df_S5_reco_qq.Histo1D({"h_LQtop_qq_invm","h_LQtop_qq_invm",25, 0, 500}, "LQtop_qq");
	auto h_SMW_qq_invm = df_S5_reco_qq.Histo1D({"h_SMW_qq_invm","h_SMW_qq_invm",25, 0, 500}, "SMW_qq");
	auto h_SMtop_qq_invm = df_S5_reco_qq.Histo1D({"h_SMtop_qq_invm","h_SMtop_qq_invm",25, 0, 500}, "SMtop_qq");

	auto h_chi2_enu = df_S5_reco_enu.Histo1D({"h_chi2_enu","h_chi2_enu",30, 0, 30000}, "chi2_enu");
	auto h_LQtop_enu_invm = df_S5_reco_enu.Histo1D({"h_LQtop_enu_invm","h_LQtop_enu_invm",25, 0, 500}, "LQtop_enu");
	auto h_SMW_enu_invm = df_S5_reco_enu.Histo1D({"h_SMW_enu_invm","h_SMW_enu_invm",25, 0, 500}, "SMW_enu");
	auto h_SMtop_enu_invm = df_S5_reco_enu.Histo1D({"h_SMtop_enu_invm","h_SMtop_enu_invm",25, 0, 500}, "SMtop_enu");

	auto h_chi2_munu = df_S5_reco_munu.Histo1D({"h_chi2_munu","h_chi2_munu",30, 0, 30000}, "chi2_munu");
	auto h_LQtop_munu_invm = df_S5_reco_munu.Histo1D({"h_LQtop_munu_invm","h_LQtop_munu_invm",25, 0, 500}, "LQtop_munu");
	auto h_SMW_munu_invm = df_S5_reco_munu.Histo1D({"h_SMW_munu_invm","h_SMW_munu_invm",25, 0, 500}, "SMW_munu");
	auto h_SMtop_munu_invm = df_S5_reco_munu.Histo1D({"h_SMtop_munu_invm","h_SMtop_munu_invm",25, 0, 500}, "SMtop_munu");

	auto h_chi2_taunu = df_S5_reco_taunu.Histo1D({"h_chi2_taunu","h_chi2_taunu",30, 0, 30000}, "chi2_taunu");
	auto h_LQtop_taunu_invm = df_S5_reco_taunu.Histo1D({"h_LQtop_taunu_invm","h_LQtop_taunu_invm",25, 0, 500}, "LQtop_taunu");
	auto h_SMW_taunu_invm = df_S5_reco_taunu.Histo1D({"h_SMW_taunu_invm","h_SMW_taunu_invm",25, 0, 500}, "SMW_taunu");
	auto h_SMtop_taunu_invm = df_S5_reco_taunu.Histo1D({"h_SMtop_taunu_invm","h_SMtop_taunu_invm",25, 0, 500}, "SMtop_taunu");

	auto h_chi2_qq_cut1 = df_S5_reco_qq_cut1.Histo1D({"h_chi2_qq_cut1","h_chi2_qq_cut1",30, 0, 30000}, "chi2_qq");
	auto h_LQtop_qq_invm_cut1 = df_S5_reco_qq_cut1.Histo1D({"h_LQtop_qq_invm_cut1","h_LQtop_qq_invm_cut1",25, 0, 500}, "LQtop_qq");
	auto h_SMW_qq_invm_cut1 = df_S5_reco_qq_cut1.Histo1D({"h_SMW_qq_invm_cut1","h_SMW_qq_invm_cut1",25, 0, 500}, "SMW_qq");
	auto h_SMtop_qq_invm_cut1 = df_S5_reco_qq_cut1.Histo1D({"h_SMtop_qq_invm_cut1","h_SMtop_qq_invm_cut1",25, 0, 500}, "SMtop_qq");

	auto h_chi2_enu_cut1 = df_S5_reco_enu_cut1.Histo1D({"h_chi2_enu_cut1","h_chi2_enu_cut1",30, 0, 30000}, "chi2_enu");
	auto h_LQtop_enu_invm_cut1 = df_S5_reco_enu_cut1.Histo1D({"h_LQtop_enu_invm_cut1","h_LQtop_enu_invm_cut1",25, 0, 500}, "LQtop_enu");
	auto h_SMW_enu_invm_cut1 = df_S5_reco_enu_cut1.Histo1D({"h_SMW_enu_invm_cut1","h_SMW_enu_invm_cut1",25, 0, 500}, "SMW_enu");
	auto h_SMtop_enu_invm_cut1 = df_S5_reco_enu_cut1.Histo1D({"h_SMtop_enu_invm_cut1","h_SMtop_enu_invm_cut1",25, 0, 500}, "SMtop_enu");

	auto h_chi2_munu_cut1 = df_S5_reco_munu_cut1.Histo1D({"h_chi2_munu_cut1","h_chi2_munu_cut1",30, 0, 30000}, "chi2_munu");
	auto h_LQtop_munu_invm_cut1 = df_S5_reco_munu_cut1.Histo1D({"h_LQtop_munu_invm_cut1","h_LQtop_munu_invm_cut1",25, 0, 500}, "LQtop_munu");
	auto h_SMW_munu_invm_cut1 = df_S5_reco_munu_cut1.Histo1D({"h_SMW_munu_invm_cut1","h_SMW_munu_invm_cut1",25, 0, 500}, "SMW_munu");
	auto h_SMtop_munu_invm_cut1 = df_S5_reco_munu_cut1.Histo1D({"h_SMtop_munu_invm_cut1","h_SMtop_munu_invm_cut1",25, 0, 500}, "SMtop_munu");

	auto h_chi2_taunu_cut1 = df_S5_reco_taunu_cut1.Histo1D({"h_chi2_taunu_cut1","h_chi2_taunu_cut1",30, 0, 30000}, "chi2_taunu");
	auto h_LQtop_taunu_invm_cut1 = df_S5_reco_taunu_cut1.Histo1D({"h_LQtop_taunu_invm_cut1","h_LQtop_taunu_invm_cut1",25, 0, 500}, "LQtop_taunu");
	auto h_SMW_taunu_invm_cut1 = df_S5_reco_taunu_cut1.Histo1D({"h_SMW_taunu_invm_cut1","h_SMW_taunu_invm_cut1",25, 0, 500}, "SMW_taunu");
	auto h_SMtop_taunu_invm_cut1 = df_S5_reco_taunu_cut1.Histo1D({"h_SMtop_taunu_invm_cut1","h_SMtop_taunu_invm_cut1",25, 0, 500}, "SMtop_taunu");

	auto h_chi2_qq_cut2 = df_S5_reco_qq_cut2.Histo1D({"h_chi2_qq_cut2","h_chi2_qq_cut2",30, 0, 30000}, "chi2_qq");
	auto h_LQtop_qq_invm_cut2 = df_S5_reco_qq_cut2.Histo1D({"h_LQtop_qq_invm_cut2","h_LQtop_qq_invm_cut2",25, 0, 500}, "LQtop_qq");
	auto h_SMW_qq_invm_cut2 = df_S5_reco_qq_cut2.Histo1D({"h_SMW_qq_invm_cut2","h_SMW_qq_invm_cut2",25, 0, 500}, "SMW_qq");
	auto h_SMtop_qq_invm_cut2 = df_S5_reco_qq_cut2.Histo1D({"h_SMtop_qq_invm_cut2","h_SMtop_qq_invm_cut2",25, 0, 500}, "SMtop_qq");

	auto h_chi2_enu_cut2 = df_S5_reco_enu_cut2.Histo1D({"h_chi2_enu_cut2","h_chi2_enu_cut2",30, 0, 30000}, "chi2_enu");
	auto h_LQtop_enu_invm_cut2 = df_S5_reco_enu_cut2.Histo1D({"h_LQtop_enu_invm_cut2","h_LQtop_enu_invm_cut2",25, 0, 500}, "LQtop_enu");
	auto h_SMW_enu_invm_cut2 = df_S5_reco_enu_cut2.Histo1D({"h_SMW_enu_invm_cut2","h_SMW_enu_invm_cut2",25, 0, 500}, "SMW_enu");
	auto h_SMtop_enu_invm_cut2 = df_S5_reco_enu_cut2.Histo1D({"h_SMtop_enu_invm_cut2","h_SMtop_enu_invm_cut2",25, 0, 500}, "SMtop_enu");

	auto h_chi2_munu_cut2 = df_S5_reco_munu_cut2.Histo1D({"h_chi2_munu_cut2","h_chi2_munu_cut2",30, 0, 30000}, "chi2_munu");
	auto h_LQtop_munu_invm_cut2 = df_S5_reco_munu_cut2.Histo1D({"h_LQtop_munu_invm_cut2","h_LQtop_munu_invm_cut2",25, 0, 500}, "LQtop_munu");
	auto h_SMW_munu_invm_cut2 = df_S5_reco_munu_cut2.Histo1D({"h_SMW_munu_invm_cut2","h_SMW_munu_invm_cut2",25, 0, 500}, "SMW_munu");
	auto h_SMtop_munu_invm_cut2 = df_S5_reco_munu_cut2.Histo1D({"h_SMtop_munu_invm_cut2","h_SMtop_munu_invm_cut2",25, 0, 500}, "SMtop_munu");

	auto h_chi2_taunu_cut2 = df_S5_reco_taunu_cut2.Histo1D({"h_chi2_taunu_cut2","h_chi2_taunu_cut2",30, 0, 30000}, "chi2_taunu");
	auto h_LQtop_taunu_invm_cut2 = df_S5_reco_taunu_cut2.Histo1D({"h_LQtop_taunu_invm_cut2","h_LQtop_taunu_invm_cut2",25, 0, 500}, "LQtop_taunu");
	auto h_SMW_taunu_invm_cut2 = df_S5_reco_taunu_cut2.Histo1D({"h_SMW_taunu_invm_cut2","h_SMW_taunu_invm_cut2",25, 0, 500}, "SMW_taunu");
	auto h_SMtop_taunu_invm_cut2 = df_S5_reco_taunu_cut2.Histo1D({"h_SMtop_taunu_invm_cut2","h_SMtop_taunu_invm_cut2",25, 0, 500}, "SMtop_taunu");

	auto h_chi2_qq_cut3 = df_S5_reco_qq_cut3.Histo1D({"h_chi2_qq_cut3","h_chi2_qq_cut3",30, 0, 30000}, "chi2_qq");
	auto h_LQtop_qq_invm_cut3 = df_S5_reco_qq_cut3.Histo1D({"h_LQtop_qq_invm_cut3","h_LQtop_qq_invm_cut3",25, 0, 500}, "LQtop_qq");
	auto h_SMW_qq_invm_cut3 = df_S5_reco_qq_cut3.Histo1D({"h_SMW_qq_invm_cut3","h_SMW_qq_invm_cut3",25, 0, 500}, "SMW_qq");
	auto h_SMtop_qq_invm_cut3 = df_S5_reco_qq_cut3.Histo1D({"h_SMtop_qq_invm_cut3","h_SMtop_qq_invm_cut3",25, 0, 500}, "SMtop_qq");

	auto h_chi2_enu_cut3 = df_S5_reco_enu_cut3.Histo1D({"h_chi2_enu_cut3","h_chi2_enu_cut3",30, 0, 30000}, "chi2_enu");
	auto h_LQtop_enu_invm_cut3 = df_S5_reco_enu_cut3.Histo1D({"h_LQtop_enu_invm_cut3","h_LQtop_enu_invm_cut3",25, 0, 500}, "LQtop_enu");
	auto h_SMW_enu_invm_cut3 = df_S5_reco_enu_cut3.Histo1D({"h_SMW_enu_invm_cut3","h_SMW_enu_invm_cut3",25, 0, 500}, "SMW_enu");
	auto h_SMtop_enu_invm_cut3 = df_S5_reco_enu_cut3.Histo1D({"h_SMtop_enu_invm_cut3","h_SMtop_enu_invm_cut3",25, 0, 500}, "SMtop_enu");

	auto h_chi2_munu_cut3 = df_S5_reco_munu_cut3.Histo1D({"h_chi2_munu_cut3","h_chi2_munu_cut3",30, 0, 30000}, "chi2_munu");
	auto h_LQtop_munu_invm_cut3 = df_S5_reco_munu_cut3.Histo1D({"h_LQtop_munu_invm_cut3","h_LQtop_munu_invm_cut3",25, 0, 500}, "LQtop_munu");
	auto h_SMW_munu_invm_cut3 = df_S5_reco_munu_cut3.Histo1D({"h_SMW_munu_invm_cut3","h_SMW_munu_invm_cut3",25, 0, 500}, "SMW_munu");
	auto h_SMtop_munu_invm_cut3 = df_S5_reco_munu_cut3.Histo1D({"h_SMtop_munu_invm_cut3","h_SMtop_munu_invm_cut3",25, 0, 500}, "SMtop_munu");

	auto h_chi2_taunu_cut3 = df_S5_reco_taunu_cut3.Histo1D({"h_chi2_taunu_cut3","h_chi2_taunu_cut3",30, 0, 30000}, "chi2_taunu");
	auto h_LQtop_taunu_invm_cut3 = df_S5_reco_taunu_cut3.Histo1D({"h_LQtop_taunu_invm_cut3","h_LQtop_taunu_invm_cut3",25, 0, 500}, "LQtop_taunu");
	auto h_SMW_taunu_invm_cut3 = df_S5_reco_taunu_cut3.Histo1D({"h_SMW_taunu_invm_cut3","h_SMW_taunu_invm_cut3",25, 0, 500}, "SMW_taunu");
	auto h_SMtop_taunu_invm_cut3 = df_S5_reco_taunu_cut3.Histo1D({"h_SMtop_taunu_invm_cut3","h_SMtop_taunu_invm_cut3",25, 0, 500}, "SMtop_taunu");

	//df_S1_goodmuon.Snapshot("tree", "f.root");
	TString fname = "default_fname";
	if (input.Contains("LQ")){
		fname = "f_DeepFlav_2018.root";
	}else if (input.Contains("semitt")){
		fname = "Semitt_f_DeepFlav_2018.root";
	}else if (input.Contains("ditt")){
		fname = "Ditt_f_DeepFlav_2018.root";
	}
	TFile f(fname, "recreate");   

	plot( h_muon_pt, "h_muon_pt");
	plot( h_muon_eta, "h_muon_eta");
	plot( h_n_goodmuon, "h_n_goodmuon");

	plot( h_tau_pt, "h_tau_pt");
	plot( h_tau_eta, "h_tau_eta");
	plot( h_n_tau, "h_n_tau");
	plot( h_n_goodtau, "h_n_goodtau");

	plot( h_n_goodjet, "h_n_goodjet");

	plot( H_n_goodmuon, "H_n_goodelec");
	plot( H_n_goodtau, "H_n_goodtau");
	plot( H_n_goodelec, "H_n_goodelec");

	plot( h_n_goodelec11, "h_n_goodelec11");
	plot( h_n_goodelec12, "h_n_goodelec12");
	plot( h_n_goodelec13, "h_n_goodelec13");
	plot( h_n_goodelec21, "h_n_goodelec21");
	plot( h_n_goodelec22, "h_n_goodelec22");
	plot( h_n_goodelec23, "h_n_goodelec23");

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

	plot( h_chi2_qq_cut1, "h_chi2_qq_cut1");
	plot( h_LQtop_qq_invm_cut1, "h_LQtop_qq_invm_cut1");
	plot( h_SMW_qq_invm_cut1, "h_SMW_qq_invm_cut1");
	plot( h_SMtop_qq_invm_cut1, "h_SMtop_qq_invm_cut1");

	plot( h_chi2_enu_cut1, "h_chi2_enu_cut1");
	plot( h_LQtop_enu_invm_cut1, "h_LQtop_enu_invm_cut1");
	plot( h_SMW_enu_invm_cut1, "h_SMW_enu_invm_cut1");
	plot( h_SMtop_enu_invm_cut1, "h_SMtop_enu_invm_cut1");

	plot( h_chi2_munu_cut1, "h_chi2_munu_cut1");
	plot( h_LQtop_munu_invm_cut1, "h_LQtop_munu_invm_cut1");
	plot( h_SMW_munu_invm_cut1, "h_SMW_munu_invm_cut1");
	plot( h_SMtop_munu_invm_cut1, "h_SMtop_munu_invm_cut1");

	plot( h_chi2_taunu_cut1, "h_chi2_taunu_cut1");
	plot( h_LQtop_taunu_invm_cut1, "h_LQtop_taunu_invm_cut1");
	plot( h_SMW_taunu_invm_cut1, "h_SMW_taunu_invm_cut1");
	plot( h_SMtop_taunu_invm_cut1, "h_SMtop_taunu_invm_cut1");

	plot( h_chi2_qq_cut2, "h_chi2_qq_cut2");
	plot( h_LQtop_qq_invm_cut2, "h_LQtop_qq_invm_cut2");
	plot( h_SMW_qq_invm_cut2, "h_SMW_qq_invm_cut2");
	plot( h_SMtop_qq_invm_cut2, "h_SMtop_qq_invm_cut2");

	plot( h_chi2_enu_cut2, "h_chi2_enu_cut2");
	plot( h_LQtop_enu_invm_cut2, "h_LQtop_enu_invm_cut2");
	plot( h_SMW_enu_invm_cut2, "h_SMW_enu_invm_cut2");
	plot( h_SMtop_enu_invm_cut2, "h_SMtop_enu_invm_cut2");

	plot( h_chi2_munu_cut2, "h_chi2_munu_cut2");
	plot( h_LQtop_munu_invm_cut2, "h_LQtop_munu_invm_cut2");
	plot( h_SMW_munu_invm_cut2, "h_SMW_munu_invm_cut2");
	plot( h_SMtop_munu_invm_cut2, "h_SMtop_munu_invm_cut2");

	plot( h_chi2_taunu_cut2, "h_chi2_taunu_cut2");
	plot( h_LQtop_taunu_invm_cut2, "h_LQtop_taunu_invm_cut2");
	plot( h_SMW_taunu_invm_cut2, "h_SMW_taunu_invm_cut2");
	plot( h_SMtop_taunu_invm_cut2, "h_SMtop_taunu_invm_cut2");

	plot( h_chi2_qq_cut3, "h_chi2_qq_cut3");
	plot( h_LQtop_qq_invm_cut3, "h_LQtop_qq_invm_cut3");
	plot( h_SMW_qq_invm_cut3, "h_SMW_qq_invm_cut3");
	plot( h_SMtop_qq_invm_cut3, "h_SMtop_qq_invm_cut3");

	plot( h_chi2_enu_cut3, "h_chi2_enu_cut3");
	plot( h_LQtop_enu_invm_cut3, "h_LQtop_enu_invm_cut3");
	plot( h_SMW_enu_invm_cut3, "h_SMW_enu_invm_cut3");
	plot( h_SMtop_enu_invm_cut3, "h_SMtop_enu_invm_cut3");

	plot( h_chi2_munu_cut3, "h_chi2_munu_cut3");
	plot( h_LQtop_munu_invm_cut3, "h_LQtop_munu_invm_cut3");
	plot( h_SMW_munu_invm_cut3, "h_SMW_munu_invm_cut3");
	plot( h_SMtop_munu_invm_cut3, "h_SMtop_munu_invm_cut3");

	plot( h_chi2_taunu_cut3, "h_chi2_taunu_cut3");
	plot( h_LQtop_taunu_invm_cut3, "h_LQtop_taunu_invm_cut3");
	plot( h_SMW_taunu_invm_cut3, "h_SMW_taunu_invm_cut3");
	plot( h_SMtop_taunu_invm_cut3, "h_SMtop_taunu_invm_cut3");

	f.Close();

//	auto report = df_S7_base.Report();  report->Print();
  cout << "----------qq scenario--------------------------" << endl;
	auto report_qq = df_S5_reco_qq_cut1.Report();  report_qq ->Print();
  cout << "----------enu scenario--------------------------" << endl;
	auto report_enu = df_S5_reco_enu_cut1.Report();  report_enu ->Print();
  cout << "----------munu scenario--------------------------" << endl;
	auto report_munu = df_S5_reco_munu_cut1.Report();  report_munu ->Print();
  cout << "----------taunu scenario--------------------------" << endl;
	auto report_taunu = df_S5_reco_taunu_cut1.Report();  report_taunu ->Print();

}
