#!/bin/bash
version=skim_LFVv8
mc16pre=/data1/common/skimmed_NanoAOD/$version/mc/16pre
mc16post=/data1/common/skimmed_NanoAOD/$version/mc/16post
mc17=/data1/common/skimmed_NanoAOD/$version/mc/17
mc18=/data1/common/skimmed_NanoAOD/$version/mc/18
label=aug22     # Arbitrary strings
ana=$1          # stlfv or ttlfv
# syst
#syst=nom
#syst=pudown
#syst=btagdown_hf
#syst=btagdown_lf
#syst=btagdown_hfstats1
#syst=btagdown_hfstats2
#syst=btagdown_lfstats1
#syst=btagdown_lfstats2
#syst=btagdown_cferr1
#syst=btagdown_cferr2
#syst=down_jesAbsolute
#syst=down_jesBBEC1
#syst=down_jesEC2
#syst=down_jesFlavorQCD
#syst=down_jesRelativeBal
#syst=down_jesAbsolute_year
#syst=down_jesBBEC1_year
#syst=down_jesEC2_year
syst=down_jesRelativeSample_year
target=${label}_${ana}/${syst} # Arbitrary folder name
mkdir -p ${target}
# 16pre
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/DYJetsToLL_M-10to50 ${target}/DYJetsToLL_M-10to50_16pre_${syst}.root &> ${target}/DYJetsToLL_M-10to50_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/DYJetsToLL_M-50_amcatnlo ${target}/DYJetsToLL_M-50_amcatnlo_16pre_${syst}.root &> ${target}/DYJetsToLL_M-50_amcatnlo_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/DYJetsToLL_M-50_madgraph ${target}/DYJetsToLL_M-50_madgraph_16pre_${syst}.root &> ${target}/DYJetsToLL_M-50_madgraph_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/ST_t-channel_antitop ${target}/ST_t-channel_antitop_16pre_${syst}.root &> ${target}/ST_t-channel_antitop_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/ST_t-channel_top ${target}/ST_t-channel_top_16pre_${syst}.root &> ${target}/ST_t-channel_top_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/ST_tW_antitop ${target}/ST_tW_antitop_16pre_${syst}.root &> ${target}/ST_tW_antitop_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/ST_tW_top ${target}/ST_tW_top_16pre_${syst}.root &> ${target}/ST_tW_top_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/TTTo2L2Nu ${target}/TTTo2L2Nu_16pre_${syst}.root &> ${target}/TTTo2L2Nu_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/TTToHadronic ${target}/TTToHadronic_16pre_${syst}.root &> ${target}/TTToHadronic_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/TTToSemiLeptonic ${target}/TTToSemiLeptonic_16pre_${syst}.root &> ${target}/TTToSemiLeptonic_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/TTWJetsToLNu ${target}/TTWJetsToLNu_16pre_${syst}.root &> ${target}/TTWJetsToLNu_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/TTWJetsToQQ ${target}/TTWJetsToQQ_16pre_${syst}.root &> ${target}/TTWJetsToQQ_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/TTZToLLNuNu ${target}/TTZToLLNuNu_16pre_${syst}.root &> ${target}/TTZToLLNuNu_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/TTZToQQ ${target}/TTZToQQ_16pre_${syst}.root &> ${target}/TTZToQQ_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/WJetsToLNu_HT-0To100 ${target}/WJetsToLNu_HT-0To100_16pre_${syst}.root &> ${target}/WJetsToLNu_HT-0To100_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/WJetsToLNu_HT-100To200 ${target}/WJetsToLNu_HT-100To200_16pre_${syst}.root &> ${target}/WJetsToLNu_HT-100To200_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/WJetsToLNu_HT-200To400 ${target}/WJetsToLNu_HT-200To400_16pre_${syst}.root &> ${target}/WJetsToLNu_HT-200To400_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/WJetsToLNu_HT-400To600 ${target}/WJetsToLNu_HT-400To600_16pre_${syst}.root &> ${target}/WJetsToLNu_HT-400To600_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/WJetsToLNu_HT-600To800 ${target}/WJetsToLNu_HT-600To800_16pre_${syst}.root &> ${target}/WJetsToLNu_HT-600To800_16pre_${syst}.out &
sleep 15m
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/WJetsToLNu_HT-800To1200 ${target}/WJetsToLNu_HT-800To1200_16pre_${syst}.root &> ${target}/WJetsToLNu_HT-800To1200_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/WJetsToLNu_HT-1200To2500 ${target}/WJetsToLNu_HT-1200To2500_16pre_${syst}.root &> ${target}/WJetsToLNu_HT-1200To2500_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/WJetsToLNu_HT-2500ToInf ${target}/WJetsToLNu_HT-2500ToInf_16pre_${syst}.root &> ${target}/WJetsToLNu_HT-2500ToInf_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/WW ${target}/WW_16pre_${syst}.root &> ${target}/WW_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/WZ ${target}/WZ_16pre_${syst}.root &> ${target}/WZ_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/ZZ ${target}/ZZ_16pre_${syst}.root &> ${target}/ZZ_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/QCD_Pt-15To20_MuEnrichedPt5    ${target}/QCD_Pt-15To20_16pre_${syst}.root    &> ${target}/QCD_Pt-15To20_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/QCD_Pt-20To30_MuEnrichedPt5    ${target}/QCD_Pt-20To30_16pre_${syst}.root    &> ${target}/QCD_Pt-20To30_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/QCD_Pt-30To50_MuEnrichedPt5    ${target}/QCD_Pt-30To50_16pre_${syst}.root    &> ${target}/QCD_Pt-30To50_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/QCD_Pt-50To80_MuEnrichedPt5    ${target}/QCD_Pt-50To80_16pre_${syst}.root    &> ${target}/QCD_Pt-50To80_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/QCD_Pt-80To120_MuEnrichedPt5   ${target}/QCD_Pt-80To120_16pre_${syst}.root   &> ${target}/QCD_Pt-80To120_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/QCD_Pt-120To170_MuEnrichedPt5  ${target}/QCD_Pt-120To170_16pre_${syst}.root  &> ${target}/QCD_Pt-120To170_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/QCD_Pt-170To300_MuEnrichedPt5  ${target}/QCD_Pt-170To300_16pre_${syst}.root  &> ${target}/QCD_Pt-170To300_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/QCD_Pt-300To470_MuEnrichedPt5  ${target}/QCD_Pt-300To470_16pre_${syst}.root  &> ${target}/QCD_Pt-300To470_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/QCD_Pt-470To600_MuEnrichedPt5  ${target}/QCD_Pt-470To600_16pre_${syst}.root  &> ${target}/QCD_Pt-470To600_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/QCD_Pt-600To800_MuEnrichedPt5  ${target}/QCD_Pt-600To800_16pre_${syst}.root  &> ${target}/QCD_Pt-600To800_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/QCD_Pt-800To1000_MuEnrichedPt5 ${target}/QCD_Pt-800To1000_16pre_${syst}.root &> ${target}/QCD_Pt-800To1000_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/QCD_Pt-1000_MuEnrichedPt5      ${target}/QCD_Pt-1000_16pre_${syst}.root      &> ${target}/QCD_Pt-1000_16pre_${syst}.out &
sleep 15m
# 16post
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/DYJetsToLL_M-10to50 ${target}/DYJetsToLL_M-10to50_16post_${syst}.root &> ${target}/DYJetsToLL_M-10to50_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/DYJetsToLL_M-50_amcatnlo ${target}/DYJetsToLL_M-50_amcatnlo_16post_${syst}.root &> ${target}/DYJetsToLL_M-50_amcatnlo_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/DYJetsToLL_M-50_madgraph ${target}/DYJetsToLL_M-50_madgraph_16post_${syst}.root &> ${target}/DYJetsToLL_M-50_madgraph_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/ST_t-channel_antitop ${target}/ST_t-channel_antitop_16post_${syst}.root &> ${target}/ST_t-channel_antitop_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/ST_t-channel_top ${target}/ST_t-channel_top_16post_${syst}.root &> ${target}/ST_t-channel_top_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/ST_tW_antitop ${target}/ST_tW_antitop_16post_${syst}.root &> ${target}/ST_tW_antitop_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/ST_tW_top ${target}/ST_tW_top_16post_${syst}.root &> ${target}/ST_tW_top_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/TTTo2L2Nu ${target}/TTTo2L2Nu_16post_${syst}.root &> ${target}/TTTo2L2Nu_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/TTToHadronic ${target}/TTToHadronic_16post_${syst}.root &> ${target}/TTToHadronic_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/TTToSemiLeptonic ${target}/TTToSemiLeptonic_16post_${syst}.root &> ${target}/TTToSemiLeptonic_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/TTWJetsToLNu ${target}/TTWJetsToLNu_16post_${syst}.root &> ${target}/TTWJetsToLNu_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/TTWJetsToQQ ${target}/TTWJetsToQQ_16post_${syst}.root &> ${target}/TTWJetsToQQ_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/TTZToLLNuNu ${target}/TTZToLLNuNu_16post_${syst}.root &> ${target}/TTZToLLNuNu_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/TTZToQQ ${target}/TTZToQQ_16post_${syst}.root &> ${target}/TTZToQQ_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/WJetsToLNu_HT-0To100 ${target}/WJetsToLNu_HT-0To100_16post_${syst}.root &> ${target}/WJetsToLNu_HT-0To100_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/WJetsToLNu_HT-100To200 ${target}/WJetsToLNu_HT-100To200_16post_${syst}.root &> ${target}/WJetsToLNu_HT-100To200_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/WJetsToLNu_HT-200To400 ${target}/WJetsToLNu_HT-200To400_16post_${syst}.root &> ${target}/WJetsToLNu_HT-200To400_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/WJetsToLNu_HT-400To600 ${target}/WJetsToLNu_HT-400To600_16post_${syst}.root &> ${target}/WJetsToLNu_HT-400To600_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/WJetsToLNu_HT-600To800 ${target}/WJetsToLNu_HT-600To800_16post_${syst}.root &> ${target}/WJetsToLNu_HT-600To800_16post_${syst}.out &
sleep 15m
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/WJetsToLNu_HT-800To1200 ${target}/WJetsToLNu_HT-800To1200_16post_${syst}.root &> ${target}/WJetsToLNu_HT-800To1200_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/WJetsToLNu_HT-1200To2500 ${target}/WJetsToLNu_HT-1200To2500_16post_${syst}.root &> ${target}/WJetsToLNu_HT-1200To2500_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/WJetsToLNu_HT-2500ToInf ${target}/WJetsToLNu_HT-2500ToInf_16post_${syst}.root &> ${target}/WJetsToLNu_HT-2500ToInf_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/WW ${target}/WW_16post_${syst}.root &> ${target}/WW_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/WZ ${target}/WZ_16post_${syst}.root &> ${target}/WZ_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/ZZ ${target}/ZZ_16post_${syst}.root &> ${target}/ZZ_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/QCD_Pt-15To20_MuEnrichedPt5    ${target}/QCD_Pt-15To20_16post_${syst}.root    &> ${target}/QCD_Pt-15To20_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/QCD_Pt-20To30_MuEnrichedPt5    ${target}/QCD_Pt-20To30_16post_${syst}.root    &> ${target}/QCD_Pt-20To30_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/QCD_Pt-30To50_MuEnrichedPt5    ${target}/QCD_Pt-30To50_16post_${syst}.root    &> ${target}/QCD_Pt-30To50_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/QCD_Pt-50To80_MuEnrichedPt5    ${target}/QCD_Pt-50To80_16post_${syst}.root    &> ${target}/QCD_Pt-50To80_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/QCD_Pt-80To120_MuEnrichedPt5   ${target}/QCD_Pt-80To120_16post_${syst}.root   &> ${target}/QCD_Pt-80To120_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/QCD_Pt-120To170_MuEnrichedPt5  ${target}/QCD_Pt-120To170_16post_${syst}.root  &> ${target}/QCD_Pt-120To170_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/QCD_Pt-170To300_MuEnrichedPt5  ${target}/QCD_Pt-170To300_16post_${syst}.root  &> ${target}/QCD_Pt-170To300_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/QCD_Pt-300To470_MuEnrichedPt5  ${target}/QCD_Pt-300To470_16post_${syst}.root  &> ${target}/QCD_Pt-300To470_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/QCD_Pt-470To600_MuEnrichedPt5  ${target}/QCD_Pt-470To600_16post_${syst}.root  &> ${target}/QCD_Pt-470To600_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/QCD_Pt-600To800_MuEnrichedPt5  ${target}/QCD_Pt-600To800_16post_${syst}.root  &> ${target}/QCD_Pt-600To800_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/QCD_Pt-800To1000_MuEnrichedPt5 ${target}/QCD_Pt-800To1000_16post_${syst}.root &> ${target}/QCD_Pt-800To1000_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/QCD_Pt-1000_MuEnrichedPt5      ${target}/QCD_Pt-1000_16post_${syst}.root      &> ${target}/QCD_Pt-1000_16post_${syst}.out &

