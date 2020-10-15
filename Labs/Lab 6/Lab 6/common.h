#ifndef __COMMON_H__
#define __COMMON_H__
#include "rt.h"

#define NUM_DRIVERS 3

extern CSemaphore race_start;
extern CSemaphore race_over;

extern CSemaphore pit_entry_light;
extern CSemaphore pit_exit_light;
extern CSemaphore car_in_pit;

#endif

