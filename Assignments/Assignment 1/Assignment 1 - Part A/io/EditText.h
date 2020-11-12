#pragma once
#include "../rt.h"
#include "Widget.h"

#define BUF_LEN 1024
class EditText :
	public Widget, public ActiveClass
{
public:
    EditText(uint32_t width = 0, char prefix = ':') :
		prefix(prefix), width(width), Widget() , idx(0) {
		reset();
		Resume();
	}
	void reset() {
		uint32_t col = width;
		if (col <= 0) {
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			col = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		}
		if (col != sb.col) {
			free_buf();
			create_buf(1, col);
		}
		set_display();
	}
private:
    uint32_t width;
	char prefix;
	char buffer[BUF_LEN] = { 0 };
	char cmd[BUF_LEN] = { 0 };
	char msg[BUF_LEN] = { 0 };
	uint32_t idx;

	void set_display() {
		ZeroMemory(sb.win_buf[0], sb.col);
		if (strlen(msg)) {
			strcpy_s(sb.win_buf[0], sb.col, msg);
		}
		else {
			sb.win_buf[0][0] = prefix;
			strcpy_s(sb.win_buf[0] + 1, sb.col - 1, buffer);
		}
	}

	int main() {
		char c;
		while (true) {
			if (_kbhit()) {
				// Clear message
				msg[0] = '\0';
				c = _getch();
				switch (c) {
				case 8: // Backspace
					if(idx)
						buffer[--idx] = '\0';
					break;
				case '\r': // Enter (For Debug)
					handle_command();
					break;
				default:
					buffer[idx++] = c;
				}
			}
			if (idx == 2 && buffer[0] != '/') {
				handle_command();
			}
			reset();
		}
		return 0;
	}

	void handle_command() {
		if (!idx)
			return;
		idx = 0;
		ZeroMemory(buffer, BUF_LEN);
		strcpy_s(msg, "Unknown command");
	}
};

