#ifndef __VISOR_H__
#define __VISOR_H__
#include "rt.h"
#include <random>
#include "common.h"
#include "Driver.h"
#include "Actor.h"

#define VISOR_RATE 1000
#define VISOR_RESET_TIME 1000
#define VISOR_INTERVAL 10
class Visor : public Actor
{
private:
	int main() {
		while (!race_over.Read()) {
			service_start.Wait();
			// Car has entered pit, do stuff here
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

