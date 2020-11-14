#pragma once
#include "Widget.h"
#include "../plumbing.h"
#include "../common.h"
class ElevatorFloorWidget :
    public Widget
{
public:
    ElevatorFloorWidget() :
        loc(0) {}
    void reset(window_dim_t wd) {
		uint32_t col = ELEVATOR_WIDGET_WIDTH;
		uint32_t row = wd.row;
        _reset_buf(row, col);
        set_display();
    }
    void set_loc(uint32_t l) { loc = l; }
    uint32_t get_loc() { return loc; }
private:
    uint32_t loc;
    void set_display() {
        uint32_t base_height = (sb.row + ELEVATOR_DOOR_HEIGHT) / 2 - 1;
        double l = (double)(loc % FLOOR_DISTANCE)/ FLOOR_DISTANCE;
        uint32_t offset = (base_height + (uint32_t)(l*sb.row));
        uint8_t floor = loc / FLOOR_DISTANCE;
        if (offset > sb.row) {
            offset %= sb.row;
            floor++;
        }
		for (int i = 0; i < sb.row; i++) {
			for (int j = 0; j < sb.col; j++) {
                if(i == offset)
                    if(j == 0)
						sb.win_buf[i][j] = floor + '0';
                    else
					sb.win_buf[i][j] = '_';
                else
					sb.win_buf[i][j] = ' ';
			}
		}
    }
};

