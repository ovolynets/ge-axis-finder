#ifndef TAXISFINDER
#include "TAxisFinder.h"
#endif

#include "TProof.h"

#include <iostream>
#include <cmath>

#include "MyParameters.h"
#include "MyOutputParameters.h"

using namespace std;

void TAxisFinder::FillOccupancyHistogramsMC_side_proof()
{

  /**
   * Fill the occupancy histograms for the side case, using proof to process
   * events in parallel. The idea is the same as for
   * FillOccupancyHistogramsMC_top_proof() but the procedure
   * has to include separate files for different simulated axis angle
   */

  cout<<"Information about simulation:"<<endl;
  cout<<"******************************************************"<<endl;
  cout<<"  Source position angle (relative coordinates) = "<<fSourceAngle<<endl;
  cout<<"  Loop over phi = "<<fAngles_min<<".."<<fAngles_max<<" with step "<<fAngles_step<<endl;;
  cout<<"******************************************************"<<endl<<endl;

  // The number fAngles_N increases every following file that corresponds to another
  // axis angle in simulation. Such a procedure is used in case the file is not found,
  // so it is just skipped and proceeds to the next file
  fAngles_N = 0;

  Int_t nentries;
  Double_t angle;
  TChain *ch;
  char* filename;

  MyOutputParameters *outpars;

  TProof *p = TProof::Open("lite://");
  p->SetParallel(6);
  //    p->SetParameter("PROOF_UseTreeCache",(Int_t)0);
  //    p->SetParameter("PROOF_CacheSize",(Int_t)1000000);

  MyParameters* mypars = new MyParameters("set1");
  mypars->SetAngles(fAngles_min, fAngles_max, fAngles_step);
  mypars->SetPeakEnergy(fPeakEnergy);

  p->AddInput(mypars);
  p->Load("proof/MyParameters.h+", kTRUE);
  p->Load("proof/MyOutputParameters.h+", kTRUE);

  // Initial guess on the number of files to be used
  Int_t nsteps = int((fAngles_max-fAngles_min)/fAngles_step)+1;

  cout<<"Starting loop over files and events..."<<endl;
  for (int iLoop=0;iLoop<nsteps;iLoop++)
  {
    // This is relative angles used in simulation.
    // There the source is always at phi=0, so the shift of the real angle (155 in my case) is applied
    angle = fAngles_min + iLoop*fAngles_step - fSourceAngle;

    ch = new TChain("wfTree");

    // Adding all MC files for the current angle
    for (size_t i=0; i<fPathMC.size(); i++)
    {
      filename = Form("%s/Occ_%ddeg_%s_%04.2f*.root", fPathMC[i].Data(), int(angle), fCase.Data(), fPeakEnergy);
      cout<<filename<<endl;
      ch -> Add(filename);
    }

    nentries = ch->GetEntries();
    if (nentries==0) { cout<<"No entries found. Skipping the file..."<<endl; delete ch; continue;}
    if (fMaxEvents>0 && fMaxEvents<nentries) nentries = fMaxEvents;
    cout<<"Processing "<<nentries<<" events ..."<<endl;

    // Fill the angle which corresponds to the current file
    fAngles[fAngles_N] = fAngles_min + iLoop * fAngles_step; // Now without the source angles as we want to know the absolute angle
    cout<<"Now: absolute angle (to be plotted) = "<<fAngles[fAngles_N]<<", relative angle (in the simulation) = "<<angle<<endl;

    // Set input parameters
    mypars->SetCurrentAngle(fAngles[fAngles_N]);
    mypars->SetSourceAngle(fSourceAngle);
    // Process events
    ch->SetProof();
    ch->Process("proof/TMCOccSideSelector.C+");

    // Extract output pars
    outpars = (MyOutputParameters*)p->GetOutputList()->FindObject(Form("outpars_%04.1f",fAngles[fAngles_N]));

    // Extract the occupancy histograms
    for (int iLayer=0; iLayer<fNLayers; iLayer++)
    {
      fOccupancyHistMC[iLayer][fAngles_N]= new TH1F(Form("hOccMC_%s_%4.1f",fLayerName[iLayer].Data(),fAngles[fAngles_N]),"hOccMC",6,0,360.);
      for (int i=0; i<6; i++)
      {
        fOccupancyHistMC[iLayer][fAngles_N]->SetBinContent(i+1, outpars->occ[0][iLayer*6 + i]);
        fOccupancyHistMC[iLayer][fAngles_N]->SetBinError(i+1, sqrt(outpars->occ[0][iLayer*6 + i]));
      }
    }

// Commented below part is used for debug.
// In case also the histograms are filled in proof -> read them
// Used for debug and benchmarking, to compare execution time of optimized/non-optimized processing

//    for (int iLayer=0; iLayer<3; iLayer++)
//     {
//       fOccupancyHistMC[iLayer][fAngles_N] = (TH1F*)p->GetOutputList()->FindObject(Form("hOccMC_%d_%04.1f", iLayer, fAngles[fAngles_N]));
//       fOccupancyHistMC[iLayer][fAngles_N]->SetName(Form("hOccMC_%s_%04.1f",fLayerName[iLayer].Data(),fAngles[fAngles_N]));
//     }
    delete ch; // To avoid memory leaks
    fAngles_N++; // Increase the number of angles used
  }
  cout<<"  ... loop over files and events finished."<<endl;
  delete p;
}
