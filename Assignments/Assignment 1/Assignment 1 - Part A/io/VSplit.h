#pragma once
#include "ScreenWidget.h"
class VHalf :
    public Widget
{
public:
	void reset(window_dim_t wd) {
		_reset_buf(wd.row, wd.col/2);
		_draw_children(wd);
	}
};

class VSplit :
    public ScreenWidget
{
public:
	VSplit() {
		place_widget(&left, -1, -1);
		place_widget(&right, 1, -1);
	}

	void place_left(Widget *w, int8_t ax, int8_t ay) {
		left.place_widget(w, ax, ay);
	}

	void place_right(Widget *w, int8_t ax, int8_t ay) {
		right.place_widget(w, ax, ay);
	}
private:
	VHalf left;
	VHalf right;
};

