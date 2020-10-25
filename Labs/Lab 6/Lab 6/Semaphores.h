#ifndef __SEMAPHORES_H__
#define __SEMAPHORES_H__
#include "rt.h"
#include "common.h"

class Semaphores {
public:
	CSemaphore race_start = CSemaphore("race_start", 0, NUM_DRIVERS);
	CSemaphore race_over = CSemaphore("race_over", 0, 1);

	CSemaphore pit_entry_light = CSemaphore("pel", 1, 1);
	CSemaphore pit_exit_light = CSemaphore("pel2", 0, 1);

	CSemaphore car_in_pit = CSemaphore("cip", 0, 1);

	CSemaphore service_start = CSemaphore("service_start", 0, NUM_TECHNICIANS);

	CSemaphore refuelling = CSemaphore("rflng", 0, 1);
	CSemaphore refuelling_done = CSemaphore("rflng_done", 0, 1);
	CSemaphore refuelling_ready = CSemaphore("rflng_rdy", 0, 1);

	CSemaphore visor = CSemaphore("visor", 0, 1);
	CSemaphore visor_done = CSemaphore("visor_done", 0, 1);
	CSemaphore visor_ready = CSemaphore("visor_rdy", 0, 1);

	CSemaphore debris = CSemaphore("debris", 0, 1);
	CSemaphore debris_done = CSemaphore("debris_done", 0, 1);
	CSemaphore debris_ready = CSemaphore("debris_rdy", 0, 1);

	CSemaphore jacking_front = CSemaphore("jacking_front", 0, 1);
	CSemaphore jacking_front_done = CSemaphore("jacking_front_done", 0, 1);
	CSemaphore jacking_front_ready = CSemaphore("jacking_front_rdy", 0, 1);
	CSemaphore jacking_back = CSemaphore("jacking_back", 0, 1);
	CSemaphore jacking_back_done = CSemaphore("jacking_back_done", 0, 1);
	CSemaphore jacking_back_ready = CSemaphore("jacking_back_rdy", 0, 1);

	CSemaphore wheel_f_service_start = CSemaphore("wheel_f_service_start", 0, 2);
	CSemaphore wheel_b_service_start = CSemaphore("wheel_b_service_start", 0, 2);

	CSemaphore wheelnut_front_left = CSemaphore("wheelnut_front_left", 0, 1);
	CSemaphore wheelnut_front_left_done = CSemaphore("wheelnut_front_left_done", 0, 1);
	CSemaphore wheelnut_front_left_ready = CSemaphore("wheelnut_front_left_rdy", 0, 1);
	CSemaphore wheelnut_back_left = CSemaphore("wheelnut_back_left", 0, 1);
	CSemaphore wheelnut_back_left_done = CSemaphore("wheelnut_back_left_done", 0, 1);
	CSemaphore wheelnut_back_left_ready = CSemaphore("wheelnut_back_left_rdy", 0, 1);
	CSemaphore wheelnut_front_right = CSemaphore("wheelnut_front_right", 0, 1);
	CSemaphore wheelnut_front_right_done = CSemaphore("wheelnut_front_right_done", 0, 1);
	CSemaphore wheelnut_front_right_ready = CSemaphore("wheelnut_front_right_rdy", 0, 1);
	CSemaphore wheelnut_back_right = CSemaphore("wheelnut_back_right", 0, 1);
	CSemaphore wheelnut_back_right_done = CSemaphore("wheelnut_back_right_done", 0, 1);
	CSemaphore wheelnut_back_right_ready = CSemaphore("wheelnut_back_right_rdy", 0, 1);

	CSemaphore wheelremoval_front_left = CSemaphore("wheelremoval_front_left", 0, 1);
	CSemaphore wheelremoval_front_left_done = CSemaphore("wheelremoval_front_left_done", 0, 1);
	CSemaphore wheelremoval_front_left_ready = CSemaphore("wheelremoval_front_left_rdy", 0, 1);
	CSemaphore wheelremoval_back_left = CSemaphore("wheelremoval_back_left", 0, 1);
	CSemaphore wheelremoval_back_left_done = CSemaphore("wheelremoval_back_left_done", 0, 1);
	CSemaphore wheelremoval_back_left_ready = CSemaphore("wheelremoval_back_left_rdy", 0, 1);
	CSemaphore wheelremoval_front_right = CSemaphore("wheelremoval_front_right", 0, 1);
	CSemaphore wheelremoval_front_right_done = CSemaphore("wheelremoval_front_right_done", 0, 1);
	CSemaphore wheelremoval_front_right_ready = CSemaphore("wheelremoval_front_right_rdy", 0, 1);
	CSemaphore wheelremoval_back_right = CSemaphore("wheelremoval_back_right", 0, 1);
	CSemaphore wheelremoval_back_right_done = CSemaphore("wheelremoval_back_right_done", 0, 1);
	CSemaphore wheelremoval_back_right_ready = CSemaphore("wheelremoval_back_right_rdy", 0, 1);

	CSemaphore wheelrefitting_front_left = CSemaphore("wheelrefitting_front_left", 0, 1);
	CSemaphore wheelrefitting_front_left_done = CSemaphore("wheelrefitting_front_left_done", 0, 1);
	CSemaphore wheelrefitting_front_left_ready = CSemaphore("wheelrefitting_front_left_rdy", 0, 1);
	CSemaphore wheelrefitting_back_left = CSemaphore("wheelrefitting_back_left", 0, 1);
	CSemaphore wheelrefitting_back_left_done = CSemaphore("wheelrefitting_back_left_done", 0, 1);
	CSemaphore wheelrefitting_back_left_ready = CSemaphore("wheelrefitting_back_left_rdy", 0, 1);
	CSemaphore wheelrefitting_front_right = CSemaphore("wheelrefitting_front_right", 0, 1);
	CSemaphore wheelrefitting_front_right_done = CSemaphore("wheelrefitting_front_right_done", 0, 1);
	CSemaphore wheelrefitting_front_right_ready = CSemaphore("wheelrefitting_front_right_rdy", 0, 1);
	CSemaphore wheelrefitting_back_right = CSemaphore("wheelrefitting_back_right", 0, 1);
	CSemaphore wheelrefitting_back_right_done = CSemaphore("wheelrefitting_back_right_done", 0, 1);
	CSemaphore wheelrefitting_back_right_ready = CSemaphore("wheelrefitting_back_right_rdy", 0, 1);
};

#endif
