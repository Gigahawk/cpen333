#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

// Concurrent threads printing will mangle console output,
// make sure only one thread is printing at a time
mutex m;

void m_thread(int id, int count, const char* name);

int main(void) {

	cout << "Creating child threads" << endl;
	thread t0(m_thread, 0, 10, "World");
	thread t1(m_thread, 1, 20, "Galaxy");
	thread t2(m_thread, 2, 30, "Universe");

	m.lock();
	cout << "Child threads started" << endl;
	m.unlock();

	m.lock();
	cout << "Waiting for thread0 to terminate..." << endl;
	m.unlock();
	t0.join();

	m.lock();
	cout << "Waiting for thread1 to terminate..." << endl;
	m.unlock();
	t1.join();

	m.lock();
	cout << "Waiting for thread2 to terminate..." << endl;
	m.unlock();
	t2.join();

	return 0;
}

void m_thread(int id, int count, const char* name)
{
	for (int i = 0; i < count; i++) {
		this_thread::sleep_for(chrono::milliseconds(500));
		m.lock();
		cout << "[THREAD:" << id << "] Hello " << name << " " << i << endl;
		m.unlock();
	}
}
