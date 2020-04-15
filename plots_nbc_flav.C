#include "tdrstyle.C"

void makePlot_nb(TFile * f, TString l="h_n_bjets_l", TString m="h_n_bjets_m", TString t="h_n_bjets_t", float x1=0, float x2=10, float y1=0, float y2=300){

  TH1F * h_l = (TH1F*) f->Get(Form("%s", l.Data()));
  TH1F * h_m = (TH1F*) f->Get(Form("%s", m.Data()));
  TH1F * h_t = (TH1F*) f->Get(Form("%s", t.Data()));

  TCanvas * c = new TCanvas(Form("b_%s",l.Data()),"b",1);
  h_l->SetLineColor(2);
  h_m->SetLineColor(4);
  h_t->SetLineColor(8);
  h_l->SetAxisRange(0,350,"Y");
  h_m->SetAxisRange(0,350,"Y");
  h_t->SetAxisRange(0,350,"Y");
  h_l->Draw("HIST");
  h_m->Draw("SameHIST");
  h_t->Draw("SameHIST");

//  TLegend * l = new TLegend(x1,y1,x2,y2);
//  l->AddEntry(h_l,"l","L");
//  l->AddEntry(h_m,"m","L");
//  l->AddEntry(h_t,"t","L");
//  l->SetTextSize(0.04);
//  l->SetFillColor(0);
//  l->SetLineColor(0);
//  l->Draw();

//  c->Print(Form("ncjets_lmt.pdf"));
}
void makePlot_nc(TFile * F, TString l="h_n_cjets_l", TString m="h_n_cjets_m", TString t="h_n_cjets_t", float x1=0, float x2=10, float y1=0, float y2=300){

  TH1F * H_l = (TH1F*) F->Get(Form("%s", l.Data()));
  TH1F * H_m = (TH1F*) F->Get(Form("%s", m.Data()));
  TH1F * H_t = (TH1F*) F->Get(Form("%s", t.Data()));

  TCanvas * c = new TCanvas(Form("c_%s",l.Data()),"c",1);
  H_l->SetLineColor(2);
  H_m->SetLineColor(4);
  H_t->SetLineColor(8);
  H_l->SetAxisRange(0,350,"Y");
  H_m->SetAxisRange(0,350,"Y");
  H_t->SetAxisRange(0,350,"Y");
  H_l->Draw("HIST");
  H_m->Draw("SameHIST");
  H_t->Draw("SameHIST");
}

void plots_nbc(){

  setTDRStyle();

  TFile * file = new TFile("f.root");

  makePlot_nb(file);
  makePlot_nc(file);
}
