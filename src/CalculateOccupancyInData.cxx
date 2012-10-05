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

void TAxisFinder::CalculateOccupancyInData()
{
  /**
   * Fit segment energy spectra and calculate the occupancy.
   * If specified, the background is subtracted according to the
   * livetimes of the (BG+source) and (BG alone) measurements
   */

  // Read data histograms
  TFile *fileData = new TFile(Form("out/%s/data/Occ_data_fits_%s_%s_%4.2f.root", fRunName.Data(), fCase.Data(), fSourceName.Data(), fPeakEnergy ), "read");

  // Channel energy for 19 channels
  TH1F *h[19];

  TCanvas *cFits = (TCanvas* )fileData->Get("c1");
  // Retrieve histograms from the canvas
  for (int iChan = 1; iChan < 19; iChan++)
  {
    h[iChan] = (TH1F*)cFits->FindObject(Form("h_%d",iChan));
    h[iChan]->SetLineColor(1);
    h[iChan]->GetYaxis()->SetLabelSize(0.12);
    h[iChan]->GetXaxis()->SetLabelSize(0.08);
    h[iChan]->GetXaxis()->SetNdivisions(505);
  }

  // Subtract background if specified
  double livetimeSig[19] = {0};
  double livetimeBG[19] = {0};
  TH1F *hBG[19];
  if (fPathBG != "") //<--- here is the check if background has to be subtracted
  {
    // Read the livetimes from the corresponding text files
    ifstream fileLTSig(Form("livetime/%s_%s_livetime.txt", fRunName.Data(), fSourceName.Data()) );
    ifstream fileLTBG(Form("livetime/%s_background_livetime.txt", fRunName.Data()) );
    // Read the background energy spectra
    TFile *fileBG = new TFile(Form("%s/Occ_data_fits_%s_%s_%4.2f.root", fPathBG.Data(), fCase.Data(), fSourceName.Data(), fPeakEnergy ), "read");
    TCanvas *cFitsBG = (TCanvas* )fileBG->Get("c1");
    int tt;
    // Skip first line that corresponds to the core in both files - we don't use core in the analysis
    char temp[256];
    fileLTSig.getline(temp, 256);
    fileLTBG.getline(temp, 256);
    for (int iChan = 1; iChan < 19; iChan++)
    {
      // Read the livetimes for each segment and extract the BG histograms
      fileLTSig >> tt >> livetimeSig[iChan];
      fileLTBG  >> tt >> livetimeBG[iChan];
      cout<<"Live time ratio "<<livetimeSig[iChan]/livetimeBG[iChan]<<endl;
      hBG[iChan] = (TH1F*)cFitsBG->FindObject(Form("h_%d",iChan));
      hBG[iChan] ->SetName(Form("hBG_%d",iChan));
    }
  }

  //
  // Fit the spectra and extract parameters, taking into account background if enede
  //
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  // For fitting spectrum and obtaining occupancy
  TCanvas *c1 = new TCanvas("c1","c1",5,5,1800,1100);
  c1->Divide(6, 3, 0.001, 0.001);

  // Temporary variable for fit names
  TString f_name;

  // Fit width
  Float_t fitWidth[19];
  // Gauss-fit functions for the peaks
  TF1 *f[19];
  for (int iChan=1; iChan<19; iChan++)
  {
    f_name = Form("fgaus_%d",iChan);

    // Segment 9 and core in SII have bad energy resolution - broaden the fit ranges
    if ( (iChan==9 || iChan==0) && (fPeakEnergy>0.7) ) // To avoid inclusion of 0.6 MeV line into segment 9
      fitWidth[iChan] = 0.050;  // 100 keV
    else
      fitWidth[iChan] = 0.020;  // 40 keV

    // Define the fit function
    f[iChan] = new TF1(f_name,"gausn(0)+[3]+[4]/(exp(2*(x-[1])/[2])+1)",fPeakEnergy-fitWidth[iChan],fPeakEnergy+fitWidth[iChan]);
    f[iChan]->SetNpx(500);
    f[iChan]->SetLineColor(2);
    f[iChan]->SetLineWidth(1);
  }

  // Find the maximum value in the ranges about fPeakEnergy
  // Used to plot all the histograms with the same scale
  float currMaxContent = -100;
  for (int iChan = 1; iChan < 19; iChan++)
  {
    h[iChan]->Rebin(3);
    h[iChan]->GetXaxis()->SetRangeUser(fPeakEnergy-fitWidth[iChan]*1.1,fPeakEnergy+fitWidth[iChan]*1.1);
    if (h[iChan]->GetMaximum() > currMaxContent && iChan>0 )
      currMaxContent = h[iChan]->GetMaximum();
  }

  // Fit parameters
  Double_t pars[19][5];
//  Double_t par_errors[19][5];

  Float_t sum[19];        // Overall sum in the peak (from the fitting, not summing!!)
  Float_t sum_error[19];  // Sum error
  // Output text file
  ofstream streamout(Form("results/Occ_pars_%s_%4.2f_%s.txt",fSourceName.Data(), fPeakEnergy, fCase.Data()));
  streamout<<"No.\tSum\t\tSumErr"<<endl;

  // Fit all energy spectra and write the output parameters
  for (int iChan = 1; iChan < 19; iChan++)
  {
    c1->cd(fCanvasPos[iChan]-6); // Here -6 as we skip the core
    gPad->SetLogy();
    gPad->SetTopMargin(0.05);
    gPad->SetLeftMargin(0.2);
    gPad->SetRightMargin(0.0);

    f[iChan]->SetParameters(0.05, fPeakEnergy, 0.001, 0., 0.);
    f[iChan]->SetParLimits(0,0.,200.);
    f[iChan]->SetParLimits(1,fPeakEnergy-0.005,fPeakEnergy+0.005);
    f[iChan]->SetParLimits(2,0.0001, 0.01);
    f[iChan]->SetParLimits(3, -20., 1000);
    f[iChan]->SetParLimits(4,-100., 100);
    h[iChan]->GetYaxis()->SetRangeUser(0.2, 2 * currMaxContent);

    f_name = Form("fgaus_%d",iChan);

    TH1F *hh = new TH1F(*h[iChan]);
    hh->SetLineColor(kBlue);
    hh->SetName(Form("hFull_%d",iChan));
    hh->Draw();

    // Subtract background if specified
    if (fPathBG != "")
    {
      hBG[iChan]->Rebin(3);
      hBG[iChan]->Sumw2();
      hBG[iChan]->Scale(livetimeSig[iChan]/livetimeBG[iChan]);
      TF1 *fff = (TF1*)hBG[iChan]->FindObject(f_name.Data());
      if (fff) delete (fff);
      hBG[iChan]->SetLineColor(2);
      hBG[iChan]->Draw("same");
      h[iChan]->Sumw2();
      h[iChan]->Add(hBG[iChan], -1);
    }
    // Draw and fit
    h[iChan]->Draw("same");
    // Fit several times to improve the results
    h[iChan]->Fit(f_name,"RlqN+");
    h[iChan]->Fit(f_name,"RlqN+");
    h[iChan]->Fit(f_name,"Rlq+");

    // Extract the fit pars
    f[iChan]->GetParameters(pars[iChan]);
//    for (int iPar=0;iPar<5;iPar++) par_errors[iChan][iPar] = f[iChan]->GetParError(iPar);

    sum[iChan]       = pars[iChan][0]/h[iChan]->GetBinWidth(1);
//    sum_error[iChan] = par_errors[iChan][0]/h[iChan]->GetBinWidth(1);
    sum_error[iChan] = sqrt(sum[iChan]);

    streamout<<iChan<<"\t"<<setprecision(4)<<sum[iChan]<<"\t\t"<<sum_error[iChan]<<endl;
    cout<<"Sum of segm. "<<iChan<<"'s peak is "<<sum[iChan]<<endl;

    // For nice plots - add segment number
    TPaveText *pave = new TPaveText(0.24, 0.75, 0.5, 0.93, "NDC");
    pave->SetTextSize(0.15);
    pave->SetTextColor(4);
    pave->SetFillColor(0);
    pave->AddText(Form("%d",iChan));
    pave->Draw("same");
  }

  // Close the text file and write the canvases into .eps and .root files
  streamout.close();
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("out/%s/data/Occ_data_fits_noBG_%s_%s_%4.2f.eps", fRunName.Data(), fCase.Data(), fSourceName.Data(), fPeakEnergy));
  c1->SaveAs(Form("out/%s/data/Occ_data_fits_noBG_%s_%s_%4.2f.root", fRunName.Data(), fCase.Data(), fSourceName.Data(), fPeakEnergy));

  //
  // Compile and plot the occupancy histograms
  //
  gStyle->SetOptTitle(0);

  TCanvas *c2[fNLayers];
  TString cname;

