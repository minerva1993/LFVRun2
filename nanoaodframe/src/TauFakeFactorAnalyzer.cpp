/*
 * TauFakeFactorAnalyzer.cpp
 *
 *  Created on: June 14, 2023
 *      Author: Jiwon Park
 */

#include "TauFakeFactorAnalyzer.h"
#include "utility.h"

TauFakeFactorAnalyzer::TauFakeFactorAnalyzer(TTree *t, std::string outfilename, std::string year, std::string syst, std::string jsonfname, string globaltag, int nthreads, std::string mode)
:NanoAODAnalyzerrdframe(t, outfilename, year, syst, jsonfname, globaltag, nthreads), _syst(syst), _year(year), _mode(mode)
{
    _syst = syst;
}

// Define your cuts here
void TauFakeFactorAnalyzer::defineCuts() {

    if (_mode == "lss") { // loose tau vs jet && SS
        addCuts("nmuonpass == 1 && nvetoelepass == 0 && nvetomuons == 0 && PV_npvsGood > 0 && nloosetaupass == 1 && ncleantaupass == 0 && mutau_charge_loose > 0 && ncleanjetspass >= 3 && ncleanbjetspass == 1","0");
    } else if (_mode == "los") { // loose tau vs jet && OS
        addCuts("nmuonpass == 1 && nvetoelepass == 0 && nvetomuons == 0 && PV_npvsGood > 0 && nloosetaupass == 1 && ncleantaupass == 0 && mutau_charge_loose < 0 && ncleanjetspass >= 3 && ncleanbjetspass == 1","0");
    } else if (_mode == "tss") { // tight tau vs jet && SS
        addCuts("nmuonpass == 1 && nvetoelepass == 0 && nvetomuons == 0 && PV_npvsGood > 0 && nloosetaupass == 1 && ncleantaupass == 1 && mutau_charge > 0 && ncleanjetspass >= 3 && ncleanbjetspass == 1","0");
    } else if (_mode == "tos") { // tight tau vs jet && OS
        addCuts("nmuonpass == 1 && nvetoelepass == 0 && nvetomuons == 0 && PV_npvsGood > 0 && nloosetaupass == 1 && ncleantaupass == 1 && mutau_charge < 0 && ncleanjetspass >= 3 && ncleanbjetspass == 1","0");
    } else {
        std::cout << "WRONG MODE!!!!" << std::endl;
    }
}

void TauFakeFactorAnalyzer::defineMoreVars() {

    addVar({"Muon1_charge", "Muon_charge[0]", ""});

    addVar({"Tau1_pt", "Tau_pt[0]", ""});
    addVar({"Tau1_pt_gen", "(Tau_pt_gen.size()>0) ? Tau_pt_gen[0] : -1", ""});
    addVar({"Tau1_charge", "Tau_charge[0]", ""});
    addVar({"mutau_charge", "Muon1_charge * Tau1_charge", ""});

    // EventWeights
    if (_syst == "data") {
        addVar({"eventWeight", "1.0"});
        if (_mode == "lss" or _mode == "los") {
            addVar({"Tau1_pt_loose", "Tau_pt_loose[0]", ""});
            addVar({"Tau1_charge_loose", "Tau_charge_loose[0]", ""});
            addVar({"Tau1_pt_loose_gen", "(Tau_pt_loose_gen.size()>0) ? Tau_pt_loose_gen[0] : -1", ""});
            addVar({"mutau_charge_loose", "Muon1_charge * Tau1_charge_loose", ""});
        }
    } else {
        addVar({"eventWeight_genpu", "unitGenWeight * TopPtWeight * puWeight[0] * L1PreFiringWeight_Nom"});
        addVar({"eventWeight_mu", "muonWeightId[0] * muonWeightIso[0] * muonWeightTrg[0]"});
        if (_mode == "lss" or _mode == "los") {
            addVar({"eventWeight_tau", "tauWeightIdVsJet_loose[0][0] * tauWeightIdVsEl_loose[0][0] * tauWeightIdVsMu_loose[0][0]"});
        } else {
            addVar({"eventWeight_tau", "tauWeightIdVsJet[0][0] * tauWeightIdVsEl[0][0] * tauWeightIdVsMu[0][0]"});
        }
        addVar({"eventWeight_nobtag", "eventWeight_genpu * eventWeight_mu * eventWeight_tau"});
        addVar({"eventWeight", "eventWeight_nobtag * btagWeight_DeepFlavB[0]"});
    }

    // define variables that you want to store
    addVartoStore("run");
    addVartoStore("event");
    addVartoStore("Tau1.*");
    addVartoStore("eventWeight.*");
}

void TauFakeFactorAnalyzer::bookHists() {


    if (_syst != "data") {
        //We anyway need this for bSF rescaling
        add1DHist({"h_nevents", ";Number of events w/o b SF;Events", 2, -0.5, 1.5}, "one", "eventWeight", "_nobtag", "0", "");
    }

    maxstep = "";
    if (_mode == "lss" or _mode == "los") {
        add1DHist({"h_tau1_pt", ";#tau_{h} p_{T} (GeV);Events", 20, 0, 400}, "Tau1_pt_loose", "eventWeight", "", "0", maxstep);
        add1DHist({"h_tau1_gen_pt", ";#tau_{h} p_{T} (GeV) (MC: Tau_genPartFlav == 5);Events", 20, 0, 400}, "Tau1_pt_loose_gen", "eventWeight", "", "0", maxstep);
    } else {
        add1DHist({"h_tau1_pt", ";#tau_{h} p_{T} (GeV);Events", 20, 0, 400}, "Tau1_pt", "eventWeight", "", "0", maxstep);
        add1DHist({"h_tau1_gen_pt", ";#tau_{h} p_{T} (GeV) (MC: Tau_genPartFlav == 5);Events", 20, 0, 400}, "Tau1_pt_gen", "eventWeight", "", "0", maxstep);
    }
}
