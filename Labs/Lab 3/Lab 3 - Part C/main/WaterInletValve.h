#ifndef __WATER_INLET_VALVE_H__
#define __WATER_INLET_VALVE_H__
#include "../rt.h"
#include "StateMachine.h"

#define WIV_TRANSITION_TIME_AVG 2.0
#define WIV_TRANSITION_JITTER 0.5

#define WIV_NUM_STATES 2


class WaterInletValve: public StateMachine
{
public:
	WaterInletValve(int id, StateMachine* parent) :
		StateMachine(
			WIV_TRANSITION_TIME_AVG,
			WIV_TRANSITION_JITTER,
			WIV_NUM_STATES,
			messages,
			"WIV",
			id,
			parent=parent
		) {}
private:
	const char* messages[WIV_NUM_STATES] = {
		"OPENED",
		"CLOSED"
	};
};

#endif

