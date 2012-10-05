// Copyright (C) 2009-2012
//
// Author: Oleksandr Volynets
// email:  alexjaguarvol@gmail.com
//
// This is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


#ifndef TAXISFINDER
#include "TAxisFinder.h"
#endif

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

#include "MyParameters.h"

void TAxisFinder::FillDataSpectraHistograms()
{
  /**
   * Fill the data occupancy histograms from files with energy deposits
   */

  // Channel energy for 19 channels
  TH1F *h[19];
//  for (int i=0;i<19;i++)
//    h[i] = new TH1F(Form("h_%d",i),Form("Segment %d;Energy [MeV];Events [1/keV]",i),1600,fPeakEnergy-0.2,fPeakEnergy+0.2);

  // Read the chain
  TChain *chain = new TChain("PSTree");
  chain->Add(fPathData.Data());

  Int_t nentries = chain->GetEntries();
  if (fMaxEvents>0 && fMaxEvents<nentries) nentries = fMaxEvents;
  cout<<"Processing "<<nentries<<" events at proof ..."<<endl;


  //
  // Open and fill histograms via proof - see description in the selector source file
  //
  // Feed input parameters
  MyParameters* mypars = new MyParameters("set1");
  mypars->SetPeakEnergy(fPeakEnergy);

  // Open proof locally
  TProof *p = TProof::Open("lite://");
  p->SetParallel(8); // number of jobs
  // Load input parameter: the header and parameters object
  p->Load("proof/MyParameters.h+", kTRUE);
  p->AddInput(mypars);

  // Tell root to run the chain on proof, and process it
  chain->SetProof();
  chain->Process("proof/TDataOccSelector.C+");
  //  chain->Process("proof/TDataOccSelector.C+");

  // Retrieve energy spectra from proof
  for (int iChan = 0; iChan < 19; iChan++)
  {
    h[iChan] = (TH1F*) p->GetOutputList()->FindObject(Form("h_%d",iChan));
    h[iChan]->SetLineColor(1);
    h[iChan]->GetYaxis()->SetLabelSize(0.12);
    h[iChan]->GetXaxis()->SetLabelSize(0.08);
    h[iChan]->GetXaxis()->SetNdivisions(505);
  }

  p->Close();
  delete p;

  //
  // Plot and fit the energy spectra
  //

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  // For drawing spectra
  TCanvas *c1 = new TCanvas("c1","c1",5,5,1800,1100);
//  c1->Divide(6, 4, 0.001, 0.001);
  c1->Divide(6, 3, 0.001, 0.001);

  // Plot width
  Float_t plotWidth[19];
  for (int iChan=0; iChan<19; iChan++)
  {
    // The core and segment 9 for SII have very bad energy resolution,
    // set plot ranges broader
    if (iChan==9 || iChan==0)
      plotWidth[iChan] = 0.050;  // 100 keV
    else
      plotWidth[iChan] = 0.020;  // 40 keV
  }

  for (int iChan = 0; iChan < 19; iChan++)
    h[iChan]->GetXaxis()->SetRangeUser(fPeakEnergy-plotWidth[iChan]*1.1,fPeakEnergy+plotWidth[iChan]*1.1);

  // Plot energy spectra for all segments
  for (int iChan = 1; iChan < 19; iChan++)
  {
    // Here -6 as we skip the core, which is plotted at pad no. 6 alone in
    // the first row of the canvas
    c1->cd(fCanvasPos[iChan]-6);
    gPad->SetLogy();
    gPad->SetTopMargin(0.05);
    gPad->SetLeftMargin(0.2);
    gPad->SetRightMargin(0.0);

    h[iChan]->Draw();

    // Add segment number
    TPaveText *pave = new TPaveText(0.24, 0.75, 0.5, 0.93, "NDC");
    pave->SetTextSize(0.15);
    pave->SetTextColor(4);
    pave->SetFillColor(0);
    pave->AddText(Form("%d",iChan));
    pave->Draw("same");
  }

  // Plot and save to file
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("out/%s/data/Occ_data_fits_%s_%s_%4.2f.eps", fRunName.Data(), fCase.Data(), fSourceName.Data(), fPeakEnergy));
  c1->SaveAs(Form("out/%s/data/Occ_data_fits_%s_%s_%4.2f.root", fRunName.Data(), fCase.Data(), fSourceName.Data(), fPeakEnergy));

  return ;
}
