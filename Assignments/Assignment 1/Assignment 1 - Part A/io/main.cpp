#include <iostream>
#include <windows.h>
#include "../rt.h"
#include "Widget.h"
#include "EditText.h"

using namespace std;


int main(void) {
	Widget screen;
	EditText edit_text;

	while (true) {
		screen.reset();
		screen.place_widget(edit_text,
			0, screen.sb.row - 1);
		screen.draw();
	}
	return 0;
}