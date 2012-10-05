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


#include "TAxisFinder.h"
#include <iostream>
#include "stdlib.h"

#include "TStyle.h"

// #include "/home/pclg-23/volynets/.root/macros/logon.C"
// #include "/home/alex/.root/macros/logon.C"

// You are supposed to put the logon.C file in your home directory, just
// as specified in one of the commented part above
#include "logon.C"

using namespace std;

int main(int argc, char**argv)
{
  // The program accepts one mandatory argument - the .ini file
  if (argc<2)
  {
    cout<<"Usage: "<<argv[0]<<" [ini file]. Exit now..."<<endl;
    exit(0);
  }

  // This is needed if the scripts are to be run via ssh:
  // tell ROOT not to draw any canvases on the screen, BUT
  // it will create them, and save to files, in the same way,
  // just will not draw them
  gROOT->SetBatch(true);

  // Create the analysis object
  TAxisFinder *af = new TAxisFinder();
  // Read the ini file
  af->ReadIniFile(argv[1]);

  // By default, process all events, but a smaller number can be specified for tests
  af -> SetMaxEvents(-1);

// The following part should be uncommented if the data occupancy histograms do not exist yet
// (if you just start with the program, they most probably don't).
// Uncomment the following 3 lines to produce those, then comment back and proceed
// with the axis search

//  af -> FillDataSpectraHistograms();
//  af -> CalculateOccupancyInData();
//  return 1;


  // Set the output
  af->SetOutputFile();


  // Nice plotting style
  SetStyles();
  // Starting to fill MC histograms. This is the most time consuming part
  af->FillOccupancyHistogramsMC();
  // Read the measured occupancy histograms
  af->ReadOccupancyHistogramsData();

  // Reset plotting style
  SetStyles();

  // Compare the measured occupancy distribution to all simulated
  // ones (for each axes orientation angle):
  // scale the distributions and calculate chi2
  // Note that the program is only designed to save the histograms and NOT to calculate chi2.
  // Whether chi2 or epsilon test statistic is used is decided outside - and are compared.
  // Such approach saves a lot of time not to process all the data twice, but only to fill
  // the occupancy histograms, and only then to calculate the test statistic
  af->CalculateChi2();

  // Write the histograms into files, close the file
  af->Finalize();

  return 0;
}
