#pragma once
#include <string>
#include <functional>
#include "Logger.h"
#include "common.h"

using namespace std;

class Shibboleth : public Logger
{
public:
	Shibboleth() {
		log_name = string("SHIBBOLETH");
	}

	/**
	 * @brief Simulate logging into SSC, Canvas, etc. via Shibboleth
	 * @param username username
	 * @param password password
	 * @return Fake JSON Web Token like string
	*/
	string getToken(string username, string password) {
		string header = "AAAA";
		string claim;
		string sig = "SHIB";
		string jwt;

		log("Authentication request from %s", username.c_str());

		// Use password as username for mockup
		if (username.compare(password) != 0) {
			log("Authentication failed");
			return "";
		}

		// Generate ID based on username
		int id = id_from_str(username);

		// Provide professor token
		if (username.compare("pwalls") == 0)
			claim = "PROF";
		else if (username.compare("ono") == 0)
			claim = "PRES";
		else
			claim = "STUD";
		jwt = header + "." + claim + "." + to_string(id) + "." + sig;
		log("Credentials validated, sending token: %s", jwt.c_str());
		return jwt;
	}
};

