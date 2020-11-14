#pragma once
#include "Widget.h"
#include "../common.h"

class ElevatorQueueWidget :
    public Widget
{
public:
    ElevatorQueueWidget(int8_t* fs) : floor_stack(fs){}

    void reset(window_dim_t wd) {
        uint32_t col = ELEVATOR_WIDGET_WIDTH;
        uint32_t row = 1;
        _reset_buf(row, col);
        set_display();
    }
private:
    int8_t* floor_stack;
    char buf[BUF_LEN] = { 0 };
    void set_display() {
        string list = "";
        for (int i = 0; i < 10; i++) {
            if (floor_stack[i] != -1) {
                list += to_string(floor_stack[i]) + ",";
            }
        }
        sprintf_s(buf, "Q: %s", list.c_str());
        strncpy_s(sb.win_buf[0], sb.col, buf, sb.col - 1);
    }
};

