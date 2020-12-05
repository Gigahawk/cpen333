#include "President.h"
#include "SSC.h"

President* President::inst = nullptr;
void President::investigate_cases() {
	SSC ssc;
	Shibboleth s;
	string token = s.getToken(username, password);
	log("Investigating misconduct cases");
	for (auto c : cases) {
		log("Student ID %d vs. Prof ID %d", c.sre.stud_id, c.sre.prof_id);
		log("Professor's Statement: %s", c.sce.msg.c_str());
		log("Student's Response: %s", c.sce.msg.c_str());
		log("Hmm...");
		// Normally deliberation/a hearing would happen here, let's assume
		// the student is always guilty.
		log("GUILTY");

		ssc.suspend_student(token, c.sre.stud_id, c.sre.course_id);
	}
}
