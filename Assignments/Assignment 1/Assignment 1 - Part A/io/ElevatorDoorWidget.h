#pragma once
#include "Widget.h"
#include "../common.h"
#include "../plumbing.h"

class ElevatorDoorWidget :
    public Widget
{
public:
	ElevatorDoorWidget(uint8_t id) :
		id(id), loc(0), status(STATUS_IDLE) {}

    void reset(window_dim_t wd) {
		uint32_t col = ELEVATOR_DOOR_WIDTH;
		uint32_t row = ELEVATOR_DOOR_HEIGHT;
		_reset_buf(row, col);
		set_display();
    }
	void set_status(int8_t s) { status = s; }
	void set_loc(uint32_t l) { loc = l; }
	void set_waiting(bool w) { waiting = w; }
private:
	uint8_t id;
	int8_t status;
	uint32_t loc;
	bool waiting;
	void set_display() {
		bool open = false;
		if (status == STATUS_IDLE && (loc % FLOOR_DISTANCE) == 0)
			open = true;
		if (waiting)
			open = true;

		for (int i = 0; i < sb.row; i++) {
			for (int j = 0; j < sb.col; j++) {
				if (i != 0 && i != sb.row - 1 && j != 0 && j != sb.col - 1) {
					if (open)
						sb.win_buf[i][j] = ' ';
					else
						sb.win_buf[i][j] = '#';
				}
				else {
					sb.win_buf[i][j] = '#';
				}
			}
		}
		sb.win_buf[0][0] = id + '0';
		if(status == STATUS_UP)
			sb.win_buf[0][sb.col - 1] = '^';
		if(status == STATUS_DOWN)
			sb.win_buf[0][sb.col - 1] = 'v';
		if(status == STATUS_FAULT)
			sb.win_buf[0][sb.col - 1] = 'F';
	}
};

