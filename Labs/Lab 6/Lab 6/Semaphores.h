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
};

#endif
