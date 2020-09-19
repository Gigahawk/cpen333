#include "../rt.h"
#include <iostream>

using namespace std;

// Use std::filesystem to get executable path where possible
#if _MSVC_LANG >= 201703L
#define ENABLE_FS_EXT
#include <filesystem>
namespace fs = std::filesystem;
#endif

string getParentDir(string path);

int main(int argc, char* argv[])
{
	string child_path = getParentDir(string(argv[0])) + "child_rt.exe";
	cout << "Creating child processes..." << endl;
	// Usage: child_rt.exe id count name
	CProcess p0((child_path + " 0 100 World").c_str(),
		NORMAL_PRIORITY_CLASS,
		OWN_WINDOW,
		SUSPENDED);

	CProcess p1((child_path + " 1 200 Galaxy").c_str(),
		NORMAL_PRIORITY_CLASS,
		OWN_WINDOW,
		SUSPENDED);

	CProcess p2((child_path + " 2 300 Universe").c_str(),
		NORMAL_PRIORITY_CLASS,
		OWN_WINDOW,
		SUSPENDED);

	cout << "Waiting 5 seconds" << endl;
	Sleep(5000);
	cout << "Starting child processes" << endl;
	p0.Resume();
	p1.Resume();
	p2.Resume();
	cout << "Waiting 5 seconds" << endl;
	Sleep(5000);
	cout << "Suspending child processes" << endl;
	p0.Suspend();
	p1.Suspend();
	p2.Suspend();
	cout << "Waiting 5 seconds" << endl;
	Sleep(5000);

	cout << "Resuming child processes" << endl;
	p0.Resume();
	p1.Resume();
	p2.Resume();

	cout << "Waiting for child 0 to terminate" << endl;
	p0.WaitForProcess();
	cout << "Waiting for child 1 to terminate" << endl;
	p1.WaitForProcess();
	cout << "Waiting for child 2 to terminate" << endl;
	p2.WaitForProcess();

	return 0;
}

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
