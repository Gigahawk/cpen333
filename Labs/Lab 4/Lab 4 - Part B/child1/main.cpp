#include "../rt.h"
#include "../common.h"
#include <random>
#include <windows.h>

using namespace std;


int main() {
	CCondition stop("stop_p");
	CTypedPipe<pipe1_data> pipe1("p1");
	default_random_engine re;
	re.seed(1234567890);
	uniform_int_distribution<> wait(0, 3000);
	uniform_int_distribution<> id(0, 99999);
	uniform_int_distribution<> perms(0, 0xFFFF);
	uniform_int_distribution<> pos(0, positions.size() - 1);
	uniform_int_distribution<> name(0, usernames.size() - 1);
	pipe1_data d;
	const pipe1_data* dp = &d;

	cout << "Infinitely sending data to pipeline p1, waiting for stop condition" << endl;

	while (1) {
		if (stop.Test())
			break;
		d.id = id(re);
		strcpy_s(d.username, usernames[name(re)]);
		strcpy_s(d.position, positions[pos(re)]);
		d.permissions = perms(re);
		printf(
			"Writing employee with { id: %d, username: %s, position: %s, permissions: %04X } to pipeline\n",
			d.id, d.username, d.position, d.permissions);
		pipe1.Write(dp);
		Sleep(wait(re));
	}
	return 0;
}
