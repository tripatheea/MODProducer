#include <iostream>
#include <unordered_map>

#include "fastjet/ClusterSequence.hh"
#include "csv.cc"

using namespace std;
using namespace fastjet;

class Event {
	
	int run_number, event_number;
	vector<PseudoJet> particles;
	unordered_map<string, vector<string > > triggers;	// The key is name of the trigger. The vector must have three elements- fired (boolean), prescale 1 (int) and prescale 2 (int). 


	public:
		Event(int, int);

		int size();
		double calculate_N_tilde(double R, double pt_cut);	// R, pt_cut. R is the cone radius.

		vector<PseudoJet> get_jets(JetDefinition jet_def, double pt_cut);	// JetDefinition, pt_cut (Fastjet)
		vector<PseudoJet> get_all_particles();

		void add_particle(double px, double py, double pz, double energy);	// px, py, pz, energy.
		void add_triggers(unordered_map<string, vector< string> > triggers);	// px, py, pz, energy.
		void write_to_file(string filename);	// Will append if file already exists.

		vector<string> get_assigned_trigger();
};

Event::Event(int run_number, int event_number) : run_number(run_number), event_number(event_number) {}

int Event::size() {
	return Event::particles.size();
}

double Event::calculate_N_tilde(double R, double pt_cut) {
	vector<PseudoJet> particles = this->get_all_particles();

	double N_tilde_current_event = 0.00;

	for(int i = 0; i < particles.size(); i++) {
		double pt_i = particles[i].pt();
		double pt_iR = 0.00;
		
		for(int j = 0; j < particles.size(); j++) {
			double pt_j = particles[j].pt();
			double squared_distance = particles[i].squared_distance(particles[j]);			// squared_distance instead of delta_R to speed things up.

			if (R*R > squared_distance)					// heavisideStep
				pt_iR += pt_j;
		}

		if (pt_iR > pt_cut)								// heavisideStep
			N_tilde_current_event += pt_i / pt_iR;
	}

	return N_tilde_current_event;
}

vector<PseudoJet> Event::get_jets(JetDefinition jet_def, double pt_cut) {
	vector<PseudoJet> particles = get_all_particles();

	// Run the clustering, extract the jets using fastjet.
	ClusterSequence cs(particles, jet_def);
	return cs.inclusive_jets(pt_cut);
}

vector<PseudoJet> Event::get_all_particles() {
	return particles;
}

void Event::add_particle(double px, double py, double pz, double energy) {
	Event::particles.push_back(PseudoJet(px, py, pz, energy));
}

void Event::add_triggers(unordered_map<string, vector< string> > triggers) {
	Event::triggers = triggers;
}

void Event::write_to_file(string filename) {
	ofstream file_to_write (filename, ios_base::app);

	int event_number = Event::event_number;
	int run_number = Event::run_number;

	vector<PseudoJet> particles = get_all_particles();

	for (int i = 0; i < particles.size(); i++) {
		file_to_write << run_number << "," << event_number << "," << particles[i].px() << "," << particles[i].py() << "," << particles[i].pz() << "," << particles[i].E() << endl;
	}
}

vector<string> Event::get_assigned_trigger() {
	// Since multiple triggers fire for a single event, this method selects one trigger out of all those that were fired and returns it.
	// Right now, it just uses the order of the triggers as defined in the array triggers below. 
	// There should be a better way to do this.

	vector<string> triggersThatMatter {"HLT_L1Jet6U", "HLT_L1Jet10U", "HLT_Jet15U", "HLT_Jet30U", "HLT_Jet50U", "HLT_Jet70U", "HLT_Jet100U"};

	for(unsigned int i = 0; i < triggersThatMatter.size(); i++) {
		
		vector<string> current_trigger = Event::triggers[triggersThatMatter[i]]; // This vector holds {fired, prescale_1, prescale_2}.
		
		if (stoi(current_trigger[0]) == 1) {
			// This trigger was fired. Pack it into a vector and return it.
			// Does not include fired because we only return those triggers which were fired
			std::vector<string> assigned_trigger {triggersThatMatter[i], current_trigger[1], current_trigger[2]};		// {trigger_name, prescale_1, prescale_2}
			return assigned_trigger;
		}

	}

	// No trigger was fired for this event.
	return vector<string>();
}