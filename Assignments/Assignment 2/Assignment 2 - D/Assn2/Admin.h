#pragma once
#include <vector>
#include <string>
#include "Logger.h"
#include "common.h"

using namespace std;

class Admin :
    public Logger
{
public:
    static Admin* get_instance() {
        if (inst == nullptr) {
            inst = new Admin();
        }
		return inst;
    }

    void notify_unplaced(StudentEntry s) {
        unplaced.push_back(s);
    }

    void arrange_placement_meetings() {
        log("Checking list of unplaced students");
        for (auto s : unplaced) {
            log("%s is unplaced, scheduling meeting and sending email", s.username.c_str());
        }
    }

    void destroy() {
        log("Resetting data");
        while (!unplaced.empty())
            unplaced.pop_back();
    }
private:
    vector<StudentEntry> unplaced;
    static Admin* inst;
    Admin() {
        log_name = string("ADMIN");
    }
};

