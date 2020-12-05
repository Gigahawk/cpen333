#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "common.h"
#include "Shibboleth.h"
#include "SSC.h"
#include "Logger.h"

using namespace std;

class Prof :
    public Logger
{
private:
    default_random_engine re;
    string password;

public:
    Prof() {
        log_name = string("PROFESSOR");
    }
    vector<GradeEntry> grades;
    string username;
    void set_password(string s) { password = s; }
    string login() {
        Shibboleth p;
        log("Logging in as %s", username.c_str());
        string token = p.getToken(username, password);
        log("Token recieved: %s", token.c_str());
        return token;
    }

    void submit_grades() {
        string token = login();
        SSC s;
        s.submit_grades(token, grades);
        log("%s submitted grades to database", username.c_str());
    }

};

