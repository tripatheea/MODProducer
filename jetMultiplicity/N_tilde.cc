#include <iostream>

#include "event.cc"


#include "fastjet/ClusterSequence.hh"

using namespace std;

double string_to_double(string s);

int main() {
	ifstream file("../PFCandidate.csv");
	CSVRow row;

	double Run, px, py, pz, energy;	
	double R = 0.5;
	double pt_cut = 50.00;
	double particleType;

	string event_id;

	int i = 1;

	Event apple = Event(149291, 644983557);

	while(file >> row) {

		event_id = row[1];
		px = string_to_double(row[2]);
		py = string_to_double(row[3]);
		pz = string_to_double(row[4]);
		energy = string_to_double(row[5]);

		PseudoJet current_particle = PseudoJet(px, py, pz, energy);

		if ("644983557" == event_id) {
			apple.add_particle(px, py, pz, energy);
		}
	}

	cout << apple.get_size() << endl;
	apple.write_to_file("banana.csv", false);
}

double string_to_double(string s) {
  double d;
  stringstream ss(s); // Turn the string into a stream
  ss >> d; 			  // Convert
  return d;
}

