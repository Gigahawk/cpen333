#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include "../rt.h"
#include "../common.h"
#include "../plumbing.h"

#define STATUS_IDLE 0
#define STATUS_UP 1
#define STATUS_DOWN 2

#define FLOOR_DISTANCE 10000

using namespace std;

void log_msg(const char* format, ...);
bool floor_cmp(uint8_t x, uint8_t y);
uint8_t last_floor = 0;
uint8_t num;
uint8_t status = STATUS_IDLE;

int main(int argc, char* argv[]) {
	CMailbox mb;
	UINT mb_msg;
	uint32_t loc = 0;
	bool fault = false;

	// Target floors need to be queueable to support
	// "send[ing] the elevator ... [to an intermediate floor] ...
	// when passing that floor on its way to somewhere else" (pg. 2 of lab manual)
	vector<uint8_t> floor_stack;

	if (argc < 2)
		num = 1;
	else
		num = atoi(argv[1]);
	log_msg("Starting");

	while (true) {
		if (mb.TestForMessage()) {
			mb_msg = mb.GetMessageA();

			if (mb_msg < 10) { // Handle floor request
				log_msg("New floor target: %d", mb_msg);
				floor_stack.push_back(mb_msg);
			}
			else if (mb_msg == FAULT_TRIGGER) {
				log_msg("Fault triggered");
				fault = true;
			}
			else if (mb_msg == FAULT_RELEASE) {
				log_msg("Fault released");
				fault = false;
			}
		}
		// Elevator should discard all commands when fault is active
		if (fault) {
			while (!floor_stack.empty()) {
				floor_stack.pop_back();
			}
			continue;
		}

		if (floor_stack.empty()) {
			status = STATUS_IDLE;
			continue;
		}

		if (floor_stack.back() * 100 == loc) {
			last_floor = floor_stack.back();
			log_msg("Floor reached: %d", last_floor);
			floor_stack.pop_back();
		}

		if (floor_stack.size() == 1) {
			if (floor_stack.back() > last_floor) {
				status = STATUS_UP;
			}
			else if (floor_stack.back() < last_floor) {
				status = STATUS_DOWN;
			}
		}

		// Move requests for floors that have already been passed
		sort(floor_stack.begin(), floor_stack.end(), floor_cmp);

		if (status == STATUS_UP)
			loc++;
		else if (status == STATUS_DOWN)
			loc--;
		Sleep(10);
	}

	return 0;
}

void log_msg(const char* format, ...)
{
	printf("[Elevator: %d] ", num);
	va_list argptr;
	va_start(argptr, format);
	vprintf(format, argptr);
	va_end(argptr);
	printf("\n");
}

/**
 * @brief Compare floors based on the current floor
 *		Moves the next closest floors to the end of the vector.
 *		Floors past the last visited floor are moved to the front of the vector.
 * @param x first floor to compare
 * @param y second floor to compare
 * @return output for sort()
*/
bool floor_cmp(uint8_t x, uint8_t y)
{
	if (status == STATUS_IDLE)
		return false;

	bool b = x < y;

	if (x > last_floor && y > last_floor)
		return !b;
	else if (x < last_floor && y < last_floor)
		return b;
	else if (status == STATUS_UP)
		return b;
	else
		return !b;
}

