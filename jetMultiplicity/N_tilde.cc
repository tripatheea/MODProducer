#include <iostream>
#include <unordered_map>

#include "event.cc"


#include "fastjet/ClusterSequence.hh"

using namespace std;

unordered_map<string, vector<string> > get_trigger_info(int start, int end);

int main() {
	ifstream pfCandidatesFile("../PFCandidate.csv");

	ofstream fmatch("antikt_multiplicities.csv", ios::out);
	CSVRow row;

	double px, py, pz, energy;	
	double R = 0.5;
	double pt_cut = 50.00;

	int run_number, event_number;
	int event_count = 1;
	
	int last_event_number = 0;
	Event * current_event = new Event(146511, 138867139);
	int no_of_triggers = 7; 	// Depends on the analysis we're doing.

	while(pfCandidatesFile >> row) {

		run_number = stoi(row[0]);
		event_number = stoi(row[1]);
		px = stod(row[2]);
		py = stod(row[3]);
		pz = stod(row[4]);
		energy = stod(row[5]);

		PseudoJet current_particle = PseudoJet(px, py, pz, energy);

		if (last_event_number != event_number) {

			cout << "Processing event number: " << event_number << " which is # " << event_count << endl;
			

			// We've moved on to a different event.
			// That means, the class current_event contains all the particles that it's supposed to.
			
			// Add all the trigger info for this event.
			
			int line_start = (event_count - 1) * no_of_triggers + 1;

			current_event->add_triggers(get_trigger_info(line_start, line_start + no_of_triggers - 1));

			// All triggers stored.

			// Now retrieve the assigned trigger and use that to determine:
			// a) whether to record the current event or not.
			// b) what prescale to use.

			// Calculate jet size (fastjet)
			JetDefinition jet_def(antikt_algorithm, R);
			current_event->calculate_jets(jet_def, pt_cut);

			vector<string> assigned_trigger = current_event->get_assigned_trigger();

			if (assigned_trigger.size() > 0) {

				// Record things only if at least one trigger was fired.

				int prescale_1 = stoi(assigned_trigger[1]);
				int prescale_2 = stoi(assigned_trigger[2]);

				// Calculate N_tilde.
				double N_tilde = current_event->calculate_N_tilde(0.5, 50.00);

				
				vector<PseudoJet> jets = current_event->get_jets();
				
				fmatch << N_tilde << " " << jets.size() << " " << prescale_1 << " " << prescale_2 << endl;
			}

			// We've calculated all we need. Now delete the old pointer (for the previous instance of event) and create a new one.
			delete current_event;

			Event * current_event = new Event(run_number, event_number);

			last_event_number = event_number;

			event_count++;
		}

		current_event->add_particle(px, py, pz, energy);
	}

	// Need to do all these stuff one last time for the final event.

	// Add all the trigger info for this event.
			
	int line_start = (event_count - 1) * no_of_triggers + 1;

	current_event->add_triggers(get_trigger_info(line_start, line_start + no_of_triggers - 1));

	// All triggers stored.

	// Now retrieve the assigned trigger and use that to determine:
	// a) whether to record the current event or not.
	// b) what prescale to use.

	// Calculate jet size (fastjet)
	JetDefinition jet_def(antikt_algorithm, R);
	current_event->calculate_jets(jet_def, pt_cut);

	vector<string> assigned_trigger = current_event->get_assigned_trigger();

	if (assigned_trigger.size() > 0) {

		// Record things only if at least one trigger was fired.

		int prescale_1 = stoi(assigned_trigger[1]);
		int prescale_2 = stoi(assigned_trigger[2]);

		// Calculate N_tilde.
		double N_tilde = current_event->calculate_N_tilde(0.5, 50.00);

		// Calculate jet size (fastjet)
		JetDefinition jet_def(antikt_algorithm, R);
		vector<PseudoJet> jets = current_event->get_jets();
		
		fmatch << event_number << "," << N_tilde << "," << jets.size() << "," << assigned_trigger[0] << "," << prescale_1 << "," << prescale_2 << endl;
	}

}

unordered_map<string, vector<string> > get_trigger_info(int start, int end) {		// start to end inclusive.

	// Read the triggers.csv file from line 'start' to line 'end', compile the info and return it.
	ifstream triggersFile("../triggers.csv");

	unordered_map<string, vector<string> > triggers;

	CSVRow row;
	int lineno = 1;
	while(triggersFile >> row) {

		if (lineno >= start) {
			vector<string> current_trigger {row[3], row[4], row[5]};
			triggers[row[2]] = current_trigger;
		}

		if (lineno == end) {
			return triggers;
			break;
		}

		lineno++;
	}

	return triggers;
}

