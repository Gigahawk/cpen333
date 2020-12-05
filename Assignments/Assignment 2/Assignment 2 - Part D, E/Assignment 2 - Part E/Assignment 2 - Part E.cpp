#include "pch.h"
#include "windows.h"
#include "CppUnitTest.h"
#include "../Assignment 2 - Part D/common.h"
#include "../Assignment 2 - Part D/common.cpp"
#include "../Assignment 2 - Part D/GenericException.h"
#include "../Assignment 2 - Part D/Logger.h"
#include "../Assignment 2 - Part D/Logger.cpp"
#include "../Assignment 2 - Part D/Database.h"
#include "../Assignment 2 - Part D/Database.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Assignment2PartE
{
	TEST_CLASS(Assignment2PartE)
	{
	public:
		TEST_METHOD_INITIALIZE(Setup)
		{
			Database* db = Database::get_instance();
			CourseEntry c;
			c.code = 669;
			c.faculty = Major::APSC;
			c.id = id_from_str(course_to_str(c));
			c.term = Term::W1;
			c.credits = 5;
			c.seats = 1;
			c.timeslot = 99;
			db->push_course(c);
		}
		TEST_METHOD_CLEANUP(Cleanup)
		{
			Database* db = Database::get_instance();
			db->destroy();
			delete db;
		}

		TEST_METHOD(MissingFromDatabase)
		{
			Database* db = Database::get_instance();
			try {
				db->get_course(999);
			}
			catch (DatabaseException&) {
				return;
			}
			// Error, database should have thrown an exception
			Assert::Fail();
		}

		TEST_METHOD(AvailableInDatabase)
		{
			Database* db = Database::get_instance();
			// Should not crash
			db->get_course(id_from_str("APSC669"));
		}

		TEST_METHOD(RegistrationSuspension)
		{
			Database* db = Database::get_instance();
			string name = "cjasper";
			uint16_t sid = id_from_str(name);
			uint16_t cid = id_from_str("APSC669");
			db->push_student(name);
			db->suspend_student(sid);
			Assert::IsFalse(db->attempt_registration(sid, cid));
			Sleep(11000);
			Assert::IsTrue(db->attempt_registration(sid, cid));
		}

		TEST_METHOD(RegistrationFull)
		{
			Database* db = Database::get_instance();
			string name1 = "cjasper";
			string name2 = "bjasper";
			uint16_t sid1 = id_from_str(name1);
			uint16_t sid2 = id_from_str(name2);
			uint16_t cid = id_from_str("APSC669");
			db->push_student(name1);
			db->push_student(name2);
			Assert::IsTrue(db->attempt_registration(sid1, cid));
			Assert::IsFalse(db->attempt_registration(sid2, cid));
		}

		TEST_METHOD(RegistrationConflict)
		{
			Database* db = Database::get_instance();

			CourseEntry c;
			c.code = 666;
			c.faculty = Major::APSC;
			c.id = id_from_str(course_to_str(c));
			c.term = Term::W1;
			c.credits = 5;
			c.seats = 1;
			c.timeslot = 99;
			db->push_course(c);

			string name = "cjasper";
			uint16_t sid = id_from_str(name);
			// Both courses have a timeslot ID of 99
			uint16_t cid1 = id_from_str("APSC669");
			uint16_t cid2 = id_from_str("APSC666");
			db->push_student(name);
			Assert::IsTrue(db->attempt_registration(sid, cid1));
			Assert::IsFalse(db->attempt_registration(sid, cid2));
		}
	};
}
