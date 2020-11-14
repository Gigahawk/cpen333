#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include "../rt.h"
#include "../common.h"
#include "../plumbing.h"
#include "../ElevatorMonitor.h"

#define STOP_COUNT 100
#define LOOP_DELAY 10

using namespace std;

void log_msg(const char* format, ...);
bool floor_cmp(uint8_t x, uint8_t y);
void remove_dup();
uint8_t target_floor();
uint8_t last_floor = 0;
uint8_t num;
uint8_t status = STATUS_IDLE;
bool waiting = false;
uint32_t loc = 0;

// Target floors need to be queueable to support
// "send[ing] the elevator ... [to an intermediate floor] ...
// when passing that floor on its way to somewhere else" (pg. 2 of lab manual)
vector<uint8_t> floor_stack;

int main(int argc, char* argv[]) {
	CMailbox mb;
	UINT mb_msg;
	bool fault = false;
	e_status_t e_status;
	uint16_t stop_count = 0;
	bool sim_end = false;

	if (argc < 2)
		num = 1;
	else
		num = atoi(argv[1]);
	log_msg("Starting");

	ElevatorMonitor em(num);

	while (true) {
		e_status = { status, loc, target_floor(), last_floor, waiting, {0} };
		for (int i = 0; i < 10; i++) {
			if (i < floor_stack.size())
				e_status.floor_stack[i] = floor_stack[i];
			else
				e_status.floor_stack[i] = -1;
		}
		em.Update_Status(e_status);

		if (mb.TestForMessage() && !sim_end) {
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
				status = STATUS_IDLE;
			}
			else if (mb_msg == SIM_END) {
				log_msg("Ending simulation");
				sim_end = true;
				while (!floor_stack.empty()) {
					floor_stack.pop_back();
				}
				floor_stack.push_back(0);
			}
		}
		// Elevator should discard all commands when fault is active
		if (fault) {
			while (!floor_stack.empty()) {
				floor_stack.pop_back();
			}
			status = STATUS_FAULT;
			continue;
		}

		if (stop_count) {
			waiting = true;
			stop_count--;
			if (!stop_count)
				status = STATUS_IDLE;
			Sleep(LOOP_DELAY);
			continue;
		}
		else {
			waiting = false;
		}

		if (floor_stack.empty()) {
			status = STATUS_IDLE;
			continue;
		}

		if (floor_stack.back() * FLOOR_DISTANCE == loc) {
			last_floor = floor_stack.back();
			log_msg("Floor reached: %d", last_floor);
			floor_stack.pop_back();
			stop_count = STOP_COUNT;
			continue;
		}

		if (status == STATUS_IDLE && !(loc % FLOOR_DISTANCE)) {
			log_msg("Updating status: floor=%f", (double)loc / FLOOR_DISTANCE);
			if (floor_stack.back() > (double)loc / FLOOR_DISTANCE) {
				log_msg("Going up");
				status = STATUS_UP;
			}
			else if (floor_stack.back() < (double)loc / FLOOR_DISTANCE) {
				log_msg("Going down");
				status = STATUS_DOWN;
			}
		}

		remove_dup();

		// Move requests for floors that have already been passed
		sort(floor_stack.begin(), floor_stack.end(), floor_cmp);

		if (status == STATUS_UP)
			loc++;
		else if (status == STATUS_DOWN)
			loc--;

		//if (status != STATUS_IDLE)
		//	log_msg("Location: %d", loc);

		Sleep(LOOP_DELAY);
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
	if (status != STATUS_UP && status != STATUS_DOWN)
		return false;

	bool b = x < y;
	double lf = (double)loc / FLOOR_DISTANCE;

	if (x > lf && y > lf)
		return !b;
	else if (x < lf && y < lf)
		return b;
	else if (status == STATUS_UP)
		return b;
	else if (status == STATUS_DOWN)
		return !b;
	return false;
}

// Remove duplicate requests
void remove_dup()
{
	auto end = floor_stack.end();
	for (auto it = floor_stack.begin(); it != end; it++) {
		end = remove(it + 1, end, *it);
	}
	floor_stack.erase(end, floor_stack.end());
}

uint8_t target_floor()
{
	if(floor_stack.empty())
		return last_floor;
	if (status == STATUS_UP)
		return *max_element(floor_stack.begin(), floor_stack.end());
	else if (status == STATUS_DOWN)
		return *min_element(floor_stack.begin(), floor_stack.end());
	return last_floor;
}


