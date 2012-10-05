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


// Example of class for combined parameters

#ifndef ROOT_MyParameters
#define ROOT_MyParameters

#include "TNamed.h"

class MyParameters : public TNamed {
public:

  float fAngles_min;
  float fAngles_max;
  float fAngles_step;

  float fPeakEnergy;

  // Now variables related only to the side case:
  float fSourceAngle;
  float fCurrentAngle;


public:
  MyParameters(const char *s = "mypar") : TNamed(s,"") { }
  virtual ~MyParameters() { }

  void SetAngles(const float min, const float max, const float step)
  {
    fAngles_min = min;
    fAngles_max = max;
    fAngles_step = step;
  }

  void SetPeakEnergy(const float en) { fPeakEnergy = en; }
  void SetSourceAngle(const float ang) { fSourceAngle = ang; }
  void SetCurrentAngle(const float ang) { fCurrentAngle = ang; }

  float GetMinAngle() { return fAngles_min; }
  float GetMaxAngle() { return fAngles_max; }
  float GetStepAngle() { return fAngles_step; }

  float GetPeakEnergy() { return fPeakEnergy; }

  float GetSourceAngle() { return fSourceAngle; }
  float GetCurrentAngle() { return fCurrentAngle; }

  // We need to stream this class
  ClassDef(MyParameters, 1)  // Structure to define many parameters in one go
};

#endif
