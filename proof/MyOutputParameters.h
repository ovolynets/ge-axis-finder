/*
 * MyOutputParameters.h
 *
 *  Created on: Jun 2, 2011
 *      Author: alex
 */

// Example of class for combined parameters

#ifndef ROOT_MyOutputParameters
#define ROOT_MyOutputParameters

#include "TNamed.h"
#include "TCollection.h"

class MyOutputParameters : public TNamed {
public:

  static const int NSEG = 18;
  static const int MAXNANGLES = 200;
  int occ[MAXNANGLES][NSEG];

public:
  MyOutputParameters(const char *s = "myoutpars") : TNamed(s,"") { memset(occ, 0, sizeof(occ)); }
  virtual ~MyOutputParameters() { }

  int Merge(TCollection* list)
  {
    if (list)
    {
      MyOutputParameters *xh = 0;
      TIter nxh(list);
      while (( xh = (MyOutputParameters*) nxh() ))
      {
        for (int i=0; i<xh->MAXNANGLES; i++)
        {
          for (int j=0; j<xh->NSEG; j++)
          {
            occ[i][j]+=xh->occ[i][j];
          }
        }
      }
    }
    return 1;
  }

  // We need to stream this class
  ClassDef(MyOutputParameters, 1)  // Structure to define many parameters in one go
};

#endif

