/**
 *  Plot the measured energy spectra:
 *  1. (BG+source) and (source-only)
 *  2. Fit components: Gauss+sigmoid
 */

#include "TF1.h"
#include "TStyle.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TProof.h"
#include "TLine.h"
#include "TText.h"
#include "TPaveText.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

using namespace std;

#include "/home/pclg-23/volynets/.root/macros/logon.C"
// #include "/home/alex/.root/macros/logon.C"

void PlotDataSpectrum()
{
  SetStyles();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.15);
  // Channel energy for 19 channels
//   TH1F *h[19];
//   for (int i=0;i<19;i++)
//     h[i] = new TH1F(Form("h_%d",i),Form("Segment %d;Energy [MeV];Events [1/keV]",i),1600,fPeakEnergy-0.2,fPeakEnergy+0.2);

  // Reading the tree
//   TFile *f = new TFile("../out/Run06/data/Occ_data_fits_top_Co60_1.33.root");
  TFile *f = new TFile("../out/Run06/data/Occ_data_fits_noBG_top_Th228_0.58.root");
  TCanvas *c = (TCanvas*)f->Get("c1");
  int segID = 3;
  TH1F *hFull = (TH1F*)c->FindObject(Form("hFull_%d", segID));
  TH1F *hBG = (TH1F*)c->FindObject(Form("hBG_%d", segID));
  TH1F *h = (TH1F*)c->FindObject(Form("h_%d", segID));
  TCanvas *c2 = new TCanvas("c2","c2",5,5,800,600);

  hFull->GetXaxis()->SetTitleSize(0.07);
  hFull->GetXaxis()->SetTitleOffset(1.05);
  hFull->GetXaxis()->SetLabelSize(0.07);
  hFull->GetXaxis()->SetNdivisions(505);
//   hFull->GetXaxis()->SetLabelOffset(0.55);
//   hFull->GetXaxis()->SetLabelOffset(99);
//   hFull->GetXaxis()->SetTickLength(0);

  hFull->GetYaxis()->SetTitleSize(0.09);
  hFull->GetYaxis()->SetTitleOffset(0.8);
  hFull->GetYaxis()->SetLabelSize(0.07);
  hFull->GetYaxis()->SetNdivisions(505);
//   h->GetXaxis()->SetLabelSize();
//   h->GetXaxis()->SetNdivisions();
  hFull->GetYaxis()->SetRangeUser(0.5, 200);
  hFull->GetXaxis()->SetRangeUser(0.56, 0.595);
  TF1 *fun = (TF1*) h->GetListOfFunctions()->At(0);
  TF1 *fSigmoid = new TF1(*fun);
  TF1 *fGaus = new TF1(*fun);
  fGaus->SetParameter(3,0);
  fGaus->SetParameter(4,0);
  fSigmoid->SetParameter(0, 0);
  cout<<fSigmoid->GetParameter(2)<<endl;
//   cout<<fun->GetParameter(1)<<endl;


  hFull->GetYaxis()->SetTitle("Events [(1 keV)^{-1}]");
  hFull->GetXaxis()->SetTitle("Energy [MeV]");

  hFull->SetLineColor(kBlack);
  hFull->SetLineWidth(2);
  hBG->SetLineColor(kGreen);
  hBG->SetLineWidth(2);
  h->SetLineWidth(2);
  h->SetLineColor(kBlue);
  gPad->SetLogy();

  hFull->Draw("hist");
  hBG->Draw("e0same");
//   h->Draw("same");
//   gPad->RedrawAxis();


  cout << hFull->GetBinWidth(10)<<endl;
  TLegend *leg = new TLegend(0.2, 0.7, 0.6, 0.95);
  leg->SetLineColor(0);
  leg->SetFillColor(0);
  leg->AddEntry(hFull, "Measured", "l");
  leg->AddEntry(hBG, "Background", "l");
//  leg->AddEntry(h, "Subtracted", "l");
  leg->Draw("same");

  c2->SaveAs(Form("Spectra_Seg%d.eps", segID));
//   delete c2;



//   return;
  TCanvas *c3 = new TCanvas("c3","c3",5,5,800,600);
  gPad->SetLogy();
  h->SetLineWidth(2);

  h->GetXaxis()->SetTitle("Energy [MeV]");
  h->GetYaxis()->SetTitle("Events [(1 keV)^{-1}]");
  h->GetXaxis()->SetTitleSize(0.07);
  h->GetXaxis()->SetTitleOffset(1.05);
  h->GetXaxis()->SetLabelSize(0.07);
  h->GetXaxis()->SetNdivisions(505);

  h->GetYaxis()->SetTitleSize(0.09);
  h->GetYaxis()->SetTitleOffset(0.8);
  h->GetYaxis()->SetLabelSize(0.07);
  h->GetYaxis()->SetNdivisions(505);
