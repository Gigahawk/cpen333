#include "Driver.h"
#include "globals.h"


/**
 * @brief Add fuel to the driver's car
 * @param amt amount of fuel to add
 * @return true if the tank is full, else false
*/
bool Driver::add_fuel(int32_t amt)
{
	if (fuel > INT32_MAX - amt) {
		fuel = INT32_MAX;
		return true;
	}
	fuel += amt;
	return false;
}

/**
 * @brief Clean visor of driver
 * @param amt amount to clear
 * @return true if the visor is clear, else false
*/
bool Driver::clean_visor(int32_t amt)
{
	if (visor_clarity > INT32_MAX - amt) {
		visor_clarity = INT32_MAX;
		return true;
	}
	visor_clarity += amt;
	return false;
}

/**
 * @brief Clean intake of driver's car
 * @param amt amount to clear
 * @return true if the intake is clear, else false
*/
bool Driver::clean_intake(int32_t amt)
{
	if (intake_flow > INT32_MAX - amt) {
		intake_flow = INT32_MAX;
		return true;
	}
	intake_flow += amt;
	return false;
}

int Driver::main(void)
{
	rnd.seed((uint32_t)this);
	race_start.Wait();
	while (laps < MAX_LAPS) {
		if (needs_pit)
			needs_pit = !enter_pit();
		while (progress < LAP_LENGTH) {
			consume_resources();
			if (disqualified)
				goto race_end;
			progress += speed;
			Sleep(1);
			next_speed();
		}
		progress = 0;
		laps += 1;
	}
	progress = LAP_LENGTH;
race_end:
	return 0;
}

void Driver::next_speed()
{
	speed = (int32_t)round(speed_dist(rnd));
}

int32_t Driver::visor_buildup()
{
	return (int32_t)round(visor_dist(rnd));
}

int32_t Driver::intake_buildup()
{
	return (int32_t)round(intake_dist(rnd));
}

void Driver::consume_resources()
{
	fuel -= FUEL_CONS_COEFF * speed;
	tire_health -= TIRE_DEG_COEFF * speed;
	visor_clarity -= visor_buildup();
	intake_flow -= intake_buildup();
	if (
		fuel < 0 || tire_health < 0 || visor_clarity < 0
		|| intake_flow < 0) {
		disqualified = true;
	}
	if (
		fuel < LOW_FUEL_THRESH
		|| tire_health < LOW_TIRE_THRESH
		|| visor_clarity < LOW_VISOR_THRESH) {
		needs_pit = true;
	}
}

bool Driver::enter_pit()
{
	if(!pit_entry_light.Read())
		return false;
	speed = 0;
	*curr_driver = this;
	car_in_pit.Signal();
	while(!pit_exit_light.Read());
	*curr_driver = nullptr;
	car_in_pit.Wait();
	return true;
}
