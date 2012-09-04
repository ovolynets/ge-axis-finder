/**
 *  Read and plot the occupancy distribution for three layers superimposed.
 *  Two pads are created on the canvas, one for the nominal Siegfried-II-like detector
 *  and the one with lower impurity levels.
 */

#include "/home/pclg-23/volynets/.root/macros/logon.C"
// #include "/home/alex/.root/macros/logon.C"

int GetCanvas_combine3layers()
{

  SetStyles();
//  gROOT->SetStyle("Plain");
  gStyle->SetLabelFont(42,"XYZ");
  gStyle->SetTitleFont(42,"XYZ");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

  TString DS[2];
  DS[0] = "OccPlots_top_1.33.root";
  DS[1] = "OccPlots_top_1.33_newimp_-45to45_1deg.root";
  Double_t ang = 0;

  TString layers[3] = {"top", "middle", "bottom"};

  TFile *f;
  TCanvas *c;
  TH1F *hMC[3], *hMC_null;

  TLine *lines[3][6];
  Int_t chanID[3][6] = {{12,11,10,9,8,7},  // Top layer
                        {15,14,13,6,5,4},  // Middle layer
                        {18,17,16,3,2,1}}; // Bottom layer
  Float_t y;
  Float_t y2;
  Float_t x;
  TText t;

  t.SetTextSize(0.1);
  t.SetTextAlign(22);

  TCanvas *c2 = new TCanvas("c1", "c1", 50, 50, 1600, 600);
  c2->cd();

  TPad *pad[2];
  pad[0] = new TPad("c1_1", "c1_1", 0., 0., 0.55, 1.);
  pad[1] = new TPad("c1_2", "c1_2", 0.55, 0., 1., 1.);

  c2->cd();
  pad[0]->Draw();
  c2->cd();
  pad[1]->Draw();

  pad[0]->SetLeftMargin(0.1/0.55);
  pad[1]->SetLeftMargin(0.01);
  pad[0]->SetRightMargin(0.01);
  pad[1]->SetRightMargin(0.01);

  //   c2->Divide(2, 1, 0., 0.);


  for (int iImp=0; iImp<2; ++iImp)
  {
    // Get all the plots from the file
    f = new TFile(Form("rootfiles/%s",DS[iImp].Data()),"read");
    for (int iLayer = 0; iLayer<3; iLayer++)
    {
      c = (TCanvas*)f->Get(Form("%s/canvas_%4.1f",layers[iLayer].Data(),ang));
      hMC[iLayer] = (TH1F*)c->GetPrimitive(Form("hOccMC_%s_%04.1f",layers[iLayer].Data()));
      hMC[iLayer]->SetLineWidth(2);
    }
    cout<<hMC[1]->GetBinContent(1)<<endl;

  //   hMC[1]->SetLineWidth(1);
    hMC[0]->SetLineColor(kBlue);
    hMC[1]->SetLineColor(kRed);
    hMC[2]->SetLineColor(kBlack);
    hMC[0]->SetLineStyle(2);
    hMC[1]->SetLineStyle(1);
    hMC[2]->SetLineStyle(9);


    double factor = 4./6.;

//     c2->cd(iImp+1);
    pad[iImp]->cd();
    // Draw large pad first - middle layer
    TString cname = Form("c%d_1",iImp);

    TPad *pad1 = new TPad(cname.Data(),cname.Data(), 0, 0.4, 1, 1);
    pad1->Draw();
    pad1->SetBottomMargin(0.0);
    pad1->SetTopMargin(0.05);
    pad1->SetRightMargin(0.01);

    pad1->SetLeftMargin(0.1/0.55);
    pad1->SetRightMargin(0.01);
    pad1->SetTicky();


    hMC[0]->Scale(1./hMC[0]->Integral());
    hMC[1]->Scale(1./hMC[1]->Integral());
    hMC[2]->Scale(1./hMC[2]->Integral());
    hMC[1]->GetYaxis()->SetTitle("Norm. occupancy");
    hMC[1]->GetXaxis()->SetTitle("Segment");
    hMC[1]->GetXaxis()->SetTickLength(0);
    hMC[1]->GetXaxis()->SetLabelOffset(99);
    hMC[1]->GetXaxis()->SetTitleOffset(99);

    hMC[1]->GetYaxis()->SetRangeUser(-0.01, 0.2);
    hMC[1]->GetYaxis()->SetLabelSize(0.12);
    hMC[1]->GetYaxis()->SetTitleSize(0.12);
    hMC[1]->GetYaxis()->SetTitleOffset(0.8);
    pad1->cd();

    if (iImp==1)
    {
      pad1->SetLeftMargin(0.);
      pad1->SetLeftMargin(0.01);
      pad1->SetRightMargin(0.01);
      hMC[1]->GetYaxis()->SetLabelOffset(99);
    }

    hMC[1]->Draw("hist");
    pad1->Update();

    y = pad1->GetUymin()+0.05*(pad1->GetUymax()-pad1->GetUymin());
    t.SetTextColor(kBlack);
    for (int i=0; i<6; i++)
    {
      x = 30+i*60;
      t.DrawText(x,y,Form("%d",chanID[2][i]));
    }

    y = pad1->GetUymin()+0.14*(pad1->GetUymax()-pad1->GetUymin());
    t.SetTextColor(kRed);
    for (int i=0; i<6; i++)
    {
      x = 30+i*60;
      t.DrawText(x,y,Form("%d",chanID[1][i]));
    }

    y = pad1->GetUymin()+0.23*(pad1->GetUymax()-pad1->GetUymin());
    t.SetTextColor(kBlue);
    for (int i=0; i<6; i++)
    {
      x = 30+i*60;
      t.DrawText(x,y,Form("%d",chanID[0][i]));
    }
    for (int i=0; i<3; ++i)
    {
      cout<<"layer "<<layers[i]<<endl;
      for (int k=0; k<6; ++k)
      {
        cout<<hMC[i]->GetBinContent(k+1)<<"  ";
      }
      cout<<endl;
    }

    // Pad 2
//     c2->cd(iImp+1);
    pad[iImp]->cd();
    cname = Form("c%d_2", iImp);
    TPad *pad2 = new TPad(cname.Data(),cname.Data(),0,0,1,0.4);
    pad2->SetTopMargin(0.);
    pad2->SetBottomMargin(0.05);
    pad2->SetLeftMargin(0.1/0.55);
    pad2->SetRightMargin(0.01);
    pad2->Draw();
    pad2->cd();
//     pad2->SetGridy();
    pad2->SetTicky();

    hMC_null = new TH1F(*hMC[1]);
    hMC_null -> Add(hMC_null,-1);
    TH1F *hSetup = hMC_null;
    hSetup->GetYaxis()->SetTitle("Deviation ");
    hSetup->GetXaxis()->SetTitle("Segment");

//     hSetup->SetLabelOffset(99);
//     hSetup->GetXaxis()->SetTitleOffset(0.8);
    hSetup->GetXaxis()->SetLabelOffset(99);
    hSetup->GetXaxis()->SetTitleOffset(99);
    hSetup->GetXaxis()->SetTitleSize(0.1);
    hSetup->GetXaxis()->SetTickLength(0);

    hSetup->GetYaxis()->SetNdivisions(30505);
    hSetup->GetYaxis()->SetLabelSize(0.18);
    hSetup->GetYaxis()->SetLabelOffset(0.01);
    hSetup->GetYaxis()->SetTitleSize(0.19);
//     hSetup->GetYaxis()->SetTitleOffset(0.9*factor);
    hSetup->GetYaxis()->SetTitleOffset(0.52);

    hMC[0]->Add(hMC[1],-1);
    hMC[2]->Add(hMC[1],-1);
  //   hSetup->GetYaxis()->SetRangeUser(-0.0029, 0.0029);

    if (iImp==1)
    {
      pad2->SetLeftMargin(0.01);
      pad2->SetRightMargin(0.01);
      hSetup->GetYaxis()->SetLabelOffset(99);
    }


/*    hMC[0]->Scale(1000.);
    hMC[2]->Scale(1000);*/
    hMC_null->Draw("hist");
    hMC[0]->Draw("samehist");
    hMC[2]->Draw("samehist");

//     hSetup->GetYaxis()->SetRangeUser(-3.9, 3.9);
    hSetup->GetYaxis()->SetRangeUser(-0.0039, 0.0039);
    // Now fill zeros to create an empty histogram
    hMC[0]->Draw("samehist");

//     if (iImp==0)
//     {
//       TPaveText *pave = new TPaveText(0.06, 0.85, 0.15, 1., "NDC");
//       pave->SetTextFont(42);
//       pave->SetTextSize(0.14);
//       pave->AddText("10^{-3}#times");
//       pave->SetFillColor(0);
//       pave->SetShadowColor(0);
//       pave->SetLineColor(0);
//       pave->SetFillStyle(0);
//       pave->Draw();
//     }

    pad2->Update();


    pad1->cd();
    if (iImp==0)
    {
      TLegend *leg = new TLegend(0.4, 0.3, 0.8, 0.65);
      leg->SetFillColor(0);
      leg->AddEntry(hMC[0], "Top layer", "l");
      leg->AddEntry(hMC_null, "Middle layer", "l");
      leg->AddEntry(hMC[2], "Bottom layer", "l");
      leg->Draw();
    }

  }
//   c2->SaveAs(Form("out/%s_%s.eps",DS[iFile].Data(), layers[iLayer].Data()));
  c2->Update();

  c2->SaveAs("Syst_impur_0.0_0.35__0.35_0.55.eps");
//   c2->SaveAs("AllLayersNormalized.eps");
}
