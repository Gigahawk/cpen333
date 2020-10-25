#ifndef __CAR_DRIVER_H__
#define __CAR_DRIVER_H__
#include "rt.h"
#include "olcPixelGameEngine/olcPixelGameEngine.h"
#include <random>
#include "common.h"
#include "Actor.h"

#define MAX_LAPS 100
#define LAP_LENGTH 10000

#define SPEED_AVG 100
#define SPEED_JITTER 100

#define FUEL_CONS_COEFF 20000
#define LOW_FUEL_THRESH INT32_MAX/4

#define TIRE_DEG_COEFF 10000
#define LOW_TIRE_THRESH 10000


#define VISOR_BUILDUP_AVG 100
#define VISOR_BUILDUP_JITTER 100
#define LOW_VISOR_THRESH 10000

// Let's say Driver is unaware of intake buildup
#define INTAKE_BUILDUP_AVG 100
#define INTAKE_BUILDUP_JITTER 100

class Driver : public Actor
{
public:
	Driver(olc::Pixel c) :
		progress(0), laps(0), needs_pit(false),
		fuel(INT32_MAX), tire_health(INT32_MAX),
		visor_clarity(INT32_MAX), intake_flow(INT32_MAX),
		disqualified(false), speed(0), color(c) {}
	double get_lap_progress() { return (double)progress/LAP_LENGTH; }
	double get_progress() { return ((double)progress/LAP_LENGTH + laps)/MAX_LAPS; }
	uint32_t get_laps() { return laps; }
	double get_fuel() { return (double)fuel/INT32_MAX; }
	double get_tire_health() { return (double)tire_health/INT32_MAX; }
	double get_visor_clarity() { return (double)visor_clarity/INT32_MAX; }
	bool get_needs_pit() { return needs_pit; }
	bool add_fuel(int32_t amt);
	bool clean_visor(int32_t amt);
	bool clean_intake(int32_t amt);
	olc::Pixel color;
private:
	uint32_t progress;
	uint32_t laps;
	int main(void);
	bool needs_pit;
	bool disqualified;
	int32_t fuel;
	int32_t tire_health;
	int32_t visor_clarity;
	int32_t intake_flow;
	default_random_engine rnd;
	uniform_int_distribution<> speed_dist = uniform_int_distribution<>(SPEED_AVG - SPEED_JITTER, SPEED_AVG + SPEED_JITTER);
	uniform_int_distribution<> visor_dist = uniform_int_distribution<>(VISOR_BUILDUP_AVG - VISOR_BUILDUP_JITTER, VISOR_BUILDUP_AVG + VISOR_BUILDUP_JITTER);
	uniform_int_distribution<> intake_dist = uniform_int_distribution<>(INTAKE_BUILDUP_AVG - INTAKE_BUILDUP_JITTER, INTAKE_BUILDUP_AVG + INTAKE_BUILDUP_JITTER);
	void next_speed();
	int32_t visor_buildup();
	int32_t intake_buildup();
	uint32_t speed;
	void consume_resources();
	bool enter_pit();

};
#endif
