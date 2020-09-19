#include "rt.h"

int main()
{
	cout << "Creating Child Processes.....\n";

	//	Create process objects p1,p2 and p3. Arguments are as follows
	//	1)	Pathlist to the child process executable i.e. the runnable child program
	//	2)	The process priority. Allowable values are
	//		HIGH_PRIORITY_CLASS
	//		IDLE_PRIORITY_CLASS
	//		NORMAL_PRIORITY_CLASS	(this is the default if you do not specify).
	//		REALTIME_PRIORITY_CLASS 	(use with extreme caution)
	//	3)	Whether the child process uses its own window or uses that of the parent. Values are
	//		OWN_WINDOW or PARENT_WINDOW. Default is PARENT_WINDOW	 if not specified
	//	4)	The initial processing state of the process, Allowable values are
	//		ACTIVE or SUSPENDED. Default is ACTIVE if not specifie

	//Usage: Q1Child1.exe count name
	CProcess p1("./Q1Child1/debug/Q1Child1.exe 100 World",	// pathlist to child program executable
		NORMAL_PRIORITY_CLASS,			// priority
		OWN_WINDOW,						// process has its own window
		ACTIVE							// process is active immediately
	);

	CProcess p2("./Q1Child2/debug/Q1Child2.exe 200 Galaxy",	// pathlist to child program executable
		NORMAL_PRIORITY_CLASS,			// priority
		OWN_WINDOW,						// process has its own window
		ACTIVE							// process is active immediately
	);

	CProcess p3("./Q1Child3/debug/Q1Child3.exe 300 Universe",	// pathlist to child program executable	plus some arguments
		NORMAL_PRIORITY_CLASS,			// priority
		OWN_WINDOW,						// process has its own window
		ACTIVE
	);

	cout << "Child Processes Activated.....\n";

	cout << "Waiting For Child1 to Terminate.....\n";
	p1.WaitForProcess();

	cout << "Waiting For Child2 to Terminate.....\n";
	p2.WaitForProcess();					// wait for the child process to end

	cout << "Waiting For Child3 to Terminate.....\n";
	p3.WaitForProcess();					// wait for the child process to end

	return 0;
}