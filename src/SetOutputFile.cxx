#ifndef TAXISFINDER
#include "TAxisFinder.h"
#endif

void TAxisFinder::SetOutputFile()
{
/*
 * Opens the output file and creates internal directories for each layer
 */

  fOutFile = new TFile(Form("out/%s/OccPlots_%s_%4.2f.root", fRunName.Data(), fCase.Data(), fPeakEnergy),"recreate");
  for (int i=0; i<fNLayers; i++)
  {
    fOutFile->mkdir(fLayerName[i]);
  }
};
