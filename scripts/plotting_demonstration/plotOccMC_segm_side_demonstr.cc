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


/**
 *  Calculate the occupancy histogram for a given axis angle for the side case.
 *  Read the event information from the corresponding file and prepare
 *  a canvas with the occupancy distribution, and write it into the .root file
 */

#include <iostream>
#include <math.h>

#include "TROOT.h"
#include "TString.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TStyle.h"
#include "TLine.h"
#include "TText.h"

#include "/home/pclg-23/volynets/.root/macros/logon.C"

using namespace std;

Int_t GetSegID(const Double_t, const Double_t, const Double_t);

int main(int argc, char**argv)
//void plotOccMC_segm_side_demonstr()
{
  SetStyles();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TCanvas* c1 = new TCanvas("c1","Occupancy",5,5,1024,800);
//  c1->Divide(2,1,0.0001,0.0001);

//  Float_t peak_en = -1;       // Sum all energies
  Float_t peak_en = 2.6145; // 2.6MeV
//  Float_t peak_en = 1.62; // DEP 1.62MeV
  TString sLayer;
  Int_t layerID = 1;
  switch (layerID)
  {
    case 0: sLayer = TString("bottom"); break;
    case 1: sLayer = TString("middle"); break;
    case 2: sLayer = TString("top"); break;
    default: cout<<"Bad!!"<<endl; break;
  };


  TChain *ch1 = new TChain("wfTree");

  Float_t source_angle = 155;
//  Float_t axis_angle = 30+90;
//  Float_t axis_angle = -20+180;
  Float_t axis_angle = -20;
//  Float_t axis_angle = 30;
  Float_t diff_angle = axis_angle-source_angle;
  while (diff_angle < -45) diff_angle+=90.;
  cout<<diff_angle<<endl;
//  Float_t axis_angle = 135;
//  Float_t axis_angle = 175;
  ch1->Add(Form("/mnt/scratch/volynets/Work/mage/SII_gammas/new/side/2.61/root_occ/1/Occ_%ddeg_side_2.61_1.root",int(diff_angle)));
  cout<<"Processing file "<<Form("/mnt/scratch/volynets/Work/mage/SII_gammas/new/side/2.61/root_occ/1/Occ_%ddeg_side_2.61_1.root",int(diff_angle))<<endl;

  const int MAXNHITS=1000;
  Float_t hit_E[MAXNHITS], phi[MAXNHITS], z[MAXNHITS];
  Int_t nhits;
  ch1->SetBranchStatus("*",0);
  ch1->SetBranchStatus("hits_totnum", 1);    ch1->SetBranchAddress("hits_totnum",&nhits);
  ch1->SetBranchStatus("hits_edep", 1);      ch1->SetBranchAddress("hits_edep", hit_E);
  ch1->SetBranchStatus("endp_hole_phi", 1);  ch1->SetBranchAddress("endp_hole_phi", phi);
  ch1->SetBranchStatus("endp_hole_zpos", 1); ch1->SetBranchAddress("endp_hole_zpos",  z);

  Int_t nentries = ch1->GetEntries();
  cout<<"Found "<<nentries<<" entries. Processing..."<<endl;

  Double_t SegEnergy[18], CoreEnergy;
  Double_t Pi = 180.;
  Int_t lID = 0;
  Int_t hitSegID = -1;
  Int_t firedSegID = -1;
  float energycut = 0.05;
  Int_t Nseg;

  TH1F *hOccMC = new TH1F(Form("hOccMC_%d",int(axis_angle)),"Occupancy in segments; #phi [deg.];Occupancy",6,0,360.);

  // Filling the MC histograms
  for (int iEntry=0;iEntry<nentries;iEntry++)
//  for (int iEntry=0;iEntry<100000;iEntry++)
  {
    ch1->GetEntry(iEntry);
    if (iEntry%100000==0) cout<<"Now entry "<<iEntry<<endl;

    for (int iSeg=0;iSeg<18;iSeg++)
    {  SegEnergy[iSeg] = 0;  }
    CoreEnergy = 0;
    Int_t firedSegID = -1;

    for (int iHit=0; iHit<nhits; iHit++)
    {
      // Here hitSegID is the ID of the segment with the given hit
      hitSegID = GetSegID(source_angle+diff_angle, z[iHit], phi[iHit]);
      SegEnergy[hitSegID] += hit_E[iHit];
      CoreEnergy += hit_E[iHit];
    }

    for (int iSeg = 0; iSeg < 18; iSeg++)
    {
      // Fired segment should have energy close to the energy of the core
      if ( fabs(SegEnergy[iSeg]-CoreEnergy)<energycut && CoreEnergy>0.150)
      {
        firedSegID = iSeg;
      }
      if (SegEnergy[iSeg]>0.150) Nseg++;
    }

    // If no segment was fired, continue
    if (firedSegID<0) continue;

    lID = firedSegID/6;

    if (fabs(SegEnergy[firedSegID]-peak_en)<0.002)
      hOccMC->AddBinContent(firedSegID%6+1);

  }


  hOccMC->Draw();
  hOccMC->SetLineColor(2);
  hOccMC->SetLineWidth(3);
  hOccMC->GetYaxis()->SetLabelSize(0.03);
  hOccMC->GetXaxis()->SetTitleOffset(0.9);
  hOccMC->GetYaxis()->SetTitleOffset(1.1);
  hOccMC->GetYaxis()->SetTitleSize(0.05);
  hOccMC->GetXaxis()->SetTitleSize(0.05);

  hOccMC->GetYaxis()->SetRangeUser(0,1.2*hOccMC->GetMaximum());


  // Now draw segment boundaries
  TLine *lines[5];
  Int_t chanID[6] = {15,14,13,6,5,4};

  Float_t y_t;
  Float_t y1, y2;
  Float_t x;
  TText t;
  t.SetTextSize(0.05);
  t.SetTextAlign(22);
//  t.SetTextColor(4);
  gPad->SetRightMargin(0.01);
  gPad->Update();
  y_t = gPad->GetUymin()+0.1*(gPad->GetUymax()-gPad->GetUymin());

  for (int i=0; i<6; i++)
  {
    x = i*60+30;
    t.DrawText(x,y_t,Form("%d",chanID[i]));

    x = (i+1)*60;
    y1 = gPad->GetUymin();
    y2 = hOccMC->GetBinContent(hOccMC->FindBin(x));
    if (i==5) continue;
    lines[i] = new TLine(x,y1,x,y2);
    lines[i]->SetLineStyle(9);
    lines[i]->SetLineColor(2);
    lines[i]->SetLineWidth(3);
    lines[i]->Draw();
  }

  c1->RedrawAxis();
//  c1->SaveAs(Form("Side_Occ_MC_demonstr_%d_%4.2f.eps",int(axis_angle-source_angle),peak_en));
//  c1->SaveAs(Form("Side_Occ_MC_demonstr_%d_%4.2f.root",int(axis_angle-source_angle),peak_en));
  c1->SaveAs(Form("Side_Occ_MC_demonstr_%d_%4.2f.eps",int(axis_angle),peak_en));
  c1->SaveAs(Form("Side_Occ_MC_demonstr_%d_%4.2f.root",int(axis_angle),peak_en));
  return 1;
}


Int_t GetSegID(const Double_t SegAngle/*deg*/, const Double_t z/*cm*/, const Double_t angle/*deg*/)
{
  // See comments in the main TAxisFinder-based program
  const Double_t H=7.0;
  const Int_t Pi = 180.;
  Double_t tPhi;
  Int_t lID;   // layer number (0..2)
  Int_t cID;   // column number (0..6)
  lID = int((z+H/2.0)/(H/3.0));
  if (lID>2) lID=2;
  if (lID<0) lID=0;

  tPhi = angle + SegAngle;
  while (tPhi>=360) tPhi-=360;
  while (tPhi<0)   tPhi+=360;
  cID = int(tPhi/(Pi/3));
  if (cID>5) {cout<<"Warning, cID>5 = "<<cID<<"  "<<tPhi<<endl; cID=5;}
  if (cID<0) {cout<<"Warning, cID<0 = "<<cID<<endl; cID=0;}

  return (lID * 6 + cID);
}
