#include <iostream>
#include <unordered_map>

#include "fastjet/ClusterSequence.hh"
#include "csv.cc"

using namespace fastjet;
using namespace std;



double stringToDouble(string s);

double calculate_N_tilde(vector<PseudoJet> particles, double R, double pt_cut);	// R is the cone size.

int main() {

	ifstream file("../PFCandidate.csv");	
	ofstream fmatch ("antikt_multiplicities.dat", ios::out);

	string event_id;
	CSVRow row;
	
	vector<PseudoJet> particles_current_event;
	int event_count = 1;

	double Run, px, py, pz, energy;	
	double R = 0.5;
	double pt_cut = 50.00;



	string last_event_id = "644983557";


	int totalParticles = 1;

	JetDefinition jet_def(antikt_algorithm, R);
	
	int lines = 1;
	while(file >> row) {
		
		lines++;

		event_id = row[1];
		px = stringToDouble(row[2]);
		py = stringToDouble(row[3]);
		pz = stringToDouble(row[4]);
		energy = stringToDouble(row[5]);

		// std:: cout << px << " " << py << std::endl;
		PseudoJet current_particle = PseudoJet(px, py, pz, energy);

		// std::cout << std::endl << std::endl << std::endl << std::endl;

		// std::cout << "Current event ID " << event_id << std::endl;
		// std::cout << "Last event ID " << last_event_id << std::endl;

		// std::cout << event_count << std::endl;






		if (last_event_id != event_id) {
			// We've moved on to a different event.
			// That means, the vector particles_current_event is complete.

			// std:: cout << "Hey there Dellilah!" << std:: endl;

			// std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
			std::cout << event_count << ") " << last_event_id << " Number of particles = " << particles_current_event.size() << ". New event id = " << event_id << std::endl;


			event_count++; 




			totalParticles += particles_current_event.size();

			// So calculate everything from this vector before emptying it for the next event.
			double N_tilde_current_event = calculate_N_tilde(particles_current_event, R, pt_cut);

			// Next, run the clustering, extract the jets using fastjet.
			ClusterSequence cs(particles_current_event, jet_def);
			vector<PseudoJet> jets = cs.inclusive_jets(pt_cut);	

			// Write results from N_tilde and fastjet to a file.
			fmatch << N_tilde_current_event << "\t" << jets.size() << endl;

			// We've calculated all we need. Now empty the vector particles_current_event to make room for particles of new event.
			vector<PseudoJet>().swap(particles_current_event);

		}
		// Enter the current particle to the newly cleared vector.
		particles_current_event.push_back(current_particle);

		last_event_id = event_id;
	}

	

	std::cout << "Total events: " << event_count << std::endl;
	std::cout << "Total lines: " << lines << std::endl;
	std:: cout << "Total number of particles: " << totalParticles << std::endl;

}

double calculate_N_tilde(vector<PseudoJet> particles, double R, double pt_cut) {		// R is the cone size.
	double N_tilde_current_event = 0.00;

	for(int i = 0; i < particles.size(); i++) {
		double pt_i = particles[i].pt();
		double pt_iR = 0.00;
		
		for(int j = 0; j < particles.size(); j++) {
			double pt_j = particles[j].pt();
			double delta_Rij = particles[i].delta_R(particles[j]);

			if (R - delta_Rij > 0)					// heavisideStep
				pt_iR += pt_j;
		}

		if (pt_iR - pt_cut > 0)						// heavisideStep
			N_tilde_current_event += pt_i / pt_iR;
	}

	return N_tilde_current_event;
}

double stringToDouble(string s) {
  double d;
  stringstream ss(s); // Turn the string into a stream
  ss >> d; 			  // Convert
  return d;
}

