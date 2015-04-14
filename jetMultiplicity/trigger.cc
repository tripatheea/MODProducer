#include <iostream>


using namespace std;

class Trigger {
	
	string name_;
	bool fired_;
	pair<int, int> prescales_;

	public:
		Trigger(string name, pair<int, int> prescales, bool fired);
		Trigger();

		string get_name();
		pair<int, int> get_prescales();
		bool fired();
		bool is_valid();

		
};

Trigger::Trigger(string name, pair<int, int> prescales, bool fired) : name_(name), prescales_(prescales), fired_(fired) {}
Trigger::Trigger() {}

string Trigger::get_name() {
	return Trigger::name_;
}

pair<int, int> Trigger::get_prescales() {
	return Trigger::prescales_;
}

bool Trigger::fired() {
	return Trigger::fired_;
}

bool Trigger::is_valid() {
	return ( ! name_.empty());
}