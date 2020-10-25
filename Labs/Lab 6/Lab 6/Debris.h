#ifndef __DEBRIS_H__
#define __DEBRIS_H__
#include "rt.h"
#include <random>
#include "common.h"
#include "Driver.h"
#include "Actor.h"

class Debris : public Actor
{
private:
	int main() {
		while (!race_over.Read()) {
			service_start.Wait();
			// Car has entered pit, do stuff here
			state = "Preparing";
			Sleep(tech_dist(rnd));
			debris.Signal();
			state = "Cleaning intake";
			while (!(*curr_driver)->clean_intake(DEBRIS_RATE))
				Sleep(DEBRIS_INTERVAL);
			debris.Wait();
			debris_done.Signal();
			state = "Resetting";
			// Wait for reset;
			Sleep(DEBRIS_RESET_TIME);
			debris_ready.Signal();
			state = "Idle";
		}
		return 0;
	}
};
#endif
