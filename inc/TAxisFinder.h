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


#ifndef TAXISFINDER
#define TAXISFINDER

#include <TROOT.h>
#include <TString.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TGraph.h>


/// Main analysis class
class TAxisFinder
{
public:
  //! Default constructor
  TAxisFinder();

  //! Default destructor
  virtual ~TAxisFinder();

  //! Get segment ID by z and phi coordinates
  //! for the segmentation starting at phi = 0
  Int_t GetSegID (const Double_t z, const Double_t angle);

  //! Get segment ID by z and phi coordinates for the top case
  //! for the segmentation starting at (-seg_angle), which is equivalent to the
  //! <110> axis orientation at seg_angle.
  //! Note: the function is obsolete and was transferred to TMCOccTopSelector
  Int_t GetSegID_top (const Double_t seg_angle, const Double_t z, const Double_t angle);

  //! Get segment ID by z and phi coordinates for the side case
  //! for the segmentation starting at phi = 0, for the current axis orientation.
  //! Note: the function is obsolete and was transferred to TMCOccSideSelector
  Int_t GetSegID_side (const Double_t rad_angle, const Double_t z, const Double_t angle);

  //! Set maximum number of entries to be processed, -1 if all
  void SetMaxEvents(Int_t events){fMaxEvents = events;}

  //! Open and setup the output file
  void SetOutputFile();

  //! Calculate test statistic for the filled MC and data histograms
  //! for each possible axis orientation parameter
  void CalculateChi2();

  //! Fill spectra histograms in data by calling proof
  void FillDataSpectraHistograms();
  //! Calculate occupancy in data from the spectrum,
  //! taking into account background if needed
  void CalculateOccupancyInData();
  //! Extract the occupancy histograms from file in data
  void ReadOccupancyHistogramsData();

  //! Fill occupancy histograms in simulations by calling
  //! corresponding functions for the top and side case
  void FillOccupancyHistogramsMC();

  //! Write the histograms, close the output file
  void Finalize();

  //! Read ini file with the analysis parameters
  void ReadIniFile(const char* filename);

  //! Set branches for MC.
  //! Note: obsolete, used only for non-parallel tests and benchmarks
  void SetBranches(TChain* ch);
  //! Find the source position for the side case if the axis is known - the inverse problem
  void FindSourcePosition();

private:
  //! Fill occupancy histograms for the top case in simulations by calling proof
  void FillOccupancyHistogramsMC_top_proof();
  //! Fill occupancy histograms for the side case in simulations by calling proof
  void FillOccupancyHistogramsMC_side_proof();

  //! Fill occupancy histograms for the top case in simulations,
  //! non-parallel version, used for benchmarking
  void FillOccupancyHistogramsMC_top_noparal();
  //! Fill occupancy histograms for the top case in simulations,
  //! non-parallel optimized version, used for benchmarking
  void FillOccupancyHistogramsMC_top_noparal_optimized();

  //! Fill occupancy histograms for the side case in simulations,
  //! non-parallel version, used for benchmarking
  void FillOccupancyHistogramsMC_side_noparal();

protected:

  static const Int_t fNLayers = 3;        ///< Number of detector layers
  static const Int_t MAXNHITS = 1000;     ///< Maximal number of hits per event
  static const Int_t MAXNANGLES = 1000;   ///< Maximal number of MC files

  TString fPathDataOcc;  ///< Data files with occupancies

  //! Path to measured energy deposits from a source,
  //! used to calculate occupancy distributions in data.
  TString fPathData;

  //! Path to measured energy deposits from background,
  //! used to calculate occupancy distributions from background
  TString fPathBG;

  //! Vector of paths to energy deposits from simulation,
  //! used to calculate occupancy distributions
  std::vector<TString> fPathMC;

  TString fCase;                  ///< Case name. Options: top/side
  TString fRunName;               ///< Run name. Format: RunNN_[more description if needed]
  TString fSourceName;            ///< Source used. Options: Co60/Th228
  TString fLayerName[fNLayers];   ///< Layer names: bottom, middle, top

  TFile* fOutFile;                ///< Output file with occupancy plots for each angle
  TFile* fFileDataOcc;            ///< File containing pre-calculated occupancies in Data

  TH1F* fOccupancyHistData[fNLayers];           ///< Histograms for occupancy in Data for each layer
  TH1F* fOccupancyHistMC[fNLayers][MAXNANGLES]; ///< Histograms for occupancy in MC for each layer and angle

  TGraph *fChi2Graph[fNLayers];   ///< Chi2 graph for each layers

  Double_t fRadiationAngle;       ///< Radiation angle of the source for the side case
  Double_t fPeakEnergy;           ///< Energy peak used for the occupancy calculations

  Int_t nhits;                    ///< TTree branch names: number of (combined) hits
  Float_t hit_E[MAXNHITS];        ///< TTree branch names: hit energy
  Float_t phi[MAXNHITS];          ///< TTree branch names: phi coordinate of hit endpoint
  Float_t z[MAXNHITS];            ///< TTree branch names: z coordinate of hit endpoint

  Int_t fCanvasPos[19];           ///< Map of pad ID for segments for plotting

  Int_t fAngles_N;                      ///< Number of angles for which chi2 is calculated
  Float_t fAngles_min;                  ///< Minimal angle for calculations
  Float_t fAngles_max;                  ///< Maximal angle for calculations
  Float_t fAngles_step;                 ///< Step of angular loop
  Double_t fAngles[MAXNANGLES];         ///< Values of angles at which chi2 is calculated
  Double_t fChi2[fNLayers][MAXNANGLES]; ///< Array of values of chi2, for each layer and each angle


  Int_t fMaxEvents;                     ///< Maximum number of events to be processed
  Double_t fSourceAngle;                ///< Angle of the source, irrelevant for the top case
  Double_t fAxisAngle;                  ///< Axis of the detector, irrelevant for the top and side case, only for GII tests

};

#endif
