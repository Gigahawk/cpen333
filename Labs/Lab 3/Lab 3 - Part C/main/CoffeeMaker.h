#ifndef __COVFEFE_MAKER_H__
#define __COVFEFE_MAKER_H__
#include "../rt.h"
#include "StateMachine.h"
#include "WaterInletValve.h"
#include "Motor.h"

#define CM_TRANSITION_TIME_AVG 1.0
#define CM_TRANSITION_JITTER 0.5

#define CM_NUM_STATES 5
#define CM_NUM_SUBMACHINES 2


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
			id,
			nullptr,
			(const StateMachine**)_sms,
			CM_NUM_SUBMACHINES,
			(const StateMachine**)_sms_trans_start,
			(const StateMachine**)_sms_trans_end
		) {
		wiv = new WaterInletValve(0, this);
		mot = new Motor(1, this);
		_sms[0] = (StateMachine*)wiv;
		_sms[1] = (StateMachine*)mot;

		_sms_trans_start[0] = (StateMachine*)wiv;
		_sms_trans_end[0] = (StateMachine*)wiv;

		_sms_trans_start[2] = (StateMachine*)mot;
		_sms_trans_end[2] = (StateMachine*)mot;
	}
private:
	WaterInletValve* wiv;
	Motor* mot;
	const char* messages[CM_NUM_STATES] = {
		"FILLED WATER TANK",
		"PREPARED FILTER",
		"GROUND BEAN POWDER",
		"DRIPPED BEAN JUICE",
		"DRANK BEAN JUICE"
	};
	const StateMachine* _sms[CM_NUM_SUBMACHINES];
	const StateMachine* _sms_trans_start[CM_NUM_STATES];
	const StateMachine* _sms_trans_end[CM_NUM_STATES];
};

#endif
