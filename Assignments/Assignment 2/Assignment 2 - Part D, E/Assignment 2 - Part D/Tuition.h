#pragma once
#include <vector>
#include <string>

using namespace std;

struct TuitionItem {
	string name;
	double cost;
};
class Tuition: public vector<TuitionItem>
{
public:
	double total() {
		double sum = 0.0;
		for (auto i = this->begin(); i != this->end(); i++) {
			sum += i->cost;
		}
		return sum;
	}
	string as_string() {
		string out = "\n";
		for (auto i = this->begin(); i != this->end(); i++) {
			out += i->name + ": $" + to_string(i->cost) + "\n";
		}
		return out;
	}
};

