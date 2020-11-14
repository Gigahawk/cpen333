#include "common.h"

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
