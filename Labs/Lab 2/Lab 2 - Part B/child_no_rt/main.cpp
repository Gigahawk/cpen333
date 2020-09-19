#include <iostream>
#include <windows.h>

using namespace std;

int main(int argc, char* argv[])
{
	int id = 0;
	int count = 200;
	const char* default_name = "World";
	char* name;
	if (argc >= 2)
		id = atoi(argv[1]);
	if (argc >= 3)
		count = atoi(argv[2]);
	if (argc >= 4)
		name = argv[3];
	else
		name = (char*)default_name;
	for (int i = 0; i < count; i++) {
		cout << "[CHILD:" << id << "] Hello " << name << " " << i <<"\n";
		Sleep(50);
	}
	return 0;
}
