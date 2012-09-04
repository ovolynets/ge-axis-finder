#include "TH1F.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TFile.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TPaveText.h"
#include "TString.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

#include "/home/pclg-23/volynets/.root/macros/logon.C"

using namespace std;

void SetGraphStyle(TGraph *gr, int num)
{
  gr->SetMarkerStyle(21+num);
  gr->SetMarkerSize(1);
  gr->SetMarkerColor(30+num);
//  gr->SetTitle("");
//  gr->GetXaxis()->SetTitle("Angle [^{o}]");
//  gr->GetYaxis()->SetTitle("#chi^{2}");
  gr->GetXaxis()->SetRangeUser(0,360);
//   gr->GetYaxis()->SetRangeUser(0.5,2);
  gr->GetYaxis()->SetRangeUser(0.001,0.1);

  gr->GetXaxis()->SetLabelOffset(99);
};

int CalcFitChi2()
{
  SetStyles();

  TRandom3 r;
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
  const Float_t fChi2_min_x[nFiles][nLayers] = { {-12,-10, 13},
                                                 {-10, 0, -5},
                                                 {-8, 5, 2},
                                                 { 0,-8, -5},
/*                                                 {160,170,165}, // This is for background-ignored
                                                 {165,170,165},*/
                                                 {160,165,158}, // This is for background-subtracted
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

/*
// For chi2 criteria
    const Float_t fChi2_min_x[nFiles][nLayers] = { {-10,-8, 10},
                                                 {-10, 0, -5},
                                                 {-8, 5, 0},
                                                 { 0,  -5, -5},
                                                 {150,170,160},
                                                 {155,165,160},
                                                 {0,0,0},
                                                 {0,0,0} };
*/
  const Float_t fChi2_width_const = 10;

/*
  const Float_t fChi2_width[nFiles][nLayers] = { {10, 10, 10},
                                                 {10, 10, 10},
                                                 {10, 10, 10},
                                                 {10, 10, 10},
                                                 {10, 10, 10},
                                                 {10, 10, 10},
                                                 {10, 10, 10},
                                                 {10, 10, 10} };
*/

  TFile *f;
  TH1F *hMC;
  TH1F *hData;

  TGraph *fChi2[nLayers][nFiles];

//  TF1* fFit = new TF1("MyFit","pol2");
  TF1* fFit = new TF1("MyFit","[0]+[1]*(x-[2])**2");
  Float_t chi2_min_x, chi2_width;

  TCanvas* fCanvas[nLayers][nFiles];
  char* name;
  double pars[3], par_errors[3];
  double min, error, weight;
  TGraph* fTempChi2;
  ofstream fOut(Form("%s_results_%ddeg.txt",method.Data(), int(fChi2_width_const)));
  ofstream fOut_tex(Form("%s_results_tex_%ddeg.txt",method.Data(), int(fChi2_width_const)));

  TCanvas *c;
  TH1F *h;
  double chi2_curr[1000];
  int nchi2 = 0;
  TString canvasname;
  float angle = 0;
  double angles[1000];

//   int iFile = 5;
//  for (int iFile = 0; iFile<6; iFile ++)
   for (int iFile = 0; iFile<13; iFile ++)
  {
    fOut_tex<<columnprefix[iFile].Data();
//    fOut_tex<<layers[iLayer];
//    for (int iFile = 0; iFile<nFiles; iFile ++)
    for (int iLayer = 0; iLayer<3; iLayer++)
    {
      if (DS[iFile]=="") continue;
//      cout<<Form("rootfiles/OccPlots_%s.root",DS[iFile].Data())<<endl;
      f = new TFile(Form("rootfiles/OccPlots_%s.root",DS[iFile].Data()),"read");
//       f = new TFile(Form("rootfiles/bu_no9seg/OccPlots_%s.root",DS[iFile].Data()),"read");
      gDirectory->cd(Form("%s",layers[iLayer].Data()));
      nchi2 = 0;
      for (int iCanvas=0; iCanvas<gDirectory->GetListOfKeys()->GetSize(); iCanvas++)
      {
        canvasname = gDirectory->GetListOfKeys()->At(iCanvas)->GetName();
        if (canvasname.Index("canvas")<0) continue;
        canvasname.Remove(0, 7);
//        cout<<canvasname.Data()<<"123"<<endl;
        angle = canvasname.Atof();
//        cout<<angle<<endl;
        angles[nchi2] = angle;
        c = (TCanvas*)gDirectory->Get(Form("canvas_%4.1f",angle));
        hMC = (TH1F*)c->GetPrimitive(Form("hOccMC_%s_%04.1f",layers[iLayer].Data(),angle));
        hData = (TH1F*)c->GetPrimitive(Form("histOcc_data_%s",layers[iLayer].Data()));
//        cout<<angle<<"  "<<Form("hOcc_data_%s",layers[iLayer].Data())<<endl;
        chi2_curr[nchi2] = 0;
//         cout<<hData->GetSum()<<endl;

//         // Ensemble test
//         // Vary the data bin content by gaus(0, sigma = sqrt(bincontent))
//         cout<<"--------------------------------------"<<endl;
//         cout<<"**************************************"<<endl;
//         cout<<"--------------------------------------"<<endl;
//         cout<<endl;
//         cout<<"Warning: ensemble test. Data bin content is varied by +-sigma"<<endl;
//         cout<<endl;
//         cout<<"--------------------------------------"<<endl;
//         cout<<"**************************************"<<endl;
//         cout<<"--------------------------------------"<<endl;
//         float bc; //bin content
//         double variation; // variation in bin content

        for (int iBin=0; iBin<6; iBin++)
        {
//           // ensemble test from here
//           bc = hData -> GetBinContent(i+1);
//           variation = r.Gaus(0, TMath::Sqrt(bc));
//           cout<<"Bin "<<i<<" will be changed from "<<bc<<" by "<<variation<<endl;
//           bc += variation;
//           hData -> SetBinContent(i+1, bc);
//           // ensemble test to here

          if (hData->GetBinContent(iBin+1)>0)
          {
            if (method=="epsilon")
              chi2_curr[nchi2] += pow((hData->GetBinContent(iBin+1)-hMC->GetBinContent(iBin+1)),2)/pow(hData->GetBinContent(iBin+1),2);
            else if (method=="chi2")
              chi2_curr[nchi2] += pow((hData->GetBinContent(iBin+1)-hMC->GetBinContent(iBin+1)),2)/pow(hData->GetBinContent(iBin+1),1)/hData->GetSum();
            else {cout<<"Wrong method"<<endl; exit(0);}
          }
//            chi2_curr[nchi2] += pow(hData->GetBinContent(iBin+1)-hMC->GetBinContent(iBin+1),2)/hData->GetBinContent(iBin+1) ;
        }
        nchi2++;
        delete c;
      }

      name = Form("c_%s_%s",DS[iFile].Data(),layers[iLayer].Data());
      fCanvas[iLayer][iFile] = new TCanvas(name,name,100+500*(iFile/3)+20*(iFile%3),50+200*iLayer+20*(iFile%3),400,300);
//      fCanvas[iLayer][iFile]->cd();
      fChi2[iLayer][iFile] = new TGraph(nchi2, angles, chi2_curr);
      fTempChi2 = fChi2[iLayer][iFile];

//      fTempChi2 = (TGraph*)f->Get(Form("%s/Graph",layers[iLayer].Data()));
//      fChi2[iLayer][iFile] = fTempChi2;
      fTempChi2->SetTitle(layers[iLayer].Data());

//      cout<<fTempChi2->GetY()[2]<<endl;
      gPad->SetLeftMargin(0.2);
      gPad->SetBottomMargin(0.15);
      gPad->SetTopMargin(0.02);
      gPad->SetRightMargin(0.01);
      gPad->SetLogy();
      gPad->SetTicky();
      gPad->SetGrid();
      fTempChi2->SetMarkerStyle(2);
      fTempChi2->SetMarkerSize(1.2);
      fTempChi2->SetMarkerColor(6);

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
      fTempChi2->Draw("ap");
      fTempChi2->GetXaxis()->SetTitle("#phi_{<110>} [deg.]");
      fTempChi2->GetXaxis()->SetTitleOffset(1);
      fTempChi2->GetXaxis()->SetTitleSize(0.07);
      fTempChi2->GetXaxis()->SetLabelSize(0.08);

      fTempChi2->GetYaxis()->SetTitle("Test statistic, #epsilon");
      fTempChi2->GetYaxis()->SetTitleOffset(1.12);
      fTempChi2->GetYaxis()->SetTitleSize(0.09);
      fTempChi2->GetYaxis()->SetLabelSize(0.1);

      TPaveText *pave = new TPaveText(0.3, 0.89, 0.7, 0.96,"NDC");
      pave->SetTextFont(42);
      pave->SetLineColor(0);
      pave->SetFillColor(0);
      pave->SetFillStyle(0);
      pave->SetTextAlign(12);
      pave->AddText(Form("%s layer", layers[iLayer].Data()));
      pave->SetTextSize(0.1);
      pave->Draw("same");

      min = 0; weight = 0;
//      if (iLayer == 0 && iFile==3) fTempChi2->SetMaximum(1400);
/*
      if (iLayer == 0 && iFile==3)
      {
        fTempChi2->SetMaximum(8);
      }
*/
/*
      else if (iLayer == 2 && iFile==5)
      {
        fTempChi2->SetMaximum(0.1);
      }
*/
//      else
      {

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
        fTempChi2->Fit("MyFit","WRMq+");

  //      fTempChi2->Draw("samep");

        fFit->GetParameters(pars);
        for (int i=0; i<fFit->GetNpar(); i++) par_errors[i] = fFit->GetParError(i);
  //      cout<<pars[0]<<"  "<<pars[1]<<"  "<<pars[2]<<endl;

  //      min = -pars[1]/(2*pars[2]);
        min = pars[2];
  //      min = int(min*100)/100.;
  //      weight = 1/(fFit->GetChisquare()/fFit->GetNDF());
        weight = 1/fFit->Eval(min);
        cout<<DS[iFile].Data()<<", "<<layers[iLayer].Data()<<",  min at "<<min<<"; "<<weight<<endl;
      }
      fOut<<min<<"\t"<<par_errors[2]<<"\t"<<weight<<"\t\t\t";
      fOut_tex<<setprecision(4)<<" & \\textbf{"<<min<<"$\\pm$ "<<setprecision(3)<<par_errors[2]<<"} & "<<setprecision(3)<<weight;
//      fOut_tex<<setprecision(4)<<" & "<<min<<"$\\pm$ "<<setprecision(3)<<par_errors[2]<<" & "<<setprecision(3)<<weight;

       fCanvas[iLayer][iFile]->SaveAs(Form("out/%s/%ddeg/Occ_Chi2_%s_%s.eps",method.Data(),int(fChi2_width_const),DS[iFile].Data(),layers[iLayer].Data()));

      f->Close();
    }

    fOut<<endl;
    fOut_tex<<" \\\\"<<endl;

//    delete c2;
  }
  return 1;
}
