#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine/olcPixelGameEngine.h"
#include "rt.h"
#include "common.h"

#include "Driver.h"
#include "Supervisor.h"

#define MARGIN 5
#define LIGHT_RAD 5
#define STANDINGS_WIDTH 200
#define STANDINGS_ROW_HEIGHT 10
#define STANDINGS_CHAR_LEN 32
#define CAR_SIZE 5
#define TRACK_WIDTH CAR_SIZE*NUM_DRIVERS
#define TRACK_HEIGHT (ScreenHeight() - (MARGIN*2))

CSemaphore race_start("race_start", 0, NUM_DRIVERS);
CSemaphore race_over("race_over", 0, 1);
CSemaphore pit_entry_light("pel", 1, 1);
CSemaphore pit_exit_light("pel2", 0, 1);
CSemaphore car_in_pit("cip", 0, 1);

bool comp_place(Driver* d1, Driver* d2) {
	return d1->get_progress() > d2->get_progress();
}

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
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

		race_start.Signal(NUM_DRIVERS);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame, draws random coloured pixels
		Clear(olc::WHITE);
		draw_track();
		draw_pit_lights();
		draw_standings();
		draw_drivers();
		return true;
	}
private:
	Driver* drivers[NUM_DRIVERS];
	Driver* standings[NUM_DRIVERS];
	Supervisor* supervisor;
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
			y = MARGIN + drivers[i]->get_lap_progress() * TRACK_HEIGHT;
			FillRect(x, y, w, h, drivers[i]->color);
		}
	}

	void draw_pit_lights() {
		int32_t x1, x2, y, r;
		olc::Pixel c1, c2;
		r = LIGHT_RAD;
		x1 = MARGIN + TRACK_WIDTH + MARGIN + LIGHT_RAD;
		x2 = x1 + LIGHT_RAD + MARGIN + LIGHT_RAD;
		y = MARGIN + LIGHT_RAD;

		c1 = pit_entry_light.Read() ? olc::GREEN : olc::RED;
		c2 = pit_exit_light.Read() ? olc::GREEN : olc::RED;
		FillCircle(x1, y, r, c1);
		FillCircle(x2, y, r, c2);
	}

	void draw_standings() {
		int32_t x, y;
		sort_standings();
		x = ScreenWidth() - MARGIN - STANDINGS_WIDTH;
		for (int i = 0; i < NUM_DRIVERS; i++) {
			y = MARGIN + i*STANDINGS_ROW_HEIGHT;
			char s[STANDINGS_CHAR_LEN] = { 0 };
			sprintf_s(s, STANDINGS_CHAR_LEN, "%d, F: %d, T: %d, NP: %d",
				i + 1,
				(int32_t)(standings[i]->get_fuel() * 100),
				(int32_t)(standings[i]->get_tire_health() * 100),
				standings[i]->get_needs_pit());
			DrawString(x, y, string(s), olc::BLACK, 1);
		}
	}


	void sort_standings() {
		sort(standings, standings + NUM_DRIVERS, comp_place);
	}

};

int main()
{
	Example demo;
	if (demo.Construct(800, 480, 1, 1))
		demo.Start();
	return 0;
}
