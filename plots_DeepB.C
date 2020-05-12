#include "tdrstyle.C"

void makePlot_DeepB(TFile * f, TString b, TString c, TString lf, float x1, float y1, float x2, float y2){

  TH1F * h_b = (TH1F*) f->Get(Form("%s", b.Data()));
  TH1F * h_c = (TH1F*) f->Get(Form("%s", c.Data()));
  TH1F * h_lf = (TH1F*) f->Get(Form("%s", lf.Data()));

  TCanvas * C = new TCanvas(Form("c_%s",b.Data()),"c",1);
  h_b->Scale(1.0/(h_b->Integral()+h_c->Integral()+h_lf->Integral()));
  h_c->Scale(1.0/(h_b->Integral()+h_c->Integral()+h_lf->Integral()));
  h_lf->Scale(1.0/(h_b->Integral()+h_c->Integral()+h_lf->Integral()));
  h_b->SetLineColor(2);
  h_c->SetLineColor(4);
  h_lf->SetLineColor(8);
  h_b->SetAxisRange(0,1,"X");
  h_c->SetAxisRange(0,1,"X");
  h_lf->SetAxisRange(0,1,"X");
  h_b->SetAxisRange(0,0.5,"Y");
  h_c->SetAxisRange(0,0.5,"Y");
  h_lf->SetAxisRange(0,0.5,"Y");
  h_b->GetXaxis()->SetTitle("Jet_btagDeepFlavB");
  h_b->GetYaxis()->SetTitle("Number of Jets");
  h_b->Draw("HIST");
  h_c->Draw("SameHIST");
  h_lf->Draw("SameHIST");


  TLegend * l = new TLegend(x1,y1,x2,y2);

  l->AddEntry(h_b,"b jets","l");
  l->AddEntry(h_c,"c jets","l");
  l->AddEntry(h_lf,"light jets","l");
  l->SetTextSize(0.04);
  l->SetFillColor(0);
  l->SetLineColor(0);
  l->Draw();

}

void plots_DeepB(){

  setTDRStyle();

    TFile * file = new TFile("f_DeepFlav_2018.root");
//    TFile * file = new TFile("tt_f_DeepFlav_2018.root");

  makePlot_DeepB(file,"h_DeepB_b","h_DeepB_c","h_DeepB_lf",0.7,0.7,0.8,0.85 );
}
