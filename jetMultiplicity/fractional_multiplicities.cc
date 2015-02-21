#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TGraph.h"
#include "TVectorT.h"

#include "TCanvas.h"
#include "TPad.h"
#include "TLatex.h"
#include "TH1F.h"
#include "TH2F.h"

#include "TStyle.h"
#include "TGaxis.h"
#include "TRandom.h"
#include "TLegend.h"

#include "TMath.h"

// void fractional_multiplicities() {
//   std::ifstream infile("antikt_multiplicities.dat");

//   TFile * rootFile_;
//   TTree * multiplicityTree_;

//   TCanvas *c2e = new TCanvas("ntilde_antikt", "Ntilde and Antikt Multiplicity", 1000, 600);
  
//   gStyle->SetOptStat(false);
//   TH1F *h1 = new TH1F("Antikt Multiplicity", "", 1001, -0.5, 6.0);
//   TH1F *h2 = new TH1F("Ntilde", "", 1001, -0.5, 6.0);

//   double mul;
//   int antikt;

//   while(infile >> mul >> antikt) {
//     h1->Fill(antikt);
//     h2->Fill(mul);
//   }


  
//   h1->Draw();
//   c2e->Update();

//   // Scale h2 to the pad coordinates.
//   Float_t rightmax = 1.1 * h2->GetMaximum();
//   Float_t scale = gPad->GetUymax() / rightmax;

//   h2->SetLineColor(kRed);
//   h2->Scale(scale);
//   h2->Draw("same");

//   // Draw an axis on the right side.
//   TGaxis *axis = new TGaxis(gPad->GetUxmax(), gPad->GetUymin(), gPad->GetUxmax(), gPad->GetUymax(), 0, rightmax, 510, "+L");
//   axis->SetLineColor(kRed);
//   axis->SetLabelColor(kRed);
//   axis->Draw();

//   c2e->Update();

//   TLegend *leg = new TLegend(0.1, 0.7, 0.48, 0.9);
//   // leg->SetHeader("Legend");
//   leg->AddEntry(h1, "Antikt");
//   leg->AddEntry(h2, "Ntilde");
//   leg->Draw();

// }



void fractional_multiplicities() {
  std::ifstream infile("antikt_multiplicities.dat");

  TFile * rootFile_;
  TTree * multiplicityTree_;

  // Create, fill and project a 2D histogram.
  TCanvas *c2e = new TCanvas("c2e", "c2e", 600, 400);
  TH2F *h2 = new TH2F("Antikt Multiplicity and Ntilde", "", 1001, -0.5, 6.0, 1001, -0.5, 6.0);

  rootFile_ = new TFile("antikt_multiplicities.root", "RECREATE");
  multiplicityTree_ = new TTree("Multiplicity", "Multiplicity");
  
  double mul;
  int antikt;

  multiplicityTree_->Branch("multiplicity", &mul, "mul/D");
  multiplicityTree_->Branch("antikt", &antikt, "antikt/I");

  while(infile >> mul >> antikt) {
    multiplicityTree_->Fill();
    h2->Fill(log(mul), log(antikt));
  }

  rootFile_->cd();
  
  multiplicityTree_->Write();

  rootFile_->Close();

  // Draw.
  TH1D * projh2X = h2->ProjectionX();
  TH1D * projh2Y = h2->ProjectionY();
  
  h2->Draw("LEGO");

}
