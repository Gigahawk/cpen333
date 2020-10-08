#include "../rt.h"
#include "../common.h"
#include <random>
#include <windows.h>

using namespace std;

int main() {
	CCondition stop("stop_p");
	CTypedPipe<pipe2_data> pipe2("p2");
	default_random_engine re;
	re.seed(987654321);
	uniform_int_distribution<> wait(0, 3000);
	uniform_int_distribution<> year(1908, 2020);
	uniform_int_distribution<> make(0, makes.size() - 1);
	uniform_int_distribution<> model(0, models.size() - 1);
	pipe2_data d;
	const pipe2_data* dp = &d;

	cout << "Infinitely sending data to pipeline p2, waiting for stop condition" << endl;

	while (1) {
		if (stop.Test())
			break;
		strcpy_s(d.make, makes[make(re)]);
		strcpy_s(d.model, models[model(re)]);
		d.year = year(re);
		printf(
			"Writing car with { make: %s, model: %s, year: %d } to pipeline\n",
			d.make, d.model, d.year);
		pipe2.Write(dp);
		Sleep(wait(re));
	}
	return 0;
}
