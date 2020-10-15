#ifndef __SUPERVISOR_H__
#define __SUPERVISOR_H__

#include "rt.h"
#include "common.h"
#include "Semaphores.h"

class Supervisor : public ActiveClass, public Semaphores
{
private:
	int main() {
		while (!race_over.Read()) {
			while (!car_in_pit.Read());
			pit_entry_light.Wait();
			// Car has entered pit, do stuff here
			Sleep(2000);
			// Read will occasionally break and leave semaphore
			// waited, sleep a little to reduce likelihood of that happening
			refuelling_done.Wait();
			// Signal car to exit
			pit_exit_light.Signal();
			// Wait for car to exit
			while(car_in_pit.Read());

			// Wait for techs to reset before resetting lights
			refuelling_ready.Wait();
			pit_exit_light.Wait();
			pit_entry_light.Signal();
		}
		return 0;
	}
};
#endif