#include "tdrstyle.C"

void makePlot(TFile * f_s, TFile * f_b1, TFile * f_b2,  TString name, TString ytitle, TString xtitle, float x1, float y1, float x2, float y2){

  TH1F * h_s = (TH1F*) f_s->Get(Form("%s", name.Data()));
  TH1F * h_b1 = (TH1F*) f_b1->Get(Form("%s", name.Data()));
  TH1F * h_b2 = (TH1F*) f_b2->Get(Form("%s", name.Data()));

  TCanvas * c = new TCanvas(Form("c_%s",name.Data()),"c",1);
  h_s->Sumw2();
  h_b1->Sumw2();
  h_b2->Sumw2();
  h_s->Scale(1.0/h_s->Integral());
  h_b1->Scale(1.0/h_b1->Integral());
  h_b2->Scale(1.0/h_b2->Integral());
  h_s->SetLineColor(4);
  h_b1->SetLineColor(2);
  h_b2->SetLineColor(8);
	h_s->SetLineWidth(2);
	h_b1->SetLineWidth(2);
	h_b2->SetLineWidth(2);
  h_s->SetStats(0);
  h_s->SetTitle("");
  double max_h_s = h_s->GetMaximum();
  double max_h_b1 = h_b1->GetMaximum();
  double max_h_b2 = h_b2->GetMaximum();
  double max = max_h_b1;
  if( max_h_s > max_h_b1) max = max_h_s;
	if( max_h_b2 > max_h_s) max = max_h_b2;
  h_s->SetMaximum(max+0.2);
	h_s->GetXaxis()->SetTitle(Form("%s",xtitle.Data()));
	h_s->GetYaxis()->SetTitle(Form("%s",ytitle.Data()));
  h_s->Draw("E");
  h_s->Draw("SameHist"); 
//  h_s->Draw("Hist");
	h_b1->Draw("SameE");
  h_b1->Draw("SameHist");
	h_b2->Draw("SameE");
  h_b2->Draw("SameHist");

  TLegend * l = new TLegend(x1,y1,x2,y2);
  l->AddEntry(h_s,"LQ signal","L");
  l->AddEntry(h_b1,"Semileptonic ttbar","L");
  l->AddEntry(h_b2,"Dileptonic ttbar","L");
  l->SetTextSize(0.04);
  l->SetFillColor(0);
  l->SetLineColor(0);
  l->Draw();

  c->Print(Form("%s.pdf",name.Data()));

}


