#ifndef __COVFEFE_MAKER_H__
#define __COVFEFE_MAKER_H__
#include "../rt.h"
#include "StateMachine.h"

#define CM_TRANSITION_TIME_AVG 1.0
#define CM_TRANSITION_JITTER 0.5

#define CM_NUM_STATES 5


class CoffeeMaker: public StateMachine
{
public:
	CoffeeMaker(int id) :
		StateMachine(
			CM_TRANSITION_TIME_AVG,
			CM_TRANSITION_JITTER,
			CM_NUM_STATES,
			messages,
			"CM",
			id
		) {}
private:
	const char* messages[CM_NUM_STATES] = {
		"FILLING WATER TANK",
		"PREPARING FILTER",
		"LOADING BEAN POWDER",
		"DRIPPING BEAN JUICE",
		"DRINKING BEAN JUICE"
	};
};

#endif
