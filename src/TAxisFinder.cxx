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
