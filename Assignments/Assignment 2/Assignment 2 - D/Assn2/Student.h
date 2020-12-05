#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "common.h"
#include "Shibboleth.h"
#include "SSC.h"
#include "Logger.h"

#define MAX_COURSES 8

using namespace std;

class Student :
    public Logger
{
private:
	default_random_engine re;
    string password;
    uint8_t want_courses = MAX_COURSES;
public:
    Student() {
        log_name = string("STUDENT");
    }
    vector<Major> prefs;
    //vector<GradeEntry> grades;
    string username;
    void set_password(string s) { password = s; }
    string login() {
        Shibboleth s;
        log("Logging in as %s", username.c_str());
        string token = s.getToken(username, password);
        log("Token recieved: %s", token.c_str());
        return token;
    }

    void submit_2nd_year_prefs() {
        string token = login();
        SSC s;
        s.submit_2nd_year_prefs(token, prefs);
    }

    void register_for_courses() {
        string token = login();
        SSC s;
        StudentEntry se = s.get_student_info(token);
        vector<CourseEntry> cc = s.get_course_catalog();
        shuffle(cc.begin(), cc.end(), re);
        log("%s is attempting to register for %d courses", username.c_str(), want_courses);
        while (!cc.empty() && want_courses) {
            CourseEntry cce = cc.back();
            cc.pop_back();
            // Skip wrong faculty courses to reduce log output
            if (cce.faculty != se.placement)
                continue;
            if (s.attempt_registration(token, cce.id))
                want_courses--;
        }
        log("%s was able to register for %d courses", username.c_str(), MAX_COURSES - want_courses);
    }

};

