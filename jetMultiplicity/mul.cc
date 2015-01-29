#include <iostream>
#include "csv.cc"

#include <TFile.h>
#include <TTree.h>


using namespace std;

double stringToDouble(string s);

int main() {
  std::ifstream file("mul.csv");

  TFile * rootFile_;
  TTree * multiplicityTree_;

  rootFile_ = new TFile("mul", "RECREATE");
  multiplicityTree_ = new TTree("Multiplicities", "Jet Multiplicities");
  
  double mul;

  multiplicityTree_->Branch("multiplicity", &mul, "mul/D");

  CSVRow row;
  while(file >> row) {
    mul = stringToDouble(row[0]);
    multiplicityTree_->Fill();
  }
  
  rootFile_->cd();
  multiplicityTree_->Write();
  rootFile_->Close();

  return 0;
}

double stringToDouble(string s) {
  double d;
  stringstream ss(s); //turn the string into a stream
  ss >> d; //convert
  return d;
}