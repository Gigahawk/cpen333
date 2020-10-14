/*
**	Win32 code program to solve the graphical problem illustrated previously. The double Producer
**	Single Consumer arrangement. Let's assume this is both the consumer and the Parent program
*/

#include 	<stdio.h>
#include	"..\rt.h"			// change pathlist to "rt.h" to suit location on disk

struct		mydatapooldata {		// template for the data in a datapool
	int 	x;						// just a single int
};
struct		mydatapooldata2 {		// template for the data in a datapool
	float 	x;						// just a single float
};

struct		mydatapooldata* TheDataPointer;		// pointer to the actual data pool
struct		mydatapooldata* TheDataPointer2;		// pointer to the actual data pool

int 	main()
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

//
//	Now that we have created the semaphores, let's create the child process
//

	CProcess p1("dpsc_child.exe 1",	// pathlist to child program executable
		NORMAL_PRIORITY_CLASS,			// priority
		OWN_WINDOW,					// process has its own window
		ACTIVE							// process is active immediately
	);
	CProcess p2("dpsc_child.exe 2",	// pathlist to child program executable
		NORMAL_PRIORITY_CLASS,			// priority
		OWN_WINDOW,					// process has its own window
		ACTIVE							// process is active immediately
	);

	printf("Consumer Process Running.....\n");

	for(int i = 0; i < 20; i++) {
		cout << "Hit RETURN to Consume Data.....\n";
		getchar();
		while (1) {
			if (ps1.Wait(0) == WAIT_OBJECT_0) {
				cout << "Consumed " << TheDataPointer->x << "\n";
				cs1.Signal();
				break;
			}
			else if (ps2.Wait(0) == WAIT_OBJECT_0) {
				cout << "Consumed " << TheDataPointer2->x << "\n";
				cs2.Signal();
				break;
			}
		}
	}
	getchar();

	//	p1.WaitForProcess() ;		// wait for the child process to terminate if necessary
	//	p2.WaitForProcess() ;		// wait for the child process to terminate if necessary

	return 0;				// sempahores destroyed here
}