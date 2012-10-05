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

#include <iostream>
using namespace std;

void TAxisFinder::FillOccupancyHistogramsMC()
{
  /**
   * Fill the occupancy histograms in simulation.
   * The function calls different functions, depending on the case
   */
  cout<<"Filling MC histograms... "<<endl;

  // Check what case to execute:
  if (fCase.Index("top")>-1)
  {  FillOccupancyHistogramsMC_top_proof(); }               // Top, on proof
//  {  FillOccupancyHistogramsMC_top_noparal(); }           // Top, non-parallel version for benchmarking
//  {  FillOccupancyHistogramsMC_top_noparal_optimized(); } // Top, non-parallel version, optimized, for benchmark
  else if (fCase=="side")
  {  FillOccupancyHistogramsMC_side_proof(); }              // Side, on proof
//  {  FillOccupancyHistogramsMC_side_noparal(); }          // Side, non-parallel version, for benchmark
  else
  // Side case, the inverse problem - to reconstruct the source angle for the known axes orientation
  {  FindSourcePosition(); }

}
