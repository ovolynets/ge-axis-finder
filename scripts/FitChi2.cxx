#include <iomanip>

void SetGraphStyle(TGraph *gr, int num)
{
  gr->SetMarkerStyle(21+num);
  gr->SetMarkerSize(1);
  gr->SetMarkerColor(30+num);
//  gr->SetTitle("");
//  gr->GetXaxis()->SetTitle("Angle [^{o}]");
//  gr->GetYaxis()->SetTitle("#chi^{2}");
  gr->GetXaxis()->SetRangeUser(0,360);
  gr->GetYaxis()->SetRangeUser(0.5,2);

  gr->GetXaxis()->SetLabelOffset(99);
};

int FitChi2()
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptTitle(0);


  const int nFiles = 100;
  TString DS[nFiles];
  DS[0] = "top_0.58";
  DS[1] = "top_1.17";
  DS[2] = "top_1.33";
  DS[3] = "top_2.61";
  DS[4] = "side_0.58";
  DS[5] = "side_2.61";
  DS[6] = "side_GII_0.24";
  DS[7] = "side_GII_2.61";
  Double_t ang[nFiles] = {0,0,0,0,180,180,90,90};

  const int nLayers = 3;
  TString layers[nLayers] = {"top", "middle", "bottom"};

  // For epsilon criteria

  const Float_t fChi2_min[nFiles][nLayers] = { {-10,-5, 10},
                                               {-10, 0, -5},
                                               {-10, 0, -2},
                                               { 0,  0, 0},
                                               {150,170,160},
                                               {160,170,165},
                                               {0,0,0},
                                               {0,0,0} };
/*
// For chi2 criteria
  const Float_t fChi2_min[nFiles][nLayers] = { {-10,-8, 10},
                                               {-10, 0, -5},
                                               {-8, 5, 0},
                                               { 0,  -5, -5},
                                               {150,170,160},
                                               {155,165,160},
                                               {0,0,0},
                                               {0,0,0} };
*/
  const Float_t fChi2_width[nFiles][nLayers] = { {10, 10, 10},
                                                 {10, 10, 10},
                                                 {10, 10, 10},
                                                 {10, 10, 10},
                                                 {10, 10, 10},
                                                 {10, 10, 10},
                                                 {0,0,0},
                                                 {0,0,0} };

  TFile *f;
  TH1F *hMC;
  TH1F *hData;

  Int_t chanID[3][6] = {{12,11,10,9,8,7},  // Top layer
                        {15,14,13,6,5,4},  // Middle layer
                        {18,17,16,3,2,1}}; // Bottom layer

  TGraph *fChi2[nLayers][nFiles];

  TF1* fFit = new TF1("MyFit","pol2");
  Float_t chi2_min, chi2_width;

  TCanvas* fCanvas[nLayers][nFiles];
  char* name;
  double pars[3], par_errors[3];
  double min, weight;
  TGraph* fTempChi2;
  ofstream fOut("results.txt");
  ofstream fOut_tex("results_tex.txt");

  for (int iLayer = 0; iLayer<3; iLayer++)
  {
    fOut_tex<<layers[iLayer];
//    for (int iFile = 0; iFile<nFiles; iFile ++)
    for (int iFile = 0; iFile<6; iFile ++)
    {
      name = Form("c_%s_%s",DS[iFile].Data(),layers[iLayer].Data());
      fCanvas[iLayer][iFile] = new TCanvas(name,name,100+500*(iFile/3)+20*(iFile%3),50+200*iLayer+20*(iFile%3),500,400);

      if (DS[iFile]=="") continue;
      f = new TFile(Form("rootfiles/epsilon/OccPlots_%s.root",DS[iFile].Data()),"read");
      fTempChi2 = (TGraph*)f->Get(Form("%s/Graph",layers[iLayer].Data()));
      fChi2[iLayer][iFile] = fTempChi2;
      fTempChi2->SetTitle(layers[iLayer].Data());

//      cout<<fTempChi2->GetY()[2]<<endl;
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.15);
      gPad->SetTopMargin(0.02);
      gPad->SetRightMargin(0.01);
      fTempChi2->SetMarkerStyle(2);
      fTempChi2->SetMarkerSize(1.2);
      fTempChi2->SetMarkerColor(6);
      fTempChi2->SetMinimum(0);
      if (iLayer == 0 && iFile==3) fTempChi2->SetMaximum(1400);
      fTempChi2->Draw("ap");
      fTempChi2->GetXaxis()->SetTitle("Angle [deg.]");
      fTempChi2->GetXaxis()->SetTitleOffset(0.9);
      fTempChi2->GetXaxis()->SetTitleSize(0.06);
      fTempChi2->GetXaxis()->SetLabelSize(0.05);

      fTempChi2->GetYaxis()->SetTitle("#chi^{2}");
      fTempChi2->GetYaxis()->SetTitleOffset(0.8);
      fTempChi2->GetYaxis()->SetTitleSize(0.09);
      fTempChi2->GetYaxis()->SetLabelSize(0.05);


      // ***************
      // Now fitting:
      // ***************
      chi2_min = fChi2_min[iFile][iLayer];
      chi2_width = fChi2_width[iFile][iLayer];
      fFit->SetRange(chi2_min-chi2_width, chi2_min+chi2_width);
      fFit->SetParameters(0,1,1);
      fFit->SetLineColor(1);
      fFit->SetLineWidth(3);
      fTempChi2->Fit("MyFit","Rq+");

      TPaveText *pave = new TPaveText(0.45, 0.9, 0.65, 0.97,"NDC");
      pave->SetLineColor(0);
      pave->SetFillColor(0);
      pave->AddText(layers[iLayer]);
      pave->SetTextSize(0.1);
      pave->Draw("same");
//      fTempChi2->Draw("samep");

      fFit->GetParameters(pars);
      for (int i=0; i<fFit->GetNpar(); i++) par_errors[i] = fFit->GetParError(i);
//      cout<<pars[0]<<"  "<<pars[1]<<"  "<<pars[2]<<endl;

      min = -pars[1]/(2*pars[2]);
      min = int(min*100)/100.;
      weight = 1/(fFit->GetChisquare()/fFit->GetNDF());
//      cout<<DS[iFile].Data()<<", "<<layers[iLayer].Data()<<",  min at "<<min<<"; "<<weight<<endl;
      fOut<<min<<"\t"<<weight<<"\t\t\t";
      fOut_tex<<setprecision(4)<<" & \\textbf{"<<min<<"} & "<<setprecision(2)<<weight;

      fCanvas[iLayer][iFile]->SaveAs(Form("out/Occ_Chi2_%s_%s.eps",DS[iFile].Data(),layers[iLayer].Data()));

//      SetGraphStyle(fRatioGraph[iLayer][iFile], iFile);
      f->Close();
    }

    fOut<<endl;
    fOut_tex<<" \\\\"<<endl;

//    c2->SaveAs(Form("out/graph_%s.eps", layers[iLayer].Data()));
//    delete c2;
  }
  return 1;
}
