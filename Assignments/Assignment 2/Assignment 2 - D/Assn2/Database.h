#pragma once
#include <vector>
#include "Logger.h"
#include "common.h"

using namespace std;

struct StudentEntry {
    uint16_t id;
    string username;
    vector<Major> prefs;
};

class Database :
    public Logger
{
public:
    static Database* getInstance() {
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
        return nullptr;
    }
    vector<StudentEntry> table;
    static Database* inst;
    Database() {
        log_name = string("DATABASE");
    }
};

