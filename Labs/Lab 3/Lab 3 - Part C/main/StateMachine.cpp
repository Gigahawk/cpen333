#include "StateMachine.h"
#include "mutex_printf.h"
#include <random>


void StateMachine::start() const
{
	if (WaitForThread(0) == WAIT_TIMEOUT)
		this->Post(START_MSG);
}

void StateMachine::stop() const
{
	if (WaitForThread(0) == WAIT_TIMEOUT)
		this->Post(STOP_MSG);
}

void StateMachine::reset() const
{
	cs = 0;
}

bool StateMachine::Resumed() const
{
	return WaitForThread(100) == WAIT_TIMEOUT;
}

int StateMachine::main()
{
	mb.TestForMessage();
	for (uint32_t i = 0; i < n_submachines; i++) {
		submachines[i]->Resume();
		submachines[i]->WaitForThread(10);
	}
	reset_transition_time();
	print_message("Ready");
	while(cs < states) {
		if (mb.TestForMessage() == TRUE) {
			handle_mb_message(mb.GetMessageA());
		}
		if (idle)
			continue;
		Sleep(int(CLOCK_RESOLUTION * 1000));
		if (submachine_trans_start && submachine_trans_start[cs])
			if (submachine_trans_start[cs]->is_idle() &&
				submachine_trans_start[cs]->WaitForThread(0) == WAIT_TIMEOUT)
				submachine_trans_start[cs]->start();

		if (submachine_trans_end && submachine_trans_end[cs]) {
			curr_transition_time = INT_MAX;
			if (submachine_trans_end[cs]->WaitForThread(0) == WAIT_OBJECT_0)
				curr_transition_time = 0;
		} else {
			curr_transition_time -= int(CLOCK_RESOLUTION * 1000);
		}
		if (curr_transition_time <= 0) {
			next_state();
		}
	}
	return 0;
}

void StateMachine::print_message(const char* format, ...) const
{
	char buf[MESSAGE_LEN];

	va_list argptr;
	va_start(argptr, format);
	vsprintf_s(buf, format, argptr);
	va_end(argptr);
	if (parent != nullptr) 
		parent->print_message("#%s%d: %s", name, id, buf);
	else
		mutex_printf("#%s%d: %s\n", name, id, buf);
}

void StateMachine::reset_transition_time()
{
	const double lower = -transition_jitter / 2;
	const double upper = transition_jitter / 2;
	std::uniform_real_distribution<double> un(lower, upper);
	std::default_random_engine re;
	curr_transition_time = int((transition_time_avg + un(re)) * 1000.0);
}

void StateMachine::next_state()
{
	reset_transition_time();
	print_message("[%d] %s", cs, state_messages[cs]);
	cs++;
}

void StateMachine::handle_mb_message(UINT msg)
{
	switch (msg) {
		case START_MSG:
			if (idle)
				print_message("Starting");
			idle = false;
			break;
		case STOP_MSG:
			if (!idle)
				print_message("Stopping");
			idle = true;
			break;
		default:
			print_message("Warning: Unknown message recieved: %d", msg);
	}
}

