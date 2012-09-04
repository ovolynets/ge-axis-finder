#include "/home/pclg-23/volynets/.root/macros/logon.C"

void ensemble_read()
{
  gStyle->SetOptStat(0);

  const int nFiles = 100;
  TString DS[nFiles];
  DS[0] = "top_0.58";
  DS[1] = "top_1.17";
  DS[2] = "top_1.33";
  DS[3] = "top_2.61";
  DS[4] = "side_0.58";
  DS[5] = "side_2.61";

  DS[6] = "top_1.17_syst";
  DS[7] = "top_1.33_syst";
  DS[8] = "top_1.33_5mmoff_45deg";
  DS[9] = "top_1.33_5mmoff_90deg";
  DS[10]= "top_2.61_5mmoff_45deg";
  DS[11]= "top_2.61_5mmoff_90deg";
  DS[12]= "top_1.33_newimp_-45to45_1deg";

  int iFile = 3;
  float mean = 0;
  if (DS[iFile].Index("side")>-1) mean = 165;
  ifstream fIn(Form("ensembletest_%s_epsilon_results_15deg_10k.txt",DS[iFile].Data()) );
//   ifstream fIn("epsilon_results_15deg_ensembletest_10k.txt");

  // Read the runconfiguration.db
  std::stringstream datastream;
  std::string       datastring;

  const int nLayers = 3;
  float vals[nLayers];
  float errs[nLayers];
  float w[nLayers];

  TH1F *hMin[nLayers];

  TString layers[nLayers] = {"top", "middle", "bottom"};

  for (int i = 0; i<nLayers; ++i)
  {
    TString hname = Form("%s layer", layers[i].Data());
    TString htitle = Form("%s layer;#Phi_{min. #epsilon};Entries", layers[i].Data());
    hMin[i] = new TH1F(hname.Data(), htitle.Data(), 200, mean-50, mean+50);
    hMin[i]->GetXaxis()->SetTitleOffset(0.7);
    hMin[i]->GetXaxis()->SetTitleSize(0.06);
    hMin[i]->GetXaxis()->SetLabelSize(0.04);

    hMin[i]->GetYaxis()->SetTitleOffset(1.5);
    hMin[i]->GetYaxis()->SetTitleSize(0.06);
    hMin[i]->GetYaxis()->SetLabelSize(0.05);
  }


  // A safe way to read data using stringstream
  // and to avoid duplication of the last line
  while (getline(fIn, datastring)) {

    // Skip empty lines
    if (datastring.empty()) continue;

    // Read the line and stream the name and the time stamp
    datastream.clear();
    datastream.str(datastring);
    float temp;
    for (int i=0; i<nLayers; ++i)
    {
      datastream >> temp; vals[i] = temp;
      datastream >> temp; errs[i] = temp;
      datastream >> temp; w[i] = temp;
      hMin[i]->Fill(vals[i]);
    }
  }
  fIn.close();
  TCanvas *c1 = new TCanvas("c1","c1",5,5,1600,600);
  c1->Divide(3,1,0.001,0.001);
  for (int i=0; i<nLayers; ++i)
  {
    c1->cd(i+1);
    gPad->SetLeftMargin(0.2);
    gPad->SetBottomMargin(0.12);
    hMin[i]->Draw();
    hMin[i]->Fit("gaus","q");
    TF1 *f = (TF1*) (hMin[i]->GetListOfFunctions()->At(0));
    cout<<"Half width at half maximum = "<<f->GetParameter(2)*2.355/2.<<endl;
    cout<<"Sigma (uncertainty) = "<<f->GetParameter(2)/2.<<endl;

  }

  c1->SaveAs(Form("Ensemble_test_%s.eps", DS[iFile].Data()));
}
