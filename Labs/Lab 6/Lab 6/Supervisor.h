#ifndef __SUPERVISOR_H__
#define __SUPERVISOR_H__

#include "rt.h"
#include "common.h"
#include "Actor.h"

class Supervisor : public Actor
{
private:
	int main() {
		while (!race_over.Read()) {
			while (!car_in_pit.Read());
			pit_entry_light.Wait();
			// Car has entered pit, do stuff here
			service_start.Signal(NUM_TECHNICIANS);
			state = "Waiting for service";
			refuelling_done.Wait();
			visor_done.Wait();
			debris_done.Wait();
			jacking_front_done.Wait();
			jacking_back_done.Wait();
			// Signal car to exit
			pit_exit_light.Signal();

			// Wait for techs to reset before resetting lights
			state = "Waiting for reset";
			refuelling_ready.Wait();

			visor_ready.Wait();

			debris_ready.Wait();

			jacking_front_ready.Wait();
			jacking_back_ready.Wait();

			wheelnut_front_left_ready.Wait();
			wheelnut_front_right_ready.Wait();
			wheelnut_back_left_ready.Wait();
			wheelnut_back_right_ready.Wait();

			wheelremoval_front_left_ready.Wait();
			wheelremoval_front_right_ready.Wait();
			wheelremoval_back_left_ready.Wait();
			wheelremoval_back_right_ready.Wait();

			wheelrefitting_front_left_ready.Wait();
			wheelrefitting_front_right_ready.Wait();
			wheelrefitting_back_left_ready.Wait();
			wheelrefitting_back_right_ready.Wait();

			pit_exit_light.Wait();
			pit_entry_light.Signal();
			state = "Idle";
		}
		return 0;
	}
};
#endif