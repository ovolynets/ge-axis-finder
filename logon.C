// only one thing is done here: set my own drawing style

void SetStyles()
{
   // http://root.cern.ch/root/html/TStyle.html
   TStyle *MyStyle = new TStyle("MyStyle","My Style");

   // marker  http://root.cern.ch/root/html/TAttMarker.html
   //MyStyle->SetMarkerStyle(1); // better for scattering plot
   //MyStyle->SetMarkerStyle(6); // better for normal plot
   //MyStyle->SetMarkerStyle(20); // better for data points

   // canvas
   MyStyle->SetCanvasBorderMode(0);
   MyStyle->SetCanvasColor(0);

   // grid
   MyStyle->SetGridWidth(0.3);

   // pad
   MyStyle->SetPadBorderMode(0);
   MyStyle->SetPadColor(0);

   MyStyle->SetPadTopMargin(0.02);
   MyStyle->SetPadBottomMargin(0.1);
   MyStyle->SetPadRightMargin(0.03);
   MyStyle->SetPadLeftMargin(0.12);

   // frame - thing arounded by axes
   MyStyle->SetFrameFillColor(0);
   MyStyle->SetFrameBorderMode(0);

/*
   // title for histogram
   MyStyle->SetTitleFillColor(0);
   MyStyle->SetTitleBorderSize(0);
   MyStyle->SetTitleFont(132,"hist");
   MyStyle->SetTitleSize(0.06,"hist");
   MyStyle->SetTitleX(0.15);
   MyStyle->SetTitleY(0.95);
*/

   // title for axes
   MyStyle->SetLabelSize(0.04,"XYZ");
   MyStyle->SetTitleSize(0.05,"XYZ");
   MyStyle->SetLabelFont(42,"XYZ");
   MyStyle->SetTitleFont(42,"XYZ");
//   MyStyle->SetLabelFont(132,"XYZ");
//   MyStyle->SetTitleFont(132,"XYZ");
//   MyStyle->SetLabelOffset(0.015, "x");
//   MyStyle->SetLabelOffset(1.1, "y");

   MyStyle->SetTitleOffset(0.9, "x");
   MyStyle->SetTitleOffset(1.1, "y");

   // status canvas
   MyStyle->SetStatBorderSize(1);
   MyStyle->SetStatFont(42);
   MyStyle->SetStatColor(0);
   MyStyle->SetStatX(0.95); //top right corner x
   MyStyle->SetStatY(0.95); //top right corner y

   MyStyle->SetOptStat("emr");
//   MyStyle->SetOptFit(1);
   //MyStyle->SetOptStat(0);
   //MyStyle->SetOptFit(0);

   // legend
   MyStyle->SetLegendBorderSize(0);
   //other styles have to be set like:
   //leg->SetFillColor(0);
   //leg->SetTextFont(132);

   // colorful scattering plot
   //MyStyle->SetPalette(2);
   //http://root.cern.ch/root/html/TColor.html#TColor:SetPalette
   MyStyle->SetPalette(1,0); //http://root.cern.ch/root/html/TColor.html#TColor:SetPalette

   gROOT->SetStyle("MyStyle"); //set current style to MyStyle
   gROOT->ForceStyle(); // force everything using current style

}

void logon()
{
   SetStyles();
}

