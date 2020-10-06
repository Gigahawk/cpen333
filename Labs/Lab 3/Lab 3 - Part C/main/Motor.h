#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "../rt.h"
#include "StateMachine.h"

#define MOT_TRANSITION_TIME_AVG 3.0
#define MOT_TRANSITION_JITTER 0.5

#define MOT_NUM_STATES 2


class Motor: public StateMachine
{
public:
	Motor(int id, StateMachine* parent) :
		StateMachine(
			MOT_TRANSITION_TIME_AVG,
			MOT_TRANSITION_JITTER,
			MOT_NUM_STATES,
			messages,
			"MOT",
			id,
			parent=parent
		) {}
private:
	const char* messages[MOT_NUM_STATES] = {
		"SPUN UP",
		"SPUN DOWN"
	};
};

#endif

