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


// The selector skeleton was automatically created by ROOT.
// Here is the information from ROOT:

//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jun 21 11:11:07 2011 by ROOT version 5.28/00b
// from TChain PSTree/
//////////////////////////////////////////////////////////

#ifndef TDataOccSelector_h
#define TDataOccSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TSelector.h>

class TDataOccSelector : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   Float_t fPeakEnergy;
   // histograms
   TH1F           *fHisto[19];

   // Declaration of leaf types
   Int_t           Cha_TotalNum;
   Int_t           Cha_MCAEnergy[20];   //[Cha_TotalNum]
   Float_t         Cha_Energy[20];

   // List of branches
   TBranch        *b_Cha_TotalNum;   //!
   TBranch        *b_Cha_MCAEnergy;   //!
   TBranch        *b_Cha_Energy;   //!

   TDataOccSelector(TTree * /*tree*/ =0) { }
   virtual ~TDataOccSelector() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(TDataOccSelector,0);
};

#endif

#ifdef TDataOccSelector_cxx
void TDataOccSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Cha_TotalNum", &Cha_TotalNum, &b_Cha_TotalNum);
   fChain->SetBranchAddress("Cha_MCAEnergy", Cha_MCAEnergy, &b_Cha_MCAEnergy);
   fChain->SetBranchAddress("Cha_Energy", Cha_Energy, &b_Cha_Energy);
}

Bool_t TDataOccSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef TDataOccSelector_cxx
