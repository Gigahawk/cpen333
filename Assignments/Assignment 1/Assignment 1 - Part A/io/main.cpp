#include <iostream>
#include <windows.h>
#include "../rt.h"
#include "VSplit.h"
#include "EditText.h"
#include "ElevatorWidget.h"

using namespace std;


int main(void) {
	CMailbox mb;
	VSplit screen;
	EditText edit_text;
	ElevatorWidget elev1(1);
	ElevatorWidget elev2(2);
	screen.place_left(&elev1,
		-1, -1);
	screen.place_right(&elev2,
		-1, -1);
	screen.place_widget(&edit_text,
		-1, 1);

	while (true) {
		// Forward all messages to edit_text
		if (mb.TestForMessage())
			edit_text.Post(mb.GetMessageA());
		screen.reset(get_window_dims());
		screen.draw();
	}
	return 0;
}