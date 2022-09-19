nohup bash -c 'source /home/itseyes/github/LFVRun2_ndf_integration/nanoaodframe/scripts/processmc.sh stlfv btagup_hf' > out_mc_btagup_hf &
sleep 30m
nohup bash -c 'source /home/itseyes/github/LFVRun2_ndf_integration/nanoaodframe/scripts/processmc.sh stlfv btagup_lf' > out_mc_btagup_lf &
sleep 30m
nohup bash -c 'source /home/itseyes/github/LFVRun2_ndf_integration/nanoaodframe/scripts/processmc.sh stlfv btagdown_hf' > out_mc_btagdown_hf &
sleep 30m
nohup bash -c 'source /home/itseyes/github/LFVRun2_ndf_integration/nanoaodframe/scripts/processmc.sh stlfv btagdown_lf' > out_mc_btagdown_lf &
sleep 30m
nohup bash -c 'source /home/itseyes/github/LFVRun2_ndf_integration/nanoaodframe/scripts/processmc.sh stlfv btagdown_jes' > out_mc_btagdown_jes &
sleep 30m
