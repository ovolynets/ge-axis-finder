/**
 *  Plot the finalpoints distribution of the hits after drift for either case
 */

#include "TROOT.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TLine.h"
#include "TText.h"
#include "TVector2.h"
#include "TMath.h"

#include <cmath>
#include <iostream>

using namespace std;

#include "/home/pclg-23/volynets/.root/macros/logon.C"

int main()
//int plot_phi_hits()
{
  TString ccase("Side");
//  TString ccase("Top");

  SetStyles();

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetTitleOffset(1.2,"y");
  gStyle->SetLabelSize(0.03,"y");
  gStyle->SetOptStat(0);



  // Read the tree
  TChain *ch1 = new TChain("wfTree");
  if (ccase=="Top")
    ch1->Add("/mnt/scratch/volynets/Work/mage/SII_gammas/new/top/2.61/root_occ/Occ_top_*.root");
  else if (ccase=="Side")
  {
    ch1->Add("/mnt/scratch/volynets/Work/mage/SII_gammas/new/side/2.61/root_occ/1/Occ_0deg_side_*.root");
    ch1->Add("/mnt/scratch/volynets/Work/mage/SII_gammas/new/side/2.61/root_occ/2/Occ_0deg_side_*.root");
    ch1->Add("/mnt/scratch/volynets/Work/mage/SII_gammas/new/side/2.61/root_occ/3/Occ_0deg_side_*.root");
    ch1->Add("/mnt/scratch/volynets/Work/mage/SII_gammas/new/side/2.61/root_occ/4/Occ_0deg_side_*.root");
  }
  else
  {
    cout<<"Bad case. Exit now..."<<endl;
    return -1;
  }

  // Branch addresses etc.
  Int_t nhits;
  ch1->SetBranchAddress("hits_totnum",&nhits);
  Float_t hit_x[1000];
  ch1->SetBranchAddress("hits_xpos",hit_x);
  Float_t hit_y[1000];
  ch1->SetBranchAddress("hits_ypos",hit_y);
  Float_t hit_z[1000];
  ch1->SetBranchAddress("hits_zpos",hit_z);
  Float_t hit_E[1000];
  ch1->SetBranchAddress("hits_edep",hit_E);

  Float_t phi[1000];
  ch1->SetBranchAddress("endp_hole_phi",phi);
  Float_t r[1000];
  ch1->SetBranchAddress("endp_hole_rho",r);
  Float_t z[1000];
  ch1->SetBranchAddress("endp_hole_zpos",z);

  Int_t nentries = ch1->GetEntries();
  cout<<"Found "<<nentries<<" entries. Processing..."<<endl;

  TH1F *hHits = new TH1F("hHits","Hit position;#phi_{hit} [deg];Events [1/deg]",360,0,360);
  TH1F *hEP = new TH1F("hEP","End points of the trajectories; #phi_{EP} [deg];Events [1/deg]",360,0,360);

  TVector2 v;
  Double_t Pi = TMath::Pi();
  Double_t phi_t = 0;

  // Loop over events
  for (int iEvt = 0; iEvt < nentries; iEvt++)
//  for (int iEvt = 0; iEvt < 100000; iEvt++)
  {
    ch1->GetEntry(iEvt);
    if (iEvt%100000==0) cout<<iEvt<<" events processes."<<endl;
    for (int j=0; j<nhits; j++)
    {
       if (fabs(hit_z[j])>11.666) continue;
       v.Set(hit_x[j], hit_y[j]);
//       phi_t = v.Phi()*180/Pi;
       phi_t = v.Phi()*180/Pi + 155;
       while (phi_t>360) phi_t-=360;
       while (phi_t<0) phi_t+=360;
       hHits->Fill(phi_t);


       phi_t = phi[j];
       if (ccase=="Side")
         phi_t+=155;
       while (phi_t>360) phi_t-=360;
       hEP->Fill(phi_t);
    }
  }

//  h->SetMinimum(0);
  if (ccase=="Top")
  {
    hHits->GetYaxis()->SetRangeUser(0, 55000); // Top case
    hEP->GetYaxis()->SetRangeUser(0, 55000); // Top case
  }
  else
  {
    hHits->GetYaxis()->SetRangeUser(0, 25000); // Side case
    hEP->GetYaxis()->SetRangeUser(0, 25000); // Side case
  }


  // Hit distribution
  TCanvas *cHits = new TCanvas("cHits","cHits",5,5,1024,800);
  hHits->Draw();
  cHits->Update();
  cHits->SaveAs(Form("%s_Hits_phi_demonstr.eps",ccase.Data()));



  // End points distribution
  TCanvas *cEP = new TCanvas("cEPs","cEP",5,5,1024,800);
  hEP->Draw();

  // Draw segment boundaries
  Int_t chanID[3][6] = {{12,11,10,9,8,7},  // Top layer
                        {15,14,13,6,5,4},  // Middle layer
                        {18,17,16,3,2,1}}; // Bottom layer
  TLine *lines[6];
  TText t;
  Float_t x_t, y_t;

  t.SetTextSize(0.04);
  t.SetTextAlign(22);

  Double_t Phi_start; // Segmentation angle
  Phi_start = 20; // Segmentation angle

  Double_t y = 0; // Bincontent at coordinate phi
  for (int i=0; i<6; i++)
  {
    phi_t = Phi_start+60*i;

    y = hEP->GetBinContent(hEP->FindBin(phi_t));
    lines[i] = new TLine(phi_t,0,phi_t,y);
    lines[i]->SetLineStyle(9);
    lines[i]->SetLineColor(2);
    lines[i]->SetLineWidth(3);
    lines[i]->Draw();
  }

  cEP->Update();

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

/*
  TLegend *leg = new TLegend(0.5,0.8,0.89,0.89);
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->AddEntry(h,"Simulated occupancy","l");
  leg->AddEntry(h_segm,"Rebinned sim. occupancy","l");
  leg->AddEntry(lines_r[0],"Segmentation boundaries","l");

  leg->Draw("same");
*/

  cEP->Update();
  cEP->SaveAs(Form("%s_Finalpoints_phi_demonstr.eps", ccase.Data()));

}
