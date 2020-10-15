#ifndef __CAR_DRIVER_H__
#define __CAR_DRIVER_H__
#include "rt.h"
#include "olcPixelGameEngine/olcPixelGameEngine.h"
#include <random>

#define AVG_SPEED 10
#define SPEED_STDDEV 5
#define MAX_LAPS 20
#define LAP_LENGTH 100000000
#define FUEL_CONS_COEFF 3
#define TIRE_DEG_COEFF 1
#define LOW_FUEL_THRESH 10000
#define LOW_TIRE_THRESH 10000

class Driver : public ActiveClass
{
public:
	Driver(olc::Pixel c) :
		progress(0), laps(0), needs_pit(false), 
		fuel(INT32_MAX), tire_health(INT32_MAX),
		disqualified(false), speed(0), color(c) {}
	uint32_t get_progress() { return progress; }
	double get_progress_percent() { return (double)progress/LAP_LENGTH; }
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
	normal_distribution<double> dist = normal_distribution<double>(AVG_SPEED, SPEED_STDDEV);
	void next_speed();
	uint32_t speed;
	void consume_resources();
};
#endif
