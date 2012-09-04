#include "/home/pclg-23/volynets/.root/macros/logon.C"

int CombineAngles()
{

  SetStyles();
//  gROOT->SetStyle("Plain");
  gStyle->SetLabelFont(42,"XYZ");
  gStyle->SetTitleFont(42,"XYZ");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);


    float energy = 2.61;
    TString ccase = "Side";

//     float energy = 1.33;
//     TString ccase = "Top";

  const int nFiles = 2;
  int angles[nFiles] = {-20,30};
  TString DS[nFiles];
  for (int i=0; i<nFiles;i++)
  {
    DS[i] = Form("%s_Occ_MC_demonstr_%d_%4.2f.root",ccase.Data(), angles[i], energy);
  }

  // To draw segment boundaries
  Float_t y_t;
  TLine *lines[5];
  Int_t chanID[6] = {15,14,13,6,5,4};
  float axesyranges;
  if (energy==(float)1.33)
    axesyranges = 165000.;
  else
    axesyranges = 8990.;

  Float_t y1, y2;
  Float_t x;
  TText t;
  t.SetTextAlign(22);
//  t.SetTextFont(42);

  TFile *f;
  TCanvas *c;
  TH1F *hMC[nFiles];

  for (int iFile = 0; iFile<nFiles; iFile ++)
  {
    if (DS[iFile] == "") continue;
    f = new TFile(Form("%s",DS[iFile].Data()),"read");
    c = (TCanvas*)f->Get("c1");
    hMC[iFile] = (TH1F*)c->GetPrimitive(Form("hOccMC_%d",angles[iFile]));
  }
  TCanvas *c2 = new TCanvas("c1", "c1", 50, 50, 1600, 600);
  c2->SetRightMargin(0.0);
  c2->SetLeftMargin(0.0);
  c2->SetTopMargin(0.0);
  c2->SetBottomMargin(0.0);
  TPad *pad[2];
  pad[0] = new TPad("c1_1", "c1_1", 0., 0., 0.55, 1.);
  pad[1] = new TPad("c1_2", "c1_2", 0.55, 0., 1., 1.);

  TGaxis::SetMaxDigits(4);

  c2->cd();
  pad[0]->Draw();
  c2->cd();
  pad[1]->Draw();

  pad[0]->SetLeftMargin(0.1/0.55);
  pad[1]->SetLeftMargin(0.01);
  pad[0]->SetRightMargin(0.01);
  pad[1]->SetRightMargin(0.01);
//   c2->Divide(2,1,0.0,0.0);

  for (int iFile=0; iFile<2; ++iFile)
  {
    pad[iFile]->cd();
    gPad->SetBottomMargin(0.15);
    gPad->SetTopMargin(0.1);
    gPad->SetTicky();
    hMC[iFile]->SetLineColor(2);
    hMC[iFile]->SetLineWidth(1);
    hMC[iFile]->GetXaxis()->SetTitleOffset(0.86);
    hMC[iFile]->GetXaxis()->SetTitleSize(0.08);
    hMC[iFile]->GetXaxis()->SetLabelSize(0.05);

    hMC[iFile]->GetYaxis()->SetNdivisions(30505);
    hMC[iFile]->GetYaxis()->SetLabelSize(0.07);
    hMC[iFile]->GetYaxis()->SetTitleOffset(0.76);
    hMC[iFile]->GetYaxis()->SetTitleSize(0.12);
    hMC[iFile]->GetYaxis()->SetLabelOffset(0.01);
    if (iFile==1)
    {
//       gPad->SetLeftMargin(0.01);
      hMC[iFile]->GetYaxis()->SetLabelOffset(99);
      hMC[iFile]->GetYaxis()->SetTitleOffset(99);
    }

    hMC[iFile]->Draw();
    cout<<hMC[iFile]->GetMaximum()<<endl;
//     hMC[iFile]->GetYaxis()->SetRangeUser(0,1.2*hMC[iFile]->GetMaximum());
    hMC[iFile]->GetYaxis()->SetRangeUser(0.,axesyranges);
//     continue;

    gPad->Update();
    y_t = gPad->GetUymin()+0.1*(gPad->GetUymax()-gPad->GetUymin());

    for (int i=0; i<6; i++)
    {
      x = i*60+30;
      t.DrawText(x,y_t,Form("%d",chanID[i]));

      x = (i+1)*60;
      y1 = gPad->GetUymin();
      float binc1 = hMC[iFile]->GetBinContent(hMC[iFile]->FindBin(x)-1);
      float binc2 = hMC[iFile]->GetBinContent(hMC[iFile]->FindBin(x));
      y2 = (binc1>binc2 ? binc2 : binc1);
      if (i==5) continue;
      lines[i] = new TLine(x,y1,x,y2);
      lines[i]->SetLineStyle(9);
      lines[i]->SetLineColor(2);
      lines[i]->SetLineWidth(2);
      lines[i]->Draw();
    }
/*    TLatex tl;
    tl.SetTextAlign(22);
    x = 0.3;
    y1 = 0.55;
    tl.SetNDC();
    tl.DrawLatex(x, y1, "#phi_{#langle 110 #rangle}=-12");*/
//     tl.DrawLatex(x, y1, "#phi_{<110>}=12");
    TPaveText *pave = new TPaveText(0.3,0.81,0.7,0.89, "NDC");
    pave->SetFillColor(0);
    pave->SetTextColor(kBlue);
    pave->SetTextFont(42);
    pave->AddText(Form("#phi_{<110>}= %d", angles[iFile]));
    pave->SetFillStyle(0);
    pave->SetLineColor(0);
    pave->Draw("same");
  }
  c2->SaveAs(Form("%s_Occ_MC_demonstr_%4.2f.eps",ccase.Data(),energy));




//   t.SetTextSize(0.05);
//   t.SetTextAlign(22);
// //  t.SetTextColor(4);
//   gPad->SetRightMargin(0.01);
// //  gPad->SetTopMargin(0.01);
//   gPad->Update();
//   y_t = gPad->GetUymin()+0.1*(gPad->GetUymax()-gPad->GetUymin());
// 
//   for (int i=0; i<6; i++)
//   {
//     x = i*60+30;
//     t.DrawText(x,y_t,Form("%d",chanID[i]));
// 
//     x = (i+1)*60;
//     y1 = gPad->GetUymin();
//     y2 = hMC[0]->GetBinContent(hOccMC->FindBin(x));
//     if (i==5) continue;
//     lines[i] = new TLine(x,y1,x,y2);
//     lines[i]->SetLineStyle(9);
//     lines[i]->SetLineColor(2);
//     lines[i]->SetLineWidth(3);
//     lines[i]->Draw();
//   }

}
