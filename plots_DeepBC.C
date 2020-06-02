#include "tdrstyle.C"

void makePlot_DeepB(TFile * f, TString name, TString b, TString c, TString lf){

  TH1F * h_b = (TH1F*) f->Get(Form("%s", b.Data()));
  TH1F * h_c = (TH1F*) f->Get(Form("%s", c.Data()));
  TH1F * h_lf = (TH1F*) f->Get(Form("%s", lf.Data()));

  TCanvas * C = new TCanvas(Form("c_%s",b.Data()),"c",1);
  h_b->Scale(1.0/(h_b->Integral()+h_c->Integral()+h_lf->Integral()));
  h_c->Scale(1.0/(h_b->Integral()+h_c->Integral()+h_lf->Integral()));
  h_lf->Scale(1.0/(h_b->Integral()+h_c->Integral()+h_lf->Integral()));
  h_b->SetLineColor(2);
  h_c->SetLineColor(8);
	h_lf->SetLineColor(4);
	h_b->SetLineWidth(2);
	h_c->SetLineWidth(2);
	h_lf->SetLineWidth(2);
  h_b->SetAxisRange(0,1,"X");
  h_c->SetAxisRange(0,1,"X");
  h_lf->SetAxisRange(0,1,"X");
  h_b->SetAxisRange(0.00001,10,"Y");
  h_c->SetAxisRange(0.00001,10,"Y");
  h_lf->SetAxisRange(0.00001,10,"Y");
//  h_b->GetXaxis()->SetTitle("Jet_btagDeepFlavB");
  h_b->GetXaxis()->SetTitle("Jet_btagDeepFlavC");
  h_b->GetYaxis()->SetTitle("Number of Jets");
  h_b->Draw("HIST");
  h_c->Draw("SameHIST");
  h_lf->Draw("SameHIST");

 
	TLegend * l_b = new TLegend(0.3, 0.8, 0.5, 0.9);
	TLegend * l_c = new TLegend(0.5, 0.8, 0.7, 0.9);
	TLegend * l_lf = new TLegend(0.7, 0.8, 0.9, 0.9);

	l_b->AddEntry(h_b,"b jet","l");
	l_c->AddEntry(h_c,"c jet","l");
	l_lf->AddEntry(h_lf,"light jet","l");
	l_b->SetTextSize(0.04);
	l_c->SetTextSize(0.04);
	l_lf->SetTextSize(0.04);
	l_b->SetLineColor(0);
	l_c->SetLineColor(0);
	l_lf->SetLineColor(0);
	l_b->Draw();
	l_c->Draw();
	l_lf->Draw();

	gPad->SetLogy();
	C->Print(Form("%s.pdf",name.Data()));
}

void plots_DeepB(){

  setTDRStyle();

//    TFile * file = new TFile("f_DeepFlav_2018.root");
    TFile * file = new TFile("tt_f_DeepFlav_2018.root");

//		makePlot_DeepB(file,"h_DeepB_bclf","h_DeepB_b","h_DeepB_c","h_DeepB_lf");
		makePlot_DeepB(file,"h_DeepC_bclf","h_DeepC_b","h_DeepC_c","h_DeepC_lf");
}
