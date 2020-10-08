#include 	"..\rt.h"		// change pathlist to "rt.h" file to suit location on disk

// Let's suppose the data we want to put in the datapool is complex, i.e not just a simple
// int or char. The easiest way to store complex data is to introduce a structure template
// describing the names and types of the data we wich to group together as shown below
//  which includes three variables, two simple integers and an array of 10 integers
  
struct 	    mydatapooldata 	{		// start of structure template
		int floor ;				// floor corresponding to lifts current position
		int direction ;			// direction of travel of lift
		int floors[10] ;			// an array representing the floors and whether requests are set 
		string name;
		int age;
		string address;
} ;							// end of structure template

int 	main()
{
//	Start by making a datapool called 'Car1'. This is actual name of the global datapool
//	we are creating, which can then be shared by all other processes in the system
//	Note that we do not have to worry about which of the many processes using the datapool will
//	actually make it, because when a request is made to make a datapool, the operating system 
//	checks to see if it exists or not. If not, then the pool will be made, otherwise the process
//	will simply 'use' the pool if it finds it already exists.
//
//	Note we also have to specify the size of the datapool we are making and because we want it
//	to hold the data in the structure template above, we make it that big

	cout << "Parent attempting to create/use the datapool.....\n";
	CDataPool 		dp("Car1", sizeof(struct mydatapooldata)) ;
	
// In order to access the data pool, we need a pointer to its location in memory. This is what
// the LinkDataPool() primitive does. It gives us back a pointer to the data pool referred to by 'dp'.
// Note that because the operating system doesn't know the purpose of our datapool, i.e. what data
// we are going to put in it, we have to 'cast' or 'convert' the actual pointer returned into a pointer
// to the data we want to store inside the datapool, i.e. convert it to a pointer to the structure
// mydatapooldata. Hence the pointer MyDataPool below is a pointer to a structure that we
// want to map onto the datapool
	
	
//	struct mydatapooldata 	 *MyDataPool = (struct mydatapooldata *)(0x1000) ;    // just to show an error

	struct mydatapooldata 	 *MyDataPool = (struct mydatapooldata *)(dp.LinkDataPool()) ;

	printf("Parent linked to datapool at address %p.....\n", MyDataPool) ;

// Now that we have the pointer to the datapool, we can put data into it

	cout << "Parent Writing value '10' to floor variable.....\n";
	MyDataPool->floor = 10 ;		// store 55 into the variable 'floor' in the datapool

	cout << "Parent Writing value '1' to direction variable.....\n";
	MyDataPool->direction = 1 ;		// store 1 into the variable 'direction' in the datapool

	cout << "Parent Writing value '0 1 2 3 4 5 6 7 8 9' to floors array.....\n";
	for(int i = 0; i < 10; i ++)
		MyDataPool->floors[ i ] = i ;	

	cout << "Parent Writing value 'Carl' to name string.....\n";
	MyDataPool->name = string("Carl");

	cout << "Parent Writing value '21' to age integer.....\n";
	MyDataPool->age = 21;

	cout << "Parent Writing value 'The Moon' to address string.....\n";
	MyDataPool->address = string("Carl");

//	Now that we have created the data pool and have stored data in it, it is safe to create
//	a child process that can access the data

	getchar() ;

	CProcess p1("Q4.exe",	// pathlist to child program executable
		NORMAL_PRIORITY_CLASS,			// priority
		OWN_WINDOW,						// process has its own window
		ACTIVE							// process is active immediately
	) ;

	p1.WaitForProcess() ;		// wait for the child process to Terminate

	return 0 ;				// CDataPool object 'dp' destroyed here
}