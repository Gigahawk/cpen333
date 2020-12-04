#include <stdio.h>
#include <iostream>
#include <random>
#include "Database.h"
#include "Admin.h"
#include "Student.h"

using namespace std;

vector<string> names = {
"sophia", "liam",
"olivia", "noah",
"riley", "jackson",
"emma", "aiden",
"ava", "elijah",
"isabella", "grayson",
"aria", "lucas",
"aaliyah", "oliver",
};

double lower_avg = 45.0;
double upper_avg = 100.0;
uniform_real_distribution<double> unif(lower_avg, upper_avg);
uniform_int_distribution<int> unifi(0, MajorLimit.size() - 2);
uniform_int_distribution<int> unifs(1, MajorLimit.size() - 2);
default_random_engine re;
int main() {
	cout << "Simulating \"Selection and admittance into a year 2 program\" (Case 2)" << endl;
	Database *db = Database::get_instance();
	Admin* a = Admin::get_instance();
	SSC ssc;
	vector<Student> studs;
	cout << "Prepopulating database" << endl;
	for (auto n : names) {
		db->push_student(n);
		db->set_average(id_from_str(n), unif(re));
		Student s = Student();
		s.username = n;
		// Password is valid if same as username
		s.set_password(n);
		int num_prefs = unifs(re);
		for (int i = 0; i < num_prefs; i++) {
			// This will potentially produce duplicate preferences, which would normally
			// never happen, but the sorting algorithm should be able to handle it
			s.prefs.push_back((Major)unifi(re));
		}
		studs.push_back(s);
	}

	cout << "Submitting second year preferences" << endl;
	for (auto s : studs) {
		s.submit_2nd_year_prefs();
	}
	cout << "All 2nd year selections submitted, processing pool" << endl;

	ssc.place_students();

	a->arrange_placement_meetings();

	return 0;
}
