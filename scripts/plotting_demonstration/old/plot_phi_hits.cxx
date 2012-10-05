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


int plot_phi_hits()
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
  Double_t hit_x[1000];
  ch1->SetBranchAddress("hit_x",hit_x);
  Double_t hit_y[1000];
  ch1->SetBranchAddress("hit_y",hit_y);
  Double_t hit_z[1000];
  ch1->SetBranchAddress("hit_z",hit_z);
  Double_t hit_E[1000];
  ch1->SetBranchAddress("hit_E",hit_E);

  Double_t phi[1000];
  ch1->SetBranchAddress("phi",phi);
  Double_t r[1000];
  ch1->SetBranchAddress("r",r);
  Double_t z[1000];
  ch1->SetBranchAddress("z",z);

  Int_t nentries = ch1->GetEntries();
  cout<<"Found "<<nentries<<" entries. Processing..."<<endl;

  TH1F *h = new TH1F("h1","Hit position;#phi [^{o}];Events [1/^{o}]",360,0,360);
 
  TVector2 v;
  Double_t Pi = TMath::Pi();
  Double_t phi_t;
  // Looping over events
  for (int iEvt = 0; iEvt < nentries; iEvt++)
//  for (int iEvt = 0; iEvt < 10000; iEvt++)
  {
    ch1->GetEntry(iEvt);
    if (iEvt%100000==0) cout<<iEvt<<" events processes."<<endl;
    for (int j=0; j<nhits; j++)
    {
//       cout<<hit_z[j]<<endl;
       if (fabs(hit_z[j])>11.666) continue;
       v.Set(hit_x[j], hit_y[j]);
       phi_t = v.Phi()*180/Pi + 155;
       while (phi_t>360) phi_t-=360;
       h->Fill(phi_t);
//       cout<<v.Phi()*180/Pi<<endl;
    }
  }

//  cout<<h_segm->GetBinContent(1)<<"  "<<h_segm->GetBinContent(7)<<endl;
//  h->GetYaxis()->SetRangeUser(0, h->GetMaximum()*1.2);
  h->GetYaxis()->SetRangeUser(0, 12000);
  h->GetYaxis()->SetTitleOffset(1.2);
  h->GetYaxis()->SetLabelSize(0.03);
  h->GetXaxis()->SetLabelSize(0.03);
  h->Draw();


/*
  Double_t Phi_start = 20; // Segmentation angle
  TLine *lines[6];
  Float_t phi_t;
  Double_t y = 0; // Bincontent at coordinate phi
  for (int i=0; i<6; i++)
  {
    phi_t = Phi_start+60*i;

    y = h->GetBinContent(h->FindBin(phi_t));
    lines[i] = new TLine(phi_t,0,phi_t,y);
    lines[i]->SetLineStyle(9);
    lines[i]->SetLineColor(2);
    lines[i]->SetLineWidth(3);
    lines[i]->Draw();
  }
*/

  c1->Update();

  c1->SaveAs("Hits_phi_demonstr.eps");
  return 1;
}
