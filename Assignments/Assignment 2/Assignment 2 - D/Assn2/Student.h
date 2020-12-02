#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "common.h"
#include "Shibboleth.h"
#include "SSC.h"
#include "Logger.h"
using namespace std;

class Student :
    public Logger
{
private:
    string password;
public:
    Student() {
        log_name = string("STUDENT");
    }
    vector<Major> prefs;
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
};

