#ifndef TAXISFINDER
#include "TAxisFinder.h"
#endif

#include "TROOT.h"

#include <iostream>
using namespace std;

Int_t TAxisFinder::GetSegID(const Double_t z, //mm
                         const Double_t angle //deg
                         )
{
  const Double_t H=70.;
  const Double_t Pi = 180.;
  Double_t tPhi;
  Int_t lID;   // layer number (0..2)
  Int_t cID;   // column number (0..6)
  lID = int((z+H/2.0)/(H/3.0));
  if (lID>2) lID=2;
  if (lID<0) lID=0;

  tPhi = angle;
  while (tPhi>=360) tPhi-=360;
  while (tPhi<0)    tPhi+=360;
  cID = int(tPhi/(Pi/3));
  if (cID>5) {cout<<"Warning, cID>5"<<endl; cID=5;}
  if (cID<0) {cout<<"Warning, cID<0"<<endl; cID=0;}

  return (lID * 6 + cID);
}

//------------------------------------------------------------------------------------------------------

Int_t TAxisFinder::GetSegID_top(const Double_t axis_angle, // deg : Axis angle parameter =[equiv. to]= -(segmentation angle)
                               const Double_t z,        // mm
                               const Double_t angle     // deg
                               )
{
  return GetSegID(z, angle + axis_angle);
}

//------------------------------------------------------------------------------------------------------

Int_t TAxisFinder::GetSegID_side(const Double_t rad_angle, // deg: Angle at which source is positioned
                                const Double_t z,         // cm
                                const Double_t angle      // deg
                                )
{
  return GetSegID(z, angle + rad_angle);
}
