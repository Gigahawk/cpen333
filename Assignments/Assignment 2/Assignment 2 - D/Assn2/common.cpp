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

bool compare_average(StudentEntry s1, StudentEntry s2) {
    return s1.average < s2.average;
}