sleep 15m
# 17
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/DYJetsToLL_M-10to50 ${target}/DYJetsToLL_M-10to50_17_${syst}.root &> ${target}/DYJetsToLL_M-10to50_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/DYJetsToLL_M-50_amcatnlo ${target}/DYJetsToLL_M-50_amcatnlo_17_${syst}.root &> ${target}/DYJetsToLL_M-50_amcatnlo_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/DYJetsToLL_M-50_madgraph ${target}/DYJetsToLL_M-50_madgraph_17_${syst}.root &> ${target}/DYJetsToLL_M-50_madgraph_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/ST_t-channel_antitop ${target}/ST_t-channel_antitop_17_${syst}.root &> ${target}/ST_t-channel_antitop_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/ST_t-channel_top ${target}/ST_t-channel_top_17_${syst}.root &> ${target}/ST_t-channel_top_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/ST_tW_antitop ${target}/ST_tW_antitop_17_${syst}.root &> ${target}/ST_tW_antitop_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/ST_tW_top ${target}/ST_tW_top_17_${syst}.root &> ${target}/ST_tW_top_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/TTTo2L2Nu ${target}/TTTo2L2Nu_17_${syst}.root &> ${target}/TTTo2L2Nu_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/TTToHadronic ${target}/TTToHadronic_17_${syst}.root &> ${target}/TTToHadronic_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/TTToSemiLeptonic/000 ${target}/TTToSemiLeptonic_17_${syst}_1.root &> ${target}/TTToSemiLeptonic_17_${syst}_1.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/TTToSemiLeptonic/101 ${target}/TTToSemiLeptonic_17_${syst}_2.root &> ${target}/TTToSemiLeptonic_17_${syst}_2.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/TTToSemiLeptonic/202 ${target}/TTToSemiLeptonic_17_${syst}_3.root &> ${target}/TTToSemiLeptonic_17_${syst}_3.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/TTWJetsToLNu ${target}/TTWJetsToLNu_17_${syst}.root &> ${target}/TTWJetsToLNu_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/TTWJetsToQQ ${target}/TTWJetsToQQ_17_${syst}.root &> ${target}/TTWJetsToQQ_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/TTZToLLNuNu ${target}/TTZToLLNuNu_17_${syst}.root &> ${target}/TTZToLLNuNu_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/TTZToQQ ${target}/TTZToQQ_17_${syst}.root &> ${target}/TTZToQQ_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/WJetsToLNu_HT-0To100 ${target}/WJetsToLNu_HT-0To100_17_${syst}.root &> ${target}/WJetsToLNu_HT-0To100_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/WJetsToLNu_HT-100To200 ${target}/WJetsToLNu_HT-100To200_17_${syst}.root &> ${target}/WJetsToLNu_HT-100To200_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/WJetsToLNu_HT-200To400 ${target}/WJetsToLNu_HT-200To400_17_${syst}.root &> ${target}/WJetsToLNu_HT-200To400_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/WJetsToLNu_HT-400To600 ${target}/WJetsToLNu_HT-400To600_17_${syst}.root &> ${target}/WJetsToLNu_HT-400To600_17_${syst}.out &
sleep 15m
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/WJetsToLNu_HT-600To800 ${target}/WJetsToLNu_HT-600To800_17_${syst}.root &> ${target}/WJetsToLNu_HT-600To800_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/WJetsToLNu_HT-800To1200 ${target}/WJetsToLNu_HT-800To1200_17_${syst}.root &> ${target}/WJetsToLNu_HT-800To1200_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/WJetsToLNu_HT-1200To2500 ${target}/WJetsToLNu_HT-1200To2500_17_${syst}.root &> ${target}/WJetsToLNu_HT-1200To2500_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/WJetsToLNu_HT-2500ToInf ${target}/WJetsToLNu_HT-2500ToInf_17_${syst}.root &> ${target}/WJetsToLNu_HT-2500ToInf_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/WW ${target}/WW_17_${syst}.root &> ${target}/WW_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/WZ ${target}/WZ_17_${syst}.root &> ${target}/WZ_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/ZZ ${target}/ZZ_17_${syst}.root &> ${target}/ZZ_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/QCD_Pt-15To20_MuEnrichedPt5    ${target}/QCD_Pt-15To20_17_${syst}.root    &> ${target}/QCD_Pt-15To20_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/QCD_Pt-20To30_MuEnrichedPt5    ${target}/QCD_Pt-20To30_17_${syst}.root    &> ${target}/QCD_Pt-20To30_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/QCD_Pt-30To50_MuEnrichedPt5    ${target}/QCD_Pt-30To50_17_${syst}.root    &> ${target}/QCD_Pt-30To50_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/QCD_Pt-50To80_MuEnrichedPt5    ${target}/QCD_Pt-50To80_17_${syst}.root    &> ${target}/QCD_Pt-50To80_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/QCD_Pt-80To120_MuEnrichedPt5   ${target}/QCD_Pt-80To120_17_${syst}.root   &> ${target}/QCD_Pt-80To120_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/QCD_Pt-120To170_MuEnrichedPt5  ${target}/QCD_Pt-120To170_17_${syst}.root  &> ${target}/QCD_Pt-120To170_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/QCD_Pt-170To300_MuEnrichedPt5  ${target}/QCD_Pt-170To300_17_${syst}.root  &> ${target}/QCD_Pt-170To300_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/QCD_Pt-300To470_MuEnrichedPt5  ${target}/QCD_Pt-300To470_17_${syst}.root  &> ${target}/QCD_Pt-300To470_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/QCD_Pt-470To600_MuEnrichedPt5  ${target}/QCD_Pt-470To600_17_${syst}.root  &> ${target}/QCD_Pt-470To600_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/QCD_Pt-600To800_MuEnrichedPt5  ${target}/QCD_Pt-600To800_17_${syst}.root  &> ${target}/QCD_Pt-600To800_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/QCD_Pt-800To1000_MuEnrichedPt5 ${target}/QCD_Pt-800To1000_17_${syst}.root &> ${target}/QCD_Pt-800To1000_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/QCD_Pt-1000_MuEnrichedPt5      ${target}/QCD_Pt-1000_17_${syst}.root      &> ${target}/QCD_Pt-1000_17_${syst}.out &
sleep 15m
## 18
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/DYJetsToLL_M-10to50 ${target}/DYJetsToLL_M-10to50_18_${syst}.root &> ${target}/DYJetsToLL_M-10to50_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/DYJetsToLL_M-50_amcatnlo ${target}/DYJetsToLL_M-50_amcatnlo_18_${syst}.root &> ${target}/DYJetsToLL_M-50_amcatnlo_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/DYJetsToLL_M-50_madgraph ${target}/DYJetsToLL_M-50_madgraph_18_${syst}.root &> ${target}/DYJetsToLL_M-50_madgraph_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/ST_t-channel_antitop ${target}/ST_t-channel_antitop_18_${syst}.root &> ${target}/ST_t-channel_antitop_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/ST_t-channel_top ${target}/ST_t-channel_top_18_${syst}.root &> ${target}/ST_t-channel_top_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/ST_tW_antitop ${target}/ST_tW_antitop_18_${syst}.root &> ${target}/ST_tW_antitop_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/ST_tW_top ${target}/ST_tW_top_18_${syst}.root &> ${target}/ST_tW_top_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/TTTo2L2Nu ${target}/TTTo2L2Nu_18_${syst}.root &> ${target}/TTTo2L2Nu_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/TTToHadronic ${target}/TTToHadronic_18_${syst}.root &> ${target}/TTToHadronic_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/TTToSemiLeptonic/000 ${target}/TTToSemiLeptonic_18_${syst}_1.root &> ${target}/TTToSemiLeptonic_18_${syst}_1.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/TTToSemiLeptonic/155 ${target}/TTToSemiLeptonic_18_${syst}_2.root &> ${target}/TTToSemiLeptonic_18_${syst}_2.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/TTToSemiLeptonic/310 ${target}/TTToSemiLeptonic_18_${syst}_3.root &> ${target}/TTToSemiLeptonic_18_${syst}_3.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/TTWJetsToLNu ${target}/TTWJetsToLNu_18_${syst}.root &> ${target}/TTWJetsToLNu_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/TTWJetsToQQ ${target}/TTWJetsToQQ_18_${syst}.root &> ${target}/TTWJetsToQQ_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/TTZToLLNuNu ${target}/TTZToLLNuNu_18_${syst}.root &> ${target}/TTZToLLNuNu_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/TTZToQQ ${target}/TTZToQQ_18_${syst}.root &> ${target}/TTZToQQ_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/WJetsToLNu_HT-0To100 ${target}/WJetsToLNu_HT-0To100_18_${syst}.root &> ${target}/WJetsToLNu_HT-0To100_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/WJetsToLNu_HT-100To200 ${target}/WJetsToLNu_HT-100To200_18_${syst}.root &> ${target}/WJetsToLNu_HT-100To200_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/WJetsToLNu_HT-200To400 ${target}/WJetsToLNu_HT-200To400_18_${syst}.root &> ${target}/WJetsToLNu_HT-200To400_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/WJetsToLNu_HT-400To600 ${target}/WJetsToLNu_HT-400To600_18_${syst}.root &> ${target}/WJetsToLNu_HT-400To600_18_${syst}.out &
sleep 15m
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/WJetsToLNu_HT-600To800 ${target}/WJetsToLNu_HT-600To800_18_${syst}.root &> ${target}/WJetsToLNu_HT-600To800_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/WJetsToLNu_HT-800To1200 ${target}/WJetsToLNu_HT-800To1200_18_${syst}.root &> ${target}/WJetsToLNu_HT-800To1200_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/WJetsToLNu_HT-1200To2500 ${target}/WJetsToLNu_HT-1200To2500_18_${syst}.root &> ${target}/WJetsToLNu_HT-1200To2500_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/WJetsToLNu_HT-2500ToInf ${target}/WJetsToLNu_HT-2500ToInf_18_${syst}.root &> ${target}/WJetsToLNu_HT-2500ToInf_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/WW ${target}/WW_18_${syst}.root &> ${target}/WW_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/WZ ${target}/WZ_18_${syst}.root &> ${target}/WZ_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/ZZ ${target}/ZZ_18_${syst}.root &> ${target}/ZZ_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/QCD_Pt-15To20_MuEnrichedPt5    ${target}/QCD_Pt-15To20_18_${syst}.root    &> ${target}/QCD_Pt-15To20_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/QCD_Pt-20To30_MuEnrichedPt5    ${target}/QCD_Pt-20To30_18_${syst}.root    &> ${target}/QCD_Pt-20To30_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/QCD_Pt-30To50_MuEnrichedPt5    ${target}/QCD_Pt-30To50_18_${syst}.root    &> ${target}/QCD_Pt-30To50_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/QCD_Pt-50To80_MuEnrichedPt5    ${target}/QCD_Pt-50To80_18_${syst}.root    &> ${target}/QCD_Pt-50To80_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/QCD_Pt-80To120_MuEnrichedPt5   ${target}/QCD_Pt-80To120_18_${syst}.root   &> ${target}/QCD_Pt-80To120_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/QCD_Pt-120To170_MuEnrichedPt5  ${target}/QCD_Pt-120To170_18_${syst}.root  &> ${target}/QCD_Pt-120To170_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/QCD_Pt-170To300_MuEnrichedPt5  ${target}/QCD_Pt-170To300_18_${syst}.root  &> ${target}/QCD_Pt-170To300_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/QCD_Pt-300To470_MuEnrichedPt5  ${target}/QCD_Pt-300To470_18_${syst}.root  &> ${target}/QCD_Pt-300To470_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/QCD_Pt-470To600_MuEnrichedPt5  ${target}/QCD_Pt-470To600_18_${syst}.root  &> ${target}/QCD_Pt-470To600_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/QCD_Pt-600To800_MuEnrichedPt5  ${target}/QCD_Pt-600To800_18_${syst}.root  &> ${target}/QCD_Pt-600To800_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/QCD_Pt-800To1000_MuEnrichedPt5 ${target}/QCD_Pt-800To1000_18_${syst}.root &> ${target}/QCD_Pt-800To1000_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/QCD_Pt-1000_MuEnrichedPt5      ${target}/QCD_Pt-1000_18_${syst}.root      &> ${target}/QCD_Pt-1000_18_${syst}.out &
