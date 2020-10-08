//	A Win32 C program to demonstrate the creation and use of named Pipes. Error checking has 
//	been ommitted to aid clarity

#include	"..\rt.h"			// change the pathlist to rt.h to suit location on your disk

struct		example {		// structure template for some data that we intend to write to the pipeline
	int 	x ;
	float 	y ;
} ;

// Some data to be written in to the pipeline. Note how arrays are explicitly sized

int 	i = 5 ;									// a simple int
int 	array1[ 10 ] = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 0 } ; 		// array of 10 integers

char	name[15 ] = "Hello World" ;					// a string of 15 characters
struct	 example mystruct = {2, 5.5 } ;				// a structure with an int and a float in it

int	main()
{
	cout << "Parent Process Creating the Pipeline.....\n";
	CPipe	pipe1("MyPipe", 1024) ;							// Create a pipe 'p1' with the name "MyPipe"
	CPipe pipe2("MyOtherPipe", 1024);

	cout << "Parent Process Creating Child Process to Read from Pipeline.....\n";

	CProcess p1("Q7Child.exe MyPipe",	// pathlist to child program executable				
			NORMAL_PRIORITY_CLASS,			// priority
			OWN_WINDOW,						// process has its own window					
			ACTIVE							// process is active immediately
	) ;

	CProcess p2("Q7Child.exe MyOtherPipe",	// pathlist to child program executable				
			NORMAL_PRIORITY_CLASS,			// priority
			OWN_WINDOW,						// process has its own window					
			ACTIVE							// process is active immediately
	) ;


	cout << "Hit RETURN to Write the integer " << i << " to the pipeline.....\n";
	getchar() ;

	pipe1.Write(&i, sizeof(i)) ;						// write the int 'i' to the pipe
	pipe2.Write(&i, sizeof(i)) ;						// write the int 'i' to the pipe

	cout << "Hit RETURN to Write the integer array 1 2 3 4 5 6 7 8 9 0 to the pipeline.....\n";
	getchar() ;

	pipe1.Write(&array1[0], sizeof(array1)) ;			// write the array of integers' to the pipe
	pipe2.Write(&array1[0], sizeof(array1)) ;			// write the array of integers' to the pipe
	
	cout << "Hit RETURN to Write the string " << name << " to the pipeline.....\n";
	getchar() ;

	pipe1.Write(&name[0], sizeof(name)) ;				// write the string to the pipe
	pipe2.Write(&name[0], sizeof(name)) ;				// write the string to the pipe
	
	cout << "Hit RETURN to Write the structure [" << mystruct.x << ", " << mystruct.y << "] to the pipeline.....\n";
	getchar() ;

	pipe1.Write(&mystruct, sizeof(mystruct)) ;			// write the structure to the pipeline
	pipe2.Write(&mystruct, sizeof(mystruct)) ;			// write the structure to the pipeline
		

	p1.WaitForProcess() ;
	p2.WaitForProcess() ;

	return 0 ;
}	
