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

