#include "tdrstyle.C"

void makePlot(TFile * f_s, TFile *f_b, TString name, TString ytitle, TString xtitle, float x1, float y1, float x2, float y2){

  TH1F * h_s = (TH1F*) f_s->Get(Form("%s", name.Data()));
  TH1F * h_b = (TH1F*) f_b->Get(Form("%s", name.Data()));

  TCanvas * c = new TCanvas(Form("c_%s",name.Data()),"c",1);
//  h_b->Sumw2();
//  h_s->Sumw2();
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
//  h_s->Draw("E1");
//  h_s->Draw("SameHist"); 
  h_s->Draw("Hist");
// h_b->Draw("SameE1");
  h_b->Draw("SameHist");

  TLegend * l = new TLegend(x1,y1,x2,y2);
  l->AddEntry(h_s,"LQ signal","L");
  l->AddEntry(h_b,"ttbar BG","L");
  l->SetTextSize(0.04);
  l->SetFillColor(0);
  l->SetLineColor(0);
  l->Draw();

  c->Print(Form("%s.pdf",name.Data()));

}


void plots(){

  setTDRStyle();

  TFile * f_signal = new TFile("f_DeepFlav_2018.root");
  TFile * f_ttbar = new TFile("tt_f_DeepFlav_2018.root");

//  TFile * f_signal = new TFile("f_DeepFlav_2018_bcut.root");
//  TFile * f_ttbar = new TFile("tt_f_DeepFlav_2018.root");
/*
  makePlot(f_signal, f_ttbar, "h_n_selmuon", "Normalized Entries","Muon Jet Multiplicity", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_seltau", "Normalized Entries","Tau Jet Multiplicity", 0.7, 0.75, 0.85, 0.9);*/
  makePlot(f_signal, f_ttbar, "h_n_bjet_l", "Normalized Entries","b Jet Multiplicity (L)", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_bjet_m", "Normalized Entries","b Jet Multiplicity (M)", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_bjet_t", "Normalized Entries","b Jet Multiplicity (T)", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_cjet_l", "Normalized Entries","c Jet Multiplicity (L)", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_cjet_m", "Normalized Entries","c Jet Multiplicity (M)", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_cjet_t", "Normalized Entries","c Jet Multiplicity (T)", 0.7, 0.75, 0.85, 0.9); 
 
//  makePlot(f_signal, f_ttbar, "h_goodhadron_Flav", "Normalized Entries", "Jet Flavour (lf,c,b:0,4,5)", 0.7, 0.75, 0.85, 0.9); 
  makePlot(f_signal, f_ttbar, "h_n_good_b_hadron", "Normalized Entries", "Number of b jets", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_good_c_hadron", "Normalized Entries", "Number of c jets", 0.7, 0.75, 0.85, 0.9);/*
  makePlot(f_signal, f_ttbar, "h_n_good_lf_hadron", "Normalized Entries", "Number of lf jets", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_goodjets", "Normalized Entries", "Number of good jets", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_leading_good_b_hadron_pt", "Normalized Entries","pT of leading b jet", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_leading_good_c_hadron_pt", "Normalized Entries","pT of leading c jet", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_leading_good_lf_hadron_pt", "Normalized Entries","pT of leading lf jet", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_leading_goodjet_pt", "Normalized Entries","pT of leading jet", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_leading_btagged_jet_l_pt", "Normalized Entries","pT of leading loose b-tagged jet", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_leading_btagged_jet_m_pt", "Normalized Entries","pT of leading medium b-tagged jet", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_leading_btagged_jet_t_pt", "Normalized Entries","pT of leading tight b-tagged jet", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_leading_ctagged_jet_l_pt", "Normalized Entries","pT of leading loose c-tagged jet", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_leading_ctagged_jet_m_pt", "Normalized Entries","pT of leading medium c-tagged jet", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_leading_ctagged_jet_t_pt", "Normalized Entries","pT of leading tight c-tagged jet", 0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_leading_good_b_cfW_ptsum", "Normalized Entries","pT Sum of leading b jet and leading c jet from W", 0.7, 0.75, 0.85, 0.9);
	
	makePlot(f_signal, f_ttbar, "h_leading_good_b_hadron_eta", "Normalized Entries", "eta of leading b jet",  0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_leading_good_c_hadron_eta", "Normalized Entries", "eta of leading c jet",  0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_leading_good_lf_hadron_eta", "Normalized Entries", "eta of leading lf jet",  0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_leading_goodjet_eta", "Normalized Entries", "eta of leading jet",  0.7, 0.75, 0.85, 0.9);*/
}
