import os
import sys
os.environ["CUDA_VISIBLE_DEVICES"] = "3"
import uproot
import pandas as pd
import awkward as ak
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
from keras.callbacks import EarlyStopping, ModelCheckpoint
from utils.hists import *

base_dir = os.getcwd().replace("DNN","") # Upper directory
processed = "aug22"
label = "rerun"
systs = ["nom",#"jesup","jesdown",
        #"puup","pudown","btagup_jes","btagdown_jes",
        #"btagup_hf","btagdown_hf","btagup_lf","btagdown_lf",
        ]
#systs = ["jesup","jesdown",
#systs = ["norm","jesup","jesdown","puup","pudown",]
#systs = ["btagup_jes","btagdown_jes","btagup_hf","btagdown_hf","btagup_lf","btagdown_lf",]
for syst in systs:
    #for y in ["16pre","16post","17","18"]:
    for y in ["18"]:
        #for p in ["ST","TT"]:
        for p in ["ST"]:
            print("Start "+p+" LFV Evaluation")
            inputvars = []
            if p == "ST":
                inputvars = ["Sel_muon1pt","Sel_muon1eta",
                    "Sel_tau1pt","Sel_tau1eta","Sel_tau1mass",
                    "Sel2_jet1pt","Sel2_jet2pt","Sel2_jet3pt",
                    "Sel2_jet1eta","Sel2_jet2eta","Sel2_jet3eta",
                    "Sel2_jet1mass","Sel2_jet2mass","Sel2_jet3mass",
                    "Sel2_jet1btag","Sel2_jet2btag","Sel2_jet3btag",
                    "Sys_METpt","Sys_METphi",
                    "chi2","chi2_SMW_mass","chi2_SMTop_mass",
                    "chi2_wqq_dEta","chi2_wqq_dPhi","chi2_wqq_dR",
                    "mutau_mass","mutau_dEta","mutau_dPhi","mutau_dR",
                    ]
            elif p == "TT":
                inputvars = ["Sel_muon1pt","Sel_muon1eta",
                    "Sel_tau1pt","Sel_tau1eta","Sel_tau1mass",
                    "Sel2_jet1pt","Sel2_jet2pt","Sel2_jet3pt","Sel2_jet4pt",
                    "Sel2_jet1eta","Sel2_jet2eta","Sel2_jet3eta","Sel2_jet4eta",
                    "Sel2_jet1mass","Sel2_jet2mass","Sel2_jet3mass","Sel2_jet4mass",
                    "Sel2_jet1btag","Sel2_jet2btag","Sel2_jet3btag","Sel2_jet4btag",
                    "Sys_METpt","Sys_METphi",
                    "chi2","chi2_lfvTop_mass","chi2_SMW_mass","chi2_SMTop_mass",
                    "chi2_wqq_dEta","chi2_wqq_dPhi","chi2_wqq_dR",
                    "chi2_lfvjmu_dEta","chi2_lfvjmu_dPhi","chi2_lfvjmu_dR","chi2_lfvjmu_mass",
                    "chi2_lfvjtau_dEta","chi2_lfvjtau_dPhi","chi2_lfvjtau_dR","chi2_lfvjtau_mass",
                    "chi2_lfvjmutau_dEta","chi2_lfvjmutau_dPhi","chi2_lfvjmutau_dR","chi2_lfvjmutau_mass",
                    "mutau_mass","mutau_dEta","mutau_dPhi","mutau_dR",
                    ]

            #project_dir = "nanoaodframe_"+p+"LFV/"+processed+"_"+syst+"/"+y+"/"    # MODIFY!!!
            project_dir = "/home/itseyes/github/LFVRun2_ndf_integration/nanoaodframe/aug22_stlfv/"+syst+"/"+y+"/"    # MODIFY!!!
            #path = base_dir+project_dir
            path = project_dir
            flist = os.listdir(path)
            flist = [i for i in flist if ".root" in i]
            
            model_dir = label+"_"+p+processed+"/norm/best_model.h5"
            #print(model_dir)
            model = tf.keras.models.load_model(model_dir)
            #model.summary()
            
            eval_dir = label+"_"+p+processed+"/"+syst

            weights = ["evWeight"]
            hists_path = eval_dir+"/pred_hists/"+y+"/"
            if not os.path.isdir(hists_path):
                os.makedirs(hists_path)

            for f in flist:
                fdict = f.split("_"+y)[0]
                f_dir = path+f
                outf_dir = hists_path+f.replace(".root","")+"_pred.root"
                infile = uproot.open(f_dir)
                tree = infile["outputTree2"]
                hnocut = infile["hcounter_nocut"]
                hpglepweight = infile["hnevents_pglep_cut0000"]
                hfullweight = infile["hnevents_cut0000"]
                hfinal = None
                if p == "ST":
                    hfinal = infile["hnevents_cut00000"]
                elif p == "TT":
                    hfinal = infile["hnevents_cut000000"]
                if len(tree) == 0:
                    print("No events : "+f)
                    continue
                print(f)
                pd_data = tree.arrays(inputvars,library="pd")
                pd_weight = tree.arrays(weights,library="np")
                pred_data = np.array(pd_data.filter(items = inputvars))
                pred = model.predict(pred_data,batch_size=128)
                pred = pred[:,1].tolist()
                pd_weight = pd_weight['evWeight'].tolist()
                binedges = [0,0.1,0.3,0.7,0.9,1.0]
                #dnnhist = np.histogram(pred,20,(0.0,1.0),weights=pd_weight,density=False)
                dnnhist = np.histogram(pred,bins=binedges,weights=pd_weight,density=False)
                dnnhist_entries = np.histogram(pred,bins=binedges,density=False)
                with uproot.recreate(outf_dir) as outf:
                    outf["h_dnn_pred"] = dnnhist
                    outf["h_dnn_entries"] = dnnhist_entries
                    outf["hcounter_nocut"] = hnocut
                    outf["hnevents_pglep_cut0000"] = hpglepweight
                    outf["hnevents_cut0000"] = hfullweight
                    outf["hnevents_final"] = hfinal
