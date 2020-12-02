#pragma once
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
	ENVE
};
enum AccountType {
	INVALID,
	STUDENT,
	PROFESSOR,
	PRESIDENT
};

uint16_t id_from_str(string username);
