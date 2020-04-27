void hist_LQ(){
    TFile *f = new TFile("LQ_ana.root");
    //TFile *f = new TFile("ttbar_DeepCSV_2017.root");
    //TFile *f = new TFile("ttbar_DeepCSV_2018.root");
    //TFile *f = new TFile("ttbar_DeepJet_2016.root");
    //TFile *f = new TFile("ttbar_DeepJet_2017.root");
    //TFile *f = new TFile("ttbar_DeepJet_2018.root");

    TList *list = f->GetListOfKeys();
    list->Remove(list->FindObject("Event_category"));
    list->Remove(list->FindObject("ttbar_lf"));
    list->Remove(list->FindObject("ttbar_b"));
    list->Remove(list->FindObject("ttbar_bb"));
    list->Remove(list->FindObject("ttbar_c"));
    list->Remove(list->FindObject("ttbar_cc"));
    list->Remove(list->FindObject("signal"));

    vector<string> lf;
    vector<string> b;
    vector<string> bb;
    vector<string> c;
    vector<string> cc;
    vector<string> sig;
    vector<string> hist = {"nbjets_m", "nbjets_t", "ncjets_m", "ncjets_t", "ntaujets", "jet_pt", "bjet_m_pt", "bjet_t_pt", "cjet_m_pt", "cjet_t_pt"};
    //cout << hist.size() << endl;
    gStyle->SetOptStat(0);

    for(auto&& KeyAsObj : *list){
        auto key = (TKey*)KeyAsObj;
        const char *name = key->GetName();
        string str = name;

        if(strstr(name, "_lf_")!=NULL) lf.push_back(name);
        else if(strstr(name, "_b_")!=NULL) b.push_back(name);
        else if(strstr(name, "_bb_")!=NULL) bb.push_back(name);
        else if(strstr(name, "_c_")!=NULL) c.push_back(name);
        else if(strstr(name, "_cc_")!=NULL) cc.push_back(name);
        else if(strstr(name, "_sig_")!=NULL) sig.push_back(name);
    }
/*    for(int i=0; i < b.size(); i++){
        cout << "b=" << b[i] << endl;
        cout << "bb=" << bb[i] << endl;
        cout << "c=" << c[i] << endl;
        cout << "cc=" << cc[i] << endl;
        cout << "lf=" << lf[i] << endl;
    }*/
    for(int i = 0; i < hist.size(); i++){
        TH1F *h_lf = (TH1F*) f->Get(lf[i].c_str());
        TH1F *h_b = (TH1F*) f->Get(b[i].c_str());
        TH1F *h_bb = (TH1F*) f->Get(bb[i].c_str());
        TH1F *h_c = (TH1F*) f->Get(c[i].c_str());
        TH1F *h_cc = (TH1F*) f->Get(cc[i].c_str());
        TH1F *h_sig = (TH1F*) f->Get(sig[i].c_str());
        
        TCanvas * c = new TCanvas;
        c->SetCanvasSize(650, 650);
        c->SetWindowSize(700, 700);
        c->SetLeftMargin(0.15);
        //c->SetLogy();
        if(i == 0 || i == 1 || i == 2 || i == 3 || i == 4) h_lf->SetXTitle("Number");
        else if(i == 5 || i == 6 || i == 7 || i == 8 || i == 9 ) h_lf->SetXTitle("pT(GeV)");
        
        h_lf->SetTitle(hist[i].c_str());
        h_lf->SetYTitle("Normalized Entries");
        h_lf->SetFillColor(kBlue-10);
        h_lf->SetLineColor(kBlue-10);
        h_lf->SetLineWidth(2);
        //h_lf->SetFillStyle(3022);
        h_lf->Scale(1/h_lf->Integral());

        h_b->SetFillColor(kMagenta-2);
        h_b->SetLineColor(kMagenta-2);
        h_b->SetLineWidth(2);
        h_b->SetFillStyle(3003);
        h_b->Scale(1/h_b->Integral());

        h_bb->SetFillColor(kCyan-1);
        h_bb->SetLineColor(kCyan-1);
        h_bb->SetFillStyle(3003);
        h_bb->SetLineWidth(2);
        h_bb->Scale(1/h_bb->Integral());

        //h_c->SetFillColor(kAzure+1);
        h_c->SetLineColor(kAzure+1);
        //h_c->SetFillStyle(3003);
        h_c->SetLineWidth(2);
        h_c->Scale(1/h_c->Integral());

        //h_cc->SetFillColor(kOrange-9);
        h_cc->SetLineColor(kOrange-9);
        //h_cc->SetFillStyle(3003);
        h_cc->SetLineWidth(2);
        h_cc->Scale(1/h_cc->Integral());

        h_sig->SetLineColor(kPink-2);
        h_sig->SetLineWidth(3);
        h_sig->Scale(1/h_sig->Integral());

        Double_t maxi[] = {h_b->GetMaximum(), h_bb->GetMaximum(), h_c->GetMaximum(), h_c->GetMaximum(), h_cc->GetMaximum(), h_lf->GetMaximum(), h_sig->GetMaximum()};
        Double_t maximum = maxi[0];
        for(int i = 0; i < 7; i++){
            maximum = max(maximum, maxi[i]);
        }
        h_lf->SetMaximum(maximum*1.2);
        h_lf->Draw("hist");
        h_b->Draw("hist Same");
        h_bb->Draw("hist Same");
        h_c->Draw("hist Same");
        h_cc->Draw("hist Same");
        h_sig->Draw("hist Same");

        auto leg = new TLegend(.75,.62,.92,.92,"");
        leg->AddEntry(h_lf, "tt_lf");
        leg->AddEntry(h_b, "tt_b");
        leg->AddEntry(h_bb, "tt_bb");
        leg->AddEntry(h_c, "tt_c");
        leg->AddEntry(h_cc, "tt_cc");
        leg->AddEntry(h_sig, "signal");
        leg->SetTextSize(.04);
        leg->Draw();
        
        c->Print(Form("LQ_%s.pdf",hist[i].c_str()));
    }

}


