#ifndef __CAR_DRIVER_H__
#define __CAR_DRIVER_H__
#include "rt.h"
#include "olcPixelGameEngine/olcPixelGameEngine.h"
#include <random>
#include "common.h"
#include "Semaphores.h"

#define AVG_SPEED 100
#define SPEED_JITTER 100
#define MAX_LAPS 100
#define LAP_LENGTH 10000
#define FUEL_CONS_COEFF 20000
#define TIRE_DEG_COEFF 10000
#define LOW_FUEL_THRESH INT32_MAX/4
#define LOW_TIRE_THRESH 10000

class Driver : public ActiveClass, public Semaphores
{
public:
	Driver(olc::Pixel c) :
		progress(0), laps(0), needs_pit(false), 
		fuel(INT32_MAX), tire_health(INT32_MAX),
		disqualified(false), speed(0), color(c) {}
	double get_lap_progress() { return (double)progress/LAP_LENGTH; }
	double get_progress() { return ((double)progress/LAP_LENGTH + laps)/MAX_LAPS; }
	uint32_t get_laps() { return laps; }
	double get_fuel() { return (double)fuel/INT32_MAX; }
	double get_tire_health() { return (double)tire_health/INT32_MAX; }
	bool get_needs_pit() { return needs_pit; }
	bool add_fuel(int32_t amt);
	olc::Pixel color;
private:
	uint32_t progress;
	uint32_t laps;
	int main(void);
	bool needs_pit;
	bool disqualified;
	int32_t fuel;
	int32_t tire_health;
	default_random_engine rnd;
	uniform_int_distribution<> dist = uniform_int_distribution<>(AVG_SPEED - SPEED_JITTER, AVG_SPEED + SPEED_JITTER);
	void next_speed();
	uint32_t speed;
	void consume_resources();
	bool enter_pit();

};
#endif
