#ifndef TAXISFINDER
#include "TAxisFinder.h"
#endif

#include "TCanvas.h"
#include <iostream>
using namespace std;

void TAxisFinder::Finalize()
{
  /**
   * Finalize the results, save histograms to root files
   */

  cout<<"Save occupancy histograms into file ..."<<endl;

  TCanvas* c;

  Double_t angle;
  // Save for all layers
  for (int iLayer=0; iLayer<fNLayers; iLayer++)
  {
    // The histograms are stored for each layer in separate directories
    fOutFile->cd(fLayerName[iLayer]);

    // Loop over all possible angles
    for (int iLoop=0; iLoop<fAngles_N; iLoop++)
    {
      angle = fAngles[iLoop];  // current angle
      // Create a canvas
      c = new TCanvas(Form("canvas_%4.1f",angle),"Occupancy",5,5,1024,800);
      // Draw the histograms onto the canvas
      fOccupancyHistData[iLayer]->Draw("hist e");
      fOccupancyHistData[iLayer]->SetLineColor(kBlue);
      fOccupancyHistMC[iLayer][iLoop]->Draw("hist e same");
      fOccupancyHistMC[iLayer][iLoop]->SetLineColor(kRed);

      // Write the canvas into the root file
      c->Write();
      delete c;  // delete the canvas
    }
    // Plot the chi2 values and save the canvas as well
    c = new TCanvas("c","Occupancy",5,5,1024,800);
    fChi2Graph[iLayer] = new TGraph(fAngles_N, fAngles, fChi2[iLayer]);
    fChi2Graph[iLayer]->Draw("ap*");
    fChi2Graph[iLayer]->Write();
    c->SetName(Form("out/%s/Occ_Chi2_%s_%4.2f_%s", fRunName.Data(), fCase.Data(), fPeakEnergy, fLayerName[iLayer].Data()));
    c->SaveAs(Form("out/%s/Occ_Chi2_%s_%4.2f_%s.eps", fRunName.Data(), fCase.Data(), fPeakEnergy, fLayerName[iLayer].Data()));
    c->Write();
    delete c;
  }
  cout<<"... save occupancy histograms into file done"<<endl;

  fOutFile->Close();
}
