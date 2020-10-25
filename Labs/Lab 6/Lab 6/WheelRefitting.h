#ifndef __WHEELREFITTING_H__
#define __WHEELREFITTING_H__
#include "rt.h"
#include <random>
#include "common.h"
#include "Driver.h"
#include "WheelActor.h"

class WheelRefitting : public WheelActor
{
public:
	WheelRefitting(bool front, bool left) : WheelActor(front, left) {}
private:
	int main() {
		while (!race_over.Read()) {
			wheelrefitting_start->Wait();
			// Tire has been removed, do stuff here
			if ((*curr_driver)->get_tire_health_raw() < LOW_TIRE_THRESH) {
				// Only replace tires if they are actually low
				state = "Preparing";
				Sleep(tech_dist(rnd));
				wheelrefitting->Signal();
				state = "Refitting Tire";
				Sleep(WHEELREFITTING_TIME);
				wheelrefitting->Wait();
				wheelrefitting_done->Signal();
				state = "Resetting";
				// Wait for reset;
				Sleep(WHEELREFITTING_RESET_TIME);
			}
			else {
				// Otherwise do nothing
				wheelrefitting_done->Signal();
			}

			wheelrefitting_ready->Signal();
			state = "Idle";
		}
		return 0;
	}
};
#endif
