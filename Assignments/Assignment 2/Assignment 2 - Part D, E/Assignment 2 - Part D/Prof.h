#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "common.h"
#include "Shibboleth.h"
#include "SSC.h"
#include "Logger.h"
#include "User.h"

using namespace std;

class Prof :
    public User
{
private:
    default_random_engine re;

public:
    Prof() {
        log_name = string("PROFESSOR");
    }
    vector<GradeEntry> grades;

    void submit_grades() {
        string token = login();
        SSC s;
        s.submit_grades(token, grades);
        log("%s submitted grades to database", username.c_str());
    }

    void submit_statement_of_case(string stud_name, string course_name) {
        string token = login();
        // Could also contain info such as course and misconduct type,
        // but just using a message for simplicity
        string msg = stud_name + " is bad";
        SSC s;
        s.submit_statement_of_case(token, stud_name, course_name, msg);
        log("%s submitted statement of case to SSC", username.c_str());
    }

};

