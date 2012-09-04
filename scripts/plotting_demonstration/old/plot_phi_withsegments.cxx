int plot_phi_withsegments()
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  TCanvas *c1 = new TCanvas("c1","c1",5,5,1024,800);
  // Reading the tree
  TChain *ch1 = new TChain("occu");
  ch1->Add("/mnt/scratch/volynets/Work/mage/Th228/AfterDrift/Th228_top_Drift*.root");

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

  TH1F *h      = new TH1F("h1","Occupancy vs #phi; #phi [^{o}];Events [1/^{o}]",360,0,360);

  // Creating bin array for the second, segmented, histogram
  Double_t Phi_start = 20;
  Double_t phi_t = 0;
  Double_t bins[8];
  bins[0] = 0;
  for (int i=0; i<6; i++)
  {   bins[i+1] = Phi_start+60*i;  }
  bins[7] = 360;
  TH1F *h_segm = new TH1F("h_segm","h_segm",7,bins);

  // Segment energy
  Double_t segEn[6];

  // Looping over events
  for (int iEvt = 0; iEvt < nentries; iEvt++)
//  for (int iEvt = 0; iEvt < 10000; iEvt++)
  {
    ch1->GetEntry(iEvt);
    if (iEvt%1000==0) cout<<iEvt<<" events processes."<<endl;
    for (int j=0; j<nhits; j++)
    {
       h->Fill(phi[j]);
       h_segm->Fill(phi[j]);
       if (phi[j]<bins[1]) h_segm->Fill(359.8); // If phi in the fist segment,
                                                // also add it to the last bin, representing the same segment

       if (phi[j]>=bins[6]) h_segm->Fill(0.2);  // If phi in the last segment,
                                                // do the same, add it to the first bin, representing the same segment
    }
  }

//  cout<<h_segm->GetBinContent(1)<<"  "<<h_segm->GetBinContent(7)<<endl;
  h->GetYaxis()->SetRangeUser(0, h->GetMaximum()*1.2);
  h->GetYaxis()->SetTitleOffset(1.2);
  h->GetYaxis()->SetLabelSize(0.03);
  h->GetXaxis()->SetLabelSize(0.03);
  h->Draw();

  h_segm->SetLineWidth(3);
  h_segm->SetLineColor(2);
  cout<<h->Integral("width")<<"  "<<h_segm->Integral("width")<<"  "<<h_segm->Integral("width")/h->Integral("width")<<endl;
  cout<<h->GetSum()<<"  "<<h_segm->GetSum()<<endl;
  h_segm->Scale(h->Integral("width")/(h_segm->Integral("width")));
  h_segm->Draw("same");

  c1->Update();

  TLine *lines_l[6];
  TLine *lines_r[6];
  Double_t y = 0; // Bincontent at coordinate phi
  for (int i=0; i<6; i++)
  {
    phi_t = bins[i+1];
/*
//    y = h->GetBinContent(h->FindBin(phi_t)); cout<<y<<endl;
    y = c1->GetUymax();
    lines_l[i] = new TLine(phi_t-0.5,0,phi_t-0.5,y);
    lines_l[i]->SetLineStyle(9);
    lines_l[i]->SetLineColor(4);
    lines_l[i]->SetLineWidth(2);
    lines_l[i]->Draw();
*/

    y = h_segm->GetBinContent(h_segm->FindBin(phi_t));
    lines_r[i] = new TLine(phi_t,0,phi_t,y);
    lines_r[i]->SetLineStyle(9);
    lines_r[i]->SetLineColor(2);
    lines_r[i]->SetLineWidth(3);
    lines_r[i]->Draw();

  }

  TLegend *leg = new TLegend(0.5,0.8,0.89,0.89);
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->AddEntry(h,"Simulated occupancy","l");
  leg->AddEntry(h_segm,"Rebinned sim. occupancy","l");
  leg->AddEntry(lines_r[0],"Segmentation boundaries","l");

  leg->Draw("same");

  c1->SaveAs("Occ_phi_demonstr.eps");
  return 1;
}