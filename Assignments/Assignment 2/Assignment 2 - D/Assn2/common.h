#pragma once
#include <vector>
#include <string>
#include <functional>

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
	1, //MTRL
	1, //MINE
	1, //MANU
	1, //ENVE
	0
};

enum AccountType {
	INVALID,
	STUDENT,
	PROFESSOR,
	PRESIDENT
};

// Database schema of student
struct StudentEntry {
    uint16_t id = 0;
    string username;
    double average = 0;
    vector<Major> prefs;
	Major placement = Major::INVL;
};

uint16_t id_from_str(string username);
string major_to_str(Major m);
string prefs_list_to_str(vector<Major> l);

string stud_to_str(StudentEntry s);
string seats_available(vector<uint32_t> seats);
bool compare_average(StudentEntry s1, StudentEntry s2);
