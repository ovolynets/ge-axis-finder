#include "/home/pclg-23/volynets/.root/macros/logon.C"

void PlotOccInData()
{

  SetStyles();

  // Temporary variable for fit names
//   TString fCase = "side";
//   TString fCase = "top";
//   TString fSourceName = "Co60";
//   TString fRunName = "Run06";
//   float fPeakEnergy = 1.17;

//   TString fCase = "top";
//   TString fSourceName = "Th228";
//   TString fRunName = "Run06";
//   float fPeakEnergy = 0.58;

  TString fCase = "side";
  TString fSourceName = "Th228";
  TString fRunName = "Run07";
//  float fPeakEnergy = 0.58;
  float fPeakEnergy = 2.61;


  TString fLayerName[3];
  fLayerName[0] = TString("bottom");
  fLayerName[1] = TString("middle");
  fLayerName[2] = TString("top");

  //
  // Occupancy plotting
  //
  gStyle->SetOptTitle(0);

  const int fNLayers = 3;
  TCanvas *c2[fNLayers];
  TString cname;

  TFile *fileOcc;
  TH1F *histOcc[fNLayers];
  TLine *lines[fNLayers][6];
  Int_t chanID[fNLayers][6] = {{18,17,16,3,2,1},   // Bottom layer
                               {15,14,13,6,5,4},   // Middle layer
                               {12,11,10,9,8,7} }; // Top layer

  fileOcc = new TFile(Form("../out/%s/data/Occ_data_%s_%s_%4.2f.root", fRunName.Data(), fCase.Data(), fSourceName.Data(), fPeakEnergy),"read");

  Float_t y;
  Float_t y2;
  Float_t x;
  TText t;

  // Write the core histogram

  fileOcc->cd();

  for (int iLayer = 0; iLayer<fNLayers; iLayer++)
  {
    cname = Form("c2_%s",fLayerName[iLayer].Data());
    c2[iLayer] = new TCanvas(cname.Data(),cname.Data(),5,5,1024,800);
    c2[iLayer] -> SetLeftMargin(0.2);
    c2[iLayer] -> SetTopMargin(0.1);
    c2[iLayer] -> SetBottomMargin(0.14);

    cout<<"Layer being processed: "<<fLayerName[iLayer]<<endl;

    histOcc[iLayer] = (TH1F*)fileOcc->Get(Form("%s/histOcc_data_%s",fLayerName[iLayer].Data(), fLayerName[iLayer].Data()));
/*    if (histOcc[iLayer]->GetMaximum()<1200)
      c2[iLayer] -> SetTopMargin(0.1);*/
    TGaxis::SetMaxDigits(3);
    histOcc[iLayer]->SetStats(0);
    histOcc[iLayer]->SetLineWidth(2);
    histOcc[iLayer]->SetLineColor(kBlue);
    histOcc[iLayer]->GetXaxis()->SetLabelOffset(99);
    histOcc[iLayer]->GetXaxis()->SetTitleSize(0.07);
    histOcc[iLayer]->GetXaxis()->SetTitleOffset(0.9);
    histOcc[iLayer]->GetXaxis()->SetTickLength(0.);

    histOcc[iLayer]->GetYaxis()->SetNdivisions(30505);
//     histOcc[iLayer]->GetYaxis()->SetNdivisions(30505);
    histOcc[iLayer]->GetYaxis()->SetLabelSize(0.09);
    histOcc[iLayer]->GetYaxis()->SetTitleSize(0.08);
    histOcc[iLayer]->GetYaxis()->SetTitleOffset(1.2);

    t.SetTextFont(42);
    t.SetTextSize(0.06);
    t.SetTextAlign(22);

    histOcc[iLayer]->GetYaxis()->SetRangeUser(0,1.1*histOcc[iLayer]->GetMaximum());
    histOcc[iLayer]->Draw("hist");

    c2[iLayer]->Update();
    y = gPad->GetUymin() - 0.04*( gPad->GetUymax() - gPad->GetUymin() );
    for (int i=0; i<6; i++)
    {
      x = histOcc[iLayer] -> GetXaxis() -> GetBinCenter(i+1);
      t.DrawText(x,y,Form("%d",chanID[iLayer][i]));

      if (i==5) continue;
      x = (i+1)*60.;
      y2 = (   histOcc[iLayer]->GetBinContent(i+1)  >  histOcc[iLayer]->GetBinContent(i+2)
             ? histOcc[iLayer]->GetBinContent(i+2)
             : histOcc[iLayer]->GetBinContent(i+1) );
      lines[iLayer][i] = new TLine(x,0,x,y2);
      lines[iLayer][i]->SetLineStyle(9);
      lines[iLayer][i]->SetLineColor(kRed);
      lines[iLayer][i]->SetLineWidth(3);
      lines[iLayer][i]->Draw();
    }

//     c2[iLayer]->SaveAs("123.eps");
    c2[iLayer]->SaveAs(Form("out/dataplots/Occ_data_%s_%s_%4.2f_%s_layer.eps", fCase.Data(), fSourceName.Data(), fPeakEnergy, fLayerName[iLayer].Data()));
  }

  return ;
}
