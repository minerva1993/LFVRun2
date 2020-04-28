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
  h_b->SetAxisRange(0,200,"Y");
  h_c->SetAxisRange(0,200,"Y");
  h_lf->SetAxisRange(0,200,"Y");
  h_b->GetXaxis()->SetTitle("Jet_btagDeepB");
  h_b->GetYaxis()->SetTitle("Number of Jets");
  h_b->Draw("HIST");
  h_c->Draw("SameHIST");
  h_lf->Draw("SameHIST");

//  l->AddEntry(h_l,"l","L");
//  l->AddEntry(h_m,"m","L");
//  l->AddEntry(h_t,"t","L");
//  l->SetTextSize(0.04);
//  l->SetFillColor(0);
//  l->SetLineColor(0);
//  l->Draw();

//   C->Print(Form("hadron.pdf"));

  TLegend * l_b = new TLegend(0.1, 0.1, 0.1, 0.1);
  TLegend * l_c = new TLegend(0.1, 0.1, 0.1, 0.1);
  TLegend * l_lf = new TLegend(0.1, 0.1, 0.1, 0.1);

  l_b->AddEntry(h_b,"b jets","l");
  l_c->AddEntry(h_c,"c jets","l");
  l_lf->AddEntry(h_lf,"light jets","l");
  l_b->SetTextSize(0.04);
  l_c->SetTextSize(0.04);
  l_lf->SetTextSize(0.04);
  l_b->SetLineColor(0);
  l_c->SetLineColor(0);
  l_lf->SetLineColor(0);
  l_b->Draw();
  l_c->Draw();
  l_lf->Draw();


}

void plots_hadron(){

  setTDRStyle();

  TFile * file = new TFile("f_DeepCSV_2018.root");

  makePlot_hadron(file,"h_DeepB_b","h_DeepB_c","h_DeepB_lf");
}
