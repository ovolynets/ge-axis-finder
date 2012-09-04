//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jun 21 21:07:28 2011 by ROOT version 5.28/00b
// from TChain occu/
//////////////////////////////////////////////////////////

#ifndef TMCOccSideSelector_h
#define TMCOccSideSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TSelector.h>

#include "MyOutputParameters.h"

class TMCOccSideSelector : public TSelector {
public :
  TTree *fChain;   //!pointer to the analyzed TTree or TChain

  Float_t fPeakEnergy;      ///< ENergy peak
  Float_t fCurrentAngle;    ///< Current angle being tested
  Float_t fSourceAngle;     ///< Source angle

  // Histograms for occupancy in MC
  TH1F* hist[3];            ///< Occupancy histograms for each layer, used for tests only

  MyOutputParameters *fOutPars; ///< Output results about segment occupancies

  // Declaration of leaf types
  Int_t          nhits;
  Float_t        hit_x[200];   //[nhits]
  Float_t        hit_y[200];   //[nhits]
  Float_t        hit_z[200];   //[nhits]
  Float_t        hit_E[200];   //[nhits]
  Float_t        r[200];   //[nhits]
  Float_t        phi[200];   //[nhits]
  Float_t        z[200];   //[nhits]

  // List of branches
  TBranch        *b_nhits;   //!
  TBranch        *b_hit_x;   //!
  TBranch        *b_hit_y;   //!
  TBranch        *b_hit_z;   //!
  TBranch        *b_hit_E;   //!
  TBranch        *b_r;   //!
  TBranch        *b_phi;   //!
  TBranch        *b_z;   //!

  TMCOccSideSelector(TTree * /*tree*/ =0) { fOutPars = 0; }
  virtual ~TMCOccSideSelector() { }
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

  Int_t GetSegID_side(const Double_t rad_angle, const Double_t z, const Double_t angle);

  ClassDef(TMCOccSideSelector,0);
};

#endif

#ifdef TMCOccSideSelector_cxx
void TMCOccSideSelector::Init(TTree *tree)
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


  fChain->SetBranchStatus("hits_totnum", 1);  fChain->SetBranchAddress("hits_totnum", &nhits, &b_nhits);
  //  fChain->SetBranchStatus("hit_x", 0);  fChain->SetBranchAddress("hit_x", hit_x, &b_hit_x);
  //  fChain->SetBranchStatus("hit_y", 0);  fChain->SetBranchAddress("hit_y", hit_y, &b_hit_y);
  //  fChain->SetBranchStatus("hit_z", 0);  fChain->SetBranchAddress("hit_z", hit_z, &b_hit_z);
  fChain->SetBranchStatus("hits_edep", 1);  fChain->SetBranchAddress("hits_edep", hit_E, &b_hit_E);
  //  fChain->SetBranchStatus("r", 0);      fChain->SetBranchAddress("r", r, &b_r);
  fChain->SetBranchStatus("endp_hole_phi", 1);    fChain->SetBranchAddress("endp_hole_phi", phi, &b_phi);
  fChain->SetBranchStatus("endp_hole_zpos", 1);      fChain->SetBranchAddress("endp_hole_zpos", z, &b_z);
}

Bool_t TMCOccSideSelector::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

#endif // #ifdef TMCOccSideSelector_cxx
