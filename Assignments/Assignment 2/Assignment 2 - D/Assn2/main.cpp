#include <stdio.h>
#include <iostream>
#include <random>
#include "Logger.h"
#include "Database.h"
#include "Admin.h"
#include "Student.h"
#include "Tuition.h"

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
uniform_real_distribution<double> unif_avg(lower_avg, upper_avg);
uniform_int_distribution<int> unif_maj_sel(0, MajorLimit.size() - 2);
uniform_int_distribution<int> unif_maj_num(1, MajorLimit.size() - 2);
default_random_engine re;
vector<Student> studs;


void init();
void case_2();
void case_3();

int main() {
	case_2();
	case_3();

	return 0;
}

void case_2() {
	cout << "Simulating \"Selection and admittance into a year 2 program\" (Case 2)" << endl;
	init();
	Admin* a = Admin::get_instance();
	SSC ssc;

	cout << "Submitting second year preferences" << endl;
	for (auto s : studs) {
		int num_prefs = unif_maj_num(re);
		for (int i = 0; i < num_prefs; i++) {
			// This will potentially produce duplicate preferences, which would normally
			// never happen, but the sorting algorithm should be able to handle it
			s.prefs.push_back((Major)unif_maj_sel(re));
		}
		s.submit_2nd_year_prefs();
	}
	cout << "All 2nd year selections submitted, processing pool" << endl;

	ssc.place_students();

	a->arrange_placement_meetings();
}

void case_3() {
	cout << "Simulating \"View and register for courses in T1 or T2 (or summer) and pay tuition fees\" (Case 2)" << endl;
	init();

	cout << "Populating course database" << endl;
	Database *db = Database::get_instance();
	db->populate_course_codes();
	db->print_course_db();
	SSC ssc;


	// Rerun main portion of case 2 so that students have a major that can be used
	// to check course eligibility
	cout << "Placing students in majors" << endl;
	show_log(false);
	for (auto s : studs) {
		int num_prefs = unif_maj_num(re);
		for (int i = 0; i < num_prefs; i++) {
			// This will potentially produce duplicate preferences, which would normally
			// never happen, but the sorting algorithm should be able to handle it
			s.prefs.push_back((Major)unif_maj_sel(re));
		}
		s.submit_2nd_year_prefs();
	}
	ssc.place_students();
	show_log(true);

	cout << "Opening registration" << endl;
	for (auto s : studs)
		s.register_for_courses();

	cout << "Payment is due" << endl;
	for (auto s : studs)
		s.pay_tuition();
}

void init() {
	cout << "Reinitializing singletons" << endl;
	show_log(false);
	Database *db = Database::get_instance();
	Admin* a = Admin::get_instance();
	// Clear old data
	db->destroy();
	a->destroy();
	while (!studs.empty()) {
		studs.pop_back();
	}
	cout << "Prepopulating database" << endl;
	for (auto n : names) {
		db->push_student(n);
		db->set_average(id_from_str(n), unif_avg(re));
		Student s = Student();
		s.username = n;
		// Password is valid if same as username
		s.set_password(n);
		s.set_credit_card(generate_cc());
		studs.push_back(s);
	}
	show_log(true);
}
