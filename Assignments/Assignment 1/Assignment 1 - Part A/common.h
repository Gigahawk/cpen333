#pragma once
#include <iostream>

using namespace std;

// Use std::filesystem to get executable path where possible
#if _MSVC_LANG >= 201703L
#define ENABLE_FS_EXT
#include <filesystem>
namespace fs = std::filesystem;
#endif

#define ELEVATOR_WIDGET_WIDTH 20
#define ELEVATOR_DOOR_WIDTH 7
#define ELEVATOR_DOOR_HEIGHT 7
#define BUF_LEN 1024

/**
 * @brief get the parent dir of a path

 * @param path path to get parent dir of
*/
string getParentDir(string path);
