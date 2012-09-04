#ifndef TAXISFINDER
#include "TAxisFinder.h"
#endif

#include "TProof.h"

#include <iostream>
#include <cmath>

#include "MyParameters.h"
#include "MyOutputParameters.h"

using namespace std;

void TAxisFinder::FillOccupancyHistogramsMC_top_proof()
{
  /**
   * Fill the occupancy histograms for the top case, using proof to process
   * events in parallel.
   */

  cout<<"Information about simulation:"<<endl;
  cout<<"******************************************************"<<endl;
  cout<<"  Loop over phi = "<<fAngles_min<<".."<<fAngles_max<<" with step "<<fAngles_step<<endl;
  cout<<"******************************************************"<<endl<<endl;

  cout<<"Reserving memory for the histograms..."<<flush;
  fAngles_N = int((fAngles_max-fAngles_min)/fAngles_step)+1;
  for ( int iLoop = 0; iLoop < fAngles_N; iLoop ++ )
  {
    // Filling angles for plotting of the test statistic (Chi2) graph
    fAngles[iLoop] = fAngles_min + iLoop*fAngles_step;
  }
  cout<<" done."<<endl;

  cout<<"Filling MC histograms... "<<endl;

  // Open the chain and add the input MC files
  TChain *ch = new TChain("wfTree");
  for (size_t i=0; i<fPathMC.size(); i++)
    ch->Add(Form("%s/*.root",fPathMC[i].Data()));

  Int_t nentries = ch->GetEntries();
  if (fMaxEvents>0 && fMaxEvents<nentries) nentries = fMaxEvents;
  cout<<"Processing "<<nentries<<" events ..."<<endl;

  // Feed input parameters - angle range, energy line
  MyParameters* mypars = new MyParameters("set1");
  mypars->SetAngles(fAngles_min, fAngles_max, fAngles_step);
  mypars->SetPeakEnergy(fPeakEnergy);

  // Open proof locally
  TProof *p = TProof::Open("lite://");
  p->SetParallel(8); // number of jobs
  // commented part - specific parameters for optimization, usually not needed
//  p->SetParameter("PROOF_UseTreeCache",(Int_t)0);
//  p->SetParameter("PROOF_CacheSize",(Int_t)1000000);

  // Tell root to run the chain on proof
  ch->SetProof();
  p->Load("proof/MyParameters.h+", kTRUE);
  p->Load("proof/MyOutputParameters.h+", kTRUE);
  // Load input pars into the proof object
  p->AddInput(mypars);
  // Process
  ch->Process("proof/TMCOccTopSelector.C+","",nentries);

//  TH1F* hEn[18][1000];


  // Extract the output parameters - occupancy numbers for each bin
  MyOutputParameters *outpars = (MyOutputParameters*)p->GetOutputList()->FindObject("outpars");

  // Fill the occupancy histograms from these numbers
  for ( int iLoop = 0; iLoop < fAngles_N; iLoop ++ )
  {
    for ( int iLayer = 0; iLayer < 3; iLayer++)
    {
      fOccupancyHistMC[iLayer][iLoop] = new TH1F(Form("hOccMC_%s_%04.1f",fLayerName[iLayer].Data(),fAngles[iLoop]), "hOccMC", 6, 0, 360.);
      for (int i=0; i<6; i++)
      {
        fOccupancyHistMC[iLayer][iLoop]->SetBinContent(i+1, outpars->occ[iLoop][iLayer*6 + i]);
        fOccupancyHistMC[iLayer][iLoop]->SetBinError(i+1, sqrt(outpars->occ[iLoop][iLayer*6 + i]));
      }
    }
  }

// Commented below part is used for debug.
// In case also the histograms are filled in proof -> read them
// Used for debug and benchmarking, to compare execution time of optimized/non-optimized processing

//  for ( int iLoop = 0; iLoop < fAngles_N; iLoop ++ )
//  {
//    for ( int iLayer = 0; iLayer < 3; iLayer++)
//    {
//      fOccupancyHistMC[iLayer][iLoop] = (TH1F*) p->GetOutputList()->FindObject(Form("hOccMC_%d_%d", iLayer, iLoop));
//      fOccupancyHistMCNoDr[iLayer][iLoop] = (TH1F*) p->GetOutputList()->FindObject(Form("hOccMC_NoDr_%d_%d", iLayer, iLoop));
//      fOccupancyHistMC[iLayer][iLoop]->SetName(Form("hOccMC_%s_%04.1f",fLayerName[iLayer].Data(),fAngles[iLoop]));
//      for (int i=0; i<6; i++)
//        fOccupancyHistMC[iLayer][iLoop]->SetBinError(i+1, sqrt(fOccupancyHistMC[iLayer][iLoop]->GetBinContent(i+1)));
//    }
//    for (int i=0; i<18; i++)
//      hEn[i][iLoop] = (TH1F*) p->GetOutputList()->FindObject(Form("hEn_%d_%d", i, iLoop));
//  }


//  TCanvas* cMC = new TCanvas("cMC","cMC",5,5,800,600);
//  cMC->Divide(1,3);
//  for (int i=0; i<3; i++)
//  {
//    cMC->cd(i+1);
//    fOccupancyHistMC[i][fAngles_N/2]->GetYaxis()->SetRangeUser(0, 1.2 * fOccupancyHistMC[i][fAngles_N/2]->GetMaximum());
//    fOccupancyHistMC[i][fAngles_N/2]->Draw();
//  }
//  cMC->Update();
//  cMC->SaveAs(Form("out/%s/Occ_MC_%s_%s_%04.2f_layers_%04.1f.eps", fRunName.Data(), fCase.Data(), fSourceName.Data(), fPeakEnergy, fAngles[fAngles_N/2]));
//  delete cMC;

  cout<<" ... filling MC histograms done."<<endl;
  delete p;
}
