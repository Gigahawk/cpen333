#include "../rt.h"

int main(int argc, char* argv[])
{
	int count = 200;
	const char* default_name = "World";
	char* name;
	if (argc >= 2)
		count = atoi(argv[1]);
	if (argc >= 3)
		name = argv[2];
	else
		name = (char*)default_name;

	for(int i = 0; i < count; i++) {
		cout << "[CHILD:2] Hello " << name << " " << i << endl;
		Sleep(50);
	}

	return 0 ;		// exit child program by returning status value 0
				// Note we could also call exit(0) to achieve the same thing
}