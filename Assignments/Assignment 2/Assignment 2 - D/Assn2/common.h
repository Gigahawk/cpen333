#pragma once
#include <vector>
#include <string>
#include <functional>
#include <random>
#include <iterator>
#include <algorithm>

using namespace std;

enum Standing {GOOD, PROBATION, FAILED};
enum Major {
	ENPH,
	CPEN,
	MECH,
	IGEN,
	ELEC,
	CIVL,
	CHBE,
	BMEG,
	GEOE,
	MTRL,
	MINE,
	MANU,
	ENVE,
	APSC,
	INVL
};

// String names for majors
static const char* MajorStr[] = {
	"ENPH",
	"CPEN",
	"MECH",
	"IGEN",
	"ELEC",
	"CIVL",
	"CHBE",
	"BMEG",
	"GEOE",
	"MTRL",
	"MINE",
	"MANU",
	"ENVE",
	"APSC",
	"INVALID",
};

// Max admittance numbers for each major
static const vector<uint32_t> MajorLimit = {
	1,  //ENPH
	1,  //CPEN
	1,  //MECH
	1,  //IGEN
	1,  //ELEC
	1,  //CIVL
	1,  //CHBE
	1,  //BMEG
	1,  //GEOE
	1,  //MTRL
	1,  //MINE
	1,  //MANU
	1,  //ENVE
	0,  //APSC
	0
};

enum AccountType {
	INVALID,
	STUDENT,
	PROFESSOR,
	PRESIDENT
};

enum Term {
	W1, W2, S1, S2
};
static const char* TermStr[] = {
	"W1", "W2", "S1", "S2"
};

// Database schema of student
struct StudentEntry {
    uint16_t id = 0;
    string username;
    double average = 0;
    double amt_paid = 0;
    vector<Major> prefs;
	//vector<GradeEntry> grades;
	Major placement = Major::INVL;
	vector<uint16_t> registered_courses;
};

struct GradeEntry {
	uint16_t course_id = 0;
	uint16_t stud_id;
	double grade = 0;
	// Used only for reporting
	string name = "";
};

struct ProfEntry {
	uint16_t id = 0;
	string username;
	// add list of teachables later?
};

struct CourseEntry {
    uint16_t id = 0;
	Major faculty = Major::INVL;
	uint16_t code;
	Term term = Term::W1;
	uint8_t timeslot;
	uint8_t credits = 5;
	uint16_t seats = 0;
};

uint16_t id_from_str(string username);
string major_to_str(Major m);
string term_to_str(Term t);
string course_to_str(CourseEntry c);
string course_to_str_full(CourseEntry c);
string prefs_list_to_str(vector<Major> l);

string grade_report_to_str(vector<GradeEntry> ges);
double course_average(vector<GradeEntry> ges);
string stud_to_str(StudentEntry s);
string seats_available(vector<uint32_t> seats);
string generate_cc();
bool compare_average(StudentEntry s1, StudentEntry s2);

