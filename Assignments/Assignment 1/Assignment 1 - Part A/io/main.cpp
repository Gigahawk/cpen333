#include <iostream>
#include <windows.h>
#include "../rt.h"
#include "Widget.h"
#include "EditText.h"

using namespace std;


int main(void) {
	CMailbox mb;
	Widget screen;
	EditText edit_text;

	while (true) {
		// Forward all messages to edit_text
		if (mb.TestForMessage())
			edit_text.Post(mb.GetMessageA());
		screen.reset();
		screen.place_widget(edit_text,
			0, screen.sb.row - 1);
		screen.draw();
	}
	return 0;
}