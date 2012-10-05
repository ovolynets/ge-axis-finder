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

#include "TROOT.h"
#include "TChain.h"

#include <iostream>
using namespace std;

#include <math.h>
#include <sys/stat.h>

void TAxisFinder::FillOccupancyHistogramsMC_side_noparal()
{
  /**
   * Fill the occupancy histograms for the side case without parallel processing.
   * Used for benchmarking only.
   * The idea fully repeats TAxisFinder::FillOccupancyHistogramsMC_side_proof()
   * except the events are processed on a single core. For comments see
   * TAxisFinder::FillOccupancyHistogramsMC_side_proof() and
   * the selector TMCOccSideSelector
   */

  cout<<"Information about simulation:"<<endl;
  cout<<"******************************************************"<<endl;
  cout<<"  Source position angle (relative coordinates) = "<<fSourceAngle<<endl;
  cout<<"  Loop over phi = "<<fAngles_min<<".."<<fAngles_max<<" with step "<<fAngles_step<<endl;;
  cout<<"******************************************************"<<endl<<endl;

  fAngles_N = 0;  // This number is increased every new file. Used in case the new file is not found, so is skipped in the array of angles

  Int_t nentries;
  Double_t angle;
  TChain *ch;
  Double_t SegEnergy[18];
  Double_t CoreEnergy;
  Int_t lID = 0;
  Int_t hitSegID;
  Int_t firedSegID;
  char* filename;
  float energycut = 0.05;
  int nseg;
  float occ[1000][18];
  memset(occ, 0, sizeof(occ));

  // Initial guess on the number of files to be used
  Int_t nsteps = int((fAngles_max-fAngles_min)/fAngles_step)+1;

  cout<<"Starting loop over files and events..."<<endl;
  for (int iLoop=0; iLoop<nsteps; iLoop++)
  {
    // This is relative angles used in simulation.
    // There the source is always at phi=0, so the shift of the real angle (155 in my case) is applied
    angle = fAngles_min + iLoop*fAngles_step - fSourceAngle;

    ch = new TChain("wfTree");
    for (size_t i=0; i<fPathMC.size(); i++)
    {
      filename = Form("%s/Occ_%ddeg_%s_%04.2f*.root", fPathMC[i].Data(), int(angle), fCase.Data(), fPeakEnergy);
      cout<<filename<<endl;
      ch->Add(filename);
    }

    nentries = ch->GetEntries();
    if (nentries==0) { cout<<"No entries found. Skipping the file..."<<endl; delete ch; continue;}
    if (fMaxEvents>0 && fMaxEvents<nentries) nentries = fMaxEvents;
    cout<<"Processing "<<nentries<<" events ..."<<endl;

    SetBranches(ch);

    // Fill the angle which corresponds to the current file
    fAngles[fAngles_N] = fAngles_min + iLoop * fAngles_step; // Now without the source angles as we want to know the absolute angle
    cout<<"Now: absolute angle (to be plotted) = "<<fAngles[fAngles_N]<<", relative angle (in the simulation) = "<<angle<<endl;

    // Reserve memory for 3 histograms, for each layer
    for (int iLayer=0; iLayer<fNLayers; iLayer++)
    {
      fOccupancyHistMC[iLayer][fAngles_N]= new TH1F(Form("hOccMC_%s_%4.1f",fLayerName[iLayer].Data(),fAngles[fAngles_N]),"hOccMC",6,0,360.);
    }

    for (int iEntry=0;iEntry<nentries;iEntry++)
    {
      if (iEntry%100000==0) cout<<"Now entry "<<iEntry<<endl;
      ch->GetEntry(iEntry);
      for (int iSeg=0;iSeg<18;iSeg++)
      {  SegEnergy[iSeg] = 0;  }
      CoreEnergy = 0;
      nseg = 0;
      hitSegID = -1;
      firedSegID = -1;

      for (int iHit=0;iHit<nhits;iHit++)
      {
        hitSegID = GetSegID_side(fSourceAngle, z[iHit], phi[iHit]);
        SegEnergy[hitSegID] += hit_E[iHit];
        CoreEnergy += hit_E[iHit];
//         cout<<Form("entry=%d, iHit=%d, hitSegID=%d",iEntry,iHit,hitSegID)<<endl;
      }

      for (int iSeg = 0; iSeg < 18; iSeg++)
      {
        // Fired segment should have energy close to the energy of the core
        if (fabs(SegEnergy[iSeg]-CoreEnergy)<energycut && CoreEnergy>0.150)
        {
          if (firedSegID>0)
          {
            cout<<"Warning! Two segments with energy close to the core energy"<<endl;
            cout<<"Energies: seg. "<<iSeg<<" = "<<SegEnergy[iSeg]<<", seg. "<<firedSegID<<" = "<<SegEnergy[firedSegID]<<", Core energy = "<<CoreEnergy<<endl<<endl;
          }
          firedSegID = iSeg;
        }
        if (SegEnergy[iSeg]>0.1) nseg++;
      }

      ////     Just a small test, in MC if segID>0 => nseg==1
      //    True!
      //    if (segID>=0) cout<<nseg<<endl;

      // If no segment was fired, continue
      if (firedSegID<0) continue;
      // At this point nseg is always = 1, no need to additional check

      lID = firedSegID/6;
      if (fabs(SegEnergy[firedSegID]-fPeakEnergy)<0.002)
      {
        fOccupancyHistMC[lID][fAngles_N]->AddBinContent(firedSegID%6+1);
        occ[fAngles_N][firedSegID]++;
      }
    }
//     for (int i=0; i<3; i++)
    {
      cout<<"From hist = "<<fOccupancyHistMC[0][fAngles_N]->GetBinContent(1)<<endl;
      cout<<"From occ = "<<occ[fAngles_N][8]<<endl;
    }
    delete ch; // To avoid memory leaks
    fAngles_N++;
  }
  cout<<"  ... loop over files and events finished."<<endl;

}