//  TFile *fileOcc[fNLayers];
  TFile *fileOcc;
  TH1F *histOcc[fNLayers];
  TLine *lines[fNLayers][6];
  Int_t chanID[fNLayers][6] = {{18,17,16,3,2,1},   // Bottom layer
                               {15,14,13,6,5,4},   // Middle layer
                               {12,11,10,9,8,7} }; // Top layer

  fileOcc = new TFile(Form("%s/Occ_data_%s_%s_%4.2f.root", fPathDataOcc.Data(), fCase.Data(), fSourceName.Data(), fPeakEnergy),"recreate");

  Float_t y;
  Float_t y2;
  Float_t x;
  TText t;

  // Write the core histogram

  fileOcc->cd();
//  h[0]->Write();

  for (int iLayer = 0; iLayer<fNLayers; iLayer++)
  {
    // Canvas name
    cname = Form("c2_%s",fLayerName[iLayer].Data());
    c2[iLayer] = new TCanvas(cname.Data(),cname.Data(),5,5,1024,800);
    cout<<"Layer being processed: "<<fLayerName[iLayer]<<endl;

    // Write each layer in a separate internal directory
    fileOcc->mkdir(fLayerName[iLayer].Data());
    fileOcc->cd(fLayerName[iLayer].Data());

    // Book the histogram and set nice plotting style
    histOcc[iLayer] = new TH1F(Form("histOcc_data_%s",fLayerName[iLayer].Data()),"Occupancy in segments;Segment;Occupancy",6,0,360);
    histOcc[iLayer]->SetStats(0);
    histOcc[iLayer]->SetLineWidth(2);
    histOcc[iLayer]->SetLineColor(kBlue);
    histOcc[iLayer]->GetXaxis()->SetLabelOffset(99);
    histOcc[iLayer]->GetXaxis()->SetTitleSize(0.05);

    histOcc[iLayer]->GetYaxis()->SetLabelSize(0.03);
    histOcc[iLayer]->GetYaxis()->SetTitleSize(0.07);
    histOcc[iLayer]->GetYaxis()->SetTitleOffset(0.85);

    // Set histogram content and errors
    for (int i=0; i<6; i++)
    {
      cout<<chanID[iLayer][i]<<endl;
      histOcc[iLayer]->SetBinContent(i+1,sum[chanID[iLayer][i]]);
      cout << "Sum["<<chanID[iLayer][i]                        << "] = "
           << sum[chanID[iLayer][i]]                           << " +- "
           << sum_error[chanID[iLayer][i]]                     << ";  "
           << TMath::Sqrt(histOcc[iLayer]->GetBinContent(i+1)) << endl;
      histOcc[iLayer]->SetBinError(i+1,sum_error[chanID[iLayer][i]]);
    }

    histOcc[iLayer]->GetYaxis()->SetRangeUser(0,1.1*histOcc[iLayer]->GetMaximum());
    histOcc[iLayer]->Draw("hist");

    c2[iLayer]->Update();

    // Add segment numbers as text fields and lines as segment boundaries
    t.SetTextFont(42);
    t.SetTextSize(0.05);
    t.SetTextAlign(22);
    // y coordinate of the text block will slightly differ not to overlay
    y = gPad->GetUymin() - 0.03*( gPad->GetUymax() - gPad->GetUymin() );
    for (int i=0; i<6; i++)
    {
      x = histOcc[iLayer] -> GetXaxis() -> GetBinCenter(i+1);
      t.DrawText(x,y,Form("%d",chanID[iLayer][i]));
      h[chanID[iLayer][i]]->Write();

      if (i==5) continue;
      x = (i+1)*60.;
      y2 = (   histOcc[iLayer]->GetBinContent(i+1)  >  histOcc[iLayer]->GetBinContent(i+2)
             ? histOcc[iLayer]->GetBinContent(i+2)
             : histOcc[iLayer]->GetBinContent(i+1) );
      lines[iLayer][i] = new TLine(x,0,x,y2);
      lines[iLayer][i]->SetLineStyle(9);
      lines[iLayer][i]->SetLineColor(kRed);
      lines[iLayer][i]->SetLineWidth(3);
      lines[iLayer][i]->Draw();
    }

    // Write the histogram for each layer and save as a .eps file
    histOcc[iLayer]->Write();
    c2[iLayer]->RedrawAxis();
    c2[iLayer]->SaveAs(Form("out/%s/data/Occ_data_%s_%s_%4.2f_%s_layer.eps", fRunName.Data(), fCase.Data(), fSourceName.Data(), fPeakEnergy, fLayerName[iLayer].Data()));
  }
}
