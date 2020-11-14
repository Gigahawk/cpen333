#pragma once
#include "Widget.h"
#include "../ElevatorMonitor.h"
#include "../common.h"
#include "ElevatorDoorWidget.h"
#include "ElevatorFloorWidget.h"

class ElevatorWidget :
	public Widget, public ActiveClass
{
public:
    ElevatorWidget(uint8_t id) :
        id(id), em(ElevatorMonitor(id)) , ed(ElevatorDoorWidget(id))
	{
		place_widget(&ef, -1, -1);
		place_widget(&ed, 0, 0);
		Resume();
	}
    void reset(window_dim_t wd) {
		uint32_t col = ELEVATOR_WIDGET_WIDTH;
		uint32_t row = wd.row;
		_reset_buf(row, col);
		_draw_children(wd);
    }
private:
    uint8_t id;
    ElevatorMonitor em;
	ElevatorDoorWidget ed;
	ElevatorFloorWidget ef;
	int main() {
		e_status_t es;
		while (true) {
			es = em.Get_Elevator_Status();
			ed.set_status(es.status);
			ed.set_loc(es.loc);
			ef.set_loc(es.loc);
		}
		return 0;
	}
};

