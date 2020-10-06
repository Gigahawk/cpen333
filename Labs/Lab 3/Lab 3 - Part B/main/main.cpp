#include <iostream>
#include <random>
#include "CoffeeMaker.h"
#include "WashingMachine.h"
#include "StateMachine.h"
#include "../rt.h"
#include "mutex_printf.h"

#define NUM_MACHINES 5
#define CONTROL_WAIT_MAX 5000

using namespace std;

class MachineController : public ActiveClass {
public:
	MachineController(StateMachine** m, uint32_t n) : machines(m), num_machines(n) {}
private:
	StateMachine** machines;
	const uint32_t num_machines;
	int main() {
		uniform_int_distribution<> startstop(0, 1);
		uniform_int_distribution<> target(0, num_machines - 1);
		uniform_int_distribution<> wait(0, CONTROL_WAIT_MAX);
		default_random_engine re;
		// No two MachineControllers should be controlling the same 
		// machines
		re.seed((unsigned long)machines);

		while (!threads_complete()) {
			// Preload the function we want to run to reduce cycles between checking
			// if the thread is still running and posting message
			auto ss = startstop(re) ? &StateMachine::start : &StateMachine::stop;
			auto m = machines[target(re)];

			if (m->WaitForThread(0) == WAIT_OBJECT_0)
				continue;
			(*m.*(ss))();

			Sleep(wait(re));
		}
		return 0;
	}

	bool threads_complete() {
		for (uint32_t i = 0; i < num_machines; i++) {
			if (machines[i]->WaitForThread(0) == WAIT_TIMEOUT)
				return false;
		}
		return true;
	}
};

int main(void) {
	CoffeeMaker *cms[NUM_MACHINES];
	WashingMachine *wms[NUM_MACHINES];
	MachineController cmc((StateMachine**)cms, NUM_MACHINES);
	MachineController wmc((StateMachine**)wms, NUM_MACHINES);
	for (int i = 0; i < NUM_MACHINES; i++) {
		cms[i] = new CoffeeMaker(i);
		wms[i] = new WashingMachine(i);
		cms[i]->Resume();
		wms[i]->Resume();
		// Wait a little bit to ensure threads have time to start.
		// WaitForThread() can't be used to differentiate between
		// resumed and suspended threads.
		cms[i]->WaitForThread(10);
		wms[i]->WaitForThread(10);
	}
	cmc.Resume();
	wmc.Resume();
	cmc.WaitForThread();
	wmc.WaitForThread();

	for (int i = 0; i < NUM_MACHINES; i++) {
		delete cms[i];
		delete wms[i];
	}

	return 0;
}