#include "/home/pclg-23/volynets/.root/macros/logon.C"
// #include "/home/alex/.root/macros/logon.C"

int GetCanvas()
{

  SetStyles();
//  gROOT->SetStyle("Plain");
  gStyle->SetLabelFont(42,"XYZ");
  gStyle->SetTitleFont(42,"XYZ");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

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
//   Double_t ang[nFiles] = {0,0,0,0,170,170};
  Double_t ang[nFiles][3] = {
// TOP
//                               {-11,  -8,  11}, // 0.58 -> // Background-ignored values
                              {-11,  -8,  14},    // 0.58 -> // Background-subtracted values

                              { -8,   1,  -4},    // 1.17
                              { -7,   4,   1},    // 1.33
//                               {  0,  -6,  -5},    // 2.61 -> //Background-ignored values
                              {  0,  -6,  -6},    // 2.61 -> // Background-subtracted values

// SIDE
//                              {159, 170, 164},  // 0.58 -> // Background-ignored values
                              {158, 166, 156},    // 0.58 -> // Baclground-subtracted values

//                              {167, 170, 168}   // 2.61 -> // Background-ignored values
                              {162, 164, 164}     // 2.61 -> // Baclground-subtracted values
                              };

  TString layers[3] = {"top", "middle", "bottom"};

  TFile *f;
  TCanvas *c;
  TH1F *hMC, *hData;

  TLine *lines[3][6];
  Int_t chanID[3][6] = {{12,11,10,9,8,7},  // Top layer
                        {15,14,13,6,5,4},  // Middle layer
                        {18,17,16,3,2,1}}; // Bottom layer
  Float_t y;
  Float_t y2;
  Float_t x;
  TText t;

  t.SetTextFont(42);
  t.SetTextSize(0.07);
  t.SetTextAlign(22);
  TGaxis::SetMaxDigits(4);

  int iFile = 3;
//  for (int iFile = 0; iFile<nFiles; iFile ++)
  {
    if (DS[iFile] == "") continue;
    f = new TFile(Form("rootfiles/%s",DS[iFile].Data()),"read");
    for (int iLayer = 0; iLayer<3; iLayer++)
    {
      c = (TCanvas*)f->Get(Form("%s/canvas_%4.1f",layers[iLayer].Data(),ang[iFile][iLayer]));
      hData = (TH1F*)c->GetPrimitive(Form("histOcc_data_%s",layers[iLayer].Data()));
      hMC   = (TH1F*)c->GetPrimitive(Form("hOccMC_%s_%04.1f",layers[iLayer].Data(), ang[iFile][iLayer]));

      TCanvas *c2 = new TCanvas("c1", "c1", 50, 50, 800, 600);
      c2->cd();
      c2->SetRightMargin(0.01);
      c2->SetLeftMargin(0.2);
      c2->SetTopMargin(0.1);

      hData->SetLineColor(kBlue);
      hMC->SetLineColor(kRed);
      hData->SetLineWidth(2);
      hMC->SetLineWidth(2);
      hMC->SetLineStyle(9);

      TLegend *leg = new TLegend(0.4, 0.25, 0.8, 0.5);
      leg->SetFillStyle(0);
      leg->SetFillColor(0);
      leg->SetTextFont(42);
//      leg->SetBorderColor(0);
//      TLegend *leg = new TLegend(0.5, 0.11, 0.89, 0.4);
      leg->AddEntry(hData,"Measured","l");
      leg->AddEntry(hMC,"Simulated","l");
      hData->Draw("hist");

      hData->GetXaxis()->SetTitleSize(0.07);
      hData->GetXaxis()->SetTitleOffset(0.55);
      hData->GetXaxis()->SetLabelOffset(99);
      hData->GetXaxis()->SetTickLength(0);

      hData->GetYaxis()->SetTitleSize(0.09);
      hData->GetYaxis()->SetTitleOffset(1.15);
      hData->GetYaxis()->SetLabelSize(0.07);
      hData->GetYaxis()->SetNdivisions(505);
      hData->GetYaxis()->SetRangeUser(0, 1.2*hData->GetMaximum());
      hMC->Draw("samehist");
      leg->Draw();
      c2->Update();
      TPad *fPad = c2->GetPad(0);
//      cout<<fPad->GetUymin()<<"  "<<fPad->GetUymax()<<endl;

      y = fPad->GetUymin()+0.05*(fPad->GetUymax()-fPad->GetUymin());
      for (int i=0; i<6; i++)
      {
        x = 30+i*60;
        t.DrawText(x,y,Form("%d",chanID[iLayer][i]));
        if (i==5) continue;
        x = (i+1)*60.;

/*
        y2 = fPad->GetUymax();
        lines[iLayer][i] = new TLine(x,0,x,y2);
        lines[iLayer][i]->SetLineStyle(9);
        lines[iLayer][i]->SetLineColor(2);
        lines[iLayer][i]->SetLineWidth(3);
        lines[iLayer][i]->Draw();
*/
      }
      TPaveText *pave = new TPaveText(0.4, 0.88, 0.9, 0.89, "NDC");
      pave->SetTextFont(42);
      pave->SetLineColor(0);
      pave->SetFillColor(0);
//       pave->SetFillStyle(0);
      pave->SetTextAlign(13);
      pave->SetTextSize(0.08);
      TString ptext = "";
      if (DS[iFile].Index("top")>-1)
        ptext += "TOP, ";
      else
        ptext += "SIDE, ";
      ptext += Form("%s layer", layers[iLayer].Data());
      pave->AddText(ptext);
      pave->Draw("same");
      c2->SaveAs(Form("out/%s_%s.eps",DS[iFile].Data(), layers[iLayer].Data()));
//      delete c2;
    }
//    f->Close();
  }


/*
//  f1->cd("middle");
  TGraph *g1 = (TGraph*) f1->Get("middle/Graph");
  g1->SetMarkerStyle(3);
  g1->SetMarkerSize(1);
  g1->Draw("ap");
  g1->SetTitle("");
  g1->GetXaxis()->SetTitle("Angle [^{o}]");
  g1->GetYaxis()->SetTitle("#chi^{2}");
  c1->SaveAs(Form("%s.eps",name));
  g1->GetXaxis()->SetRangeUser(0,10);
*/
  
}
