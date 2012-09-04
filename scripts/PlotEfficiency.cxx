#include "TROOT.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TText.h"
#include "TLine.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TFile.h"
#include "TAxis.h"

#include <iomanip>
#include <iostream>

using namespace std;

void SetGraphStyle(TGraph *gr, int num)
{
  gr->SetMarkerStyle(22+num);
  gr->SetMarkerSize(1.2);
//  gr->SetMarkerColor(30+num);
  gr->SetMarkerColor(kBlue);
//  gr->SetTitle("");
  gr->GetXaxis()->SetTitle("Segment number");
  gr->GetYaxis()->SetTitle("Relative efficiency");
//  gr->GetXaxis()->SetTitle("Angle [^{o}]");
//  gr->GetYaxis()->SetTitle("#chi^{2}");

  gr->GetXaxis()->SetLabelOffset(99);
};

int PlotEfficiency()
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptTitle(0);


  const int nFiles = 6;
  TString DS[nFiles];
  DS[0] = "OccPlots_top_0.58.root";
  DS[1] = "OccPlots_top_1.17.root";
  DS[2] = "OccPlots_top_1.33.root";
  DS[3] = "OccPlots_top_2.61.root";
  DS[4] = "OccPlots_side_0.58.root";
  DS[5] = "OccPlots_side_2.61.root";

//  DS[5] = "OccPlots_side_GII_0.24.root";
//  DS[6] = "OccPlots_side_GII_2.61.root";
//  Double_t ang[nFiles] = {0,0,0,180,180,90,90};
  Double_t ang[nFiles] = {0,0,0,0,170,170};

  TString legtitle[nFiles];
  legtitle[0] = "Top 0.58 MeV";
  legtitle[1] = "Top 1.17 MeV";
  legtitle[2] = "Top 1.33 MeV";
  legtitle[3] = "Top 2.61 MeV";
  legtitle[4] = "Side 0.58 MeV";
  legtitle[5] = "Side 2.61 MeV";


  const int fNLayers = 3;
  TString layers[fNLayers] = {"top", "middle", "bottom"};

  TFile *f;
  TCanvas *c;
  TH1F *hMC;
  TH1F *hData;

  Int_t chanID[3][6] = {{12,11,10,9,8,7},  // Top layer
                        {15,14,13,6,5,4},  // Middle layer
                        {18,17,16,3,2,1}}; // Bottom layer

  Double_t fRatio[fNLayers][nFiles][6];
  Double_t fRatio_aver[fNLayers][6];
  Double_t fAngleGraph[6][6];
  TGraph *fRatioGraph[fNLayers][nFiles];

  memset(fRatio_aver,0,sizeof(fRatio_aver));

  Float_t y;
  TText t;

  t.SetTextSize(0.04);
  t.SetTextAlign(22);
  TLine *ll[3];

  for (int iSample=0; iSample<6; iSample++)
    for (int iSeg=0; iSeg<6; iSeg++)
      fAngleGraph[iSample][iSeg] = 30 + iSeg*60 + 3*(iSample-3); // To shift values for better performance


  for (int iLayer = 0; iLayer<3; iLayer++)
  {
    TLegend *leg = new TLegend(0.5,0.6,0.95,0.95);
    leg->SetFillColor(0);
//    leg->SetLineColor(0);
    TCanvas *c2 = new TCanvas("c2","c2",5,5,800,600);
    TH1F *h = c2->DrawFrame(0, 0.5, 360, 2);
    h->SetXTitle("Segment number");
    h->SetYTitle("Relative efficiency");
    h->GetXaxis()->SetLabelOffset(99);
    h->GetXaxis()->SetTickLength(0);
    h->GetYaxis()->SetLabelSize(0.06);
    h->GetYaxis()->SetTitleOffset(1.0);
    h->GetYaxis()->SetTitleSize(0.05);
    c2->SetRightMargin(0.02);
    c2->SetTopMargin(0.03);

    ll[iLayer] = new TLine(0, 1, c2->GetUxmax(), 1);
    ll[iLayer] -> SetLineColor(2);
    ll[iLayer] -> Draw();

    for (int iFile = 0; iFile<nFiles; iFile ++)
    {
      if (DS[iFile]=="") continue;
      // Open file ...
      f = new TFile(Form("rootfiles/%s",DS[iFile].Data()),"read");
      // ... get canvas ...
      c = (TCanvas*)f->Get(Form("%s/canvas_%4.1f",layers[iLayer].Data(),ang[iFile]));
      // ... and histograms
      hData = (TH1F*)c->GetPrimitive(Form("histOcc_data_%s",layers[iLayer].Data()));
      hMC   = (TH1F*)c->GetPrimitive(Form("hOccMC_%s_%04.1f",layers[iLayer].Data(), ang[iFile]));
      // Fill ratio array
      for (int i=0; i<6; i++)
      {
        if (hMC->GetBinContent(i+1) > 0)
          fRatio[iLayer][iFile][i] = hData->GetBinContent(i+1)/hMC->GetBinContent(i+1);
        else fRatio[iLayer][iFile][i] = 0;
        if (iFile==1 || iFile==2) fRatio_aver[iLayer][i]+=fRatio[iLayer][iFile][i];
      }
      fRatioGraph[iLayer][iFile] = new TGraph(6, fAngleGraph[iFile], fRatio[iLayer][iFile]);
      fRatioGraph[iLayer][iFile] -> SetTitle(Form("Ratio Data/MC, %s layer",layers[iLayer].Data()));
      SetGraphStyle(fRatioGraph[iLayer][iFile], iFile);

      c2->cd();
      fRatioGraph[iLayer][iFile] -> Draw("psame");
      leg->AddEntry(fRatioGraph[iLayer][iFile], legtitle[iFile].Data(),"p");
      f->Close();
    }
    leg->Draw();

    y = 0.6;
    for (int i=0; i<6; i++)
      t.DrawText(fAngleGraph[3][i],y,Form("%d",chanID[iLayer][i]));

    c2->SaveAs(Form("out/Efficiency_%s_layer.eps", layers[iLayer].Data()));
  }

  for (int iLayer = 0; iLayer<3; iLayer++)
  {
    cout<<layers[iLayer].Data();
    for (int i=0; i<6; i++)
    {
      cout<<" & \\bf{"<<chanID[iLayer][i]<<"} & "<<setprecision(3)<<fRatio_aver[iLayer][i]/2.;
    }
    cout<<" \\\\"<<endl;
  }

  return 1;
}