void plots(){

  setTDRStyle();

  TFile * f_signal = new TFile("f_DeepFlav_2018.root");
  TFile * f_semi = new TFile("Semitt_f_DeepFlav_2018.root");
  TFile * f_di = new TFile("Ditt_f_DeepFlav_2018.root");

/*
  makePlot(f_signal, f_ttbar, "h_n_selmuon", "Normalized Entries","Muon Jet Multiplicity", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_seltau", "Normalized Entries","Tau Jet Multiplicity", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_bjet_l", "Normalized Entries","b Jet Multiplicity (L)", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_bjet_m", "Normalized Entries","b Jet Multiplicity (M)", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_bjet_t", "Normalized Entries","b Jet Multiplicity (T)", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_cjet_l", "Normalized Entries","c Jet Multiplicity (L)", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_cjet_m", "Normalized Entries","c Jet Multiplicity (M)", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_cjet_t", "Normalized Entries","c Jet Multiplicity (T)", 0.7, 0.75, 0.85, 0.9); 
 
//  makePlot(f_signal, f_ttbar, "h_goodhadron_Flav", "Normalized Entries", "Jet Flavour (lf,c,b:0,4,5)", 0.7, 0.75, 0.85, 0.9); 
  makePlot(f_signal, f_ttbar, "h_n_good_b_hadron", "Normalized Entries", "Number of b jets", 0.7, 0.75, 0.85, 0.9);
  makePlot(f_signal, f_ttbar, "h_n_good_c_hadron", "Normalized Entries", "Number of c jets", 0.7, 0.75, 0.85, 0.9);
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
/*
	makePlot(f_signal, f_ttbar, "h_chi2_qq", "Normalized Entries", "minimum chi square(qq)",  0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_LQtop_qq_invm", "Normalized Entries", "Invariant mass of LQtop(qq)",  0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_SMW_qq_invm", "Normalized Entries", "Invariant mass of SMW(qq)",  0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_SMtop_qq_invm", "Normalized Entries", "Invariant mass of SMtop(qq)",  0.7, 0.75, 0.85, 0.9);

	makePlot(f_signal, f_ttbar, "h_chi2_enu", "Normalized Entries", "minimum chi square(enu)",  0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_LQtop_enu_invm", "Normalized Entries", "Invariant mass of LQtop(enu)",  0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_SMW_enu_invm", "Normalized Entries", "Invariant mass of SMW(enu)",  0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_SMtop_enu_invm", "Normalized Entries", "Invariant mass of SMtop(enu)",  0.7, 0.75, 0.85, 0.9);
	
	makePlot(f_signal, f_ttbar, "h_chi2_munu", "Normalized Entries", "minimum chi square(munu)",  0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_LQtop_munu_invm", "Normalized Entries", "Invariant mass of LQtop(munu)",  0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_SMW_munu_invm", "Normalized Entries", "Invariant mass of SMW(munu)",  0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_SMtop_munu_invm", "Normalized Entries", "Invariant mass of SMtop(munu)",  0.7, 0.75, 0.85, 0.9);

	makePlot(f_signal, f_ttbar, "h_chi2_taunu", "Normalized Entries", "minimum chi square(taunu)",  0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_LQtop_taunu_invm", "Normalized Entries", "Invariant mass of LQtop(taunu)",  0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_SMW_taunu_invm", "Normalized Entries", "Invariant mass of SMW(taunu)",  0.7, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_ttbar, "h_SMtop_taunu_invm", "Normalized Entries", "Invariant mass of SMtop(taunu)",  0.7, 0.75, 0.85, 0.9); */

	makePlot(f_signal, f_semi, f_di, "h_chi2_qq", "Normalized Entries", "minimum chi square(qq)",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_LQtop_qq_invm", "Normalized Entries", "Invariant mass of LQtop(qq)",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMW_qq_invm", "Normalized Entries", "Invariant mass of SMW(qq)",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMtop_qq_invm", "Normalized Entries", "Invariant mass of SMtop(qq)",  0.55, 0.75, 0.85, 0.9);

	makePlot(f_signal, f_semi, f_di, "h_chi2_enu", "Normalized Entries", "minimum chi square(enu)",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_LQtop_enu_invm", "Normalized Entries", "Invariant mass of LQtop(enu)",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMW_enu_invm", "Normalized Entries", "Invariant mass of SMW(enu)",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMtop_enu_invm", "Normalized Entries", "Invariant mass of SMtop(enu)",  0.55, 0.75, 0.85, 0.9);
	
	makePlot(f_signal, f_semi, f_di, "h_chi2_munu", "Normalized Entries", "minimum chi square(munu)",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_LQtop_munu_invm", "Normalized Entries", "Invariant mass of LQtop(munu)",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMW_munu_invm", "Normalized Entries", "Invariant mass of SMW(munu)",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMtop_munu_invm", "Normalized Entries", "Invariant mass of SMtop(munu)",  0.55, 0.75, 0.85, 0.9);

	makePlot(f_signal, f_semi, f_di, "h_chi2_taunu", "Normalized Entries", "minimum chi square(taunu)",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_LQtop_taunu_invm", "Normalized Entries", "Invariant mass of LQtop(taunu)",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMW_taunu_invm", "Normalized Entries", "Invariant mass of SMW(taunu)",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMtop_taunu_invm", "Normalized Entries", "Invariant mass of SMtop(taunu)",  0.55, 0.75, 0.85, 0.9);


	makePlot(f_signal, f_semi, f_di, "h_chi2_qq_cut3", "Normalized Entries", "minimum chi square(qq) after cut",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_LQtop_qq_invm_cut3", "Normalized Entries", "Invariant mass of LQtop(qq) after cut",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMW_qq_invm_cut3", "Normalized Entries", "Invariant mass of SMW(qq) after cut",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMtop_qq_invm_cut3", "Normalized Entries", "Invariant mass of SMtop(qq) after cut",  0.55, 0.75, 0.85, 0.9);

	makePlot(f_signal, f_semi, f_di, "h_chi2_enu_cut3", "Normalized Entries", "minimum chi square(enu) after cut",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_LQtop_enu_invm_cut3", "Normalized Entries", "Invariant mass of LQtop(enu) after cut",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMW_enu_invm_cut3", "Normalized Entries", "Invariant mass of SMW(enu) after cut",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMtop_enu_invm_cut3", "Normalized Entries", "Invariant mass of SMtop(enu) after cut",  0.55, 0.75, 0.85, 0.9);
	
	makePlot(f_signal, f_semi, f_di, "h_chi2_munu_cut3", "Normalized Entries", "minimum chi square(munu) after cut",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_LQtop_munu_invm_cut3", "Normalized Entries", "Invariant mass of LQtop(munu) after cut",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMW_munu_invm_cut3", "Normalized Entries", "Invariant mass of SMW(munu) after cut",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMtop_munu_invm_cut3", "Normalized Entries", "Invariant mass of SMtop(munu) after cut",  0.55, 0.75, 0.85, 0.9);

	makePlot(f_signal, f_semi, f_di, "h_chi2_taunu_cut3", "Normalized Entries", "minimum chi square(taunu) after cut",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_LQtop_taunu_invm_cut3", "Normalized Entries", "Invariant mass of LQtop(taunu) after cut",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMW_taunu_invm_cut3", "Normalized Entries", "Invariant mass of SMW(taunu) after cut",  0.55, 0.75, 0.85, 0.9);
	makePlot(f_signal, f_semi, f_di, "h_SMtop_taunu_invm_cut3", "Normalized Entries", "Invariant mass of SMtop(taunu) after cut",  0.55, 0.75, 0.85, 0.9);

}
