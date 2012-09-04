#ifndef TAXISFINDER
#include "TAxisFinder.h"
#endif

#include "TString.h"
#include "TROOT.h"

#include <iostream>
#include <math.h>

using namespace std;

void TAxisFinder::FillOccupancyHistogramsMC_top_noparal_optimized()
{
  /**
   * Fill the occupancy histograms for the top case without parallel processing
   * but with optimization.
   * Used for benchmarking only.
   * The idea fully repeats TAxisFinder::FillOccupancyHistogramsMC_top_proof()
   * except the events are processed on a single core. For comments see
   * TAxisFinder::FillOccupancyHistogramsMC_top_proof() and
   * the selector TMCOccTopSelector
   */

  cout<<"Information about simulation:"<<endl;
  cout<<"******************************************************"<<endl;
  cout<<"  Loop over phi = "<<fAngles_min<<".."<<fAngles_max<<" with step "<<fAngles_step<<endl;;
  cout<<"******************************************************"<<endl<<endl;


  cout<<"Reserving memory for the histograms..."<<flush;
  fAngles_N = int((fAngles_max-fAngles_min)/fAngles_step)+1;
  for ( int iLoop = 0; iLoop < fAngles_N; iLoop ++ )
  {
    // Filling angles for plotting of the Chi2 graph
    fAngles[iLoop] = fAngles_min + iLoop*fAngles_step;
    for ( int iLayer = 0; iLayer < fNLayers; iLayer++)
    {  fOccupancyHistMC[iLayer][iLoop] = new TH1F(Form("hOccMC_%s_%4.1f",fLayerName[iLayer].Data(),fAngles[iLoop]),"hOccMC",6,0,360.); }
  }
  cout<<" done."<<endl;


  cout<<"Filling MC histograms... "<<endl;
  Double_t iAngle = 0;
  Double_t SegEnergy[18];
  Double_t CoreEnergy;
  Int_t lID = 0;
  Int_t hitSegID = 0;
  Int_t firedSegID = -1;
  float energycut = 0.05;

  TChain *ch = new TChain("wfTree");
  for (size_t i=0; i<fPathMC.size(); i++)
    ch->Add(Form("%s/*.root",fPathMC[i].Data()));

  SetBranches(ch); // Set addresses and statuses for branches

  Int_t nentries = ch->GetEntries();

  if (fMaxEvents>0 && fMaxEvents<nentries) nentries = fMaxEvents;
    cout<<"Processing "<<nentries<<" events ..."<<endl;

  for (int iEntry=0;iEntry<nentries;iEntry++)
  {
    ch->GetEntry(iEntry);
    if (iEntry%500000==0) cout<<"Now entry "<<iEntry<<endl;
    for (int iLoop=0;iLoop<fAngles_N;iLoop++)
    {
      iAngle=fAngles[iLoop];
      for (int iSeg=0;iSeg<18;iSeg++)
      {  SegEnergy[iSeg] = 0;  }
      CoreEnergy = 0;
      firedSegID = -1;

      for (int iHit=0;iHit<nhits;iHit++)
      {
        hitSegID = GetSegID_top(iAngle, z[iHit], phi[iHit]);
        SegEnergy[hitSegID] += hit_E[iHit];
        CoreEnergy += hit_E[iHit];
      }

      for (int iSeg = 0; iSeg < 18; iSeg++)
      {
        // Fired segment should have energy close to the energy of the core
        if ( fabs(SegEnergy[iSeg]-CoreEnergy)<energycut && CoreEnergy>0.150)
        {
          if (firedSegID>0)
          {
            cout<<"Warning! Two segments with energy close to the core energy"<<endl;
            cout<<"Energies in seg. "<<iSeg<<" = "<<SegEnergy[iSeg]<<"  "<<CoreEnergy<<endl<<endl;
          }
          firedSegID = iSeg;
        }
      }

      // If no segment was fired, continue
      if (firedSegID<0) continue;
      // At this point nseg is always = 1, no need to additional check

      lID = firedSegID/6;

      if (fabs(SegEnergy[firedSegID]-fPeakEnergy)<0.002)
      {
        fOccupancyHistMC[lID][iLoop]->AddBinContent(firedSegID%6+1);
      }
    }
  }
  cout<<" ... filling MC histograms done."<<endl;
}
