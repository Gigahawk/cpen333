#pragma once
#include "Widget.h"
class ScreenWidget : public Widget
{
public:
	void reset(window_dim_t wd) {
		_reset_buf(wd.row, wd.col);
		_draw_children(wd);
	}
};

