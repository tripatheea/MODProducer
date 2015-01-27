#include <iostream>
#include "csv.cc"

using namespace std;

int jetMultiplicity() {
	std::ifstream file("../PFCandidate.csv");

	CSVRow row;
	while(file >> row) {
		px = stringToDouble(row[2]);
		py = stringToDouble(row[3]);
		pz = stringToDouble(row[4]);
		energy = stringToDouble(row[5]);
		particles.push_back( PseudoJet(px, py, pz, energy) ); 
	}
}