#include <iostream>
#include <unordered_map>

#include "fastjet/ClusterSequence.hh"
#include "csv.cc"

using namespace fastjet;
using namespace std;

double heavisideStep(double x);

double stringToDouble(string s);


int main() {
	ifstream file("../PFCandidate.csv");

	unordered_map< string, vector<PseudoJet> > events;		

	double Run, px, py, pz, energy;
	string event_id;

	double R = 0.5;
	double pt_cut = 50;

	CSVRow row;
	while(file >> row) {
		event_id = row[1];
		px = stringToDouble(row[2]);
		py = stringToDouble(row[3]);
		pz = stringToDouble(row[4]);
		energy = stringToDouble(row[5]);
		PseudoJet current_particle = PseudoJet(px, py, pz, energy);

		if (events.find(event_id) != events.end()) {	
			// If we already have a vector of PseudoJets for the current event id, just append to that vector.
			events[event_id].push_back(current_particle);
		}
		else {	
			// Create a new vector of PseudoJets and insert it with the current event id as the key.
			vector<PseudoJet> new_particles;
			new_particles.push_back(current_particle);
			events[event_id] = new_particles;
		}
	}


	vector<double> N_jet;

	for(auto const iter : events) {
		
		vector<PseudoJet> particles_current_event = iter.second;

		double N_jet_current_event = 0.00;
		for(int i = 0; i < particles_current_event.size(); i++) {
			double pt_i = particles_current_event[i].pt();
			double pt_iR = 0.00;

			if (pt_i == 0.00) continue;			// Removing this line causes a divide by 0 error.
			
			for(int j = 0; j < particles_current_event.size(); j++) {
				double pt_j = particles_current_event[j].pt();
				double delta_Rij = particles_current_event[i].delta_R(particles_current_event[j]);

				pt_iR += pt_j * heavisideStep(R - delta_Rij);
			}

			N_jet_current_event += pt_i * heavisideStep(pt_iR - pt_cut) / pt_iR;
		}

		cout << N_jet_current_event << endl;
		N_jet.push_back(N_jet_current_event);
	}

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
