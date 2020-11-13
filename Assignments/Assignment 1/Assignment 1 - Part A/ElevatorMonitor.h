#pragma once

#include "rt.h"
#include "plumbing.h"

struct e_status_t {
	uint8_t status;
	uint32_t loc;
	uint8_t target_floor;
	uint8_t last_floor;
};

class ElevatorMonitor : public CDataPool, public ActiveClass
{
public:
	ElevatorMonitor(uint8_t id) :
		CDataPool(string("__DP__ELEVATOR_") + to_string(id), sizeof(e_status_t)),
		_m(new CMutex(string("__MUTEX__ELEVATOR_") + to_string(id))),
		_m_int(new CMutex(string("__MUTEX__INTERNAL_ELEVATOR_") + to_string(id)))
	{
		_status = (e_status_t*)LinkDataPool();
		_status->status = 0;
		_status->loc = 0;
		_status->target_floor = 0;
		_status->last_floor = 0;
	}
	~ElevatorMonitor() { delete _m; }

	void Update_Status(e_status_t s) {
		_m->Wait();
		*_status = s;
		_m->Signal();
	}

	e_status_t Get_Elevator_Status() {
		e_status_t tmp;
		_m->Wait();
		tmp = *_status;
		_m->Signal();
		return tmp;
	}

	// Access local copy of elevator status
	e_status_t get_status() {
		e_status_t tmp;
		// Mutex is required to prevent access while ElevatorMonitor::main()
		// is writing data.
		_m_int->Wait();
		tmp = status;
		_m_int->Signal();
		return tmp;
	}

	uint32_t distance_to_floor(uint8_t f) {
		int64_t f_loc = FLOOR_DISTANCE * f;
		e_status_t curr_status = get_status();
		int64_t target_loc = FLOOR_DISTANCE * curr_status.target_floor;
		int64_t curr_loc = curr_status.loc;

		if ((curr_status.status == STATUS_IDLE) ||
			(f_loc > curr_loc && curr_status.status == STATUS_UP) ||
			(f_loc < curr_loc && curr_status.status == STATUS_DOWN))
			return abs(f_loc - (int64_t)curr_status.loc);

		return abs(curr_loc - target_loc) + abs(target_loc - f_loc);
	}

private:
	CMutex* _m; // Mutex to handle datapool accesses
	CMutex* _m_int; // Mutex to handle local accesses
	e_status_t *_status;
	e_status_t status;

	// Dispatcher thread to "respond to changes in elevator status
	// and copy that status to local variables within the process"
	int main() {
		while (true) {
			_m_int->Wait();
			status = Get_Elevator_Status();
			_m_int->Signal();
			Sleep(10);
		}
		return 0;
	}
};

