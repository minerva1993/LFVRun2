#include "tdrstyle.C"

void makePlot(TFile * f_s, TFile *f_b, TString n_s,TString n_b, TString ytitle, TString xtitle, float x1, float y1, float x2, float y2){

  TH1F * h_s = (TH1F*) f_s->Get(Form("%s", n_s.Data()));
  TH1F * h_b = (TH1F*) f_b->Get(Form("%s", n_b.Data()));

  TCanvas * c = new TCanvas(Form("c_%s",n_s.Data()),"c",1);
  h_b->Sumw2();
  h_s->Sumw2();
  h_s->Scale(1.0/h_s->Integral());
  h_b->Scale(1.0/h_b->Integral());
  h_s->SetLineColor(4);
  h_b->SetLineColor(2);
	h_s->SetLineWidth(2);
	h_b->SetLineWidth(2);
  h_s->SetStats(0);
  h_s->SetTitle("");
  double max_h_b = h_b->GetMaximum();
  double max_h_s = h_s->GetMaximum();
  double max = max_h_b;
  if( max_h_s > max_h_b) max = max_h_s; 
  h_s->SetMaximum(max+0.04);
	h_s->GetXaxis()->SetTitle(Form("%s",xtitle.Data()));
	h_s->GetYaxis()->SetTitle(Form("%s",ytitle.Data()));
  h_s->Draw("E1");
  h_s->Draw("SameHist"); 
  h_b->Draw("SameE1");
  h_b->Draw("SameHist");

  TLegend * l = new TLegend(x1,y1,x2,y2);
  l->AddEntry(h_s,"LQ signal","L");
  l->AddEntry(h_b,"ttbar BG","L");
  l->SetTextSize(0.04);
  l->SetFillColor(0);
  l->SetLineColor(0);
  l->Draw();

  c->Print(Form("%s.pdf",n_s.Data()));

}


void plots_bpt(){

  setTDRStyle();

  TFile * f_s = new TFile("f_DeepFlav_2018.root");
  TFile * f_b = new TFile("tt_f_DeepFlav_2018.root");
	makePlot(f_s,f_b, "h_leading_good_b_hadron_pt","h_lead_se_good_b_hadron_pt_o2", "Normalized Entries", "eta of leading jet",  0.7, 0.75, 0.85, 0.9);
}
