from ROOT import *
import sys

importfile = sys.argv[1]
c1 = TCanvas('c1','c1',1000,1000)
gStyle.SetOptStat(kFALSE)
a = importfile.split("/")[-1]
b = a.replace(".root","")

#f = TFile('output.root','READ')
f = TFile(importfile,'READ')
t = f.Get('tree')
#t.SetLineWidth(2)

names = [x.GetName() for x in t.GetListOfLeaves()]

for i in names:
    print(i)
    c1.cd()
    if i.split('_')[0][0] == 'n':
        if i == 'nJet':
            string = i + '>>h(15,0,15)'
        else:
            string = i + '>>h(5,0,5)'
    elif i.split('_')[-1] == 'mass':
        string = i + '>>h(25,0,500)'
    elif i.split('_')[-1] == 'pt':
        string = i + '>>h(25,0,500)'
    elif i.split('_')[-1] == 'eta':
        string = i + '>>h(40,-4,4)'
    else:
        continue
    t.Draw(str(string)+'"')
    c1.Print("plot/"+b+"/hist_"+i+".pdf")

c1.cd()
t.Draw('nbJet_l>>h1(6,1,7)','')
t.Draw('nbJet_m>>h2(6,1,7)','','SAME')
t.Draw('nbJet_t>>h3(6,1,7)','','SAME')
h1.SetTitle('nbJet')
h1.SetLineColor(1)
h2.SetLineColor(2)
h3.SetLineColor(4)

leg = TLegend(0.7,0.8,0.9,0.9)
leg.AddEntry(h1,"Loose WP","l")
leg.AddEntry(h2,"Medium WP","l")
leg.AddEntry(h3,"Tight WP","l")
leg.Draw()
c1.Print('plot/'+b+'/hist_nbJet_wp.pdf')

c1.cd()
t.Draw('ncJet_l>>h4(6,1,7)','')
t.Draw('ncJet_m>>h5(6,1,7)','','SAME')
t.Draw('ncJet_t>>h6(6,1,7)','','SAME')
h4.SetTitle('ncJet')
h4.SetLineColor(1)
h5.SetLineColor(2)
h6.SetLineColor(4)
leg = TLegend(0.7,0.8,0.9,0.9)
leg.AddEntry(h4,"Loose WP","l")
leg.AddEntry(h5,"Medium WP","l")
leg.AddEntry(h6,"Tight WP","l")
leg.Draw()
c1.Print('plot/'+b+'/hist_ncJet_wp.pdf')

c1.cd()
t.Draw('nJet_blcl>>h7(6,1,7)','')
t.Draw('nJet_blcm>>h8(6,1,7)','','SAME')
t.Draw('nJet_blct>>h9(6,1,7)','','SAME')
t.Draw('nJet_bmcl>>h10(6,1,7)','','SAME')
t.Draw('nJet_bmcm>>h11(6,1,7)','','SAME')
t.Draw('nJet_bmct>>h12(6,1,7)','','SAME')
t.Draw('nJet_btcl>>h13(6,1,7)','','SAME')
t.Draw('nJet_btcm>>h14(6,1,7)','','SAME')
t.Draw('nJet_btct>>h15(6,1,7)','','SAME')
h7.SetTitle('nJet compare')

h7.SetLineColor(1)
h8.SetLineColor(1)
h9.SetLineColor(1)
h10.SetLineColor(2)
h11.SetLineColor(2)
h12.SetLineColor(2)
h13.SetLineColor(4)
h14.SetLineColor(4)
h15.SetLineColor(4)

h8.SetLineStyle(2)
h9.SetLineStyle(3)
h11.SetLineStyle(2)
h12.SetLineStyle(3)
h14.SetLineStyle(2)
h15.SetLineStyle(3)

leg = TLegend(0.7,0.5,0.9,0.9)
leg.AddEntry(h7,"b:loose, c:loose","l")
leg.AddEntry(h8,"b:loose, c:medium","l")
leg.AddEntry(h9,"b:loose, c:tight","l")
leg.AddEntry(h10,"b:medium, c:loose","l")
leg.AddEntry(h11,"b:medium, c:medium","l")
leg.AddEntry(h12,"b:medium, c:tight","l")
leg.AddEntry(h13,"b:tight, c:loose","l")
leg.AddEntry(h14,"b:tight, c:medium","l")
leg.AddEntry(h15,"b:tight, c:tight","l")
leg.Draw()


c1.Print('plot/'+b+'/hist_nJet_wp_comp.pdf')
