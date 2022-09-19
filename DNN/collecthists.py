import ROOT
import os
import sys

projname = "aug22"
#projname = "test"
label = "optimized"
label = "rerun"
lfvprocs = ["ST","TT"]
#lfvprocs = ["TT"]
hists = ["data_obs","hstacked_mc_h_dnn_pred",
        "LFV_STc_s","LFV_STc_v","LFV_STc_t","LFV_STu_s","LFV_STu_v","LFV_STu_t",
        "LFV_TTc_s","LFV_TTc_v","LFV_TTc_t","LFV_TTu_s","LFV_TTu_v","LFV_TTu_t",
        "TT_di","TT_semi","ST","Wjets","Others"]

systs = {"nom":"nom",
        "jesup":"jesUp","jesdown":"jesDown",
        "puup":"puUp","pudown":"puDown",
        "btagup_jes":"btag_jesUp","btagdown_jes":"btag_jesDown",
        "btagup_hf":"btag_hfUp","btagdown_hf":"btag_hfDown",
        "btagup_lf":"btag_lfUp","btagdown_lf":"btag_lfDown",
        }

systs = {"nom":"nom",
        "puup":"puUp","pudown":"puDown",
        "btagup_hf":"btag_hfUp","btagdown_hf":"btag_hfDown",
        "btagup_lf":"btag_lfUp","btagdown_lf":"btag_lfDown",
        "btagup_hfstats1":"btag_hfstats1Up","btagdown_hfstats1":"btag_hfstats1Down",
        "btagup_lfstats1":"btag_lfstats1Up","btagdown_lfstats1":"btag_lfstats1Down",
        "btagup_hfstats2":"btag_hfstats2Up","btagdown_hfstats2":"btag_hfstats2Down",
        "btagup_lfstats2":"btag_lfstats2Up","btagdown_lfstats2":"btag_lfstats2Down",
        "btagup_cferr1":"btag_cferr1Up","btagdown_cferr1":"btag_cferr1Down",
        "btagup_cferr2":"btag_cferr2Up","btagdown_cferr2":"btag_cferr2Down",

        "up_jesAbsolute":"jesAbsoluteUp","down_jesAbsolute":"jesAbsoluteDown",
        "up_jesAbsolute_year":"jesAbsolute_yearUp","down_jesAbsolute_year":"jesAbsolute_yearDown",
        "up_jesBBEC1":"jesBBEC1Up","down_jesBBEC1":"jesBBEC1Down",
        "up_jesBBEC1_year":"jesBBEC1_yearUp","down_jesBBEC1_year":"jesBBEC1_yearDown",
        "up_jesEC2":"jesEC2Up","down_jesEC2":"jesEC2Down",
        "up_jesEC2_year":"jesEC2_yearUp","down_jesEC2_year":"jesEC2_yearDown",
        "up_jesFlavorQCD":"jesFlavorQCDUp","down_jesFlavorQCD":"jesFlavorQCDDown",
        "up_jesRelativeBal":"jesRelativeBalUp","down_jesRelativeBal":"jesRelativeBalDown",
        "up_jesRelativeSample_year":"jesRelativeSample_yearUp","down_jesRelativeSample_year":"jesRelativeSample_yearDown",
        }

#systs = {"norm":"norm",
#        "jesup":"jesUp","jesdown":"jesDown",
#        "puup":"puUp","pudown":"puDown",
#        }
runs = ["run16APV","run16","run17","run18"]#,"run2"]

outfolder = "./pred_"+projname

if not os.path.isdir(outfolder):
    os.makedirs(outfolder)

for run in runs:
    rname = ""
    if run == "run16APV":
        rname = "stackhist_19.5.root"
    elif run == "run16":
        rname = "stackhist_16.8.root"
    elif run == "run17":
        rname = "stackhist_41.48.root"
    elif run == "run18":
        rname = "stackhist_59.83.root"
    elif run == "run2":
        rname = "stackhist_137.65.root"
    for lfvproc in lfvprocs:
        outfname = ""
        if lfvproc == "ST":
            outfname = outfolder+"/pred_"+run+"_cat1.root"
        elif lfvproc == "TT":
            outfname = outfolder+"/pred_"+run+"_cat2.root"
        outf = ROOT.TFile(outfname,"RECREATE")
        for key, value in systs.items():
            infilename = label+"_"+projname+"_"+lfvproc.lower()+"lfv/"+key+"/pred_hists/noblind/"+rname
            inf = ROOT.TFile(infilename)
            outf.cd()
            for h in hists:
                tmphist = inf.Get(h)
                if not tmphist:
                    print("wrong hist",h)
                    continue
                copyhname = h
                if key is not "nom":
                    copyhname = h+"_"+value
                    if "year" in key:
                        copyhname = copyhname.replace("year", "20"+run[3:5])
                print(copyhname)
                tmphist_copy = tmphist.Clone(copyhname)
                tmphist_copy.Write()
            inf.Close()
        outf.Close()
