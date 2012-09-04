// #include "/home/alex/.root/macros/logon.C"
#include "/home/pclg-23/volynets/.root/macros/logon.C"

int GetCanvas_combine_5mmoff()
{

  SetStyles();
//  gROOT->SetStyle("Plain");
  gStyle->SetLabelFont(42,"XYZ");
  gStyle->SetTitleFont(42,"XYZ");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

  const int nFiles = 5;
  TString path[nFiles];
//   float energy = 1.33;
  float energy = 2.61;
  path[0] = Form("OccPlots_top_%4.2f.root",energy);

  Float_t yaxislimits1[3][2];
  Float_t yaxislimits2[3];

  path[1] = Form("OccPlots_top_%4.2f_5mmoff_45deg.root",energy);
  path[2] = Form("OccPlots_top_%4.2f_5mmoff_90deg.root",energy);
//   if (energy==(float)1.33)
//   {
//     // By layer
//     yaxislimits1[0][0] = 28500.; yaxislimits1[0][1] = 62000.;
//     yaxislimits1[1][0] = 12200.; yaxislimits1[1][1] = 25990.;
//     yaxislimits1[2][0] = 5200.; yaxislimits1[2][1] = 11200.;
// 
//     // By file
//     yaxislimits2[0] = 690.;
//     yaxislimits2[1] = 390.;
//     yaxislimits2[2] = 190.;
//   }
//   else if (energy==(float)2.61)
//   {
//     // By layer
//     yaxislimits1[0][0] = 420.; yaxislimits1[0][1] = 920.;
//     yaxislimits1[1][0] = 220.; yaxislimits1[1][1] = 470.;
//     yaxislimits1[2][0] = 122.; yaxislimits1[2][1] = 259.;
// 
//     // By file
//     yaxislimits2[0] = 19.;
//     yaxislimits2[1] = 11.9.;
//     yaxislimits2[2] = 5.9.;
//   }
//   else {cout<<"Wrong energy!"<<endl; exit(0);}

  Double_t ang = 0;

  TString legtitle[nFiles];
  legtitle[0] = "No shift";
  legtitle[1] = "r = 5mm, #phi = 45";
  legtitle[2] = "r = 5mm, #phi = 90";
  TString layers[3] = {"top", "middle", "bottom"};

  TFile *f;
  TCanvas *c;
  TH1F *hMC[nFiles], *hData, *h_null;;

  TLine *lines[3][6];
  Int_t chanID[3][6] = {{12,11,10,9,8,7},  // Top layer
                        {15,14,13,6,5,4},  // Middle layer
                        {18,17,16,3,2,1}}; // Bottom layer
  Float_t y;
  Float_t y2;
  Float_t x;
  TText t;

  t.SetTextSize(0.08);
  t.SetTextAlign(22);

  double factor = 4./6.; // Factor of pad sizes to scale the sizes of the font etc., in this case 4:6

  for (int iLayer = 0; iLayer<3; iLayer++)
  {
    TCanvas *c2 = new TCanvas("c1", "c1", 50, 50, 800, 600);
//     c2->SetRightMargin(0.01);
//     c2->SetTopMargin(0.01);

    TLegend *leg = new TLegend(0.4, 0.3, 0.8, 0.65);
//     TLegend *leg = new TLegend(0.3, 0.5, 0.8, 0.88);
    leg->SetFillColor(0);

    TPad *pad1 = new TPad("c2_1","c2_1", 0, 0.4, 1, 1);
    pad1->Draw();
    pad1->SetBottomMargin(0.0);
    pad1->SetTopMargin(0.1);
    pad1->SetLeftMargin(0.15);
    pad1->SetTicky();
//     TGaxis::SetMaxDigits(3);

    for (int iFile = 0; iFile<nFiles; iFile ++)
    {
      if (path[iFile] == "") continue;
      f = new TFile(Form("rootfiles/%s",path[iFile].Data()),"read");
      c = (TCanvas*)f->Get(Form("%s/canvas_%4.1f",layers[iLayer].Data(),ang));
      hData = (TH1F*)c->GetPrimitive(Form("histOcc_data_%s",layers[iLayer].Data()));
      hMC[iFile] = (TH1F*)c->GetPrimitive(Form("hOccMC_%s_%04.1f",layers[iLayer].Data(), ang));
      hData->SetLineWidth(3);
      hMC[iFile]->SetLineWidth(3);
      hData->SetLineColor(kBlue);
//       if (iFile==0)
//         leg->AddEntry(hData, Form("%4.2f MeV, measured",energy), "l");
      leg->AddEntry(hMC[iFile], legtitle[iFile], "l");
    }
    cout<<"layer "<<layers[iLayer]<<endl;
    cout<<"file data"<<endl;
    for (int k=0; k<6; ++k)
    {
      cout<<hData->GetBinContent(k+1)<<"  ";
    }
    cout<<endl;
    for (int i=0; i<3; ++i)
    {
      cout<<"file "<<path[i]<<endl;
      for (int k=0; k<6; ++k)
      {
        cout<<hMC[i]->GetBinContent(k+1)<<"  ";
      }
      cout<<endl;
    }

    hMC[0]->SetLineColor(kRed);
    hMC[1]->SetLineColor(kBlue);
    hMC[2]->SetLineColor(28);
//     hMC[0]->SetLineStyle(2);
    hMC[1]->SetLineStyle(6);
    hMC[2]->SetLineStyle(9);

//     cout<<hData->GetSum()<<"  "<<hMC[0]->GetSum()<<endl;

//     hData->GetYaxis()->SetRangeUser(0.5*hData->GetMaximum(), 1.2*hData->GetMaximum());
//     hData->GetYaxis()->SetRangeUser(0.6*hData->GetMaximum(), 1.3*hData->GetMaximum());
//     hMC[0]->GetYaxis()->SetRangeUser(yaxislimits1[iLayer][0], yaxislimits1[iLayer][1]);
    hMC[0]->GetXaxis()->SetTickLength(0);
    hMC[0]->GetXaxis()->SetLabelOffset(99);
    hMC[0]->GetXaxis()->SetTitleOffset(99);

    hMC[0]->GetYaxis()->SetTitle("Norm. occupancy");
    hMC[0]->GetYaxis()->SetTitleSize(0.12);
    hMC[0]->GetYaxis()->SetTitleOffset(0.63);
    hMC[0]->GetYaxis()->SetLabelSize(0.1);
    hMC[0]->GetYaxis()->SetLabelOffset(0.01);
//     hMC[0]->GetYaxis()->SetTitleOffset(0.8);
    hMC[0]->Scale(1./hMC[0]->GetSum());
    hMC[1]->Scale(1./hMC[1]->GetSum());
    hMC[2]->Scale(1./hMC[2]->GetSum());
    hMC[1] -> Add(hMC[0], -1);
    hMC[2] -> Add(hMC[0], -1);

    pad1->cd();
//     hData->Draw("hist");
    hMC[0]->Draw("hist");
    hMC[0]->GetYaxis()->SetRangeUser(-0.01,0.2);
    pad1->Update();
    y = pad1->GetUymin()+0.06*(pad1->GetUymax()-pad1->GetUymin());
    for (int i=0; i<6; i++)
    {
      x = 30+i*60;
      t.DrawText(x,y,Form("%d",chanID[iLayer][i]));
    }

    // Pad 2
    c2->cd();
    TPad *pad2 = new TPad("c1_2","c1_2", 0, 0, 1, 0.4);
    pad2->SetTopMargin(0.);
    pad2->SetBottomMargin(0.05);
    pad2->SetLeftMargin(0.15);
    pad2->Draw();
    pad2->cd();
//     pad2->SetGridy();
    pad2->SetTicky();

    h_null = new TH1F(*hMC[0]);
    h_null -> Add(hMC[0],-1);
    TH1F *hSetup = h_null;
    hSetup->GetYaxis()->SetTitle("Deviation ");
    hSetup->GetXaxis()->SetTitle("Segment");

    hSetup->GetXaxis()->SetLabelOffset(99);
    hSetup->GetXaxis()->SetTitleOffset(99);
    hSetup->GetXaxis()->SetTickLength(0);

    hSetup->GetYaxis()->SetNdivisions(510);
    hSetup->GetYaxis()->SetLabelSize(0.12);
    hSetup->GetYaxis()->SetLabelOffset(0.01);
    hSetup->GetYaxis()->SetTitleSize(0.19);
    hSetup->GetYaxis()->SetTitleOffset(0.42);
    hSetup->GetYaxis()->SetRangeUser(-0.0059, 0.0059);

//     hMC[1]->GetYaxis()->SetTitle("Deviation  ");
//     hMC[1]->GetXaxis()->SetTitle("Segment");
// 
//     hMC[1]->SetLabelOffset(99);
// //     hMC[1]->GetXaxis()->SetTitleOffset(0.8);
//     hMC[1]->GetXaxis()->SetTitleOffset(99);
//     hMC[1]->GetXaxis()->SetTitleSize(0.1);
//     hMC[1]->GetXaxis()->SetTickLength(0);
// 
//     hMC[1]->GetYaxis()->SetLabelSize(0.1/factor);
//     hMC[1]->GetYaxis()->SetTitleOffset(0.38);
//     hMC[1]->GetYaxis()->SetTitleSize(0.2);
// 
//     hMC[1]->GetYaxis()->SetNdivisions(510);
// //     hMC[1]->GetYaxis()->SetRangeUser(-yaxislimits2[iLayer],yaxislimits2[iLayer]);
//     hMC[1]->GetYaxis()->SetRangeUser(-0.005, 0.005);
//     cout<<yaxislimits2[iLayer]<<endl;
//     cout<<1.5*hMC[1]->GetMinimum()<<endl;
//     hMC[1]->GetYaxis()->SetRangeUser(5*hMC[1]->GetMinimum(),5*hMC[1]->GetMaximum());

//     hMC[0]->Draw("hist");
    h_null->Draw("hist");
    hMC[1]->Draw("histsame");
    hMC[2]->Draw("histsame");
    c2->Update();

    pad1->cd();
    leg->Draw();
//     c2->SaveAs(Form("out/%s_%s.eps",DS[iFile].Data(), layers[iLayer].Data()));
    c2->SaveAs(Form("Displacement_%4.2f_%s.eps", energy, layers[iLayer].Data()));
  }

}
