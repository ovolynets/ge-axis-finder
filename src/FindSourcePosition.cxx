#ifndef TAXISFINDER
#include "TAxisFinder.h"
#endif

#include <cmath>

#include <iostream>
using namespace std;

void TAxisFinder::FindSourcePosition()
{
  /**
   * The function solves the opposite problem - find the source position
   * if the axis angles is known. This was used to check what the source location
   * was as the side case showed significant discrepancies with the real axis orientation.
   * The comments as for FillOccupancyHistogramsMC_side_proof() are valid.
   */

  cout<<"Information about simulation:"<<endl;
  cout<<"******************************************************"<<endl;
  cout<<"  Axis position angle (relative coordinates) = "<<fAxisAngle<<endl;
  cout<<"  Loop over phi = "<<fAngles_min<<".."<<fAngles_max<<" with step "<<fAngles_step<<endl;;
  cout<<"******************************************************"<<endl<<endl;

  fAngles_N = 0;  // This number is increased every new file. Used in case the new file is not found, so is skipped in the array of angles

  Int_t nentries;
  Double_t angle;
  TChain *ch;
  Double_t SegEnergy[18];
  Int_t lID = 0;
  Int_t idseg = 0;
  char* filename;

  Int_t nsteps = int((fAngles_max-fAngles_min)/fAngles_step)+1; // Initial guess on the number of files to be used

  cout<<"Starting loop over files and events..."<<endl;
  for (int iLoop=0;iLoop<nsteps;iLoop++)
  {
    // This is relative angles used in simulation.
    // There the source is always at phi=0, so the shift of the real angle (155 in my case) is applied
    angle = fAxisAngle - (fAngles_min + iLoop*fAngles_step);

    while (angle>45) angle-=90;
    while (angle<=-45) angle+=90;

    ch = new TChain("occu");

    for (size_t i=0; i<fPathMC.size(); i++)
    {
      filename = Form("%s/Occ_%ddeg_%s_%s.root", fPathMC[i].Data(), int(angle), fSourceName.Data(), fCase.Data());
      cout<<filename<<endl;
      ch -> Add(filename);
    }

    SetBranches(ch);

    nentries = ch->GetEntries();
    if (nentries==0) { cout<<"No entries found. Skipping the file..."<<endl; delete ch; continue;}
    if (fMaxEvents>0 && fMaxEvents<nentries) nentries = fMaxEvents;
    cout<<"Processing "<<nentries<<" events ..."<<endl;

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
      if (iEntry%500000==0) cout<<"Now entry "<<iEntry<<endl;
      ch->GetEntry(iEntry);
      for (int iSeg=0;iSeg<18;iSeg++)
      {  SegEnergy[iSeg] = 0;  }

      for (int iHit=0;iHit<nhits;iHit++)
      {
        idseg = GetSegID(z[iHit], phi[iHit] + fAngles_min + iLoop*fAngles_step);
        SegEnergy[idseg] += hit_E[iHit];
      }
      for (int iSeg=0;iSeg<18;iSeg++)
      {
        lID = iSeg/6;
        if ( fabs(SegEnergy[iSeg]-fPeakEnergy)<0.002)
        { fOccupancyHistMC[lID][fAngles_N]->AddBinContent(iSeg%6+1);  }
      }
    }
    delete ch; // To avoid memory leaks
    fAngles_N++;
  }
  cout<<"  ... loop over files and events finished."<<endl;

};
