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
