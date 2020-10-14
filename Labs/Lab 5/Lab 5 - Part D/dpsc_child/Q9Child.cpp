//////////////////////////////////////////////////////////////////////////////////
//	The producer Process
//////////////////////////////////////////////////////////////////////////////////

#include 	<stdio.h>
#include	"..\rt.h"			// change pathlist to "rt.h" to suit location on disk

struct		mydatapooldata {			// template for data in a datapool
	int x;						// just a single int
};
struct		mydatapooldata2 {		// template for the data in a datapool
	float 	x;						// just a single float
};

struct		mydatapooldata* TheDataPointer;		// pointer to the data pool
struct		mydatapooldata* TheDataPointer2;		// pointer to the data pool

int main(int argc, char* argv[])
{
	CSemaphore		ps1("PS1", 0, 1);    // semaphore with initial value 0 and max value 1
	CSemaphore		ps2("PS2", 0, 1);    // semaphore with initial value 0 and max value 1
	CSemaphore		cs1("CS1", 1, 1);    // semaphore with initial value 1 and max value 1
	CSemaphore		cs2("CS2", 1, 1);    // semaphore with initial value 1 and max value 1

//	Create a datapool and then get a pointer to it

	CDataPool dp1("MyDataPoolName", sizeof(struct mydatapooldata));
	CDataPool dp2("MyDataPoolName2", sizeof(struct mydatapooldata2));
	TheDataPointer = (struct mydatapooldata*)dp1.LinkDataPool();	// get pointer to data pool
	TheDataPointer2 = (struct mydatapooldata*)dp2.LinkDataPool();	// get pointer to data pool

	CSemaphore* cs;
	CSemaphore* ps;
	struct mydatapooldata* tdp;

	int n = atoi(argv[1]);

	if (n == 1) {
		ps = &ps1;
		cs = &cs1;
		tdp = TheDataPointer;
	}
	else if (n == 2) {
		ps = &ps2;
		cs = &cs2;
		tdp = TheDataPointer2;
	}
	else {
		printf("Usage: dpsc_child.exe <1,2>\n");
		exit(-1);
	}


	cout << "Producer Process " << n << " Running.....\n";
	int v;
	for (int i = 0; i < 10; i++) {	// Produce 10 lots of data
		v = i + n * 100;
		cs->Wait();
		tdp->x = v;
		ps->Signal();
		cout << "Produced " << v << ".....\n";
		// Simulate time needed to produce data
		Sleep(1000);
	}

	return 0;
}
