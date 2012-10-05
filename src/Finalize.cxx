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
