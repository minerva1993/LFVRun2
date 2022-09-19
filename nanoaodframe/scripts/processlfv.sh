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
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/ST_LFV_TCMuTau_Scalar ${target}/ST_LFV_TCMuTau_Scalar_16pre_${syst}.root &> ${target}/ST_LFV_TCMuTau_Scalar_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/ST_LFV_TCMuTau_Vector ${target}/ST_LFV_TCMuTau_Vector_16pre_${syst}.root &> ${target}/ST_LFV_TCMuTau_Vector_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/ST_LFV_TCMuTau_Tensor ${target}/ST_LFV_TCMuTau_Tensor_16pre_${syst}.root &> ${target}/ST_LFV_TCMuTau_Tensor_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/ST_LFV_TUMuTau_Scalar ${target}/ST_LFV_TUMuTau_Scalar_16pre_${syst}.root &> ${target}/ST_LFV_TUMuTau_Scalar_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/ST_LFV_TUMuTau_Vector ${target}/ST_LFV_TUMuTau_Vector_16pre_${syst}.root &> ${target}/ST_LFV_TUMuTau_Vector_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/ST_LFV_TUMuTau_Tensor ${target}/ST_LFV_TUMuTau_Tensor_16pre_${syst}.root &> ${target}/ST_LFV_TUMuTau_Tensor_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16post}/TT_LFV_TToCMuTau_Scalar ${target}/TT_LFV_TToCMuTau_Scalar_16pre_${syst}.root &> ${target}/TT_LFV_TToCMuTau_Scalar_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/TT_LFV_TToCMuTau_Vector ${target}/TT_LFV_TToCMuTau_Vector_16pre_${syst}.root &> ${target}/TT_LFV_TToCMuTau_Vector_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/TT_LFV_TToCMuTau_Tensor ${target}/TT_LFV_TToCMuTau_Tensor_16pre_${syst}.root &> ${target}/TT_LFV_TToCMuTau_Tensor_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/TT_LFV_TToUMuTau_Scalar ${target}/TT_LFV_TToUMuTau_Scalar_16pre_${syst}.root &> ${target}/TT_LFV_TToUMuTau_Scalar_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/TT_LFV_TToUMuTau_Vector ${target}/TT_LFV_TToUMuTau_Vector_16pre_${syst}.root &> ${target}/TT_LFV_TToUMuTau_Vector_16pre_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16pre -S ${syst} --globaltag Summer19UL16APV_V7 ${mc16pre}/TT_LFV_TToUMuTau_Tensor ${target}/TT_LFV_TToUMuTau_Tensor_16pre_${syst}.root &> ${target}/TT_LFV_TToUMuTau_Tensor_16pre_${syst}.out &
# 16post
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/ST_LFV_TCMuTau_Scalar ${target}/ST_LFV_TCMuTau_Scalar_16post_${syst}.root &> ${target}/ST_LFV_TCMuTau_Scalar_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/ST_LFV_TCMuTau_Vector ${target}/ST_LFV_TCMuTau_Vector_16post_${syst}.root &> ${target}/ST_LFV_TCMuTau_Vector_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/ST_LFV_TCMuTau_Tensor ${target}/ST_LFV_TCMuTau_Tensor_16post_${syst}.root &> ${target}/ST_LFV_TCMuTau_Tensor_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/ST_LFV_TUMuTau_Scalar ${target}/ST_LFV_TUMuTau_Scalar_16post_${syst}.root &> ${target}/ST_LFV_TUMuTau_Scalar_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/ST_LFV_TUMuTau_Vector ${target}/ST_LFV_TUMuTau_Vector_16post_${syst}.root &> ${target}/ST_LFV_TUMuTau_Vector_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/ST_LFV_TUMuTau_Tensor ${target}/ST_LFV_TUMuTau_Tensor_16post_${syst}.root &> ${target}/ST_LFV_TUMuTau_Tensor_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/TT_LFV_TToCMuTau_Scalar ${target}/TT_LFV_TToCMuTau_Scalar_16post_${syst}.root &> ${target}/TT_LFV_TToCMuTau_Scalar_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/TT_LFV_TToCMuTau_Vector ${target}/TT_LFV_TToCMuTau_Vector_16post_${syst}.root &> ${target}/TT_LFV_TToCMuTau_Vector_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/TT_LFV_TToCMuTau_Tensor ${target}/TT_LFV_TToCMuTau_Tensor_16post_${syst}.root &> ${target}/TT_LFV_TToCMuTau_Tensor_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/TT_LFV_TToUMuTau_Scalar ${target}/TT_LFV_TToUMuTau_Scalar_16post_${syst}.root &> ${target}/TT_LFV_TToUMuTau_Scalar_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/TT_LFV_TToUMuTau_Vector ${target}/TT_LFV_TToUMuTau_Vector_16post_${syst}.root &> ${target}/TT_LFV_TToUMuTau_Vector_16post_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 16post -S ${syst} --globaltag Summer19UL16_V7 ${mc16post}/TT_LFV_TToUMuTau_Tensor ${target}/TT_LFV_TToUMuTau_Tensor_16post_${syst}.root &> ${target}/TT_LFV_TToUMuTau_Tensor_16post_${syst}.out &

sleep 12m

