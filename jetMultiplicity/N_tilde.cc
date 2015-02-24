#include <iostream>

#include "event.cc"


#include "fastjet/ClusterSequence.hh"

using namespace std;

int main() {
	ifstream file("../PFCandidate.csv");
	CSVRow row;

	double px, py, pz, energy;	
	double R = 0.5;
	double pt_cut = 50.00;

	int run_number, event_number;
	
	int last_event_number = 0;
	Event * current_event = new Event(149291, 644983557);

	remove("banana.csv");

	int i = 0;
	int event_count = 1;
	while(file >> row) {

		run_number = stoi(row[0]);
		event_number = stoi(row[1]);
		px = stod(row[2]);
		py = stod(row[3]);
		pz = stod(row[4]);
		energy = stod(row[5]);

		PseudoJet current_particle = PseudoJet(px, py, pz, energy);

		if (last_event_number != event_number) {

			// cout << "Processing event #: " << event_count << endl;
			
			cout << current_event->size() << endl;

			// We've moved on to a different event.
			// That means, the class current_event contains all the particles that it's supposed to.
			
			// Calculate N_tilde.
			double N_tilde = current_event->calculate_N_tilde(0.5, 50.00);
			if (N_tilde == 0.0)
				i++;

			// Write current event to a file.
			current_event->write_to_file("banana.csv");

			// We've calculated all we need. Now delete the old pointer (for the previous instance of event) and create a new one.
			delete current_event;
			Event * current_event = new Event(run_number, event_number);

			last_event_number = event_number;
			event_count++;
		}

		current_event->add_particle(px, py, pz, energy);
	}

	// Need to do all this one last time for the final event.

	cout << endl << "Total jets for which N_tilde = 0 is: " << i << endl;

}


