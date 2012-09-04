/**
 *  Read the results of the fit to the test statistic from the text file
 *  (produced by CalcFitChi2)
 *  and combine them for the final result of the axis orientation for all layers.
 */


#include "TROOT.h"

#include <fstream>
#include <iostream>

using namespace std;

void SumWithWeights()
{
  TString casename[3] = {"top", "side", "top_syst"};

  // Index of the line corresponding to particular gamma line, the last value is last_index+1
  int iStart[4] = {0, 4, 6, 8};
  TString layername[3] = {"top","middle","bottom"};

  const int NFILES = 8;
  const int fitRange = 10;
//  ifstream inp(Form("results_%dkev.txt", fitRange));
  ifstream inp(Form("epsilon_results_%ddeg.txt", fitRange));

  double nomin_val = 0;
  double denom_val = 0;
  double nomin_val_glob = 0;
  double denom_val_glob = 0;
  double nomin_err = 0;
  double denom_err = 0;
  double nomin_err_glob = 0;
  double denom_err_glob = 0;

  // [layer][fileN]
  float val[3][NFILES];
  float err[3][NFILES];
  float w[3][NFILES];
  float temp;

  for (int iFile = 0; iFile<NFILES; iFile ++)
  {
    for (int iLayer = 0; iLayer<3; iLayer++)
    {
      inp>>temp;    val[iLayer][iFile] = temp;
      inp>>temp;    err[iLayer][iFile] = temp;
      inp>>temp;    w[iLayer][iFile] = temp;
    }
  }

/*
  for (int iCase = 0; iCase < 3; iCase++)
  {
    cout<<endl<<"Now case "<<casename[iCase].Data()<<endl;
    nomin_val_glob = 0;
    denom_val_glob = 0;
    nomin_err_glob = 0;
    denom_err_glob = 0;
    for (int iLayer = 0; iLayer<3; iLayer++)
    {
      cout<<"Now layer "<<layername[iLayer].Data()<<endl;
      nomin_val = 0;
      denom_val = 0;
      nomin_err = 0;
      denom_err = 0;
      for (int iFile = iStart[iCase]; iFile<iStart[iCase+1]; iFile ++)
      {
        if (w[iLayer][iFile]!=0)
        {
          nomin_val += val[iLayer][iFile]*w[iLayer][iFile];
          denom_val += w[iLayer][iFile];
          nomin_err += err[iLayer][iFile]*w[iLayer][iFile];
          denom_err += w[iLayer][iFile];

          nomin_val_glob += val[iLayer][iFile]*w[iLayer][iFile];
          denom_val_glob += w[iLayer][iFile];
          nomin_err_glob += err[iLayer][iFile]*w[iLayer][iFile];
          denom_err_glob += w[iLayer][iFile];
        }
//         cout<<val[iLayer][iFile]<<" * "<<w[iLayer][iFile]<<";  ";
      }
      cout<<"Phi = "<<nomin_val/denom_val<<" +- "<<nomin_err/denom_err<<endl;
    }
    cout<<"Total phi = "<<nomin_val_glob/denom_val_glob<<" +- "<<nomin_err_glob/denom_err_glob<<endl;
  }
*/
// Values only for Co
  nomin_val = 0;
  denom_val = 0;
  nomin_err = 0;
  denom_err = 0;
  for (int iLayer=0; iLayer<3; iLayer++)
  {
    cout<<val[iLayer][1]<<"*"<<w[iLayer][1]<<" + "<<val[iLayer][2]<<"*"<<w[iLayer][2]<<" + ";
    nomin_val+= val[iLayer][1] * w[iLayer][1] + val[iLayer][2] * w[iLayer][2];
    denom_val+= w[iLayer][1]   + w[iLayer][2];
    nomin_err+= err[iLayer][1] * w[iLayer][1] + err[iLayer][2] * w[iLayer][2];
    denom_err+= w[iLayer][1]   + w[iLayer][2];
  }
  cout<<endl<<"Phi from Co = "<<nomin_val/denom_val<<" +- "<<nomin_err/denom_err<<endl;


// Values only for Tl
  nomin_val = 0;
  denom_val = 0;
  nomin_err = 0;
  denom_err = 0;
  for (int iLayer=0; iLayer<3; iLayer++)
  {
    cout<<val[iLayer][0]<<"*"<<w[iLayer][0]<<" + "<<val[iLayer][3]<<"*"<<w[iLayer][3]<<" + ";
    nomin_val+= val[iLayer][0] * w[iLayer][0] + val[iLayer][3] * w[iLayer][3];
    denom_val+= w[iLayer][0]   + w[iLayer][3];
    nomin_err+= err[iLayer][0] * w[iLayer][0] + err[iLayer][3] * w[iLayer][3];
    denom_err+= w[iLayer][0]   + w[iLayer][3];
  }
  cout<<endl<<"Phi from Tl = "<<nomin_val/denom_val<<" +- "<<nomin_err/denom_err<<endl;

// Values only for Tl,side
  nomin_val = 0;
  denom_val = 0;
  nomin_err = 0;
  denom_err = 0;
  for (int iLayer=1; iLayer<3; iLayer++)
  {
    cout<<val[iLayer][4]<<"*"<<w[iLayer][4]<<" + "<<val[iLayer][5]<<"*"<<w[iLayer][5]<<" + ";
    nomin_val+= val[iLayer][4] * w[iLayer][4] + val[iLayer][5] * w[iLayer][5];
    denom_val+= w[iLayer][4]   + w[iLayer][5];
    nomin_err+= err[iLayer][4] * w[iLayer][4] + err[iLayer][5] * w[iLayer][5];
    denom_err+= w[iLayer][4]   + w[iLayer][5];
  }
  cout<<endl<<"Phi from Tl side = "<<nomin_val/denom_val<<" +- "<<nomin_err/denom_err<<endl;
/*
  nomin_val = val[1][1]*w[1][1] + val[1][2]*w[1][2];
  denom_val = w[1][1] + w[1][2];
  nomin_err = err[1][1]*w[1][1] + err[1][2]*w[1][2];
  denom_err = w[1][1] + w[1][2];
*/

}
