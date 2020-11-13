#pragma once
#include "../rt.h"
#include "../plumbing.h"
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
	char mb_msg[BUF_LEN] = { 0 };
	uint32_t idx;
	IOPipe io_pipe = IOPipe(IO_PIPE);
	io_data_t io_data;
	CMailbox mb;
	UINT mb_code;

	void set_display() {
		ZeroMemory(sb.win_buf[0], sb.col);
		if (strlen(msg)) {
			strncpy_s(sb.win_buf[0], sb.col, msg, sb.col - 1);
		}
		else {
			sb.win_buf[0][0] = prefix;
			strncpy_s(sb.win_buf[0] + 1, sb.col - 1, buffer, sb.col - 2);
		}
	}

	int main() {
		char c;
		while (true) {
			if (mb.TestForMessage()) {
				mb_code = mb.GetMessageA();

				switch (mb_code) {
				case SIM_END:
					show_message("Simulation Ending");
					break;
				default:
					sprintf_s(mb_msg, "Unknown message code recieved: %d", mb_code);
					show_message(mb_msg);
				}
			}

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
		switch (buffer[0]) {
		case '1': // Inside elevator commands
		case '2':
			if (buffer[1] >= '0' && buffer[1] <= '9')
				send_command();
			else
				show_message("Unknown command");
			break;
		case 'u': // Outside up command
			if (buffer[1] >= '0' && buffer[1] <= '8')
				send_command();
			else
				show_message("Unknown command");
			break;
		case 'd': // Outside down command
			if (buffer[1] >= '1' && buffer[1] <= '9')
				send_command();
			else
				show_message("Unknown command");
			break;
		case '-': // Fault conditions
		case '+':
			if (buffer[1] == '1' || buffer[1] == '2')
				send_command();
			else
				show_message("Unknown command");
			break;
		case 'e': // End simulation
			if (buffer[1] == 'e')
				send_command();
			else
				show_message("Unknown command");
			break;
		default:
			show_message("Unknown command");
			break;
		}
	}

	void send_command() {
		strncpy_s(io_data.cmd, buffer, CMD_LEN);
		io_pipe.Write(&io_data);
		idx = 0;
		ZeroMemory(buffer, BUF_LEN);
	}

	void show_message(const char* m) {
		strcpy_s(msg, m);

		// Delete input to show message
		idx = 0;
		ZeroMemory(buffer, BUF_LEN);
	}
};

