#include "fastjet/ClusterSequence.hh"
#include <iostream>
#include "csv.cc"

using namespace fastjet;
using namespace std;

double heavisideStep(double x);

double stringToDouble(string s);

int main() {
	std::ifstream file("../PFCandidate.csv");

	vector<PseudoJet> particles;
	double Run, px, py, pz, energy;

	double R = 0.5;
	double pt_cut = 50;

	CSVRow row;
	while(file >> row) {
		px = stringToDouble(row[2]);
		py = stringToDouble(row[3]);
		pz = stringToDouble(row[4]);
		energy = stringToDouble(row[5]);
		particles.push_back(PseudoJet(px, py, pz, energy)); 
	}

	double N_tilde_jet = 0.00;
	for(int i = 0; i < particles.size(); i++) {
		double pt_i = particles[i].pt();
		double pt_iR = 0.00;

		if (pt_i == 0.00) continue;			// Removing this line causes a divide by 0 error.
		
		for(int j = 0; j < particles.size(); j++) {
			double pt_j = particles[j].pt();
			double delta_Rij = particles[i].delta_R(particles[j]);

			pt_iR += pt_j * heavisideStep(R - delta_Rij);
		}

		N_tilde_jet += pt_i * heavisideStep(pt_iR - pt_cut) / pt_iR;
	}

	cout << N_tilde_jet << endl;

}

double heavisideStep(double x) {
	return x > 0;
}

double stringToDouble(string s) {
  double d;
  stringstream ss(s); //turn the string into a stream
  ss >> d; //convert
  return d;
}