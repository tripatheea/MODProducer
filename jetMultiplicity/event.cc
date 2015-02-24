#include <iostream>

#include "fastjet/ClusterSequence.hh"
#include "csv.cc"

using namespace std;
using namespace fastjet;

class Event {
	
	int run_number, event_number;
	vector<PseudoJet> particles;

	public:
		Event(int, int);

		
		int get_size();
		double compute_N_tilde(double R, double pt_cut);	// R, pt_cut. R is the cone radius.

		vector<PseudoJet> get_jets(JetDefinition jet_def, double pt_cut);	// JetDefinition, pt_cut (Fastjet)
		vector<PseudoJet> get_all_particles();

		void add_particle(double px, double py, double pz, double energy);	// px, py, pz, energy.
		void write_to_file(string filename, bool append);

	private:
		double string_to_double(string s);

};

Event::Event(int run_number, int event_number) : run_number(run_number), event_number(event_number) {}

int Event::get_size() {
	return Event::particles.size();
}

double Event::compute_N_tilde(double R, double pt_cut) {
	
	vector<PseudoJet> particles = this->get_all_particles();

	double N_tilde = 0.00;
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
			N_tilde += pt_i / pt_iR;
	}

	return N_tilde;
}

vector<PseudoJet> Event::get_jets(JetDefinition jet_def, double pt_cut) {
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

void Event::write_to_file(string filename, bool append) {
	
	// If the second argument append is true, append to the end of filename instead of overwriting it.
	
	// ofstream file_to_write;

	// if ( ! append)
	// 	ofstream file_to_write (filename, ios::out);
	// else
	// 	ofstream file_to_write (filename, ios_base::app);

	ofstream file_to_write (filename, ios::out);
	int event_number = Event::event_number;
	int run_number = Event::run_number;

	vector<PseudoJet> particles = get_all_particles();

	for (int i = 0; i < particles.size(); i++) {
		cout << run_number << "," << event_number << "," << particles[i].px() << "," << particles[i].py() << "," << particles[i].pz() << "," << particles[i].E() << endl;
		file_to_write << run_number << "," << event_number << "," << particles[i].px() << "," << particles[i].py() << "," << particles[i].pz() << "," << particles[i].E() << endl;
	}

}