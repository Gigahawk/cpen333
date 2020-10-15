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
	CSemaphore refuelling = CSemaphore("rflng", 0, 1);
	CSemaphore refuelling_done = CSemaphore("rflng_done", 0, 1);
	CSemaphore refuelling_ready = CSemaphore("rflng_rdy", 0, 1);
};

#endif
