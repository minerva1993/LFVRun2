#include "tdrstyle.C"

void makePlot_nbc(TFile * f, TString l, TString m, TString t, float x1=0, float x2=10, float y1=0, float y2=300){

  TH1F * h_l = (TH1F*) f->Get(Form("%s", l.Data()));
  TH1F * h_m = (TH1F*) f->Get(Form("%s", m.Data()));
  TH1F * h_t = (TH1F*) f->Get(Form("%s", t.Data()));

  TCanvas * c = new TCanvas(Form("b_%s",l.Data()),"b",1);
  h_l->SetLineColor(2);
  h_m->SetLineColor(4);
  h_t->SetLineColor(8);
  h_l->SetAxisRange(0,300,"Y");
  h_m->SetAxisRange(0,300,"Y");
  h_t->SetAxisRange(0,300,"Y");
  h_l->GetXaxis()->SetTitle("Number of b-tagged Jets");
//  h_l->GetXaxis()->SetTitle("Number of c-tagged Jets");
  h_l->GetYaxis()->SetTitle("Number of Events");
  h_l->Draw("HIST");
  h_m->Draw("SameHIST");
  h_t->Draw("SameHIST");

  TLegend * l_l = new TLegend(0.1, 0.1, 0.1, 0.1);
  TLegend * l_m = new TLegend(0.1, 0.1, 0.1, 0.1);
  TLegend * l_t = new TLegend(0.1, 0.1, 0.1, 0.1);

  l_l->AddEntry(h_l,"loose","l");
  l_m->AddEntry(h_m,"medium","l");
  l_t->AddEntry(h_t,"tight","l");
  l_l->SetTextSize(0.04);
  l_m->SetTextSize(0.04);
  l_t->SetTextSize(0.04);
  l_l->SetLineColor(0);
  l_m->SetLineColor(0);
  l_t->SetLineColor(0);
  l_l->Draw();
  l_m->Draw();
  l_t->Draw();

//  c->Print(Form("ncjets_lmt.pdf"));

}

void plots_nbc(){

  setTDRStyle();

  TFile * file = new TFile("f_DeepFlav_2018.root");

  makePlot_nbc(file,"h_n_bjets_l","h_n_bjets_m","h_n_bjets_t");
//  makePlot_nbc(file,"h_n_cjets_l","h_n_cjets_m","h_n_cjets_t");

}
