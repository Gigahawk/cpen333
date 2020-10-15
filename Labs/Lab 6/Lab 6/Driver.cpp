#include "Driver.h"



int Driver::main(void)
{
	while (laps < MAX_LAPS) {
		while (progress < LAP_LENGTH) {
			//consume_resources();
			//if (disqualified)
			//	goto race_end;
			progress += speed;
			next_speed();
		}
		progress %= LAP_LENGTH + 1;
		laps += 1;
	}
	progress = 0;
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
