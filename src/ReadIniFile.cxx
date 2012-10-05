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

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>

using namespace std;


void TAxisFinder::ReadIniFile(const char* filename)
{
  /**
   * Read the ini file with paths for data and simulation, angle ranges,
   * source angle (for the side case) etc.
   */

  ifstream inifile(filename);
  if (!inifile.good())
  {
    cout<<"Ini file was not opened. Exit now ..."<<endl;
    exit(0);
  }

  std::stringstream datastream;
  std::string       datastring;

  char temp[256];
  // Read the first dummy line
  inifile.getline(temp, 256);
  char param[256];  // String for the parameter name
  TString paramS;   // Same parameter in TString
  char valueChar[256]; // Parameter value in char

  // Read ini file line by line
  while(getline(inifile,datastring))
  {
    // The reading is done via intermediate stream variable.
    // It reads the line to the string and checks if it is blank
    // If yes - skip it
    // If no - convert to a stream and read the value as from fstream
    // With this trick I avoid the mis-reading of the last line twice
    // that usually happens if the .ini file ends with a blank line

    if (datastring.empty()) continue;
    datastream.clear();
    datastream.str(datastring);

    datastream>>param;
    paramS = param;
    paramS.ToLower();

//    cout<<paramS.Data()<<endl;
    // Read all the parameters
    if (paramS.Contains("#"))  // Skip comment
      { inifile.getline(temp, 256);}

    else if (paramS.Contains("casename")) // Case
      { inifile>>valueChar; fCase = valueChar; fCase.ToLower(); }

    else if (paramS.Contains("run")) // Run name
      { inifile>>valueChar; fRunName = valueChar; }

    else if (paramS.Contains("axisangle")) // The axis angle for the inverted problem: reconstruction of the source position
      { inifile >> fAxisAngle;}

    else if (paramS.Contains("sourceangle")) // The source angle for the side case
      { inifile >> fSourceAngle;}

    else if (paramS.Contains("maxevents")) // Max. number of events
      { inifile >> fMaxEvents; }

    else if (paramS.Contains("anglesrange")) // Range of angles and step size
      { inifile >> fAngles_min >> fAngles_max >> fAngles_step;}

    else if (paramS.Contains("sourcename")) // Name of the source
      { inifile >> valueChar; fSourceName = valueChar;}

    else if (paramS.Contains("peakenergy")) // Energy peak
      { inifile >> fPeakEnergy; }

    else if (paramS.Contains("pathdata")) // Data to data files with energy deposits to calculate data occupancy
      { inifile >> valueChar; fPathData = valueChar; }

    else if (paramS.Contains("pathdatocc")) // Data to data occupancy histograms
      { inifile >> valueChar; fPathDataOcc = valueChar; }

    else if (paramS.Contains("pathbg"))     // Path to background with energy deposits to calculate data occupancy
      { inifile >> valueChar; fPathBG = valueChar; }

    else if (paramS.Contains("pathmc"))     // Path to simulated energy deposits to fill the MC occupancies
      { inifile >> valueChar; fPathMC.push_back(TString(valueChar)); }

    else
      { cout<<"Parameter "<<paramS.Data()<<" not known. Skipping the line"<<endl; inifile.getline(temp, 256); }
  }

  // Consistency check
  if (fCase=="")
    { cout<<"You did not specify the case. Exit now..."<<endl; exit(0); }
  if (fCase=="side_GII" && fAxisAngle<-10000.)
    { cout<<"You specified side_GII case but did not specify the axis angle. Exit now..."<<endl; exit(0); }

  if (fRunName=="")
    { cout<<"You did not specify the data run number. Exit now..."<<endl; exit(0); }

  if (fCase=="side" && fSourceAngle<-10000.)
    { cout<<"You specified side case but did not specify the source angle. Exit now..."<<endl; exit(0); }

  if (fAngles_step<-10000.)
    { cout<<"You did not specify the angles range correctly. Exit now..."<<endl; exit(0); }

  if (fSourceName=="")
    { cout<<"You did not specify the source. Exit now..."<<endl; exit(0); }

  if (fPeakEnergy<-10000.)
    { cout<<"You did not specify the peak energy. Exit now..."<<endl; exit(0); }

  if (fPathData=="")
    { cout<<"You did not specify the path to data. You will not be able to calculate occupancy in data."<<endl;}

  if (fPathDataOcc=="")
    { cout<<"You did not specify the path to the occupancy in data. Exit now ..."<<endl; exit(0); }

  if (!fPathMC.size())
    { cout<<"You did not specify any path to MC. Exit now ..."<<endl; exit(0); }

//  if (fPathBG=="")
//    { cout<<"You did not specify any background path. Exit now ..."<<endl; exit(0); }

  // Print summary
  cout<<endl<<"******************************************************************"<<endl
      <<"You specified the following settings for the program:"<<endl
      <<"Case: "<<fCase.Data()<<endl
      <<"Angles range: "<<fAngles_min<<".."<<fAngles_max<<", step "<<fAngles_step<<endl
      <<"Source: "<<fSourceName.Data()<<endl
      <<"Energy: "<<fPeakEnergy<<endl
      <<"Data in: "<<fPathData.Data()<<endl
      <<"Data occupancy in: "<<fPathDataOcc.Data()<<endl
      <<"MC in: ";
      for (size_t i=0; i<fPathMC.size(); i++)
        cout<<fPathMC[i].Data()<<endl;
      cout<<"******************************************************************"<<endl<<endl;

}
