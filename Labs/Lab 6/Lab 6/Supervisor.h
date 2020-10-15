#ifndef __SUPERVISOR_H__
#define __SUPERVISOR_H__

#include "rt.h"
#include "common.h"

class Supervisor : public ActiveClass
{
private:
	int main() {
		while (!race_over.Read()) {
			while (!car_in_pit.Read());
			pit_entry_light.Wait();
			// Car has entered pit, do stuff here
			Sleep(3000);
			// Signal car to exit
			pit_exit_light.Signal();
			// Wait for car to exit
			while(car_in_pit.Read());

			// Wait a bit befor resetting lights
			Sleep(1000);
			pit_exit_light.Wait();
			pit_entry_light.Signal();
		}
		return 0;
	}
};
#endif