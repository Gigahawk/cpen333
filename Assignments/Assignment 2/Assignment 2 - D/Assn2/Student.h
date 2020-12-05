#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "common.h"
#include "Shibboleth.h"
#include "SSC.h"
#include "Tuition.h"
#include "Logger.h"

#define MAX_COURSES 8

using namespace std;

class Student :
    public Logger
{
private:
	default_random_engine re;
    string password;
    string cc;
    uint8_t want_courses = MAX_COURSES;
public:
    Student() {
        log_name = string("STUDENT");
    }
    vector<Major> prefs;
    //vector<GradeEntry> grades;
    string username;
    void set_password(string s) { password = s; }
    void set_credit_card(string c) { cc = c; }
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

    void show_grade_report() {
        string token = login();
        SSC s;
        vector<GradeEntry> ges = s.get_grade_report(token);
        log("Grade report for %s: ", username.c_str());
        printf("%s", grade_report_to_str(ges).c_str());
    }

    void pay_tuition() {
        string token = login();
        SSC s;
        Tuition t = s.get_tuition(token);

        // Student can optionally pay any amount, assume they just pay the full tuition
        try {
			PaymentProcessor pp = s.pay_tuition(token);
			Payment p = pp.pay(cc);
			if (!s.verify_payment(token, p)) {
				log("Payment not accepted");
				throw StudentException("Payment error");
			}
			log("Paid successfully");
        }
        catch (SSCException& e) {
			log("Could not pay, student probably not enrolled");
        }
    }
};

