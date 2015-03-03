#include <iostream>

#include "event.cc"


#include "fastjet/ClusterSequence.hh"

using namespace std;

int main() {
	ifstream file("../PFCandidate.csv");

	ofstream fmatch("antikt_multiplicities.csv", ios::out);
	CSVRow row;

	double px, py, pz, energy;	
	double R = 0.5;
	double pt_cut = 50.00;

	int run_number, event_number;
	int event_count = 1;
	
	int last_event_number = 0;
	Event * current_event = new Event(149291, 644983557);

	while(file >> row) {

		run_number = stoi(row[0]);
		event_number = stoi(row[1]);
		px = stod(row[2]);
		py = stod(row[3]);
		pz = stod(row[4]);
		energy = stod(row[5]);

		PseudoJet current_particle = PseudoJet(px, py, pz, energy);

		if (last_event_number != event_number) {

			cout << "Processing event #: " << event_count << endl;
			event_count++;

			// We've moved on to a different event.
			// That means, the class current_event contains all the particles that it's supposed to.
			
			// Calculate N_tilde.
			double N_tilde = current_event->calculate_N_tilde(0.5, 50.00);

			// Calculate jet size (fastjet)
			JetDefinition jet_def(antikt_algorithm, R);
			vector<PseudoJet> jets = current_event->get_jets(jet_def, pt_cut);
			
			fmatch << N_tilde << "\t" << jets.size() << endl;

			// We've calculated all we need. Now delete the old pointer (for the previous instance of event) and create a new one.
			delete current_event;

			Event * current_event = new Event(run_number, event_number);

			last_event_number = event_number;
		}

		current_event->add_particle(px, py, pz, energy);
	}

	// Need to do all these stuff one last time for the final event.

	// Calculate N_tilde.
	double N_tilde = current_event->calculate_N_tilde(0.5, 50.00);

	// Calculate jet size (fastjet)
	JetDefinition jet_def(antikt_algorithm, R);
	vector<PseudoJet> jets = current_event->get_jets(jet_def, pt_cut);

	fmatch << N_tilde << "\t" << jets.size() << endl;

}


