#include <iostream>
#include <Windows.h>

#define CREATE_PROCESS_FAILURE 1000
#define SUSPEND_PROCESS_FAILURE 1001
#define RESUME_PROCESS_FAILURE 1002

using namespace std;

// Use std::filesystem to get executable path where possible
#if _MSVC_LANG >= 201703L
#define ENABLE_FS_EXT
#include <filesystem>
namespace fs = std::filesystem;
#endif

string getParentDir(string path);
void createProcess(
	string cmd, int priority, bool newwindow, bool autostart,
	PROCESS_INFORMATION* pi);
void resumeProcess(PROCESS_INFORMATION* pi);
void suspendProcess(PROCESS_INFORMATION* pi);

int main(int argc, char* argv[])
{
	PROCESS_INFORMATION pi0, pi1, pi2;
	string child_path = getParentDir(string(argv[0])) + "child_no_rt.exe";



	cout << "Creating child processes..." << endl;
	// Usage: child_no_rt.exe id count name
	createProcess(child_path + " 0 100 World",
		NORMAL_PRIORITY_CLASS,
		true,
		false,
		&pi0);

	createProcess(child_path + " 1 200 Galaxy",
		NORMAL_PRIORITY_CLASS,
		true,
		false,
		&pi1);

	createProcess(child_path + " 2 300 Universe",
		NORMAL_PRIORITY_CLASS,
		true,
		false,
		&pi2);


	cout << "Waiting 5 seconds" << endl;
	Sleep(5000);
	cout << "Starting child processes" << endl;
	resumeProcess(&pi0);
	resumeProcess(&pi1);
	resumeProcess(&pi2);
	cout << "Waiting 5 seconds" << endl;
	Sleep(5000);
	cout << "Suspending child processes" << endl;
	suspendProcess(&pi0);
	suspendProcess(&pi1);
	suspendProcess(&pi2);
	cout << "Waiting 5 seconds" << endl;
	Sleep(5000);

	cout << "Resuming child processes" << endl;
	resumeProcess(&pi0);
	resumeProcess(&pi1);
	resumeProcess(&pi2);

	cout << "Waiting for child 0 to terminate" << endl;
	WaitForSingleObject(pi0.hProcess, INFINITE);
	cout << "Waiting for child 1 to terminate" << endl;
	WaitForSingleObject(pi1.hProcess, INFINITE);
	cout << "Waiting for child 2 to terminate" << endl;
	WaitForSingleObject(pi2.hProcess, INFINITE);

	return 0;
}

/**
 * @brief Create a child process
 * @param cmd command to call
 * @param priority priority class as defined in WinBase.h
 * @param newwindow true to start process in a new window, false to use current window
 * @param autostart true to start process automatically, false to leave process suspended
 * @param pi process handle
*/
void createProcess(
	string cmd, int priority, bool newwindow, bool autostart,
	PROCESS_INFORMATION* pi)
{
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.lpTitle = (char*)cmd.c_str();

	DWORD flags = priority;
	if (newwindow)
		flags |= CREATE_NEW_CONSOLE;
	if (!autostart)
		flags |= CREATE_SUSPENDED;

	if (!CreateProcess(
		NULL,
		(char*)cmd.c_str(),
		NULL,
		NULL,
		TRUE,
		flags,
		NULL,
		NULL,
		&si,
		pi
	)) {
		cout << "Failed creating process " << cmd << endl;
		exit(CREATE_PROCESS_FAILURE);
	}
}

void suspendProcess(PROCESS_INFORMATION* pi)
{
	if (SuspendThread(pi->hThread) == -1) {
		cout << "Error suspending process with id " << pi->dwProcessId << endl;
		exit(SUSPEND_PROCESS_FAILURE);
	}
}

void resumeProcess(PROCESS_INFORMATION* pi)
{
	if (ResumeThread(pi->hThread) == -1) {
		cout << "Error resuming process with id " << pi->dwProcessId << endl;
		exit(RESUME_PROCESS_FAILURE);
	}
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
