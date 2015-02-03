#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TGraph.h"
#include "TVectorT.h"




void fractional_multiplicities() {
  std::ifstream infile("antikt_multiplicities.dat");

  TFile * rootFile_;
  TTree * multiplicityTree_;

  rootFile_ = new TFile("antikt_multiplicities.root", "RECREATE");
  multiplicityTree_ = new TTree("Multiplicity", "Multiplicity");
  
  double mul;
  int antikt;

  multiplicityTree_->Branch("multiplicity", &mul, "mul/D");
  multiplicityTree_->Branch("antikt", &antikt, "antikt/I");

  while(infile >> mul >> antikt) {
    multiplicityTree_->Fill();
    
  }

  rootFile_->cd();
  
  multiplicityTree_->Write();

  rootFile_->Close();

  // TGraph *g = new TGraph(multi, anti);  

}
