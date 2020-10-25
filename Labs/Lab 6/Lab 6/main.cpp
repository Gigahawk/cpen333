#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine/olcPixelGameEngine.h"
#include "rt.h"
#include "common.h"

#include "Driver.h"
#include "Supervisor.h"
#include "Refuelling.h"
#include "Debris.h"
#include "Visor.h"
#include "Jacking.h"
#include "WheelNut.h"
#include "WheelRemoval.h"
#include "WheelRefitting.h"
#include "globals.h"

#define MARGIN 5
#define LIGHT_RAD 5
#define STANDINGS_WIDTH 220
#define STANDINGS_ROW_HEIGHT 10
#define STANDINGS_CHAR_LEN 32
#define CAR_SIZE 10
#define TRACK_WIDTH CAR_SIZE*NUM_DRIVERS
#define TRACK_HEIGHT (ScreenHeight() - (MARGIN*2))

bool comp_place(Driver* d1, Driver* d2) {
	return d1->get_progress() > d2->get_progress();
}

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine, public Semaphores
{
public:
	Example()
	{
		// Name you application
		sAppName = "Example";
	}
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		for (int i = 0; i < NUM_DRIVERS; i++) {
			drivers[i] = new Driver(
				olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
			drivers[i]->Resume();
			standings[i] = drivers[i];
		}
		supervisor = new Supervisor();
		supervisor->Resume();

		refuelling_tech = new Refuelling();
		refuelling_tech->Resume();

		visor_tech = new Visor();
		visor_tech->Resume();

		debris_tech = new Debris();
		debris_tech->Resume();

		jacking_front_tech = new Jacking(true);
		jacking_front_tech->Resume();
		jacking_back_tech = new Jacking(false);
		jacking_back_tech->Resume();

		wheelnut_front_left_tech = new WheelNut(true, true);
		wheelnut_front_left_tech->Resume();
		wheelnut_front_right_tech = new WheelNut(true, false);
		wheelnut_front_right_tech->Resume();
		wheelnut_back_left_tech = new WheelNut(false, true);
		wheelnut_back_left_tech->Resume();
		wheelnut_back_right_tech = new WheelNut(false, false);
		wheelnut_back_right_tech->Resume();

		wheelremoval_front_left_tech = new WheelRemoval(true, true);
		wheelremoval_front_left_tech->Resume();
		wheelremoval_front_right_tech = new WheelRemoval(true, false);
		wheelremoval_front_right_tech->Resume();
		wheelremoval_back_left_tech = new WheelRemoval(false, true);
		wheelremoval_back_left_tech->Resume();
		wheelremoval_back_right_tech = new WheelRemoval(false, false);
		wheelremoval_back_right_tech->Resume();

		wheelrefitting_front_left_tech = new WheelRefitting(true, true);
		wheelrefitting_front_left_tech->Resume();
		wheelrefitting_front_right_tech = new WheelRefitting(true, false);
		wheelrefitting_front_right_tech->Resume();
		wheelrefitting_back_left_tech = new WheelRefitting(false, true);
		wheelrefitting_back_left_tech->Resume();
		wheelrefitting_back_right_tech = new WheelRefitting(false, false);
		wheelrefitting_back_right_tech->Resume();

		race_start.Signal(NUM_DRIVERS);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame, draws random coloured pixels
		Clear(olc::WHITE);
		draw_track();
		draw_pit_lights();
		draw_technicians();
		draw_standings();
		draw_drivers();
		return true;
	}
private:
	Driver* drivers[NUM_DRIVERS];
	Driver* standings[NUM_DRIVERS];
	Supervisor* supervisor;
	Refuelling* refuelling_tech;
	Visor* visor_tech;
	Debris* debris_tech;
	Jacking* jacking_front_tech;
	Jacking* jacking_back_tech;
	WheelNut* wheelnut_front_left_tech;
	WheelNut* wheelnut_front_right_tech;
	WheelNut* wheelnut_back_left_tech;
	WheelNut* wheelnut_back_right_tech;
	WheelRemoval* wheelremoval_front_left_tech;
	WheelRemoval* wheelremoval_front_right_tech;
	WheelRemoval* wheelremoval_back_left_tech;
	WheelRemoval* wheelremoval_back_right_tech;
	WheelRefitting* wheelrefitting_front_left_tech;
	WheelRefitting* wheelrefitting_front_right_tech;
	WheelRefitting* wheelrefitting_back_left_tech;
	WheelRefitting* wheelrefitting_back_right_tech;
	void draw_track() {
		int32_t x, y, w, h;
		x = MARGIN;
		y = MARGIN;
		w = TRACK_WIDTH;
		h = TRACK_HEIGHT;
		FillRect(x, y, w, h, olc::DARK_GREY);
	}

	void draw_drivers() {
		int32_t x, y, w, h;
		w = CAR_SIZE;
		h = CAR_SIZE;
		for (int i = 0; i < NUM_DRIVERS; i++) {
			x = MARGIN + i*CAR_SIZE;
			y = MARGIN + drivers[i]->get_lap_progress() * (TRACK_HEIGHT - CAR_SIZE);
			FillRect(x, y, w, h, drivers[i]->color);
		}
	}

	void draw_pit_lights() {
		int32_t x1, x2, x3, y1, y2, r;
		olc::Pixel c1, c2;
		r = LIGHT_RAD;
		x1 = MARGIN + TRACK_WIDTH + MARGIN + LIGHT_RAD;
		x2 = x1 + LIGHT_RAD + MARGIN + LIGHT_RAD;
		x3 = x2 + LIGHT_RAD + MARGIN;
		y1 = MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;

		c1 = pit_entry_light.Read() ? olc::GREEN : olc::RED;
		c2 = pit_exit_light.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c1);
		FillCircle(x2, y1, r, c2);

		DrawString(
			x3, y2, format("Supervisor: %s", supervisor->get_state().c_str()),
			olc::BLACK, 1);
	}

	void draw_technicians() {
		int32_t x1, x2, y1, y2, r;
		olc::Pixel c;

		x1 = MARGIN + TRACK_WIDTH + MARGIN + LIGHT_RAD;
		x2 = x1 + LIGHT_RAD + MARGIN;
		r = LIGHT_RAD;

		// Refuelling technician
		y1 = MARGIN + LIGHT_RAD + LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = refuelling.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Refuelling: %s", refuelling_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Visor technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = visor.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Visor: %s", visor_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Debris technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = debris.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Debris: %s", debris_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Jacking Front technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = jacking_front.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Jacking Front: %s", jacking_front_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Jacking Back technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = jacking_back.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Jacking Back: %s", jacking_back_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Wheel Nut Front Left technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = wheelnut_front_left.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Wheel Nut Front Left: %s", wheelnut_front_left_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Wheel Nut Front Right technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = wheelnut_front_right.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Wheel Nut Front Right: %s", wheelnut_front_right_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Wheel Nut Back Left technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = wheelnut_back_left.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Wheel Nut Back Left: %s", wheelnut_back_left_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Wheel Nut Back Right technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = wheelnut_back_right.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Wheel Nut Back Right: %s", wheelnut_back_right_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Wheel Removal Front Left technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = wheelremoval_front_left.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Wheel Removal Front Left: %s", wheelremoval_front_left_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Wheel Removal Front Right technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = wheelremoval_front_right.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Wheel Removal Front Right: %s", wheelremoval_front_right_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Wheel Removal Back Left technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = wheelremoval_back_left.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Wheel Removal Back Left: %s", wheelremoval_back_left_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Wheel Removal Back Right technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = wheelremoval_back_right.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Wheel Removal Back Right: %s", wheelremoval_back_right_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Wheel Refitting Front Left technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = wheelrefitting_front_left.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Wheel Refitting Front Left: %s", wheelrefitting_front_left_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Wheel Refitting Front Right technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = wheelrefitting_front_right.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Wheel Refitting Front Right: %s", wheelrefitting_front_right_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Wheel Refitting Back Left technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = wheelrefitting_back_left.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Wheel Refitting Back Left: %s", wheelrefitting_back_left_tech->get_state().c_str()),
			olc::BLACK, 1);

		// Wheel Refitting Back Right technician
		y1 += LIGHT_RAD + MARGIN + LIGHT_RAD;
		y2 = y1 - LIGHT_RAD;
		c = wheelrefitting_back_right.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y1, r, c);
		DrawString(
			x2, y2, format("Wheel Refitting Back Right: %s", wheelrefitting_back_right_tech->get_state().c_str()),
			olc::BLACK, 1);
	}

	void draw_standings() {
		int32_t x, y;
		sort_standings();
		x = ScreenWidth() - MARGIN - STANDINGS_WIDTH;
		for (int i = 0; i < NUM_DRIVERS; i++) {
			y = MARGIN + i*STANDINGS_ROW_HEIGHT;
			char s[STANDINGS_CHAR_LEN] = { 0 };
			sprintf_s(s, STANDINGS_CHAR_LEN, "%02d, F: %03d, T: %03d, L: %d",
				i + 1,
				(int32_t)(standings[i]->get_fuel() * 100),
				(int32_t)(standings[i]->get_tire_health() * 100),
				standings[i]->get_laps());
			DrawString(x, y, string(s), standings[i]->color, 1);
		}
	}


	void sort_standings() {
		sort(standings, standings + NUM_DRIVERS, comp_place);
	}

};

int main()
{
	Example demo;
	if (demo.Construct(700, 360, 2, 2))
		demo.Start();
	return 0;
}
