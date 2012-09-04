/*
 * MyParameters.h
 *
 *  Created on: Jun 2, 2011
 *      Author: alex
 */

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
