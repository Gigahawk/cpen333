#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Logger.h"
#include "GenericException.h"
#include "Database.h"
#include "Admin.h"
#include "PaymentProcessor.h"
#include "President.h"
#include "common.h"

using namespace std;

class President;

class SSC :
    public Logger
{
public:
    SSC() {
        log_name = string("SSC");
    }

    void submit_grades(string token, vector<GradeEntry> grades) {
        if (verify_token(token) != PROFESSOR) {
            log("Account is not a professor, cannot submit grades");
            throw SSCException("Invalid professor token");
        }
        uint16_t id = get_id_from_token(token);
        log("Connecting to database");
        Database* db = Database::get_instance();
        log("Submitting grades to database");
        // Ideally this would be a single database query, doing this for simplicity
        for (auto g : grades) {
            db->update_grade(g);
        }
        log("Grades submitted to database");
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

    void submit_statement_of_case(string token, string stud_name, string course_name, string msg) {
        if (verify_token(token) != PROFESSOR) {
            log("Account is not a professor, cannot submit statement of case");
            throw SSCException("Invalid professor token");
        }
        uint16_t id = get_id_from_token(token);
        uint16_t stud_id = id_from_str(stud_name);
        uint16_t course_id = id_from_str(course_name);
        SoCEntry sce;
        sce.prof_id = id;
        sce.stud_id = stud_id;
        sce.course_id = course_id;
        sce.msg = msg;
		log("Connecting to database");
        Database* db = Database::get_instance();
		log("Submitting statement of case to database");
        db->push_statement_of_case(sce);

        log("Sending email to request response from student");
    }

    bool submit_statement_of_resp(string token, uint16_t prof_id, uint16_t course_id, string msg) {
        if (verify_token(token) != STUDENT) {
            log("Account is not a student, cannot submit statement of response");
            throw SSCException("Invalid student token");
        }
        uint16_t id = get_id_from_token(token);
        SoREntry sre;
        sre.prof_id = prof_id;
        sre.stud_id = id;
        sre.course_id = course_id;
        sre.msg = msg;
		log("Connecting to database");
        Database* db = Database::get_instance();
		log("Submitting statement of response to database");
        if (db->push_statement_of_resp(sre)) {
			// If no exception thrown assume db has logged both statement of case and statement of response
			log("Retrieving matching statement of case");
            SoCEntry sce = db->get_soc(course_id, id);
			log("Sending email to president to arrange hearing");
            President* p = President::get_instance();
            p->notify_case({ sce, sre });
            return true;
        }
        return false;
    }

    vector<SoCEntry> get_misconduct_allegations(string token) {
        if (verify_token(token) != STUDENT) {
            log("Account is not a student, cannot retrieve misconduct allegations");
            throw SSCException("Invalid student token");
        }
        uint16_t id = get_id_from_token(token);
		log("Connecting to database");
        Database* db = Database::get_instance();
		log("Requesting misconduct allegations");
        return db->get_socs_by_stud_id(id);
    }

    void suspend_student(string token, uint16_t stud_id, uint16_t course_id) {
        if (verify_token(token) != PRESIDENT) {
            log("Account is not a president, cannot suspend students");
            throw SSCException("Invalid president token");
        }
		log("Connecting to database");
        Database* db = Database::get_instance();
        if (course_id) {
            GradeEntry ge;
            ge.course_id = course_id;
            ge.stud_id = stud_id;
            ge.grade = 0.0;
            log("Setting grade course %d for student %d to 0");
            db->update_grade(ge);
        }
        log("SUSPENDING STUDENT ID %d", stud_id);
        db->suspend_student(stud_id);
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

    vector<GradeEntry> get_grade_report(string token) {
        if (verify_token(token) != STUDENT) {
            log("Account is not a student, cannot return grade report");
            throw SSCException("Invalid student token");
        }
        uint16_t id = get_id_from_token(token);
		log("Connecting to database");
        Database* db = Database::get_instance();
        vector<GradeEntry> ges = db->get_grade_report(id);
        log("Retrieved grade report from database");
        return ges;

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

    Tuition get_tuition(string token) {
        if (verify_token(token) != STUDENT) {
            log("Account is not a student, cannot return student info");
            throw SSCException("Invalid student token");
        }
        uint16_t id = get_id_from_token(token);
		log("Connecting to database");
        Database* db = Database::get_instance();
        Tuition t = db->get_tuition(id);
        log("Retrieved tuition for student %d from database", id);
        cout << t.as_string() << endl;
        cout << "Total: $" << t.total() << endl;
        return t;
    }

    PaymentProcessor pay_tuition(string token) {
        Tuition t = get_tuition(token);
        uint16_t id = get_id_from_token(token);
        if (t.total() == 0.0) {
            log("Error: tuition is $0, student is probably not registered");
            throw SSCException("No Tuition");
        }
        PaymentProcessor p(t.total(), id);
        return p;
    }

    bool verify_payment(string token, Payment p) {
        if (verify_token(token) != STUDENT) {
            log("Account is not a student, cannot return student info");
            throw SSCException("Invalid student token");
        }
        uint16_t id = get_id_from_token(token);
        if (p.sig.compare("PAID") != 0) {
            log("Invalid payment certificate");
            return false;
        }
        if (p.id != id) {
            log("Payment certifite ID mismatch");
            return false;
        }
		log("Payment validated");
		log("Connecting to database");
        Database* db = Database::get_instance();
        db->pay_tuition(id, p.amt);
        return true;
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

