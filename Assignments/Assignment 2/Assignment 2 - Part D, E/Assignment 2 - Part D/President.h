#pragma once
#include <vector>
#include "User.h"
#include "Shibboleth.h"
#include "common.h"

// Use singleton pattern so that we have persistent
// storage to simulate an email inbox
class President :
    public User
{
public:
    static President* get_instance() {
        if (inst == nullptr)
            inst = new President();
		return inst;
    }

    void notify_case(Case c) {
        cases.push_back(c);
    }

    void investigate_cases();
private:
    string username = "ono";
    string password = "ono";
    vector<Case> cases;
    static President* inst;
    President() {
        log_name = string("PRES");
    }
};

