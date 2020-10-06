#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__
#include <iostream>
#include <windows.h>

#include "../rt.h"

#define MESSAGE_LEN 100
#define CLOCK_RESOLUTION 0.01
#define START_MSG 1000
#define STOP_MSG 1001

class StateMachine : public ActiveClass
{
public:
	void start() const;
	void stop() const;
	void print_message(const char* format, ...) const;
	void reset() const;
	bool is_idle() const { return idle; }
	bool Resumed() const;
protected:
	StateMachine(
		const double tta, const double tj, const uint32_t s,
		const char** m, const char* n, const int id,
		const StateMachine* parent = nullptr,
		const StateMachine** sms = nullptr, uint32_t n_sms = 0,
		const StateMachine** sms_trans_start = nullptr, const StateMachine** sms_trans_end = nullptr) :
		transition_time_avg(tta), transition_jitter(tj),
		states(s), state_messages(m), name(n), id(id),
		parent(parent), submachines(sms), n_submachines(n_sms),
		submachine_trans_start(sms_trans_start), submachine_trans_end(sms_trans_end),
		cs(0), idle(true), curr_transition_time(INT_MAX) {}
	const double transition_time_avg;
	const double transition_jitter;
	const uint32_t states;
	const char** state_messages;
	const StateMachine* parent;
	const StateMachine** submachines;
	const StateMachine** submachine_trans_start;
	const StateMachine** submachine_trans_end;
	const uint32_t n_submachines;
	const char* name;
	const int id;
private:
	int main();
	bool idle;
	mutable uint32_t cs;
	int curr_transition_time;
	void reset_transition_time();
	void next_state();
	CMailbox mb;
	void handle_mb_message(UINT msg);
};

#endif
