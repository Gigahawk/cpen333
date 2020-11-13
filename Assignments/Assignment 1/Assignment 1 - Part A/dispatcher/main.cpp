#include <iostream>
#include <windows.h>
#include "../rt.h"
#include "../common.h"
#include "../plumbing.h"

using namespace std;

int main(int argc, char* argv[]) {
	io_data_t io_data;
	IOPipe io_pipe(IO_PIPE);
	string path = getParentDir(string(argv[0]));
	string io_path = path + "io.exe";
	string elevator_path = path + "elevator.exe";

	CProcess io_proc(io_path.c_str(),
		NORMAL_PRIORITY_CLASS,
		OWN_WINDOW,
		//PARENT_WINDOW,
		ACTIVE);

	CProcess elevator_proc1((elevator_path + " 1").c_str(),
		NORMAL_PRIORITY_CLASS,
		OWN_WINDOW,
		ACTIVE);

	CProcess elevator_proc2((elevator_path + " 2").c_str(),
		NORMAL_PRIORITY_CLASS,
		OWN_WINDOW,
		ACTIVE);
	while (true) {
		io_pipe.Read(&io_data);
		cout << "Command: " << io_data.cmd << endl;
		io_proc.Post(123);
		elevator_proc1.Post(3);
	}
	return 0;
}