// //   h->GetYaxis()->SetRangeUser(0.5, 200);
// //   h->GetXaxis()->SetRangeUser(0.56, 0.595);


  h->Draw("hist");
  fSigmoid->SetLineColor(kGreen);
  fSigmoid->SetLineWidth(2);
  fGaus->SetLineColor(kMagenta);
  fGaus->SetLineStyle(9);
  fGaus->SetLineWidth(2);
  fun->SetLineWidth(2);

  fSigmoid->Draw("same");
  fGaus->Draw("same");
  fun->Draw("same");
  gPad->RedrawAxis();
  TH1F *hSignal = new TH1F(*h);
/*
  hSignal->Add(fSigmoid, -1);
  hSignal->SetLineColor(kBlue);
  hSignal->Draw("same");
*/

  TLegend *leg = new TLegend(0.7, 0.7, 0.95, 0.95);
  leg->SetLineColor(0);
  leg->SetFillColor(0);
  leg->AddEntry(h, "1. Orig. spectrum", "l");
//   leg->AddEntry(hSignal, "2. Signal spectrum", "l");
  leg->AddEntry(fun, "3. Orig. fit", "l");
  leg->AddEntry(fGaus, "4. Gaus part", "l");
  leg->AddEntry(fSigmoid, "5. Sigmoid part", "l");
//   leg->Draw("same");

  //
  // Now draw the arrows to specify "A", "B", and "C" components
  // A = Gaus amplitude = par[0]
  // B = Constant = par[3]
  // C = Sigmoid amplitude = par[4]
  //

  gPad->Update();
  // Arrow coordinates
  double x1, x2, y1, y2;
  // Text "A", "B", "C"
  TText t;
  t.SetTextFont(42);
  t.SetTextSize(0.05);
  t.SetTextAlign(22);

  // Draw "A"
  x1 = fun->GetParameter(1);
//   x1 = 1.3325;
  y1 = gPad->GetUymin();
  x2 = x1;
  y2 = fGaus->GetParameter(0)/(TMath::Sqrt(2*TMath::Pi()) * fGaus->GetParameter(2));
  TArrow *arrA = new TArrow(x1, y1, x2, y2, 0.03, "|>");
  arrA->SetFillColor(kMagenta);
  arrA->SetLineColor(kMagenta);
  arrA->SetAngle(30);
  arrA->SetLineWidth(2);
  arrA->Draw();

  gPad->Update();
  TPaveText *pave = new TPaveText(x1-0.012, 10, x1-0.004, 100);
  pave->SetFillColor(0);
  pave->SetLineColor(0);
  pave->AddText("A/(#sqrt{2#pi}#sigma)");
  pave->SetTextFont(42);
  pave->SetTextSize(0.05);
  pave->SetTextAlign(11);
  pave->SetTextColor(kMagenta);
  pave->Draw("same");

  // An arrow to show what is A
  TArrow *arrHelper = new TArrow(x1-0.006, 30, x1-0.0002, 10, 0.03, "|>");
  arrHelper->SetFillColor(kMagenta);
  arrHelper->SetLineColor(kMagenta);
  arrHelper->SetAngle(30);
  arrHelper->SetLineWidth(2);
  arrHelper->Draw();
//   t.DrawText(x1, y2*1.5, "A/(#sqrt{2*#pi}#sigma)");

  // Draw "B"
  x1 = 0.565;
//   x1 = 1.35;
  y1 = gPad->GetUymin();
  x2 = x1;
  y2 = fun->GetParameter(3);
  TArrow *arrB = new TArrow(x1, y1, x2, y2, 0.03, "|>");
  arrB->SetFillColor(kBlack);
  arrB->SetLineColor(kBlack);
  arrB->SetLineWidth(2);
  arrB->SetAngle(30);
  arrB->Draw();

  t.SetTextColor(kBlack);
  t.DrawText(x1-0.001, y2*0.5, "B");

  // Draw "C"

  x1 = 0.565;
//   x1 = 1.32;
  y1 = fun->GetParameter(3);
  x2 = x1;
  y2 = fun->GetParameter(3)+fun->GetParameter(4);

  TLine *l = new TLine(gPad->GetUxmin(), y1, fun->GetParameter(1)+3*fun->GetParameter(2), y1);
  l->SetLineStyle(5);  l->SetLineColor(kBlack); l->SetLineWidth(2);
  l->Draw();
  TArrow *arrC = new TArrow(x1, y1, x2, y2, 0.03, "<|>");
  arrC->SetFillColor(kGreen);
  arrC->SetLineColor(kGreen);
  arrC->SetLineWidth(2);
  arrC->SetAngle(30);
  arrC->Draw();

  t.SetTextColor(kGreen);
  t.DrawText(x1-0.001, sqrt(y1*y2), "C");

  c3->SaveAs(Form("Spectra_Seg%d_fitcomponents.eps", segID));
  return ;

}
