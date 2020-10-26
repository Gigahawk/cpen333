#ifndef __COMMON_H__
#define __COMMON_H__
#include <iostream>
#include <vector>
#include <stdarg.h>

// Number of racers
#define NUM_DRIVERS 10
// Number of technicians triggered by supervisor
#define NUM_TECHNICIANS 5

// Racetrack stats
#define MAX_LAPS 200
#define LAP_LENGTH 1000

// Driver performance values
#define SPEED_AVG 10
#define SPEED_JITTER 10

#define FUEL_CONS_COEFF 25000
#define LOW_FUEL_THRESH INT32_MAX/2

#define TIRE_DEG_COEFF_AVG 15000
#define TIRE_DEG_COEFF_JITTER 6000
#define LOW_TIRE_THRESH INT32_MAX/2

#define VISOR_BUILDUP_AVG 100
#define VISOR_BUILDUP_JITTER 100
#define LOW_VISOR_THRESH 10000

// Driver is unaware of intake buildup
#define INTAKE_BUILDUP_AVG 100
#define INTAKE_BUILDUP_JITTER 100


// Technician performance values
#define TECHNICIAN_STARTUP_JITTER 500

#define DEBRIS_RATE 6000
#define DEBRIS_RESET_TIME 500
#define DEBRIS_INTERVAL 10

#define JACKING_TIME 2000
#define JACKING_RESET_TIME 1500

#define REFUELLING_RATE 10000000
#define REFUELLING_RESET_TIME 1000
#define REFUELLING_INTERVAL 1

#define VISOR_RATE 6000
#define VISOR_RESET_TIME 500
#define VISOR_INTERVAL 10

#define WHEELNUT_TIME 1000
#define WHEELNUT_RESET_TIME 1000

#define WHEELREFITTING_TIME 1000
#define WHEELREFITTING_RESET_TIME 1000

#define WHEELREMOVAL_TIME 1000
#define WHEELREMOVAL_RESET_TIME 1000

using namespace std;

string format(const char* fmt, ...);
#endif

