#ifndef __WHEELREMOVAL_H__
#define __WHEELREMOVAL_H__
#include "rt.h"
#include <random>
#include "common.h"
#include "Driver.h"
#include "WheelActor.h"

class WheelRemoval : public WheelActor
{
public:
	WheelRemoval(bool front, bool left) : WheelActor(front, left) {}
private:
	int main() {
		while (!race_over.Read()) {
			wheelremoval_start->Wait();
			// Nut has been removed, do stuff here
			if ((*curr_driver)->get_tire_health_raw() < LOW_TIRE_THRESH) {
				// Only replace tires if they are actually low
				state = "Preparing";
				Sleep(tech_dist(rnd));
				wheelremoval->Signal();
				state = "Removing Tyre";
				Sleep(WHEELREMOVAL_TIME);
				wheelremoval->Wait();
				wheelremoval_done->Signal();

				wheelrefitting_start->Signal();

				state = "Resetting";
				// Wait for reset;
				Sleep(WHEELREMOVAL_RESET_TIME);
			}
			else {
				// Otherwise do nothing
				wheelremoval_done->Signal();

				// Signal child to make sure they indicate ready
				wheelrefitting_start->Signal();
			}

			wheelremoval_ready->Signal();
			state = "Idle";
		}
		return 0;
	}
};
#endif
