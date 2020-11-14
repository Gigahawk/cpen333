#include "Widget.h"

window_dim_t get_window_dims()
{
	window_dim_t wd;
	uint32_t row, col;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	col = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	if (csbi.srWindow.Bottom < csbi.srWindow.Top)
		row = 1;
	else
		row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	wd.row = row;
	wd.col = col;
	return wd;
}
