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

string prefs_list_to_str(vector<Major> l)
{
    string out = "";
    for (auto p : l) {
        out += major_to_str(p) + ", ";
    }
    return out;
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

