#include <windows.h>
#include <iostream>

using namespace std;

HANDLE p_mutex;

#define MUTEX_LOCK(m) WaitForSingleObject(m, INFINITE)
#define MUTEX_UNLOCK(m) ReleaseMutex(m)

DWORD WINAPI m_thread(void* args);
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
	DWORD tid0, tid1, tid2;

	// Concurrent threads printing will mangle console output,
	// make sure only one thread is printing at a time
	p_mutex = CreateMutex(NULL, FALSE, NULL);

	cout << "Creating child threads" << endl;
	HANDLE t0 = CreateThread(
		NULL,
		0,
		m_thread,
		(void*)&ta0,
		0,
		&tid0
	);
	HANDLE t1 = CreateThread(
		NULL,
		0,
		m_thread,
		(void*)&ta1,
		0,
		&tid1
	);
	HANDLE t2 = CreateThread(
		NULL,
		0,
		m_thread,
		(void*)&ta2,
		0,
		&tid2
	);

	// CreateThread/CreateMutex could fail and leave handles as NULL,
	// for a trivial example like this it isn't worth worrying about.
	MUTEX_LOCK(p_mutex);
	cout << "Child threads started" << endl;
	MUTEX_UNLOCK(p_mutex);

	MUTEX_LOCK(p_mutex);
	cout << "Waiting for thread0 to terminate..." << endl;
	MUTEX_UNLOCK(p_mutex);
	WaitForSingleObject(t0, INFINITE);

	MUTEX_LOCK(p_mutex);
	cout << "Waiting for thread1 to terminate..." << endl;
	MUTEX_UNLOCK(p_mutex);
	WaitForSingleObject(t1, INFINITE);

	MUTEX_LOCK(p_mutex);
	cout << "Waiting for thread2 to terminate..." << endl;
	MUTEX_UNLOCK(p_mutex);
	WaitForSingleObject(t2, INFINITE);

	return 0;
}

DWORD WINAPI m_thread(void* args)
{
	thread_args* a = (thread_args*)args;
	int id = a->id;
	int count = a->count;
	const char* name = a->name;

	for (int i = 0; i < count; i++) {
		Sleep(500);
		MUTEX_LOCK(p_mutex);
		cout << "[THREAD:" << id << "] Hello " << name << " " << i << endl;
		MUTEX_UNLOCK(p_mutex);
	}
	return 0;
}
