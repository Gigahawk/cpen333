#include <iostream>
#include "../common.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#include <tchar.h>
#include <windows.h>

#include "../rt.h"

#pragma comment(lib, "Ws2_32.lib")


using namespace std;

int send_cmd(SOCKET s, const char* cmd, char* res);

int main() {
	WSADATA wsa_data;
	int ret;
	SOCKET connect_socket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	char recvbuf[BUF_LEN] = { 0 };
	char sendbuf[BUF_LEN] = { 0 };
	int num_widgets;
	CProcess server(
		"server.exe",
		NORMAL_PRIORITY_CLASS,
		OWN_WINDOW,
		ACTIVE);

	// Initialize Winsock
	ret = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (ret != 0) {
		printf("WSAStartup failed: %d\n", ret);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	ret = getaddrinfo("127.0.0.1", PORT, &hints, &result);
	if (ret != 0) {
		printf("getaddrinfo failed: %d\n", ret);
		WSACleanup();
		return 1;
	}
	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr = result;

	// Create a SOCKET for connecting to server
	connect_socket = socket(
		ptr->ai_family, ptr->ai_socktype,ptr->ai_protocol);

	if (connect_socket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Connect to server.
	ret = connect(connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (ret == SOCKET_ERROR) {
		closesocket(connect_socket);
		connect_socket = INVALID_SOCKET;
	}
	freeaddrinfo(result);
	if (connect_socket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	cout << "Checking number of widgets on server..." << endl;
	send_cmd(connect_socket, "num_widgets", recvbuf);
	num_widgets = atoi(recvbuf);
	printf("Server has %d widgets\n", num_widgets);

	cout << "Checking value of widgets on server..." << endl;
	for (int i = 0; i < num_widgets; i++) {
		sprintf_s(sendbuf, BUF_LEN, "get_widget %d", i);
		send_cmd(connect_socket, (const char*)sendbuf, recvbuf);
		printf("Widget %d has value %d\n", i, atoi(recvbuf));
	}
	cout << "Setting values of widgets to idx^2..." << endl;
	for (int i = 0; i < num_widgets; i++) {
		sprintf_s(sendbuf, BUF_LEN, "set_widget %d %d", i, i*i);
		send_cmd(connect_socket, (const char*)sendbuf, recvbuf);
	}
	cout << "Checking value of widgets on server..." << endl;
	for (int i = 0; i < num_widgets; i++) {
		sprintf_s(sendbuf, BUF_LEN, "get_widget %d", i);
		send_cmd(connect_socket, (const char*)sendbuf, recvbuf);
		printf("Widget %d has value %d\n", i, atoi(recvbuf));
	}
	cout << "Killing server..." << endl;
	send_cmd(connect_socket, "kill", recvbuf);

	cout << "Waiting for server to die..." << endl;
	server.WaitForProcess();
	return 0;
}

// Format the response string to contain just the latest result
// res should always be guaranteed to end with PROMPT
void format_res(char* res) {
	// Strip prompt and trailing newline
	res[strlen(res) - (strlen(PROMPT) + 1)] = 0;

	// Search for last occurence of newline (indicates response from server)
	const char* last = strrchr(res, '\n');

	// No result, empty result buffer and return
	if (last == nullptr) {
		res[0] = 0;
	} else {
		// Move result to beginning of string
		strcpy_s(res, strlen(last - 1), last + 1);
	}
}

void get_prompt(SOCKET s, char* res, uint32_t retries=10) {
	int ret;
	char* mark = res;

	for(uint32_t i = 0; i < retries; i++) {
		ret = send(s, "\n", 1, 0);
		if (ret == SOCKET_ERROR) {
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(s);
			WSACleanup();
			exit(1);
		}
		if (ret != 1) {
			printf("error: sent %d bytes, expected %d\n", ret, 1);
			closesocket(s);
			WSACleanup();
			exit(1);
		}
		ret = recv(s, mark, BUF_LEN - (res - mark), 0);
		if (strcmp(mark, PROMPT)) {
			// recv doesn't seem to write null byte, ensure string ends with the prompt
			mark[ret] = 0;
			format_res(res);
			return;
		}
		mark += ret;
	}
	printf("error: did not get prompt\n");
	closesocket(s);
	WSACleanup();
	exit(1);
}

int send_cmd(SOCKET s, const char* cmd, char* res) {
	int ret;

	printf("Sending command: %s\n", cmd);
	get_prompt(s, res);

	// Send an initial buffer
	ret = send(s, cmd, (int)strlen(cmd), 0);
	if (ret == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		exit(1);
	}
	if (ret != strlen(cmd)) {
		printf("error: sent %d bytes, expected %d\n", ret, strlen(cmd));
		closesocket(s);
		WSACleanup();
		exit(2);
	}
	get_prompt(s, res);

	printf("Response recieved: %s\n", res);
	return ret;
}
