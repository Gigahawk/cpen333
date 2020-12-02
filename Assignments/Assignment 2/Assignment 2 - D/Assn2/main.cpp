#include <stdio.h>
#include <iostream>
#include "Database.h"
#include "Student.h"

using namespace std;

int main() {
	Database *db = Database::getInstance();
	Student s;
	db->push_student("cjasper");
	cout << "Simulating \"Selection and admittance into a year 2 program\" (Case 2)" << endl;
	s.username = "cjasper";
	s.set_password("cjasper");
	s.prefs.push_back(Major::ENPH);
	s.prefs.push_back(Major::CPEN);
	s.prefs.push_back(Major::MECH);
	s.submit_2nd_year_prefs();
	return 0;
}
