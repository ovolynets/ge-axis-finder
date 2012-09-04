#ifndef TAXISFINDER
#include "TAxisFinder.h"
#endif

#include "TString.h"
#include "TROOT.h"

#include <iostream>
#include <stdlib.h>

TAxisFinder::TAxisFinder()
{

  fAxisAngle = -999999;
  fSourceAngle = -999999;
  fAngles_step = -999999;
  fPeakEnergy = -999999.;

  fMaxEvents = -1; // By default, process all entries

  fLayerName[0] = TString("bottom");
  fLayerName[1] = TString("middle");
  fLayerName[2] = TString("top");

  int pos[19] = { 6,
                  24, 23, 22, 18, 17, 16,
                  12, 11, 10, 9, 8, 7,
                  15, 14, 13, 21, 20, 19 };

  for (int i=0; i<19; i++)
    fCanvasPos[i] = pos[i];

};

//------------------------------------------------------------------------------------------------------

TAxisFinder::~TAxisFinder() {};
