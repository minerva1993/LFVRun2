#include "tdrstyle.C"

void makePlot_cpt(TFile * f, TString name, TString n, TString i, float x1, float y1, float x2, float y2){

  TH1F * h_n = (TH1F*) f->Get(Form("%s", n.Data()));
  TH1F * h_i = (TH1F*) f->Get(Form("%s", i.Data()));

  TCanvas * c = new TCanvas(Form("c_%s",n.Data()),"c",1);
	int area = h_n->Integral()+h_i->Integral();
	h_n->Sumw2();
	h_i->Sumw2();
  h_n->Scale(1.0/area);
  h_i->Scale(1.0/area);
//	h_n->Scale(1.0/h_n->Integral());
//	h_i->Scale(1.0/h_i->Integral());
  h_n->SetLineColor(2);
  h_i->SetLineColor(4);
	h_n->SetLineWidth(2);
	h_i->SetLineWidth(2);
  h_n->SetAxisRange(0,200,"X");
  h_i->SetAxisRange(0,200,"X");
  h_n->SetAxisRange(0,0.2,"Y");
  h_i->SetAxisRange(0,0.2,"Y");
  h_n->GetXaxis()->SetTitle("pT of a leading c jet");
  h_n->GetYaxis()->SetTitle("Number of Jets");
	h_n->Draw("E1");
	h_i->Draw("SameE1");
  h_n->Draw("SameHIST");
  h_i->Draw("SameHIST");


  TLegend * l = new TLegend(x1,y1,x2,y2);

  l->AddEntry(h_n,"Events not including c from W","l");
  l->AddEntry(h_i,"Events including c from W","l");
  l->SetTextSize(0.04);
  l->SetFillColor(0);
  l->SetLineColor(0);
  l->Draw();

	c->Print(Form("%s.pdf",name.Data()));
}

void plots_cpt(){

  setTDRStyle();

    TFile * S = new TFile("f_DeepFlav_2018.root");
    TFile * BG = new TFile("tt_f_DeepFlav_2018.root");

  //  makePlot_cpt(S,"h_leading_good_cjet_W_S","h_leading_good_c_hadron_nfW_pt","h_leading_good_c_hadron_ifW_pt",0.35,0.75,0.65,0.9 );	
		makePlot_cpt(BG,"h_leading_good_cjet_W_BG","h_leading_good_c_hadron_nfW_pt","h_leading_good_c_hadron_ifW_pt",0.35,0.75,0.65,0.9 );
}
