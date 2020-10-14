#include "../rt.h"
#define EXTRA_THREADS 3

struct thread_args{
	uint8_t c1;
	uint8_t c2;
	uint8_t t;
	atomic_flag* f;
};

void move_and_print(uint8_t c1, uint8_t c2, uint8_t t, atomic_flag* f) {
	for (int i = 0; i < 50000; i++) {
		while (atomic_flag_test_and_set(f));
		MOVE_CURSOR(c1, c2);
		printf("Thread %d", t);
		fflush(stdout);
		atomic_flag_clear(f);
	}
}

UINT __stdcall thread_fn(void *args)
{
	thread_args a = *(thread_args*)args;
	move_and_print(a.c1, a.c2, a.t, a.f);
	return 0;
}

class PrintClass : public ActiveClass
{
	int main(void) {
		CThread* threads[EXTRA_THREADS];
		thread_args ta[EXTRA_THREADS] = { 0 };
		static atomic_flag f = ATOMIC_FLAG_INIT;
		for (int i = 0; i < EXTRA_THREADS; i++) {
			ta[i].c1 = 10;
			ta[i].c2 = (i + 2) * 10;
			ta[i].t = i + 2;
			ta[i].f = &f;
			threads[i] = new CThread(
				thread_fn,
				ACTIVE,
				(void*)&ta[i]
			);
		}
		move_and_print(5, 5, 1, &f);
		for (int i = 0; i < EXTRA_THREADS; i++) {
			threads[i]->WaitForThread();
			delete threads[i];
		}
		return 0;
	}
};

int main(void) {
	PrintClass p;
	p.Resume();
	p.WaitForThread();
	return 0;
}