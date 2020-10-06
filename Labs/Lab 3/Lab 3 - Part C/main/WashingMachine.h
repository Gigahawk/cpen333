#ifndef __WASHING_MACHINE_H__
#define __WASHING_MACHINE_H__
#include "../rt.h"
#include "StateMachine.h"
#include "WaterInletValve.h"
#include "WaterOutletValve.h"
#include "Motor.h"

#define WM_TRANSITION_TIME_AVG 1.0
#define WM_TRANSITION_JITTER 0.5

#define WM_NUM_STATES 4
#define WM_NUM_SUBMACHINES 3


class WashingMachine : public StateMachine
{
public:
	WashingMachine(int id) :
		StateMachine(
			WM_TRANSITION_TIME_AVG,
			WM_TRANSITION_JITTER,
			WM_NUM_STATES,
			messages,
			"WM",
			id,
			nullptr,
			(const StateMachine**)_sms,
			WM_NUM_SUBMACHINES,
			(const StateMachine**)_sms_trans_start,
			(const StateMachine**)_sms_trans_end
		) {
		wiv = new WaterInletValve(0, this);
		wov = new WaterOutletValve(1, this);
		mot = new Motor(2, this);
		_sms[0] = (StateMachine*)wiv;
		_sms[1] = (StateMachine*)wov;
		_sms[2] = (StateMachine*)mot;
		_sms_trans_start[0] = (StateMachine*)wiv;
		_sms_trans_end[0] = (StateMachine*)wiv;

		_sms_trans_start[2] = (StateMachine*)wov;
		_sms_trans_end[2] = (StateMachine*)wov;

		_sms_trans_start[1] = (StateMachine*)mot;
		_sms_trans_end[3] = (StateMachine*)mot;
	}
private:
	WaterInletValve* wiv;
	WaterOutletValve* wov;
	Motor* mot;
	const char* messages[WM_NUM_STATES] = {
		"TANK FILLED",
		"CLOTHES WASHED",
		"TANK DRAINED",
		"CLOTHES SPUN"
	};
	const StateMachine* _sms[WM_NUM_SUBMACHINES];
	const StateMachine* _sms_trans_start[WM_NUM_STATES];
	const StateMachine* _sms_trans_end[WM_NUM_STATES];

};

#endif
