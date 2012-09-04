#ifndef TAXISFINDER
#include "TAxisFinder.h"
#endif

#include <iostream>
#include "stdlib.h"

#include "TRandom3.h"

using namespace std;

void TAxisFinder::ReadOccupancyHistogramsData()
{
  /**
   * Read the measured occupancy histograms from file
   */

  cout<<"Reading data histograms..."<<endl;

  char* histname;
  // file name with the histograms
  char* filename = Form("%s/Occ_data_%s_%s_%4.2f.root", fPathDataOcc.Data(), fCase.Data(), fSourceName.Data(), fPeakEnergy);

  fFileDataOcc = new TFile(filename, "read");

  // Read the histograms for each layer
  for (int iLayer=0; iLayer<fNLayers; iLayer++)
  {
    // Each layer corresponds to its internal directory in the file
    fFileDataOcc->cd(Form("%s",fLayerName[iLayer].Data()));
    histname = Form("%s/histOcc_data_%s", fLayerName[iLayer].Data(), fLayerName[iLayer].Data());
    cout<<"   ... opening histograms "<<histname<<" in file "<<filename<<endl;

    fOccupancyHistData[iLayer] = (TH1F*)fFileDataOcc->Get(histname);
    if (!fOccupancyHistData[iLayer])
    {  cout<<"Data histograms were not open. Exiting... "<<endl;  exit(1); }
    // Segment 9 for the side case is considered bad due to bad energy resolution,
    // set the bin content to zero
    if (iLayer==2 && fCase.Index("top")<0 )
    {
      cout<<"Setting segment 9 content to 0"<<endl;
      fOccupancyHistData[iLayer] -> SetBinContent(4,0);
      fOccupancyHistData[iLayer] -> SetBinError(4,0);
    }
  }

  // Check if the histograms were read correctly
  if (fOccupancyHistData[0]->GetSum() > 0)
    cout<<" ... reading data histograms was successful."<<endl<<endl;
  else
  { cout<<" ... reading data histograms failed. Exiting..."<<endl; exit(1); }
}
