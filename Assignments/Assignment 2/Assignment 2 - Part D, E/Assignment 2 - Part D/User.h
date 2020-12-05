#pragma once
#include "common.h"
#include "Shibboleth.h"
#include "Logger.h"
class User : public Logger
{
protected:
    string password;
public:
    string username;
    void set_password(string s) { password = s; }
    string login() {
        Shibboleth p;
        log("Logging in as %s", username.c_str());
        string token = p.getToken(username, password);
        log("Token recieved: %s", token.c_str());
        return token;
    }
};

