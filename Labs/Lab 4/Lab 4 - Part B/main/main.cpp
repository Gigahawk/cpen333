#include "../rt.h"
#include "../common.h"
#include "../mutex_printf.h"

CCondition stop_t("stop_t");
CCondition stop_p("stop_p");

UINT __stdcall thread1(void* args) {
	CTypedPipe<pipe1_data> pipe1("p1");
	pipe1_data d;
	while (1) {
		if (stop_t.Test())
			break;
		pipe1.Read(&d);
		mutex_printf(
			"Read employee with { id: %d, username: %s, position: %s, permissions: %04X } from pipeline\n",
			d.id, d.username, d.position, d.permissions);
	}
	return 0;
}

UINT __stdcall thread2(void* args) {
	CTypedPipe<pipe2_data> pipe2("p2");
	pipe2_data d;
	while (1) {
		if (stop_t.Test())
			break;
		pipe2.Read(&d);
		mutex_printf(
			"Read car with { make: %s, model: %s, year: %d } from pipeline\n",
			d.make, d.model, d.year);
	}
	return 0;
}

int main() {
	bool should_break = false;
	CThread t1(thread1);
	CThread t2(thread2);
	CProcess p1(
		"child1.exe",
		NORMAL_PRIORITY_CLASS,
		OWN_WINDOW);
	CProcess p2(
		"child2.exe",
		NORMAL_PRIORITY_CLASS,
		OWN_WINDOW);

	cout << "Child processes and threads started, press any key to exit" << endl;
	while (1) 
		if (_kbhit())
			break;
	// Stop threads first (otherwise pX.Read() will hang)
	stop_t.Signal();
	t1.WaitForThread();
	t2.WaitForThread();

	stop_p.Signal();
	p1.WaitForProcess();
	p2.WaitForProcess();

	return 0;
}