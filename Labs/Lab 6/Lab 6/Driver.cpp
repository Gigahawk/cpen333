#include "Driver.h"



int Driver::main(void)
{
	race_start.Wait();
	while (laps < MAX_LAPS) {
		if (needs_pit)
			enter_pit();
		while (progress < LAP_LENGTH) {
			consume_resources();
			if (disqualified)
				goto race_end;
			progress += speed;
			next_speed();
		}
		progress = 0;
		laps += 1;
	}
race_end:
	return 0;
}

void Driver::next_speed()
{
	speed =  (int32_t)round(dist(rnd));
}

void Driver::consume_resources()
{
	fuel -= FUEL_CONS_COEFF * speed;
	tire_health -= TIRE_DEG_COEFF * speed;
	if (fuel < 0 || tire_health < 0) {
		disqualified = true;
	}
	if (fuel < LOW_FUEL_THRESH || tire_health < LOW_TIRE_THRESH) {
		needs_pit = true;
	}
}

bool Driver::enter_pit()
{
	if(!pit_entry_light.Read()) 
		return false;
	speed = 0;
	car_in_pit.Signal();
	while(!pit_exit_light.Read());
	car_in_pit.Wait();
	return true;
}
