#!/bin/bash
if [ -z "$1" ]
then
    echo "No Input Argument"
else
    #systs=( norm jesup jesdown puup pudown btagup_jes btagdown_jes btagup_hf btagdown_hf btagup_lf btagdown_lf )
    systs=( nom )
    for syst in "${systs[@]}"; do
        cd ${1}/${syst}/pred_hists/
        rm -rf Run2_${syst}_pred.root
        hadd Run2_${syst}_pred.root 1*/Run*.root
        rm -rf plot* stack*.root
#        mkdir -p noblind
#        mkdir -p blind
#        rm -rf *blind/*
        mkdir -p simple
        rm -rf simple/*
        #for i in run2 16pre 16post 17 18
        for i in 18
        do
            python ../../../drawhists.py -SYS ${syst} -Y $i -B
            python ../../../drawhists.py -SYS ${syst} -Y $i -L -B
#            mv plot* noblind
#            mv stackhist* noblind
            mv plot* simple
            mv stackhist* simple
        done
        cd -
    done
fi
