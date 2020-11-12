#pragma once
#include "../rt.h"

struct screen_buffer {
	uint32_t row, col;
	char** win_buf;
	char* char_buf;
};

class Widget
{
public:
	Widget() {
		sb.row = 0;
		sb.col = 0;
		sb.win_buf = nullptr;
		sb.char_buf = nullptr;
		reset();
	}
	void place_widget(Widget w, uint32_t x, uint32_t y) {
		for (uint32_t i = y; i < y + w.sb.row && i < sb.row; i++) {
			for (uint32_t j = x; j < x + w.sb.col && j < sb.col; j++) {
				sb.win_buf[i][j] = w.sb.win_buf[i - y][j - x];
			}
		}
	}
	void draw() {
		// Disable console cursor
		CONSOLE_CURSOR_INFO cci;
		GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
		cci.bVisible = false;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);

		int k = 0;
		char tmp;
		for (uint32_t i = 0; i < sb.row; i++) {
			for (uint32_t j = 0; j < sb.col; j++) {
				tmp = sb.win_buf[i][j];
				if (tmp < 32 || tmp > 126)
					tmp = ' ';
				sb.char_buf[k++] = tmp;
			}
			sb.char_buf[k++] = '\n';
		}
		// Remove last newline
		sb.char_buf[--k] = '\0';
		MOVE_CURSOR(0, 0);
		printf(sb.char_buf);
	};
	void reset() {
		uint32_t row, col;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		col = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		if (csbi.srWindow.Bottom < csbi.srWindow.Top)
			row = 1;
		else
			row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		if (row != sb.row || col != sb.col) {
			free_buf();
			create_buf(row, col);
		}
	}

	screen_buffer sb;
protected:
	void free_buf() {
		if(sb.char_buf)
			free(sb.char_buf);
		if (sb.win_buf) {
			for (uint32_t i = 0; i < sb.row; i++) {
				if(sb.win_buf[i])
					free(sb.win_buf[i]);
			}
			free(sb.win_buf);
		}
	}

	void create_buf(uint32_t row, uint32_t col) {
		sb.win_buf = (char**)malloc(row * sizeof(char*));
		for (uint32_t i = 0; i < row; i++) {
			sb.win_buf[i] = (char*)malloc(col * sizeof(char));
		}

		sb.char_buf = (char*)malloc((1 + row * (col + 1)) * sizeof(char));
		sb.row = row;
		sb.col = col;
	}

};

