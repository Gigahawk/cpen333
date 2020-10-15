#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine/olcPixelGameEngine.h"
#include "rt.h"
#include "Driver.h"

#define NUM_DRIVERS 10

#define TRACK_MARGIN 2

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
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame, draws random coloured pixels
		Clear(olc::WHITE);
		draw_track();
		draw_drivers();
		return true;
	}
private:
	Driver* drivers[NUM_DRIVERS];
	int32_t track_len = ScreenHeight() - (TRACK_MARGIN*2);
	void draw_track() {
		int32_t x, y, w, h;
		x = TRACK_MARGIN;
		y = TRACK_MARGIN;
		w = NUM_DRIVERS;
		h = track_len;
		FillRect(x, y, w, h, olc::DARK_GREY);
	}

	void draw_drivers() {
		for (int i = 0; i < NUM_DRIVERS; i++) {
			int32_t x = TRACK_MARGIN + i;
			int32_t y = TRACK_MARGIN + drivers[i]->get_progress_percent() * track_len;
			Draw(x, y, drivers[i]->color);
		}
	}
};

int main()
{
	Example demo;
	if (demo.Construct(320, 240, 3, 3))
		demo.Start();
	return 0;
}
