#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Logger.h"
#include "Database.h"
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
        }
        uint16_t id = get_id_from_token(token);
		log("Conecting to database");
        Database* db = Database::getInstance();
		log("Submitting preferences to database");
        db->set_prefs(id, prefs);
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

