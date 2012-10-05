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


#define TMCOccTopSelector_cxx
// The selector skeleton was automatically created by ROOT.
// Here is the information from ROOT:

// The class definition in TMCOccTopSelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("TMCOccTopSelector.C")
// Root > T->Process("TMCOccTopSelector.C","some options")
// Root > T->Process("TMCOccTopSelector.C+")
//

#include "TMCOccTopSelector.h"
#include <TH2.h>
#include <TStyle.h>
#include <TVector3.h>

#include "MyParameters.h"
#include "MyOutputParameters.h"

#include <iostream>
#include <cmath>

using namespace std;

Int_t TMCOccTopSelector::GetSegID_top(
    const Double_t axis_angle, // deg : Axis angel parameter = -(segmentation angle)
    const Double_t zz,         // mm
    const Double_t angle       // deg
)
{
  /**
   * Get segment ID by the point location. \n
   * Convention for segmentation in simulation:\n
   * bottom layer have segment 0 to 5, \n
   * middle layer have segment 6 to 11, \n
   * bottom layer have segment 12 to 17. \n
   * The algorithm is to determine the layer number and the column
   * number, then combine
   */
  const Double_t H=70.;
  const Double_t Pi = 180.;
  Double_t tPhi;
  Int_t lID;   // segment layer number (0=bottom .. 2=top)
  Int_t cID;   // segment column number (0..6)

  lID = int((zz+H/2.0)/(H/3.0));
  if (lID>2) lID=2;
  if (lID<0) lID=0;

  tPhi = angle + axis_angle;
  // Normalize the angle
  while (tPhi>=360) tPhi-=360;
  while (tPhi<0)    tPhi+=360;
  cID = int(tPhi/(Pi/3));
  if (cID>5) {cout<<"Warning, cID>5"<<endl; cID=5;}
  if (cID<0) {cout<<"Warning, cID<0"<<endl; cID=0;}

  // Return the segment number
  return (lID * 6 + cID);
}

void TMCOccTopSelector::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();

}

void TMCOccTopSelector::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();

  // Extract the input parameters from memory
  MyParameters *mypar = dynamic_cast<MyParameters *>(fInput->FindObject("set1"));

  fAngles_min   = mypar->fAngles_min;    // minimal angle to scan
  fAngles_max   = mypar->fAngles_max;    // maximal angle to scan
  fAngles_step  = mypar->fAngles_step;   // angle step
  fPeakEnergy   = mypar->fPeakEnergy;    // energy line

  // Number of angles
  fAngles_N = int((fAngles_max-fAngles_min)/fAngles_step)+1;

  /**
   *  To optimize the performance, I do no fill the histograms with occupancies
   *  but calculate the values of occupancies in each bin. This saves the time that is wasted
   *  when the TH1F::Fill() function is called.
   */
  for ( int iLoop = 0; iLoop < fAngles_N; iLoop ++ )
  {
    // Calculate all angles
    fAngles[iLoop] = fAngles_min + iLoop*fAngles_step;

//    For tests: also fill the histograms to benchmark
//    for ( int iLayer = 0; iLayer < 3; iLayer++)
//    {
//      hist[iLayer][iLoop] = new TH1F(Form("hOccMC_%d_%d", iLayer, iLoop), "hOccMC", 6, 0, 360.);
//      histNoDr[iLayer][iLoop] = new TH1F(Form("hOccMC_NoDr_%d_%d", iLayer, iLoop), "hOccMC", 6, 0, 360.);
//      fOutput->Add(hist[iLayer][iLoop]);
//      fOutput->Add(histNoDr[iLayer][iLoop]);
//    }
//    for (int i=0; i<18; i++)
//    {
//      hEn[i][iLoop] = new TH1F(Form("hEn_%d_%d", i, iLoop), "hEn", 1200, 0., 600.);
//      fOutput->Add(hEn[i][iLoop]);
//    }
  }

  // It is very important to add every object to be extracted later
  // to the fOutput object
  fOutPars = new MyOutputParameters("outpars");
  fOutput->Add(fOutPars);
}

