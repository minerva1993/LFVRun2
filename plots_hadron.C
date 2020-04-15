#include "tdrstyle.C"

void makePlot_hadron(TFile * f, TString b, TString c, TString lf, float x1=0, float x2=10, float y1=0, float y2=300){

  TH1F * h_b = (TH1F*) f->Get(Form("%s", b.Data()));
  TH1F * h_c = (TH1F*) f->Get(Form("%s", c.Data()));
  TH1F * h_lf = (TH1F*) f->Get(Form("%s", lf.Data()));

  TCanvas * C = new TCanvas(Form("c_%s",b.Data()),"c",1);
  h_b->SetLineColor(2);
  h_c->SetLineColor(4);
  h_lf->SetLineColor(8);
  h_b->SetAxisRange(0,1,"X");
  h_c->SetAxisRange(0,1,"X");
  h_lf->SetAxisRange(0,1,"X");
  h_b->SetAxisRange(0,300,"Y");
  h_c->SetAxisRange(0,300,"Y");
  h_lf->SetAxisRange(0,300,"Y");
  h_b->Draw("HIST");
  h_c->Draw("SameHIST");
  h_lf->Draw("SameHIST");

//  TLegend * l = new TLegend(x1,y1,x2,y2);
//  l->AddEntry(h_l,"l","L");
//  l->AddEntry(h_m,"m","L");
//  l->AddEntry(h_t,"t","L");
//  l->SetTextSize(0.04);
//  l->SetFillColor(0);
//  l->SetLineColor(0);
//  l->Draw();

//   C->Print(Form("hadron.pdf"));
}

void plots_hadron(){

  setTDRStyle();

  TFile * file = new TFile("f_flav.root");

  makePlot_hadron(file,"h_Jet_btagDeepB_b","h_Jet_btagDeepB_c","h_Jet_btagDeepB_lf");
  makePlot_hadron(file,"h_Jet_btagDeepC_b","h_Jet_btagDeepC_c","h_Jet_btagDeepC_lf");
  makePlot_hadron(file,"h_Jet_btagDeepLF_b","h_Jet_btagDeepLF_c","h_Jet_btagDeepLF_lf");
  makePlot_hadron(file,"h_CvsB_b","h_CvsB_c","h_CvsB_lf");
  makePlot_hadron(file,"h_CvsL_b","h_CvsL_c","h_CvsL_lf");
}
