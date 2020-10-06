#ifndef __WATER_OUTLET_VALVE_H__
#define __WATER_OUTLET_VALVE_H__
#include "../rt.h"
#include "StateMachine.h"

#define WOV_TRANSITION_TIME_AVG 2.0
#define WOV_TRANSITION_JITTER 0.5

#define WOV_NUM_STATES 2


class WaterOutletValve: public StateMachine
{
public:
	WaterOutletValve(int id, StateMachine* parent) :
		StateMachine(
			WOV_TRANSITION_TIME_AVG,
			WOV_TRANSITION_JITTER,
			WOV_NUM_STATES,
			messages,
			"WOV",
			id,
			parent=parent
		) {}
private:
	const char* messages[WOV_NUM_STATES] = {
		"OPENED",
		"CLOSED"
	};
};

#endif