Bool_t TMCOccTopSelector::Process(Long64_t entry)
{
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // It can be passed to either TMCOccTopSelector::GetEntry() or TBranch::GetEntry()
  // to read either all or the required parts of the data. When processing
  // keyed objects with PROOF, the object is already loaded and is available
  // via the fObject pointer.
  //
  // This function should contain the "body" of the analysis. It can contain
  // simple or elaborate selection criteria, run algorithms on the data
  // of the event and typically fill histograms.
  //
  // The processing can be stopped by calling Abort().
  //
  // Use fStatus to set the return value of TTree::Process().
  //
  // The return value is currently not used.

  fChain->GetEntry(entry);

  Double_t iAngle = 0;     // Current angle
  Double_t SegEnergy[18];  // Segment energies
  Double_t CoreEnergy;     // Core energy as the sum over all segments
//  Int_t lID = 0;
  Int_t hitSegID;          // Segment of the current hit
  Int_t firedSegID;        // Fired segment ID
  Int_t nseg;              // Number of fired segments

  float energycut = 0.05;  // Energy cut |E_core-E_segment|<=50 keV

  // Calculate occupancies for all axes orientations
  for (int iLoop=0; iLoop<fAngles_N; iLoop++)
  {
    firedSegID = -1;
    hitSegID = -1;
    nseg = 0;
    CoreEnergy = 0;

    iAngle=fAngles[iLoop];  // Current angle
    memset(SegEnergy, 0, sizeof(SegEnergy)); // Zero the array fast

    // Calculate the segment energies
    for (int iHit=0; iHit<nhits; iHit++)
    {
      // Here hitSegID is the ID of the segment with the given hit
      hitSegID = GetSegID_top(iAngle, z[iHit], phi[iHit]);
      SegEnergy[hitSegID] += hit_E[iHit];
      CoreEnergy += hit_E[iHit];
    }

    // Make the event selection
    for (int iSeg = 0; iSeg < 18; iSeg++)
    {
//      if (SegEnergy[iSeg]>0)
//        hEn[iSeg][iLoop]->Fill(1000*SegEnergy[iSeg]);

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
      if (SegEnergy[iSeg]>0.150) nseg++;
    }

    // If no segment was fired, continue
    if (firedSegID<0) continue;

    // At this point nseg is always = 1, no need to additional check on nseg == 1
    // (since E_core-E_segment<50 and therefore only 1 segment can have E_segment > 150)

//    lID = firedSegID/6;

    if (fabs(SegEnergy[firedSegID]-fPeakEnergy)<0.002)
    {
      // Calculate number of events in the "peak"
      // according to event selection in MC
      fOutPars->occ[iLoop][firedSegID]++;
      // hist[lID][iLoop]->AddBinContent(segID%6+1); // not used anymore
    }

//
//    For tests and benchmarking
//
//    for (int iSeg=0; iSeg<18; iSeg++)
//    {
//      lID = iSeg/6;
//      //        if (SegEnergy[iSeg]>0.020)
//      if (fPeakEnergy<0)
//      {
//        if (SegEnergy[iSeg]>0.020)
//        {  hist[lID][iLoop]->AddBinContent(iSeg%6+1); }
//      }
//      else
//      {
//        if (fabs(SegEnergy[iSeg]-fPeakEnergy)<0.002)
//        {  hist[lID][iLoop]->AddBinContent(iSeg%6+1); }
//      }
//    }
  }
  return kTRUE;
}

void TMCOccTopSelector::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.

}

void TMCOccTopSelector::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.


//
// The histograms extraction are not used anymore (see comments above)
// but left over here for possible tests
//
//
//  for ( int iLoop = 0; iLoop < fAngles_N; iLoop ++ )
//  {
//    for ( int iLayer = 0; iLayer < 3; iLayer++)
//    {
//      hist[iLayer][iLoop] = dynamic_cast<TH1F*>(fOutput->FindObject(Form("hOccMC_%d_%d", iLayer, iLoop)));
//      histNoDr[iLayer][iLoop] = dynamic_cast<TH1F*>(fOutput->FindObject(Form("hOccMC_NoDr_%d_%d", iLayer, iLoop)));
//    }
//    for (int i=0; i<18; i++)
//      hEn[i][iLoop] = dynamic_cast<TH1F*>(fOutput->FindObject(Form("hEn_%d_%d", i, iLoop)));
//  }

  // Only the output parameters object is extracted after internal merging
  fOutPars = dynamic_cast<MyOutputParameters*>(fOutput->FindObject("outpars"));

}
