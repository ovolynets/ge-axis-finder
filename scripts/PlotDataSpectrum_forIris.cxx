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

void PlotDataSpectrum_forIris()
{
  SetStyles();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  // Channel energy for 19 channels
//   TH1F *h[19];
//   for (int i=0;i<19;i++)
//     h[i] = new TH1F(Form("h_%d",i),Form("Segment %d;Energy [MeV];Events [1/keV]",i),1600,fPeakEnergy-0.2,fPeakEnergy+0.2);

  // Reading the tree
//   TFile *f = new TFile("../out/Run06_BG/data/Occ_data_fits_top_Th228_2.61.root");
//   TCanvas *c = (TCanvas*)f->Get("c1");
//   TH1F *h = (TH1F*)c->FindObject("h_15");

  TFile *f = new TFile("h15.root");
  TH1F *h = (TH1F*)f->Get("h15");

  TCanvas *c2 = new TCanvas("c2","c2",5,5,800,600);

  h->GetYaxis()->SetLabelSize();
  h->GetXaxis()->SetLabelSize();
//   h->GetXaxis()->SetNdivisions(505);
  h->GetXaxis()->SetNdivisions();
//   h->GetYaxis()->SetRangeUser(0,4000);
  TF1 *fun = (TF1*) h->GetListOfFunctions()->At(0);
  delete fun;

  h->GetYaxis()->SetTitle("Entries");
  h->GetXaxis()->SetTitle("Energy [MeV]");

//   h->Rebin(1);
  h->GetXaxis()->SetRangeUser(0., 3.);
  h->GetYaxis()->SetRangeUser(0.5, 100000);

  h->Draw();

  gPad->SetLogy();

  c2->SaveAs("h15.eps");
  return ;

}
