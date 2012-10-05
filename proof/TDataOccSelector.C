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


#define TDataOccSelector_cxx
// The selector skeleton was automatically created by ROOT.
// Here is the information from ROOT:

// The class definition in TDataOccSelector.h has been generated automatically
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
// Root > T->Process("TDataOccSelector.C")
// Root > T->Process("TDataOccSelector.C","some options")
// Root > T->Process("TDataOccSelector.C+")
//

#include "TDataOccSelector.h"
#include <TStyle.h>

#include "MyParameters.h"

#include <iostream>
#include <cmath>

using namespace std;


void TDataOccSelector::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();

}

void TDataOccSelector::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();

  // Extract the input parameters from memory
  MyParameters *mypar = dynamic_cast<MyParameters *>(fInput->FindObject("set1"));

  fPeakEnergy   = mypar->fPeakEnergy; // used energy line

  // Book occupancy histograms to be filled
  for (int i=0;i<19;i++)
  {
    fHisto[i] = new TH1F(Form("h_%d",i),Form("h_%d",i),9000,0.,3);
    // It is very important to add everything to be extracted later
    // to the fOutput object
    fOutput->Add(fHisto[i]);
  }

}

Bool_t TDataOccSelector::Process(Long64_t entry)
{
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // It can be passed to either TDataOccSelector::GetEntry() or TBranch::GetEntry()
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

  //
  // Custom processing parts starts here
  //
  Int_t nseg = 0;     // Number of fired segments
  Int_t segID = -1;   // Fired segment ID

  Float_t energydiffcut = 0.05; // |E_segment - E_core| cut
  Float_t energycut = 0.15;             // Single segment cut: 150 keV for 0.58, 1.17 and 1.33;
  if (fPeakEnergy>2.) energycut = 0.2;  // 200 keV for 2.6

  for (int iChan = 1; iChan <= 18; iChan++)
  {
    // Fired segment should have energy close to the energy of the core
    if (fabs(Cha_Energy[iChan]-Cha_Energy[0])<energydiffcut && Cha_Energy[0]>0.15)
    {
      if (segID>0)
      {
        cout<<"Warning! Two segments with energy close to the core energy"<<endl;
        cout<<"Energies in seg. "<<iChan<<" = "<<Cha_Energy[iChan]<<"  "<<Cha_Energy[0]<<endl<<endl;
      }
      segID = iChan;
    }
  }

  // If no segment was fired, return
  if (segID<0) return kFALSE;

  for (int iChan = 1; iChan <= 18; iChan++)
  {
//    if (Cha_Energy[iChan]>0.100 && iChan!=9) nseg++; // >100/150 keV
    if (segID==9)
    {
      if (Cha_Energy[iChan]>energycut) nseg++; // >150/200 keV
    }
    else
    {
      if (Cha_Energy[iChan]>energycut && iChan!=9) nseg++; // >150/200 keV
    }
  }

  // If not a single-segment event (excluding segment 9 in the selection), return
  if (nseg!=1) return kFALSE;

  // Single segment event, fill the histograms
  fHisto[segID]->Fill(Cha_Energy[segID]);
  fHisto[0]->Fill(Cha_Energy[0]);

  return kTRUE;
}

void TDataOccSelector::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.

}

void TDataOccSelector::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.

  // Add the histograms to the output object after the internal
  // histogram merging was complete
  for (int i=0;i<19;i++)
    fHisto[i] = dynamic_cast<TH1F*>(fOutput->FindObject(Form("h_%d",i)));
}
