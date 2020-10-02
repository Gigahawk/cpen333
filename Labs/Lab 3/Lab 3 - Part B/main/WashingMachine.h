#ifndef __WASHING_MACHINE_H__
#define __WASHING_MACHINE_H__
#include "../rt.h"
#include "StateMachine.h"

#define WM_TRANSITION_TIME_AVG 1.0
#define WM_TRANSITION_JITTER 0.5

#define WM_NUM_STATES 4


class WashingMachine: public StateMachine
{
public:
	WashingMachine(int id) :
		StateMachine(
			WM_TRANSITION_TIME_AVG,
			WM_TRANSITION_JITTER,
			WM_NUM_STATES,
			messages,
			"WM",
			id
		) {}
private:
	const char* messages[WM_NUM_STATES] = {
		"FILLING",
		"WASHING",
		"DRAINING",
		"SPINNING"
	};
};

#endif

