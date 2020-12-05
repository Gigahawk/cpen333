#include "common.h"


uint16_t id_from_str(string username)
{
    hash<string> hasher;
    return hasher(username);
}

string major_to_str(Major m)
{
    return string(MajorStr[m]);
}

string term_to_str(Term t)
{
    return string(TermStr[t]);
}

string course_to_str(CourseEntry c)
{
    return major_to_str(c.faculty) + to_string(c.code);
}

string course_to_str_full(CourseEntry c)
{
    string out = string("\n");
    out += "Course Code: " + course_to_str(c) + "\n";
    out += "ID: " + to_string(c.id) + "\n";
    out += "Term: " + term_to_str(c.term) + "\n";
    out += "Timeslot: " + to_string(c.timeslot) + "\n";
    out += "Seats: " + to_string(c.seats) + "\n";
    return out;
}

string prefs_list_to_str(vector<Major> l)
{
    string out = "";
    for (auto p : l) {
        out += major_to_str(p) + ", ";
    }
    return out;
}

string grade_report_to_str(vector<GradeEntry> ges)
{
    string out = "\n";
    for (auto g : ges) {
        out += g.name + ": ";
        if (g.grade == -1)
            out += "NONE\n";
        else
            out += to_string(g.grade) + "\n";
    }
    return out;
}

double course_average(vector<GradeEntry> ges)
{
    double avg = 0.0;
    for (auto g : ges) {
        avg += g.grade;
    }
    avg /= ges.size();
    return avg;
}

string stud_to_str(StudentEntry s)
{
    string out = string("\n");
    out += "Username: " + string(s.username) + "\n";
    out += "ID: " + to_string(s.id) + "\n";
    out += "Average: " + to_string(s.average) + "\n";
    out += "Placement Prefs: " + prefs_list_to_str(s.prefs) + "\n";
    out += "Placement: " + major_to_str(s.placement) + "\n";
    return out;
}

string seats_available(vector<uint32_t> seats)
{
    string out = "Seats Available:\n";
    for (auto i = 0; i < seats.size(); i++) {
        out += MajorStr[i] + string(": ") + to_string(seats[i]) + "\n";
    }
    return out;
}

string generate_cc()
{
	// Avoiding writing a generator for valid luhn numbers,
	// just use a pregenerated list
	default_random_engine re;
	static const vector<string> ccs = {
		"4716300754905439",
		"4024007156652215",
		"4556591882185063214",
		"5488931044140065",
		"5491637984720643",
		"5206696044555304",
		"372548830448872",
		"343454574559584",
		"349389116539716",
		"6011266609684718",
		"6011307300601724",
		"6011229260159835293",
		"3589913343632847",
		"3530417693936618",
		"3545498424422125494",
		"5567795756964242",
		"5475611900471771",
		"5573367565736856",
		"30266306092272",
		"30426638895137",
		"30262609152228",
		"36815313458555",
		"36986963300916",
		"36150893214628",
		"5038788540271942",
		"6763719671526605",
		"6763974852301373",
		"4026696900594596",
		"4844516943738440",
		"4026894167623154",
		"6373065751925867",
		"6372245492763683",
		"6393452607156473"
	};
	uniform_int_distribution<int> unif_cc = uniform_int_distribution<int>(0, ccs.size());
	return ccs[unif_cc(re)];
}

bool compare_average(StudentEntry s1, StudentEntry s2) {
    return s1.average < s2.average;
}

