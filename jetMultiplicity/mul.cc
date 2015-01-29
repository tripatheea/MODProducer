#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"




void mul() {
  std::ifstream infile("mul.dat");

  TFile * rootFile_;
  TTree * multiplicityTree_;

  rootFile_ = new TFile("mul.root", "RECREATE");
  multiplicityTree_ = new TTree("Multiplicities", "Jet Multiplicities");
  
  double mul;

  multiplicityTree_->Branch("multiplicity", &mul, "mul/D");

  while(infile >> mul) {
    multiplicityTree_->Fill();
  }

  rootFile_->cd();
  multiplicityTree_->Write();
  rootFile_->Close();

}
