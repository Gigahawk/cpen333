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
protected:
	StateMachine(
		const double tta, const double tj, const uint32_t s, 
		const char** m, const char* n, const int id): 
		transition_time_avg(tta), transition_jitter(tj),
		states(s), state_messages(m), name(n), id(id),
		cs(0), idle(true) {}
	const double transition_time_avg;
	const double transition_jitter;
	const uint32_t states;
	const char** state_messages;
	const char* name;
	const int id;
private:
	int main();
	void print_message(const char* format, ...);
	bool idle;
	uint32_t cs;
	int curr_transition_time;
	void reset_transition_time();
	void next_state();
};

#endif
