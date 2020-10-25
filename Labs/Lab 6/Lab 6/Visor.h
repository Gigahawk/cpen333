#ifndef __VISOR_H__
#define __VISOR_H__
#include "rt.h"
#include <random>
#include "common.h"
#include "Driver.h"
#include "Actor.h"

class Visor : public Actor
{
private:
	int main() {
		while (!race_over.Read()) {
			service_start.Wait();
			// Car has entered pit, do stuff here
			state = "Preparing";
			Sleep(tech_dist(rnd));
			visor.Signal();
			state = "Cleaning visor";
			while (!(*curr_driver)->clean_visor(VISOR_RATE))
				Sleep(VISOR_INTERVAL);
			visor.Wait();
			visor_done.Signal();
			state = "Resetting";
			// Wait for reset;
			Sleep(VISOR_RESET_TIME);
			visor_ready.Signal();
			state = "Idle";
		}
		return 0;
	}
};

#endif

