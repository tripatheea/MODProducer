#include "fastjet/ClusterSequence.hh"
#include <iostream>
#include "csv.cc"

using namespace fastjet;
using namespace std;



double stringToDouble(string s);

int main () {
  vector<PseudoJet> particles;
  double Run, px, py, pz, energy;

  std::ifstream file("PFCandidate.csv");
  // std::ifstream file("AK5PFJets.csv");

  CSVRow row;
  while(file >> row) {
  	px = stringToDouble(row[3]);
  	py = stringToDouble(row[4]);
  	pz = stringToDouble(row[5]);
  	energy = stringToDouble(row[6]);
	particles.push_back( PseudoJet(px, py, pz, energy) ); 
  }

  // choose a jet definition
  double R = 0.5;
  JetDefinition jet_def(antikt_algorithm, R);

  // run the clustering, extract the jets
  ClusterSequence cs(particles, jet_def);
  vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets());

  // print out some infos
  cout << "Clustering with " << jet_def.description() << endl;

  // print the jets
  cout <<   "        pt y phi" << endl;

  double maxPT = 0.0;
  double minPT = 1000000000;

  for (unsigned i = 0; i < jets.size(); i++) {
    cout << endl << "jet " << i << ": "<< jets[i].pt() << " " << jets[i].rap() << " " << jets[i].phi() << endl;
    vector<PseudoJet> constituents = jets[i].constituents();

    if (jets[i].pt() > maxPT) {
    	maxPT = jets[i].pt();
    }

    if ((jets[i].pt() < minPT) && ( ! jets[i].pt() == 0.0)) {
    	minPT = jets[i].pt();
    }

    for (unsigned j = 0; j < constituents.size(); j++) {
      cout << "    constituent " << j << "'s pt: " << constituents[j].pt() << " ; ";
    }
  }

  std::cout << std::endl << "Max PT: " << maxPT << std::endl;
  std::cout << std::endl << "Min PT: " << minPT << std::endl;

} 

double stringToDouble(string s) {
  double d;
  stringstream ss(s); //turn the string into a stream
  ss >> d; //convert
  return d;
}