#include "tdrstyle.C"

void makePlot_pt_lmt(TFile * f,TString name, TString l, TString m, TString t, TString xt,  float x1, float y1, float x2, float y2){

  TH1F * h_l = (TH1F*) f->Get(Form("%s", l.Data()));
  TH1F * h_m = (TH1F*) f->Get(Form("%s", m.Data()));
  TH1F * h_t = (TH1F*) f->Get(Form("%s", t.Data()));

  TCanvas * c = new TCanvas(Form("c_%s",l.Data()),"c",1);
  h_l->Scale(1.0/(h_l->Integral()));
  h_m->Scale(1.0/(h_m->Integral()));
  h_t->Scale(1.0/(h_t->Integral()));
  h_l->SetLineColor(2);
  h_m->SetLineColor(4);
  h_t->SetLineColor(8);
  h_l->SetAxisRange(0,220,"X");
  h_m->SetAxisRange(0,220,"X");
  h_t->SetAxisRange(0,220,"X");
  h_l->SetAxisRange(0,0.15,"Y");
  h_m->SetAxisRange(0,0.15,"Y");
  h_t->SetAxisRange(0,0.15,"Y");
  h_l->GetXaxis()->SetTitle(Form("pT of leading %s-tagged jet",xt.Data()));
  h_l->GetYaxis()->SetTitle("Number of Jets");
  h_l->Draw("HIST");
  h_m->Draw("SameHIST");
  h_t->Draw("SameHIST");


  TLegend * L = new TLegend(x1,y1,x2,y2);

  L->AddEntry(h_l,"loose","l");
  L->AddEntry(h_m,"medium","l");
  L->AddEntry(h_t,"tight","l");
  L->SetTextSize(0.04);
  L->SetFillColor(0);
  L->SetLineColor(0);
  L->Draw();
  
	c->Print(Form("%s.pdf",name.Data()));
}

void plots_pt_lmt(){

  setTDRStyle();

  TFile * S = new TFile("f_DeepFlav_2018.root"); 
  TFile * BG = new TFile("tt_f_DeepFlav_2018.root");

  makePlot_pt_lmt(S,"h_leading_btagged_jet_lmt_pt_S","h_leading_btagged_jet_l_pt","h_leading_btagged_jet_m_pt","h_leading_btagged_jet_t_pt","b",0.7,0.7,0.8,0.85 );
	makePlot_pt_lmt(BG,"h_leading_btagged_jet_lmt_pt_BG","h_leading_btagged_jet_l_pt","h_leading_btagged_jet_m_pt","h_leading_btagged_jet_t_pt","b",0.7,0.7,0.8,0.85 );
  makePlot_pt_lmt(S,"h_leading_ctagged_jet_lmt_pt_S","h_leading_ctagged_jet_l_pt","h_leading_ctagged_jet_m_pt","h_leading_ctagged_jet_t_pt","c",0.7,0.7,0.8,0.85 );    
	makePlot_pt_lmt(BG,"h_leading_ctagged_jet_lmt_pt_BG","h_leading_ctagged_jet_l_pt","h_leading_ctagged_jet_m_pt","h_leading_ctagged_jet_t_pt","c",0.7,0.7,0.8,0.85 );
}