# 17
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/ST_LFV_TCMuTau_Scalar ${target}/ST_LFV_TCMuTau_Scalar_17_${syst}.root &> ${target}/ST_LFV_TCMuTau_Scalar_17_${syst}.out &
#
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc18}/ST_LFV_TCMuTau_Vector ${target}/ST_LFV_TCMuTau_Vector_17_${syst}.root &> ${target}/ST_LFV_TCMuTau_Vector_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/ST_LFV_TCMuTau_Tensor ${target}/ST_LFV_TCMuTau_Tensor_17_${syst}.root &> ${target}/ST_LFV_TCMuTau_Tensor_17_${syst}.out &
#
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc18}/ST_LFV_TUMuTau_Scalar ${target}/ST_LFV_TUMuTau_Scalar_17_${syst}.root &> ${target}/ST_LFV_TUMuTau_Scalar_17_${syst}.out &
#
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc18}/ST_LFV_TUMuTau_Vector ${target}/ST_LFV_TUMuTau_Vector_17_${syst}.root &> ${target}/ST_LFV_TUMuTau_Vector_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/ST_LFV_TUMuTau_Tensor ${target}/ST_LFV_TUMuTau_Tensor_17_${syst}.root &> ${target}/ST_LFV_TUMuTau_Tensor_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/TT_LFV_TToCMuTau_Scalar ${target}/TT_LFV_TToCMuTau_Scalar_17_${syst}.root &> ${target}/TT_LFV_TToCMuTau_Scalar_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/TT_LFV_TToCMuTau_Vector ${target}/TT_LFV_TToCMuTau_Vector_17_${syst}.root &> ${target}/TT_LFV_TToCMuTau_Vector_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/TT_LFV_TToCMuTau_Tensor ${target}/TT_LFV_TToCMuTau_Tensor_17_${syst}.root &> ${target}/TT_LFV_TToCMuTau_Tensor_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/TT_LFV_TToUMuTau_Scalar ${target}/TT_LFV_TToUMuTau_Scalar_17_${syst}.root &> ${target}/TT_LFV_TToUMuTau_Scalar_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/TT_LFV_TToUMuTau_Vector ${target}/TT_LFV_TToUMuTau_Vector_17_${syst}.root &> ${target}/TT_LFV_TToUMuTau_Vector_17_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 17 -S ${syst} --globaltag Summer19UL17_V5 ${mc17}/TT_LFV_TToUMuTau_Tensor ${target}/TT_LFV_TToUMuTau_Tensor_17_${syst}.root &> ${target}/TT_LFV_TToUMuTau_Tensor_17_${syst}.out &
# 18
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/ST_LFV_TCMuTau_Scalar ${target}/ST_LFV_TCMuTau_Scalar_18_${syst}.root &> ${target}/ST_LFV_TCMuTau_Scalar_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/ST_LFV_TCMuTau_Vector ${target}/ST_LFV_TCMuTau_Vector_18_${syst}.root &> ${target}/ST_LFV_TCMuTau_Vector_18_${syst}.out &
#
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc17}/ST_LFV_TCMuTau_Tensor ${target}/ST_LFV_TCMuTau_Tensor_18_${syst}.root &> ${target}/ST_LFV_TCMuTau_Tensor_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/ST_LFV_TUMuTau_Scalar ${target}/ST_LFV_TUMuTau_Scalar_18_${syst}.root &> ${target}/ST_LFV_TUMuTau_Scalar_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/ST_LFV_TUMuTau_Vector ${target}/ST_LFV_TUMuTau_Vector_18_${syst}.root &> ${target}/ST_LFV_TUMuTau_Vector_18_${syst}.out &
#
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc17}/ST_LFV_TUMuTau_Tensor ${target}/ST_LFV_TUMuTau_Tensor_18_${syst}.root &> ${target}/ST_LFV_TUMuTau_Tensor_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/TT_LFV_TToCMuTau_Scalar ${target}/TT_LFV_TToCMuTau_Scalar_18_${syst}.root &> ${target}/TT_LFV_TToCMuTau_Scalar_18_${syst}.out &
#
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc17}/TT_LFV_TToCMuTau_Vector ${target}/TT_LFV_TToCMuTau_Vector_18_${syst}.root &> ${target}/TT_LFV_TToCMuTau_Vector_18_${syst}.out &
#
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc17}/TT_LFV_TToCMuTau_Tensor ${target}/TT_LFV_TToCMuTau_Tensor_18_${syst}.root &> ${target}/TT_LFV_TToCMuTau_Tensor_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/TT_LFV_TToUMuTau_Scalar ${target}/TT_LFV_TToUMuTau_Scalar_18_${syst}.root &> ${target}/TT_LFV_TToUMuTau_Scalar_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/TT_LFV_TToUMuTau_Vector ${target}/TT_LFV_TToUMuTau_Vector_18_${syst}.root &> ${target}/TT_LFV_TToUMuTau_Vector_18_${syst}.out &
./processnanoaod.py --analyzer ${ana} -A -Y 18 -S ${syst} --globaltag Summer19UL18_V5 ${mc18}/TT_LFV_TToUMuTau_Tensor ${target}/TT_LFV_TToUMuTau_Tensor_18_${syst}.root &> ${target}/TT_LFV_TToUMuTau_Tensor_18_${syst}.out &
