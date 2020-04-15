void histo_ttbar(){
    TFile *f = new TFile("ttbar_DeepCSV_2016.root");
    //TFile *f = new TFile("ttbar_DeepCSV_102x.root");
    //TFile *f = new TFile("ttbar_DeepJet_2016.root");
    //TFile *f = new TFile("ttbar_DeepJet_102x.root");

    TList *list = f->GetListOfKeys();
    vector<string> b;
    vector<string> bb;
    vector<string> c;
    vector<string> cc;
    vector<string> lf;
    vector<string> name = {"nbjets", "ncjets", "ntaujets"};

    gStyle->SetOptStat(0);

    for(auto&& KeyAsObj : *list){
        auto key = (TKey*)KeyAsObj;
        const char *name = key->GetName();
        string str = name;

        if(strstr(name, "ttbar_b_")!=NULL) b.push_back(name);
        else if(strstr(name, "ttbar_bb_")!=NULL) bb.push_back(name);
        else if(strstr(name, "ttbar_c_")!=NULL) c.push_back(name);
        else if(strstr(name, "ttbar_cc_")!=NULL) cc.push_back(name);
        else if(strstr(name, "ttbar_lf_")!=NULL) lf.push_back(name);
    }
/*    for(int i=0; i<3; i++){
        cout << "b=" << b[i] << endl;
        cout << "bb=" << bb[i] << endl;
        cout << "c=" << c[i] << endl;
        cout << "cc=" << cc[i] << endl;
        cout << "lf=" << lf[i] << endl;
    }*/
    for(int i = 0; i < 3; i++){
        TH1F *h_b = (TH1F*) f->Get(b[i].c_str());
        TH1F *h_bb = (TH1F*) f->Get(bb[i].c_str());
        TH1F *h_c = (TH1F*) f->Get(c[i].c_str());
        TH1F *h_cc = (TH1F*) f->Get(cc[i].c_str());
        TH1F *h_lf = (TH1F*) f->Get(lf[i].c_str());
        
        TCanvas * c = new TCanvas;
        c->SetCanvasSize(600, 600);
        c->SetWindowSize(630, 630);
        //c->SetLogy();
        if(i == 0) h_b->SetXTitle("Number of b-jets");
        else if(i == 1 ) h_b->SetXTitle("Number of c-jets");
        else h_b->SetXTitle("Number of tau-jets");

        h_b->SetTitle(name[i].c_str());
        h_b->SetYTitle("Nornalized Entries");
        h_b->SetFillColor(kBlue-10);
        h_b->SetLineColor(kBlue-10);
        h_b->SetLineWidth(2);
        //h_b->SetFillStyle(3022);
        h_b->Scale(1/h_b->Integral());

        h_bb->SetFillColor(kMagenta-2);
        h_bb->SetLineColor(kMagenta-2);
        h_bb->SetFillStyle(3003);
        h_bb->SetLineWidth(2);
        h_bb->Scale(1/h_bb->Integral());

        h_c->SetFillColor(kCyan-1);
        h_c->SetLineColor(kCyan-1);
        h_c->SetFillStyle(3003);
        h_c->SetLineWidth(2);
        h_c->Scale(1/h_c->Integral());

        //h_cc->SetFillColor(kRed-4);
        h_cc->SetLineColor(kRed-4);
        //h_cc->SetFillStyle(3003);
        h_cc->SetLineWidth(2);
        h_cc->Scale(1/h_cc->Integral());

        //h_lf->SetFillColor(kMagenta+4);
        h_lf->SetLineColor(kBlue-3);
        //h_lf->SetFillStyle(3944);
        h_lf->SetLineWidth(2);
        h_lf->Scale(1/h_lf->Integral());
        
        Double_t maxi[] = {h_b->GetMaximum(), h_bb->GetMaximum(), h_c->GetMaximum(), h_c->GetMaximum(), h_cc->GetMaximum(), h_lf->GetMaximum()};
        Double_t maximum = maxi[0];
        for(int i = 0; i < 5; i++){
            maximum = max(maximum, maxi[i]);
        }
        h_b->SetMaximum(maximum*1.2);
        h_b->Draw("hist");
        h_bb->Draw("hist Same");
        h_c->Draw("hist Same");
        h_cc->Draw("hist Same");
        h_lf->Draw("hist Same");

        auto leg = new TLegend(.8,.72,.92,.92,"");
        leg->AddEntry(h_b, "b");
        leg->AddEntry(h_bb, "bb");
        leg->AddEntry(h_c, "c");
        leg->AddEntry(h_cc, "cc");
        leg->AddEntry(h_lf, "lf");
        leg->SetTextSize(.04);
        leg->Draw();
        
        c->Print(Form("DeepCSV2016_%s.pdf",name[i].c_str()));
    }

}


