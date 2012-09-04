#include "TH1F.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TFile.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TPaveText.h"
#include "TString.h"
#include "TRandom3.h"
#include "TMath.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

#include "/home/pclg-23/volynets/.root/macros/logon.C"

using namespace std;

int CalcFitChi2_ensembletest()
{
  SetStyles();

  TRandom3 r;
  r.SetSeed(0);
  gStyle->SetOptTitle(0);

  TString method = "epsilon";
//  TString method = "chi2";

  const int nFiles = 100;
  TString DS[nFiles];
  DS[0] = "top_0.58";
  DS[1] = "top_1.17";
  DS[2] = "top_1.33";
  DS[3] = "top_2.61";
  DS[4] = "side_0.58";
  DS[5] = "side_2.61";

  DS[6] = "top_1.17_syst";
  DS[7] = "top_1.33_syst";
  DS[8] = "top_1.33_5mmoff_45deg";
  DS[9] = "top_1.33_5mmoff_90deg";
  DS[10]= "top_2.61_5mmoff_45deg";
  DS[11]= "top_2.61_5mmoff_90deg";
  DS[12]= "top_1.33_newimp_-45to45_1deg";


//  DS[6] = "side_GII_0.24";
//  DS[7] = "side_GII_2.61";

  TString columnprefix[nFiles];
  columnprefix[0] = "Top & 0.58";
  columnprefix[1] = "Top & 1.17";
  columnprefix[2] = "Top & 1.33";
  columnprefix[3] = "Top & 2.61";
  columnprefix[4] = "Side & 0.58";
  columnprefix[5] = "Side & 2.61";

  columnprefix[6] = "Top & 1.17 2mm";
  columnprefix[7] = "Top & 1.33 2mm";
  columnprefix[8] = "Top & 1.33 5mmoff45deg";
  columnprefix[9] = "Top & 1.33 5mmoff90deg";
  columnprefix[10] = "Top & 2.61 5mmoff45deg";
  columnprefix[11] = "Top & 2.61 5mmoff90deg";
  columnprefix[12] = "Top & 1.33 impur";


  const int nLayers = 3;
  TString layers[nLayers] = {"top", "middle", "bottom"};


// For epsilon criteria
  const Float_t fChi2_min_x[nFiles][nLayers] = { {-12,-10, 10},
                                                 {-10, 0, -5},
                                                 {-8, 5, 2},
                                                 { 0,-8, -5},
/*                                                 {160,170,165}, // Background-ignored
                                                 {165,170,165},
                                                 */
                                                 {160,165,158}, // Background-subtracted
                                                 {160,165,165},
                                                 // systematics begins here
                                                 {-10,0,-5},
                                                 {-7,5,0},
                                                 {-7,0,0},
                                                 {-7,5,0},
                                                 {-8,0,-5},
                                                 {-10,0,-3},
                                                 {-7,5,0} };

/*
// For chi2 criteria
  const Float_t fChi2_min_x[nFiles][nLayers] = { {-12,-10, 10},
                                                 {-10, 0, -5},
                                                 {-8, 5, 2},
                                                 { 0,-8, -5},
                                                 {155,170,160},
                                                 {160,170,165},
                                                 {-10,0,-5},
                                                 {-8,5,0} };
*/
  const Float_t fChi2_width_const = 15;

  int iFile = 3;
  TFile *f = new TFile(Form("rootfiles/OccPlots_%s.root",DS[iFile].Data()),"read");
//   f->ls();

  TH1F *hMC[3][1000];
  TH1F *hData[3];
  TH1F *hData_cur;
  TH1F *h;

  TString canvasname;
  TCanvas *c;
  int nchi2 = 0;
  float angle = 0;
  double angles[1000];

  for (int iLayer = 0; iLayer<3; iLayer++)
  {
    if (DS[iFile]=="") continue;
    gDirectory->cd(Form("%s",layers[iLayer].Data()));
    nchi2 = 0;

    for (int iCanvas=0; iCanvas<gDirectory->GetListOfKeys()->GetSize(); iCanvas++)
    {
      canvasname = gDirectory->GetListOfKeys()->At(iCanvas)->GetName();
      if (canvasname.Index("canvas")<0) continue;
      canvasname.Remove(0, 7);
  //        cout<<canvasname.Data()<<"123"<<endl;
      angle = canvasname.Atof();
      angles[nchi2] = angle;
      c = (TCanvas*)gDirectory->Get(Form("canvas_%4.1f",angle));
      h = (TH1F*)c->GetPrimitive(Form("hOccMC_%s_%04.1f",layers[iLayer].Data(),angle));
      hMC[iLayer][iCanvas] = new TH1F(*h);
      if (iCanvas == 0)
      {
        h = (TH1F*)c->GetPrimitive(Form("histOcc_data_%s",layers[iLayer].Data()));
        hData[iLayer] = new TH1F(*h);
//         float bc; //bin content
//         double variation; // variation in bin content
//         for (int iBin=0; iBin<6; iBin++)
//         {
//           // ensemble test from here
//           bc = hData -> GetBinContent(iBin+1);
//           variation = r.Gaus(0, TMath::Sqrt(bc));
//   //           cout<<"Bin "<<iBin<<" will be changed from "<<bc<<" by "<<variation<<endl;
//           bc += variation;
//           hData -> SetBinContent(iBin+1, bc);
//           // ensemble test to here
//         }

      }
      nchi2++;
    }
    gDirectory->cd("..");
  }
//   cout<<hMC[1][2]->GetBinContent(3)<<endl;

  TGraph *fChi2[nLayers][nFiles];

//  TF1* fFit = new TF1("MyFit","pol2");
  TF1* fFit = new TF1("MyFit","[0]+[1]*(x-[2])**2");
  Float_t chi2_min_x, chi2_width;

  TCanvas* fCanvas[nLayers][nFiles];
  char* name;
  double pars[3], par_errors[3];
  double min, error, weight;
  TGraph* fTempChi2;
//   ofstream fOut(Form("%s_results_%ddeg_ensembletest.txt",method.Data(), int(fChi2_width_const)), ios_base::app);
  ofstream fOut(Form("ensembletest_%s_%s_results_%ddeg.txt",DS[iFile].Data(), method.Data(), int(fChi2_width_const)));

  double chi2_curr[1000];

  for (int iEns=0; iEns<10000; ++iEns)
  {
    for (int iLayer = 0; iLayer<3; iLayer++)
    {
      hData_cur = new TH1F(*hData[iLayer]);
      float bc; //bin content
      double variation; // variation in bin content
      for (int iBin=0; iBin<6; iBin++)
      {
        // ensemble test from here
        bc = hData_cur -> GetBinContent(iBin+1);
        variation = r.Gaus(0, TMath::Sqrt(bc));
//           cout<<"Bin "<<iBin<<" will be changed from "<<bc<<" by "<<variation<<endl;
        bc += variation;
        hData_cur -> SetBinContent(iBin+1, bc);
        // ensemble test to here
      }
      for (int iCanvas=0; iCanvas<nchi2; iCanvas++)
      {

        chi2_curr[iCanvas] = 0;

        for (int iBin=0; iBin<6; iBin++)
        {
  //         cout<<"Debug:: "<<hData_cur->GetBinContent(iBin+1)<<"  "<<hMC[iLayer][iCanvas]->GetBinContent(iBin+1)<<endl;
          if (hData_cur->GetBinContent(iBin+1)>0)
          {
  //           cout<<pow((hData_cur->GetBinContent(iBin+1)-hMC[iLayer][iCanvas]->GetBinContent(iBin+1)),2)/pow(hData_cur->GetBinContent(iBin+1),2)<<endl;
            if (method=="epsilon")
              chi2_curr[iCanvas] += pow((hData_cur->GetBinContent(iBin+1)-hMC[iLayer][iCanvas]->GetBinContent(iBin+1)),2)/pow(hData_cur->GetBinContent(iBin+1),2);
            else if (method=="chi2")
              chi2_curr[iCanvas] +=
              pow((hData_cur->GetBinContent(iBin+1)-hMC[iLayer][iCanvas]->GetBinContent(iBin+1)),2)/pow(hData_cur->GetBinContent(iBin+1),1)/hData_cur->GetSum();
            else {cout<<"Wrong method"<<endl; exit(0);}
          }
        }
      }
      delete hData_cur;

//       name = Form("c_%s_%s",DS[iFile].Data(),layers[iLayer].Data());
//       fCanvas[iLayer][iFile] = new TCanvas(name,name,100+500*(iFile/3)+20*(iFile%3),50+200*iLayer+20*(iFile%3),400,300);
//       fCanvas[iLayer][iFile]->cd();

      fChi2[iLayer][iFile] = new TGraph(nchi2, angles, chi2_curr);
      fTempChi2 = fChi2[iLayer][iFile];

  //     fTempChi2->SetTitle(layers[iLayer].Data());

  //     cout<<fTempChi2->GetY()[2]<<endl;

//       gPad->SetLeftMargin(0.2);
//       gPad->SetBottomMargin(0.15);
//       gPad->SetTopMargin(0.02);
//       gPad->SetRightMargin(0.01);
//       gPad->SetLogy();
//       gPad->SetTicky();
//       gPad->SetGrid();
//       fTempChi2->SetMarkerStyle(2);
//       fTempChi2->SetMarkerSize(1.2);
//       fTempChi2->SetMarkerColor(6);

      if (method=="epsilon")
      {
        // epsilon:
        fTempChi2->SetMinimum(0.0048);
        fTempChi2->SetMaximum(0.3);
      }
      else
      {
        // chi2:
        fTempChi2->SetMinimum(0.0001);
        fTempChi2->SetMaximum(0.1);
      }
//       fTempChi2->Draw("ap");
//       fTempChi2->GetXaxis()->SetTitle("#phi_{<110>} [deg.]");
//       fTempChi2->GetXaxis()->SetTitleOffset(1);
//       fTempChi2->GetXaxis()->SetTitleSize(0.07);
//       fTempChi2->GetXaxis()->SetLabelSize(0.08);
// 
//       fTempChi2->GetYaxis()->SetTitle("Evaluator, #epsilon");
//       fTempChi2->GetYaxis()->SetTitleOffset(1.12);
//       fTempChi2->GetYaxis()->SetTitleSize(0.09);
//       fTempChi2->GetYaxis()->SetLabelSize(0.1);
// 
//       TPaveText *pave = new TPaveText(0.3, 0.89, 0.7, 0.96,"NDC");
//       pave->SetTextFont(42);
//       pave->SetLineColor(0);
//       pave->SetFillColor(0);
//       pave->SetFillStyle(0);
//       pave->SetTextAlign(12);
//       pave->AddText(Form("%s layer", layers[iLayer].Data()));
//       pave->SetTextSize(0.1);
//       pave->Draw("same");

      min = 0; weight = 0;

      // ***************
      // Now fitting:
      // ***************
      chi2_min_x = fChi2_min_x[iFile][iLayer];
      chi2_width = fChi2_width_const; // fChi2_width[iFile][iLayer];
      fFit->SetRange(chi2_min_x-chi2_width, chi2_min_x+chi2_width);
      if (method=="epsilon")
      {
        fFit->SetParameters(0.02, 5E-4, chi2_min_x);
        fFit->SetParLimits(0, 0, 1.5);
        fFit->SetParLimits(1, 1E-6, 1E-2);
      }
      else
      {
        fFit->SetParameters(0.0015, 1.3E-5, chi2_min_x);
        fFit->SetParLimits(0, 0.001, 0.01);
        fFit->SetParLimits(1, 1E-8, 1E-4);
      }
  //      fFit->SetParLimits(2, chi2_min_x-45, chi2_min_x+45);
      fFit->SetLineColor(1);
      fFit->SetLineWidth(3);
      fTempChi2->Fit("MyFit","WNRMq+");
      fTempChi2->Fit("MyFit","WNRMq+");
//           fTempChi2->Fit("MyFit","WRMq+");
      fTempChi2->Fit("MyFit","WNRMq+");

  //      fTempChi2->Draw("samep");

      fFit->GetParameters(pars);
      for (int i=0; i<fFit->GetNpar(); i++) par_errors[i] = fFit->GetParError(i);
  //      cout<<pars[0]<<"  "<<pars[1]<<"  "<<pars[2]<<endl;

  //      min = -pars[1]/(2*pars[2]);
      min = pars[2];
  //      min = int(min*100)/100.;
  //      weight = 1/(fFit->GetChisquare()/fFit->GetNDF());
      weight = 1/fFit->Eval(min);
  //         cout<<DS[iFile].Data()<<", "<<layers[iLayer].Data()<<",  min at "<<min<<"; "<<weight<<endl;

      fOut<<min<<"\t"<<par_errors[2]<<"\t"<<weight<<"\t\t\t";
//       cout<<min<<"\t"<<par_errors[2]<<"\t"<<weight<<"\t\t\t";

/*      fCanvas[iLayer][iFile]->
          SaveAs(Form("out/%s/%ddeg/Ensemble_Occ_Chi2_%s_%s.eps",method.Data(),int(fChi2_width_const),DS[iFile].Data(),layers[iLayer].Data()));*/
    }
    fOut<<endl;
//     cout<<endl;
  }
  return 1;
}
