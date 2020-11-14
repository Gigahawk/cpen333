#pragma once
#include "rt.h"

#define NUM_ELEVS 2

// IO Message Codes
#define SIM_END 100
#define CMD_ERR 999

#define CMD_LEN 2
#define IO_PIPE "IO_PIPE"
struct io_data_t {
	char cmd[CMD_LEN + 1];
};

typedef CTypedPipe<io_data_t> IOPipe;


// Elevator Message Codes
#define FAULT_TRIGGER 420
#define FAULT_RELEASE 9001


// Elevator Status Codes
#define STATUS_IDLE 0
#define STATUS_UP 1
#define STATUS_DOWN 2
#define STATUS_FAULT 3

// Distance between elevator floors
#define FLOOR_DISTANCE 100





