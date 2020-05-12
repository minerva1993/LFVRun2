#include "tdrstyle.C"

void makePlot_2D(TFile * f, TString b, TString c, TString lf){

  TH1F * h_b = (TH1F*) f->Get(Form("%s", b.Data()));
  TH1F * h_c = (TH1F*) f->Get(Form("%s", c.Data()));
  TH1F * h_lf = (TH1F*) f->Get(Form("%s", lf.Data()));

  TCanvas * C = new TCanvas(Form("c_%s",b.Data()),"c",1);
  h_b->SetMarkerColor(2);
  h_c->SetMarkerColor(8);
  h_lf->SetMarkerColor(4);
  h_b->SetMarkerStyle(kDot);//kFullDotSmall
  h_c->SetMarkerStyle(kDot);
  h_lf->SetMarkerStyle(kDot); 
  h_b->SetAxisRange(0., 1.5,"Y");
  h_c->SetAxisRange(0., 1.5,"Y");
  h_lf->SetAxisRange(0., 1.5,"Y");
  h_b->GetXaxis()->SetTitle("CvsL discriminator");
  h_b->GetYaxis()->SetTitle("CvsB discriminator");
  h_b->Draw("HIST");
  h_c->Draw("SameHIST");
  h_lf->Draw("SameHIST");

  TLegend * l_b = new TLegend(0.1, 0.1, 0.1, 0.1);
  TLegend * l_c = new TLegend(0.1, 0.1, 0.1, 0.1);
  TLegend * l_lf = new TLegend(0.1, 0.1, 0.1, 0.1);

  l_b->AddEntry(h_b,"b jets","p");
  l_c->AddEntry(h_c,"c jets","p");
  l_lf->AddEntry(h_lf,"light jets","p");
  l_b->SetTextSize(0.04);
  l_c->SetTextSize(0.04);
  l_lf->SetTextSize(0.04);
  l_b->SetLineColor(0);
  l_c->SetLineColor(0);
  l_lf->SetLineColor(0);
  l_b->Draw();
  l_c->Draw();
  l_lf->Draw();

   C->Print(Form("h_2D.pdf"));
}

void plots_2D(){

  setTDRStyle();

  TFile * file = new TFile("tt_f_DeepFlav_2018.root" );

  makePlot_2D(file,"h_ctag_2D_b","h_ctag_2D_c","h_ctag_2D_lf");
}
