#pragma once
#include <vector>
#include <exception>
#include <string>
#include "Logger.h"
#include "common.h"

using namespace std;


class DatabaseException : public exception {
public:
    DatabaseException(const std::string& msg) : msg(msg) {}
    virtual const char* what() const throw() {
        return msg.c_str();
    }
private:
    string msg;
};



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
        StudentEntry* se = get_student(id);
        se->prefs = prefs;
        log("Placement preferences set to %s", prefs_list_to_str(se->prefs).c_str());
    }

    void set_average(uint16_t id, double avg) {
        log("Setting average for student ID %d", id);
        StudentEntry* se = get_student(id);
        se->average = avg;
        log("Average set to %f", avg);
    }

    void set_placement(uint16_t id, Major m) {
        log("Setting major for student ID %d", id);
        StudentEntry* se = get_student(id);
        se->placement = m;
        log("Major set to %s", major_to_str(m).c_str());
    }

    vector<StudentEntry> get_students() {
        return table;
    }

    void print_db() {
        log("Printing database table");
        for (auto s : table) {
            printf("%s", stud_to_str(s).c_str());
        }
        log("End of database table");
    }

private:
    StudentEntry* get_student(uint16_t id) {
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
    vector<StudentEntry> table;
    static Database* inst;
    Database() {
        log_name = string("DATABASE");
    }
};

