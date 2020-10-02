#include "CoffeeMaker.h"
#include "WashingMachine.h"

#define NUM_MACHINES 5

int main(void) {
	CoffeeMaker *cms[NUM_MACHINES];
	WashingMachine *wms[NUM_MACHINES];
	for (int i = 0; i < NUM_MACHINES; i++) {
		cms[i] = new CoffeeMaker(i);
		wms[i] = new WashingMachine(i);
		cms[i]->Resume();
		wms[i]->Resume();
	}

	for (int i = 0; i < NUM_MACHINES; i++) {
		cms[i]->WaitForThread();
		wms[i]->WaitForThread();
	}
	return 0;
}