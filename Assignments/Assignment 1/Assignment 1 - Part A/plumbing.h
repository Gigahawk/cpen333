#pragma once
#include "rt.h"

// IO Message Codes
#define SIM_END 100

#define CMD_LEN 2
#define IO_PIPE "IO_PIPE"
struct io_data_t {
	char cmd[CMD_LEN + 1];
};

typedef CTypedPipe<io_data_t> IOPipe;


// Elevator Message Codes
#define FAULT_TRIGGER 420
#define FAULT_RELEASE 9001





