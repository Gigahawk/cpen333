#pragma once
#include <vector>
#include "../rt.h"

class Widget;
struct screen_buffer {
	uint32_t row, col;
	char** win_buf;
	char* char_buf;
};

struct child_widget_t {
	Widget *w;
	int8_t anchor_x;
	int8_t anchor_y;
};

struct window_dim_t {
	uint32_t row, col;
};

window_dim_t get_window_dims(); 

class Widget
{
public:
	Widget() {
		sb.row = 0;
		sb.col = 0;
		sb.win_buf = nullptr;
		sb.char_buf = nullptr;
	}

	virtual ~Widget() {};

	void place_widget(Widget *w, int8_t ax, int8_t ay) {
		child_widget_t cw = { w, ax, ay };
		cws.push_back(cw);
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

	virtual void reset(window_dim_t wd) = 0;
	screen_buffer sb;
protected:
	vector<child_widget_t> cws;

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

	void _draw_child(child_widget_t cw) {
		int8_t ax = cw.anchor_x;
		int8_t ay = cw.anchor_y;
		Widget* w = cw.w;

		uint32_t x_start, y_start;
		uint32_t w_x_start, w_y_start;

		if (ax == 0) {
			if (w->sb.col > sb.col) {
				x_start = 0;
				w_x_start = (w->sb.col - sb.col) / 2;
			}
			else {
				x_start = (sb.col - w->sb.col) / 2;
				w_x_start = 0;
			}
		}
		else {
			w_x_start = 0;
			x_start = 0;
		}
		if (ay == 0) {
			if (w->sb.row > sb.row) {
				y_start = 0;
				w_y_start = (w->sb.row - sb.row) / 2;
			}
			else {
				y_start = (sb.row - w->sb.row) / 2;
				w_y_start = 0;
			}
		}
		else {
			w_y_start = 0;
			y_start = 0;
		}

		for (uint32_t i = y_start, k = w_y_start; 
			i < sb.row&& k < w->sb.row; i++, k++) {
			for (uint32_t j = x_start, m = w_x_start; 
				j < sb.col && m < w->sb.col; j++, m++) {
				if (ax != 1 && ay != 1) {
					sb.win_buf[i][j] = w->sb.win_buf[k][m];
				}
				else if (ax == 1 && ay != 1) {
					sb.win_buf[i][sb.col - 1 - j] = w->sb.win_buf[k][w->sb.col - 1 - m];
				}
				else if (ax != 1 && ay == 1) {
					sb.win_buf[sb.row - 1 - i][j] = w->sb.win_buf[w->sb.row - 1 - k][m];
				}
				else if (ax == 1 && ay == 1) {
					sb.win_buf[sb.row - 1 - i][sb.col - 1 - j] = w->sb.win_buf[w->sb.row - 1 - k][sb.col - 1 - m];
				}
			}
		}
	}

	void _draw_children(window_dim_t wd) {
		for (auto it = cws.begin(); it != cws.end(); it++) {
			it->w->reset(wd);
			_draw_child(*it);
		}
	}

	void _reset_buf(uint32_t row, uint32_t col) {
		if (row != sb.row || col != sb.col) {
			free_buf();
			create_buf(row, col);
		}
	}

};

