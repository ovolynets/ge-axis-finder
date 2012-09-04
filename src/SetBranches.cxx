#ifndef TAXISFINDER
#include "TAxisFinder.h"
#endif

void TAxisFinder::SetBranches(TChain* ch)
{
  ch->SetBranchStatus("*",0);
  ch->SetBranchStatus("hits_totnum", 1);  ch->SetBranchAddress("hits_totnum",&nhits);
  ch->SetBranchStatus("hits_edep", 1);  ch->SetBranchAddress("hits_edep", hit_E);
  ch->SetBranchStatus("endp_hole_phi", 1);    ch->SetBranchAddress("endp_hole_phi", phi);
  ch->SetBranchStatus("endp_hole_zpos", 1);      ch->SetBranchAddress("endp_hole_zpos",  z);

};
