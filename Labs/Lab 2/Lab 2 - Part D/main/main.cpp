#include "../rt.h"

UINT __stdcall thread(void* args);
struct thread_args {
	int id;
	int count;
	const char* name;
};

int main(void) {
	thread_args ta0 = {
		0, 10, "World"
	};
	thread_args ta1 = {
		1, 20, "Galaxy"
	};
	thread_args ta2 = {
		2, 30, "Universe"
	};

	// Concurrent threads printing will mangle console output,
	// make sure only one thread is printing at a time
	CMutex m("print");

	cout << "Creating child threads" << endl;
	CThread t0(thread, ACTIVE, (void*)&ta0);
	CThread t1(thread, ACTIVE, (void*)&ta1);
	CThread t2(thread, ACTIVE, (void*)&ta2);

	m.Wait();
	cout << "Child threads started" << endl;
	m.Signal();

	m.Wait();
	cout << "Waiting for thread0 to terminate..." << endl;
	m.Signal();
	t0.WaitForThread();

	m.Wait();
	cout << "Waiting for thread1 to terminate..." << endl;
	m.Signal();
	t1.WaitForThread();

	m.Wait();
	cout << "Waiting for thread2 to terminate..." << endl;
	m.Signal();
	t2.WaitForThread();

	return 0;
}

UINT __stdcall thread(void* args)
{
	thread_args* a = (thread_args*)args;
	int id = a->id;
	int count = a->count;
	const char* name = a->name;
	CMutex m("print");

	for (int i = 0; i < count; i++) {
		Sleep(500);
		m.Wait();
		cout << "[THREAD:" << id << "] Hello " << name << " " << i << endl;
		m.Signal();
	}
	return 0;
}
