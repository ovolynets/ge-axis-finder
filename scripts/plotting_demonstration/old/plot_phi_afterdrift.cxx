// Copyright (C) 2009-2012
//
// Author: Oleksandr Volynets
// email:  alexjaguarvol@gmail.com
//
// This is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


int plot_phi_afterdrift()
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  TCanvas *c1 = new TCanvas("c1","c1",5,5,1024,800);
  // Reading the tree
  TChain *ch1 = new TChain("occu");
//  ch1->Add("/mnt/scratch/volynets/Work/mage/Th228/top/AfterDrift/Occ_Th228_top_*.root");
  ch1->Add("/mnt/scratch/volynets/Work/mage/Th228/10_3cm_side/AfterDrift/Occ_0deg_Th228_side.root");

  // Branch addresses etc.
  Int_t nhits;
  ch1->SetBranchAddress("nhits",&nhits);
  Float_t hit_x[1000];
  ch1->SetBranchAddress("hit_x",hit_x);
  Float_t hit_y[1000];
  ch1->SetBranchAddress("hit_y",hit_y);
  Float_t hit_z[1000];
  ch1->SetBranchAddress("hit_z",hit_z);
  Float_t hit_E[1000];
  ch1->SetBranchAddress("hit_E",hit_E);

  Double_t phi[1000];
  ch1->SetBranchAddress("phi",phi);
  Double_t r[1000];
  ch1->SetBranchAddress("r",r);
  Double_t z[1000];
  ch1->SetBranchAddress("z",z);

  Int_t nentries = ch1->GetEntries();
  cout<<"Found "<<nentries<<" entries. Processing..."<<endl;

  TH1F *h = new TH1F("h1","Final points of the trajectories; #phi [^{o}];Events [1/^{o}]",360,0,360);
 
  Double_t phi_t = 0;
  // Looping over events
  for (int iEvt = 0; iEvt < nentries; iEvt++)
//  for (int iEvt = 0; iEvt < 10000; iEvt++)
  {
    ch1->GetEntry(iEvt);
    if (iEvt%100000==0) cout<<iEvt<<" events processes."<<endl;
    for (int j=0; j<nhits; j++)
    {
      if (fabs(z[j])>11.666) continue;
      phi_t = phi[j]+155;
      while (phi_t>360) phi_t-=360;
      h->Fill(phi_t);
    }
  }

//  cout<<h_segm->GetBinContent(1)<<"  "<<h_segm->GetBinContent(7)<<endl;
//  h->GetYaxis()->SetRangeUser(0, h->GetMaximum()*1.2);
  h->GetYaxis()->SetRangeUser(0, 12000);
  h->GetYaxis()->SetTitleOffset(1.2);
  h->GetYaxis()->SetLabelSize(0.03);
  h->GetXaxis()->SetLabelSize(0.03);
  h->Draw();




  Int_t chanID[3][6] = {{12,11,10,9,8,7},  // Top layer
                        {15,14,13,6,5,4},  // Middle layer
                        {18,17,16,3,2,1}}; // Bottom layer
  TLine *lines[6];
  TText t;
  Float_t x_t, y_t;

  t.SetTextSize(0.04);
  t.SetTextAlign(22);

  Double_t Phi_start = 0; // Segmentation angle
  Double_t y = 0; // Bincontent at coordinate phi
  for (int i=1; i<6; i++)
  {
    phi_t = Phi_start+60*i;

    y = h->GetBinContent(h->FindBin(phi_t));
    lines[i] = new TLine(phi_t,0,phi_t,y);
    lines[i]->SetLineStyle(9);
    lines[i]->SetLineColor(2);
    lines[i]->SetLineWidth(3);
    lines[i]->Draw();
  }

  c1->Update();

  y_t = gPad->GetUymin()+0.08*(gPad->GetUymax()-gPad->GetUymin());

  // Drawing first bin label
  x_t = Phi_start/2;
//  t.DrawText(x_t,y_t,Form("%d",chanID[1][5]));
  for (int i=0; i<5; i++)
  {
    x_t = Phi_start+30+60*i;
    t.DrawText(x_t,y_t,Form("%d",chanID[1][i]));
  }


  // Drawing last bin label
  x_t = 330+Phi_start/2;
  t.DrawText(x_t,y_t,Form("%d",chanID[1][5]));

  c1->Update();

/*
  TLegend *leg = new TLegend(0.5,0.8,0.89,0.89);
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->AddEntry(h,"Simulated occupancy","l");
  leg->AddEntry(h_segm,"Rebinned sim. occupancy","l");
  leg->AddEntry(lines_r[0],"Segmentation boundaries","l");

  leg->Draw("same");
*/

  c1->SaveAs("Finalpoints_phi_demonstr.eps");
  return 1;
}
