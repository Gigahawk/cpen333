#ifndef __STRUCT_H__
#define __STRUCT_H__
#include <iostream>
#include <vector>
#define STR_LEN 64

using namespace std;

vector<const char*> usernames{ "Bob", "Carl", "Tom" };
vector<const char*> positions{ "Engineer", "Sales", "HR", "Accountant" };
vector<const char*> makes{ "Toyota", "Ford", "Dodge" };
vector<const char*> models{ "Camry", "Charger", "Model T", "Corolla" };

struct pipe1_data {
	char username[STR_LEN];
	char position[STR_LEN];
	uint32_t id;
	uint32_t permissions;
};

struct pipe2_data {
	char make[STR_LEN];
	char model[STR_LEN];
	uint32_t year;
};

#endif