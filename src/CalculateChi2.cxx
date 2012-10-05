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


#ifndef AXISFINDER
#include "TAxisFinder.h"
#endif

#include "TString.h"
#include "TROOT.h"

#include <iostream>
using namespace std;

void TAxisFinder::CalculateChi2()
{
  /**
   * Scale the MC histograms, calculate the chi2 value.
   * The test statistics, either chi2 or epsilon, are calculated
   * off-line, outside the program, from the occupancy histograms directly.
   * The chi2 calculation is left over here to immediately see the result in the
   * output files without the need of further processing.
   */

  cout<<"Calculation of chi2..."<<endl;

  Double_t numer, denom; // Numerator and denominator for chi2 calculations
  Double_t binData, binEData, binMC;//, binEMC;
  Double_t sumData, sumMC;

  for (int iLayer=0; iLayer<fNLayers; iLayer++)
  {
    // The histograms are stored for each layer in separate directories
    fOutFile->cd(fLayerName[iLayer]);

    sumData = fOccupancyHistData[iLayer]->GetSum(); // Total number of entries for normalizing
    for (Int_t iLoop = 0; iLoop < fAngles_N; iLoop++)
    {
      if (iLayer==2 && fOccupancyHistData[2]->GetBinContent(4)==(double)0)
      {
        /**
         *  Segment 9 for the side case has a bad energy resolution,
         * check if it the case in measurements (measured occupancy histogram
         * would have 0 content in this bin, see ReadOccupancyHistogramsData())
         */
//        cout<<"Debug:: seg 9 excluded"<<endl;
        fOccupancyHistMC[iLayer][iLoop] -> SetBinContent(4,0);
        fOccupancyHistMC[iLayer][iLoop] -> SetBinError(4,0);
      }
      // Bin content sum for normalizing
      sumMC = fOccupancyHistMC[iLayer][iLoop]->GetSum();
      // Normalize the MC histogram
      fOccupancyHistMC[iLayer][iLoop]->Scale(sumData/sumMC);
      numer = 0;
      denom = 0;
      fChi2[iLayer][iLoop] = 0;

      // Calculate the chi2 value for all bins in a given layer
      for (int i=0;i<6;i++)
      {
        // If segment 9 - skip
        if ( iLayer==2 && i==3 && fOccupancyHistData[2]->GetBinContent(4)==(double)0 ) continue;
        binData  = fOccupancyHistData[iLayer]->GetBinContent(i+1); // measured bin content
        binEData = fOccupancyHistData[iLayer]->GetBinError  (i+1); // measured bin error
        binMC   = fOccupancyHistMC[iLayer][iLoop]->  GetBinContent(i+1); // simulated bin content
//        binEMC  = fOccupancyHistMC[iLayer][iLoop]->  GetBinError  (i+1);

        numer = (binData-binMC)*(binData-binMC); // numerator of the fraction for either test statistic
        denom = binEData*binEData;               // denominator of the chi2 fraction
        //
        //
        // !!!!!!!!!!!!!!!! Whether epsilon or chi2 criteria can also be decided off-line (to avoid running
        // !!!!!!!!!!!!!!!! the program twice), in scripts/CalcFitChi2.cxx
        //
        //
        if (denom==(double)0) cout<<"Bad!!"<<endl;
        fChi2[iLayer][iLoop] += numer/denom;       // This is the value of chi2
      }
    }
  }
  cout<<" ... calculation of chi2 done."<<endl;
}
