#ifndef __CAR_DRIVER_H__
#define __CAR_DRIVER_H__
#include "rt.h"
#include "olcPixelGameEngine/olcPixelGameEngine.h"
#include <random>
#include <algorithm>
#include "common.h"
#include "Actor.h"

class Driver : public Actor
{
public:
	Driver(olc::Pixel c) :
		progress(0), laps(0), needs_pit(false),
		fuel(INT32_MAX),
		visor_clarity(INT32_MAX), intake_flow(INT32_MAX),
		disqualified(false), speed(0), color(c)
	{
		for (int i = 0; i < 4; i++)
			tire_health[i] = INT32_MAX;
	}
	double get_lap_progress() { return (double)progress/LAP_LENGTH; }
	double get_progress() { return ((double)progress/LAP_LENGTH + laps)/MAX_LAPS; }
	uint32_t get_laps() { return laps; }
	double get_fuel() { return (double)fuel/INT32_MAX; }
	double get_tire_health() { return (double)get_tire_health_raw()/INT32_MAX; }
	int32_t get_tire_health_raw();
	double get_visor_clarity() { return (double)visor_clarity/INT32_MAX; }
	bool get_needs_pit() { return needs_pit; }
	bool add_fuel(int32_t amt);
	bool clean_visor(int32_t amt);
	bool clean_intake(int32_t amt);
	void remove_tire(uint8_t idx);
	void replace_tire(uint8_t idx);
	olc::Pixel color;
private:
	uint32_t progress;
	uint32_t laps;
	int main(void);
	bool needs_pit;
	bool disqualified;
	int32_t fuel;
	int32_t tire_health[4];
	int32_t visor_clarity;
	int32_t intake_flow;
	uniform_int_distribution<> speed_dist = uniform_int_distribution<>(SPEED_AVG - SPEED_JITTER, SPEED_AVG + SPEED_JITTER);
	uniform_int_distribution<> visor_dist = uniform_int_distribution<>(VISOR_BUILDUP_AVG - VISOR_BUILDUP_JITTER, VISOR_BUILDUP_AVG + VISOR_BUILDUP_JITTER);
	uniform_int_distribution<> intake_dist = uniform_int_distribution<>(INTAKE_BUILDUP_AVG - INTAKE_BUILDUP_JITTER, INTAKE_BUILDUP_AVG + INTAKE_BUILDUP_JITTER);
	uniform_int_distribution<> tire_dist = uniform_int_distribution<>(TIRE_DEG_COEFF_AVG - TIRE_DEG_COEFF_JITTER, TIRE_DEG_COEFF_AVG + TIRE_DEG_COEFF_JITTER);
	void next_speed();
	int32_t visor_buildup();
	int32_t intake_buildup();
	uint32_t speed;
	void degrade_tires();
	void consume_resources();
	bool enter_pit();

};
#endif
