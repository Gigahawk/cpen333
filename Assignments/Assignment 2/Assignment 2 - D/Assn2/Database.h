#pragma once
#include <vector>
#include <string>
#include <random>
#include "Logger.h"
#include "GenericException.h"
#include "Tuition.h"
#include "common.h"

#define NUM_COURSES 50

using namespace std;


// Mock Database
class Database :
    public Logger
{

public:
    static Database* get_instance() {
        if (inst == nullptr) {
            inst = new Database();
        }
		return inst;
    }

    void push_student(string username) {
        uint16_t id = id_from_str(username);
        log("Creating student %s with id %d", username.c_str(), id);
        StudentEntry s;
        s.id = id;
        s.username = username;
        table.push_back(s);
    }

    void set_prefs(uint16_t id, vector<Major> prefs) {
        log("Setting placement preferences for student ID %d", id);
        StudentEntry* se = _get_student(id);
        se->prefs = prefs;
        log("Placement preferences set to %s", prefs_list_to_str(se->prefs).c_str());
    }

    void set_average(uint16_t id, double avg) {
        log("Setting average for student ID %d", id);
        StudentEntry* se = _get_student(id);
        se->average = avg;
        log("Average set to %f", avg);
    }

    void set_placement(uint16_t id, Major m) {
        log("Setting major for student ID %d", id);
        StudentEntry* se = _get_student(id);
        se->placement = m;
        log("Major set to %s", major_to_str(m).c_str());
    }

    Tuition get_tuition(uint16_t id) {
        log("Calculating tuition for student ID %d", id);
        StudentEntry* se = _get_student(id);
        Tuition t;
        vector<CourseEntry> ces;
        for (auto c : se->registered_courses)
            ces.push_back(get_course(c));
        if (!ces.size()) {
            log("Student is not registered for courses, no tuition");
            return t;
        }
        for (auto ce : ces) {
            // Calculation based on credit count would go here, assume always $500 for mockup
            t.push_back({ course_to_str(ce), 500 });
        }
        // General student fees
        t.push_back({ "UPASS", 200 });
        t.push_back({ "REC CENTER", 200 });
        return t;
    }

    bool attempt_registration(uint16_t id, uint16_t course_id) {
        StudentEntry* se = _get_student(id);
        CourseEntry* ce = _get_course(course_id);
        log("Attempting registration for %s (%d), to course %s",
            se->username.c_str(), se->id, course_to_str(*ce).c_str());
        if (se->placement != ce->faculty) {
			log("Student not in major");
            return false;
        }
        if (ce->seats < 1) {
            log("Course full");
            return false;
        }
        if (!no_conflict(*ce, se->registered_courses)) {
            log("Course conflicts with other course");
            return false;
        }
        ce->seats--;
        se->registered_courses.push_back(course_id);
        log("Registered successfully");
        return true;
    }

    void pay_tuition(uint16_t id, double amt) {
        StudentEntry* se = _get_student(id);
        log("Updating payment for %s (%d)", se->username.c_str(), se->id);
        se->amt_paid += amt;
        log("Student has paid a total of $%.2f", se->amt_paid);
    }

    vector<StudentEntry> get_students() {
        return table;
    }

    vector<CourseEntry> get_courses() {
        return course_table;
    }

    void print_db() {
        log("Printing student database table");
        for (auto s : table) {
            printf("%s", stud_to_str(s).c_str());
        }
        log("End of student database table");
    }

    void print_course_db() {
        log("Printing course database table");
        for (auto c : course_table) {
            printf("%s", course_to_str_full(c).c_str());
        }
        log("End of course database table");
    }

    void destroy() {
        log("Clearing data");
        while (!table.empty())
            table.pop_back();
    }

    void populate_course_codes() {
        for (int i = 0; i < NUM_COURSES; i++) {
            CourseEntry c;
            c.code = unif_cc(re);
            c.faculty = (Major)unif_maj(re);
            c.id = id_from_str(course_to_str(c));
            c.term = (Term)unif_term(re);
            // Make classes small to force looking for other courses
            c.seats = 1;
            // Assume timeslots for all courses are fixed, each course just gets assigned
            // some timeslot ID, courses with the same timeslot ID happen at the same time.
            c.timeslot = unif_ts(re);
            course_table.push_back(c);
        }
    }

    StudentEntry get_student(uint16_t id) {
        return *_get_student(id);
    }

    CourseEntry get_course(uint16_t id) {
        return *_get_course(id);
    }
private:
	uniform_int_distribution<int> unif_cc = uniform_int_distribution<int>(100, 599);
	uniform_int_distribution<int> unif_maj = uniform_int_distribution<int>(0, MajorLimit.size() - 2);
	uniform_int_distribution<int> unif_term = uniform_int_distribution<int>(0, 3);
	uniform_int_distribution<int> unif_ts = uniform_int_distribution<int>(0, 5);
	default_random_engine re;
    StudentEntry* _get_student(uint16_t id) {
        log("Looking for student with ID %d", id);
        for (auto i = table.begin(); i != table.end(); ++i) {
            if (i->id == id) {
                log("Found student, username: %s", i->username.c_str());
                return &(*i);
            }
        }
		log("Error: student %d not found", id);
        throw DatabaseException("Student not in database");
    }

    CourseEntry* _get_course(uint16_t id) {
        log("Looking for course with ID %d", id);
        for (auto i = course_table.begin(); i != course_table.end(); ++i) {
            if (i->id == id) {
                log("Found course: %s", course_to_str(*i).c_str());
                return &(*i);
            }
        }
		log("Error: course %d not found", id);
        throw DatabaseException("Course not in database");
    }

    bool no_conflict(CourseEntry ce, vector<uint16_t> course_ids) {
        for (auto cid : course_ids) {
            CourseEntry* cce = _get_course(cid);
            if (ce.timeslot == cce->timeslot) {
                log("%s and %s have a time conflict",
                    course_to_str(ce).c_str(), course_to_str(*cce).c_str());
                return false;
            }
        }
        return true;
    }
    vector<StudentEntry> table;
    vector<CourseEntry> course_table;
    static Database* inst;
    Database() {
        log_name = string("DATABASE");
    }
};

