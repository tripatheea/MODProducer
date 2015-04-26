#include <iostream>
#include <fstream>
#include <iomanip> 

using namespace std;

int main() {
	ifstream pfcandidates_file("../pfcandidates");
	ifstream min_bias_file("../minbias");

	ifstream min_bias_triggers_file("../triggers_minbias");
	ifstream pfcandidates_triggers_file("../triggers_pfcandidates");

	ofstream output_file("opendata.dat", ios::out);

	int pf_run_number, pf_event_number;
	double px, py, pz, energy;

	int pf_current_event_number = 0;

	// cout.precision(7);
	
	// while (pfcandidates_file >> pf_run_number >> pf_event_number >> px >> py >> pz >> energy) {
		
	// 	if (pf_current_event_number != pf_event_number) {

	// 		if (pf_current_event_number != 0) {
	// 			output_file << endl << endl << endl;
	// 			output_file << "********************  New Event  ********************" << endl << endl;
	// 			output_file << endl << endl << endl;
	// 		}

	// 		pf_current_event_number = pf_event_number;

	// 		output_file << "Run Number: " << pf_run_number << endl;
	// 		output_file << "Event Number: " << pf_event_number << endl << endl;

	// 		output_file << "####################  PFCandidates  ####################" << endl << endl;
	// 		output_file << "    px             py             pz           energy" << fixed << endl;
	// 	}

	// 	// Now start writing out the 4-vectors.
	// 	output_file << setprecision(7) << std::showpos << px << "     " << std::showpos << py << "     " << setprecision(7) << std::showpos << pz << "     " << setprecision(7) << std::showpos << energy << endl;

	// 	pf_current_event_number = pf_event_number;

	// }

	string name;
	int fired, prescale1, prescale2;

	while (pfcandidates_triggers_file >> pf_run_number >> pf_event_number >> name >> fired >> prescale1 >> prescale2) {
		
		if (pf_current_event_number != pf_event_number) {

			if (pf_current_event_number != 0) {
				output_file << endl << endl << endl;
				output_file << "********************  New Event  ********************" << endl << endl;
				output_file << endl << endl << endl;
			}

			pf_current_event_number = pf_event_number;

			output_file << "Run Number: " << pf_run_number << endl;
			output_file << "Event Number: " << pf_event_number << endl << endl;

			output_file << endl << endl;
			output_file << "####################  PFCandidate Triggers  ####################" << endl << endl;
			output_file << "Fired?   Prescale 1   Prescale 2       Name" << fixed << endl;
		}

		// Now start writing out the 4-vectors.
		output_file << "  " << fired <<  "         " << std::setw(3) << std::setfill('0') << prescale1 << std::setw(3) << std::setfill('0') << "          " << std::setw(3) << std::setfill('0') << prescale2 << "        " << name << endl;

		pf_current_event_number = pf_event_number;

	}



	return 0;
}

