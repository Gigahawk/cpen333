#include <iostream>
#include <windows.h>
#include "../rt.h"
#include "../common.h"
#include "../plumbing.h"
#include "../ElevatorMonitor.h"

using namespace std;

#define CTOI(c) c - '0'

void inside_elev(uint8_t id, uint8_t floor);
void outside_elev(bool up, uint8_t floor);
void gen_fault(bool f, uint8_t id);
void end_sim();

ElevatorMonitor* ems[NUM_ELEVS];
CProcess* elevator_procs[NUM_ELEVS];
CProcess* io_proc;

int main(int argc, char* argv[]) {
	io_data_t io_data;
	IOPipe io_pipe(IO_PIPE);
	string path = getParentDir(string(argv[0]));
	string io_path = path + "io.exe";
	string elevator_path = path + "elevator.exe";


	for (int i = 0; i < NUM_ELEVS; i++) {
		ems[i] = new ElevatorMonitor(i + 1);
		ems[i]->Resume();
		elevator_procs[i] = new CProcess(
			(elevator_path + " " + to_string(i + 1)).c_str(),
			NORMAL_PRIORITY_CLASS,
			OWN_WINDOW,
			ACTIVE);
	}

	io_proc = new CProcess(io_path.c_str(),
		NORMAL_PRIORITY_CLASS,
		//OWN_WINDOW,
		PARENT_WINDOW,
		ACTIVE);
	while (true) {
		io_pipe.Read(&io_data);
		cout << "Command recieved: " << io_data.cmd << endl;
		switch (io_data.cmd[0]) {
		case '1':
		case '2':
			inside_elev(CTOI(io_data.cmd[0]), CTOI(io_data.cmd[1]));
			break;
		case 'u':
			outside_elev(true, CTOI(io_data.cmd[1]));
			break;
		case 'd':
			outside_elev(false, CTOI(io_data.cmd[1]));
			break;
		case '-':
			gen_fault(true, CTOI(io_data.cmd[1]));
			break;
		case '+':
			gen_fault(false, CTOI(io_data.cmd[1]));
			break;
		case 'e':
			end_sim();
			break;
		default:
			io_proc->Post(CMD_ERR);
			break;
		}
	}
	return 0;
}

void inside_elev(uint8_t id, uint8_t floor)
{
	elevator_procs[id - 1]->Post(floor);
}

void outside_elev(bool up, uint8_t floor)
{
	int i;
	uint8_t target = 0;
	uint32_t distance = UINT32_MAX;
	uint32_t d;
	uint8_t s;
	bool df;
	for (i = 0; i < NUM_ELEVS; i++) {
		d = ems[i]->distance_to_floor(floor);
		s = ems[i]->dir_to_floor(floor);
		df = (s == STATUS_UP || s == STATUS_IDLE);
		if (d < distance && up == df) {
			distance = d;
			target = i;
		}
	}
	elevator_procs[target]->Post(floor);
}

void gen_fault(bool f, uint8_t id)
{
	if(f)
		elevator_procs[id - 1]->Post(FAULT_TRIGGER);
	else
		elevator_procs[id - 1]->Post(FAULT_RELEASE);
}

void end_sim() {
	io_proc->Post(SIM_END);
	for (int i = 0; i < NUM_ELEVS; i++) {
		elevator_procs[i]->Post(SIM_END);
	}
}
