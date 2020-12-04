#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Logger.h"
#include "GenericException.h"
#include "Database.h"
#include "Admin.h"
#include "common.h"

using namespace std;


class SSC :
    public Logger
{
public:
    SSC() {
        log_name = string("SSC");
    }
    void submit_2nd_year_prefs(string token, vector<Major> prefs) {
        if (verify_token(token) != STUDENT) {
            log("Account is not a student, cannot specify 2nd year preferences");
            throw SSCException("Invalid student token");
        }
        uint16_t id = get_id_from_token(token);
		log("Connecting to database");
        Database* db = Database::get_instance();
		log("Submitting preferences to database");
        db->set_prefs(id, prefs);
    }

    void place_students() {
		log("Connecting to database");
        Database* db = Database::get_instance();
		log("Requesting student list from database");
        vector<StudentEntry> list = db->get_students();
        vector<StudentEntry> placed;
        StudentEntry curr_s;
        vector<Major> curr_m;
        vector<uint32_t> major_seats = MajorLimit;
        log("Sorting students by average");
        sort(list.begin(), list.end(), compare_average);
        for (auto s : list) {
            printf("%s", stud_to_str(s).c_str());
        }
        log("Placing students");
        while (!list.empty()) {
            curr_s = list.back();
            curr_m = curr_s.prefs;
            for (auto m : curr_m) {
                if (curr_s.average < 50.0) {
                    log("Student %s is failing, will not be placed",
                        curr_s.username.c_str());
                    break;
                }
                if (major_seats[m] > 0) {
                    log(
                        "Assigning student %s to major %s",
                        curr_s.username.c_str(), major_to_str(m).c_str());
                    major_seats[m]--;
                    curr_s.placement = m;
                    break;
                }
            }
            if (curr_s.placement == Major::INVL)
                log("Student %s could not be placed", curr_s.username.c_str());
            placed.push_back(curr_s);
            list.pop_back();
        }
        log("All students placed, seats remaining:");
        printf("%s", seats_available(major_seats).c_str());
        log("Student placements:");
        for (auto s : placed) {
            printf("%s", stud_to_str(s).c_str());
        }
        log("Sending placements to database");
        // Ideally this would be a single database query, doing this for simplicity
        for (auto s : placed) {
            db->set_placement(s.id, s.placement);
        }
        log("Notifying placed students");
        for (auto s : placed) {
            if (s.placement != Major::INVL)
                notify_placement(s);
        }
        log("Notifying admin of unplaced students");
        for (auto s : placed) {
            if (s.placement == Major::INVL)
                notify_admin(s);
        }
    }

    StudentEntry get_student_info(string token) {
        if (verify_token(token) != STUDENT) {
            log("Account is not a student, cannot return student info");
            throw SSCException("Invalid student token");
        }
        uint16_t id = get_id_from_token(token);
		log("Connecting to database");
        Database* db = Database::get_instance();
        StudentEntry s = db->get_student(id);
        log("Retrieved student %s from database", s.username.c_str());
        return s;
    }

    bool attempt_registration(string token, uint16_t course_id) {
        if (verify_token(token) != STUDENT) {
            log("Account is not a student, cannot attempt registration");
            throw SSCException("Invalid student token");
        }
        uint16_t id = get_id_from_token(token);
		log("Connecting to database");
        Database* db = Database::get_instance();
		log("Attempting to register student %d to course %d", id, course_id);
        return db->attempt_registration(id, course_id);
    }

    void notify_placement(StudentEntry s) {
        log("Notifying %s via %s that they have been placed in %s",
            s.username.c_str(),
            (s.username + "@ubc.ca").c_str(),
            major_to_str(s.placement).c_str());
    }

    void notify_admin(StudentEntry s) {
        log("Notifying admin via admin@ubc.ca that %s has not been placed",
            s.username.c_str());
        Admin* a = Admin::get_instance();
        a->notify_unplaced(s);
    }

    vector<CourseEntry> get_course_catalog() {
		log("Connecting to database");
        Database* db = Database::get_instance();
        // SSC courses are publicly available, no need to check token
        return db->get_courses();
    }

private:
    vector<string> split_token(string token) {
        vector<string> sections;
        char* _token= const_cast<char*>(token.c_str());
        char* next_tok = nullptr;
        char* tok = strtok_s(_token, ".", &next_tok);
        while (tok != nullptr) {
            sections.push_back(string(tok));
            tok = strtok_s(nullptr, ".", &next_tok);
        }
        return sections;
    }
    uint16_t get_id_from_token(string token) {
        log("Extracting ID from token: %s", token.c_str());
        vector<string> sections = split_token(token);
        uint16_t id = stoi(sections[2]);
        log("ID extracted from token: %d", id);
        return id;
    }
    AccountType verify_token(string token) {
        log("Verifying token: %s", token.c_str());
        vector<string> sections = split_token(token);

        if (sections[0].compare("AAAA") != 0) {
			log("Unexpected token header");
			return AccountType::INVALID;
        }
        if (sections[3].compare("SHIB") != 0) {
			log("Token not signed by Shibolleth");
			return AccountType::INVALID;
        }

        if (sections[1].compare("STUD") == 0) {
			log("Token authenticates STUDENT with ID %s", sections[2].c_str());
            return AccountType::STUDENT;
        }
        else if (sections[1].compare("PROF") == 0) {
			log("Token authenticates PROFESSOR with ID %s", sections[2].c_str());
            return AccountType::PROFESSOR;
        }
        else if (sections[1].compare("PRES") == 0) {
			log("Token authenticates PRESIDENT with ID %s", sections[2].c_str());
            return AccountType::PRESIDENT;
        }
        log("Unexpected error with token");
        return AccountType::INVALID;

    }
};

