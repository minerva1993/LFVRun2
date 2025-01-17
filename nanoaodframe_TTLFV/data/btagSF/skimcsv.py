import pandas as pd

infiles = ['reshaping_deepJet_106XUL16postVFP_v3.csv','reshaping_deepJet_106XUL16preVFP_v2.csv',
        'reshaping_deepJet_106XUL17_v3.csv','reshaping_deepJet_106XUL18_v2.csv']

for infile in infiles:
    df = pd.read_csv(infile)
    # Central, JES, HF, LF, (HF/LF)Stats(1/2)
    systs = ['central','up_jes','down_jes','up_hf','down_hf','up_lf',
            'down_lf','up_hfstats1','down_hfstats1','up_hfstats2','down_hfstats2',
            'up_lfstats1','down_lfstats1','up_lfstats2','down_lfstats2',
            'up_cferr1','down_cferr1','up_cferr2','down_cferr2']
    skimdf = df.loc[df['sysType'].isin(systs)]
    skimdf.loc[df['OperatingPoint'] == 'shape','OperatingPoint'] = 3
    print(skimdf)
    skimdf.to_csv('skimmed_'+infile,index=False)
