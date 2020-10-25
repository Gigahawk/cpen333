#ifndef __WHEELNUT_H__
#define __WHEELNUT_H__
#include "rt.h"
#include <random>
#include "common.h"
#include "Driver.h"
#include "WheelActor.h"

class WheelNut : public WheelActor
{
public:
	WheelNut(bool front, bool left) : WheelActor(front, left) {}
	~WheelNut() {
		delete wheelnut;
		delete wheelnut_done;
		delete wheelnut_ready;
	}
private:
	int main() {
		while (!race_over.Read()) {
			wheel_service_start->Wait();
			// Car has been raised, do stuff here
			if ((*curr_driver)->get_tire_health_raw() < LOW_TIRE_THRESH) {
				// Only replace tires if they are actually low
				state = "Preparing";
				Sleep(tech_dist(rnd));
				wheelnut->Signal();
				state = "Removing nut";
				Sleep(WHEELNUT_TIME);
				// Consider nut removed for the tire to be "removed"
				(*curr_driver)->remove_tire(idx);
				wheelnut->Wait();
				state = "Waiting for service";

				signal_child();

				wheelnut->Signal();
				state = "Replacing nut";
				Sleep(WHEELNUT_TIME);
				// Tire is completely replaced when nut is replaced
				(*curr_driver)->replace_tire(idx);
				wheelnut->Wait();

				wheelnut_done->Signal();
				state = "Resetting";
				// Wait for reset;
				Sleep(WHEELNUT_RESET_TIME);
			}
			else {
				// Otherwise do nothing
				wheelnut_done->Signal();

				// Signal child to make sure they indicate ready
				signal_child();
			}
			wheelnut_ready->Signal();
			state = "Idle";
		}
		return 0;
	}
	void signal_child() {
		wheelremoval_start->Signal();
		wheelremoval_done->Wait();
		wheelrefitting_done->Wait();
	}
};
#endif

