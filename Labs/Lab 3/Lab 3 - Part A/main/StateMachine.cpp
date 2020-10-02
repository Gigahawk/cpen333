#include "StateMachine.h"
#include "mutex_printf.h"
#include <random>


int StateMachine::main()
{
	reset_transition_time();
	print_message("Ready");
	while(cs < states) {
		Sleep(int(CLOCK_RESOLUTION * 1000));
		if (idle)
			continue;
		curr_transition_time -= int(CLOCK_RESOLUTION * 1000);
		if (curr_transition_time <= 0) {
			next_state();
		}
	}
	return 0;
}

void StateMachine::print_message(const char* format, ...)
{
	char buf[MESSAGE_LEN];

	va_list argptr;
	va_start(argptr, format);
	vsprintf_s(buf, format, argptr);
	va_end(argptr);
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
