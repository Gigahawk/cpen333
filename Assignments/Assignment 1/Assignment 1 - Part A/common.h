#pragma once
#include <iostream>

using namespace std;

// Use std::filesystem to get executable path where possible
#if _MSVC_LANG >= 201703L
#define ENABLE_FS_EXT
#include <filesystem>
namespace fs = std::filesystem;
#endif

/**
 * @brief get the parent dir of a path

 * @param path path to get parent dir of
*/
string getParentDir(string path) {
#ifdef ENABLE_FS_EXT
	fs::path p(path);
	return p.remove_filename().string();
#else
	// Manual string manipulation to get parent dir if
	// std::filesystem is not available
	size_t pos, lpos = 0;
	string s;
	s += path;
	// Convert to DOS path separators for simplicity
	while ((pos = s.find("/")) != string::npos)
		s.replace(pos, 1, "\\");
	pos = 0;

	// Find last backslash
	pos = s.find_first_of("\\");
	while (pos != string::npos) {
		lpos = pos + 1;
		pos = s.find_first_of("\\", lpos);
	}
	return s.substr(0, lpos);
#endif
}
