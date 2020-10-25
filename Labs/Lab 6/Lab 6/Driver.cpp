#include "Driver.h"
#include "globals.h"


/**
 * @brief Add fuel to the driver's car
 * @param amt amount of fuel to add
 * @return true if the tank is full, else false
*/
int32_t Driver::get_tire_health_raw()
{
	int32_t o = INT32_MAX;
	for (int i = 0; i < 4; i++)
		if (tire_health[i] < o)
			o = tire_health[i];
	return o;
}
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

void Driver::remove_tire(uint8_t idx)
{
	tire_health[idx] = 0;
}

void Driver::replace_tire(uint8_t idx)
{
	tire_health[idx] = INT32_MAX;
}

int Driver::main(void)
{
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


void Driver::degrade_tires()
{
	for (int i = 0; i < 4; i++)
		tire_health[i] -= tire_dist(rnd) * speed;
}

void Driver::consume_resources()
{
	fuel -= FUEL_CONS_COEFF * speed;
	degrade_tires();
	visor_clarity -= visor_buildup();
	intake_flow -= intake_buildup();
	if (
		fuel < 0 || get_tire_health_raw() < 0 || visor_clarity < 0
		|| intake_flow < 0) {
		disqualified = true;
	}
	if (
		fuel < LOW_FUEL_THRESH
		|| get_tire_health_raw() < LOW_TIRE_THRESH
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
