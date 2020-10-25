#ifndef __REFUELLING_H__
#define __REFUELLING_H__
#include "rt.h"
#include "common.h"
#include "Driver.h"
#include "Actor.h"


#define REFUELLING_RATE 10000000
#define REFUELLING_RESET_TIME 3000
#define REFUELLING_INTERVAL 1

class Refuelling : public Actor
{
private:
	int main() {
		while (!race_over.Read()) {
			service_start.Wait();
			// Car has entered pit, do stuff here
			refuelling.Signal();
			state = "Fuelling";
			while (!(*curr_driver)->add_fuel(REFUELLING_RATE))
				Sleep(REFUELLING_INTERVAL);
			refuelling.Wait();
			refuelling_done.Signal();
			state = "Resetting";
			// Wait for reset;
			Sleep(REFUELLING_RESET_TIME);
			refuelling_ready.Signal();
			state = "Idle";
		}
		return 0;
	}
};
#endif
