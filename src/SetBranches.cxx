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

void TAxisFinder::SetBranches(TChain* ch)
{
  ch->SetBranchStatus("*",0);
  ch->SetBranchStatus("hits_totnum", 1);  ch->SetBranchAddress("hits_totnum",&nhits);
  ch->SetBranchStatus("hits_edep", 1);  ch->SetBranchAddress("hits_edep", hit_E);
  ch->SetBranchStatus("endp_hole_phi", 1);    ch->SetBranchAddress("endp_hole_phi", phi);
  ch->SetBranchStatus("endp_hole_zpos", 1);      ch->SetBranchAddress("endp_hole_zpos",  z);

};
