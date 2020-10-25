#ifndef __JACKING_H__
#define __JACKING_H__
#include "rt.h"
#include <random>
#include "common.h"
#include "Driver.h"
#include "Actor.h"

class Jacking : public Actor
{
public:
	Jacking(bool front) : front(front)
	{
		if (front) {
			jacking = &jacking_front;
			jacking_done = &jacking_front_done;
			jacking_ready = &jacking_front_ready;
		}
		else {
			jacking = &jacking_back;
			jacking_done = &jacking_back_done;
			jacking_ready = &jacking_back_ready;
		}
	}
private:
	int main() {
		while (!race_over.Read()) {
			service_start.Wait();
			// Car has entered pit, do stuff here
			if ((*curr_driver)->get_tire_health_raw() < LOW_TIRE_THRESH) {
				// Only replace tires if they are actually low
				state = "Preparing";
				Sleep(tech_dist(rnd));
				jacking->Signal();
				state = "Raising";
				Sleep(JACKING_TIME);
				jacking->Wait();
				state = "Waiting for service";

				signal_children();

				jacking->Signal();
				state = "Lowering";
				Sleep(JACKING_TIME);
				jacking->Wait();

				jacking_done->Signal();
				state = "Resetting";
				// Wait for reset;
				Sleep(JACKING_RESET_TIME);
			}
			else {
				// Otherwise do nothing
				jacking_done->Signal();

				// Signal children to make sure they indicate ready
				signal_children();
			}
			jacking_ready->Signal();
			state = "Idle";
		}
		return 0;
	}

	void signal_children() {
		if (front) {
			wheel_f_service_start.Signal(2);
			wheelnut_front_left_done.Wait();
			wheelnut_front_right_done.Wait();
		}
		else {
			wheel_b_service_start.Signal(2);
			wheelnut_back_left_done.Wait();
			wheelnut_back_right_done.Wait();
		}
	}
	bool front;
	CSemaphore* jacking;
	CSemaphore* jacking_done;
	CSemaphore* jacking_ready;
};
#endif
