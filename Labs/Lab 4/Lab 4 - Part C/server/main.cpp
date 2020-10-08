#include <sstream>
#include <iterator>
#include <vector>
#include <iostream>
#include "../common.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define NUM_WIDGETS 42

using namespace std;

int handle_command(char* in, char* out);
int widgets[NUM_WIDGETS];
string parse_and_invoke(string cmd);
bool kill;

int main() {
	kill = false;
	WSADATA wsa_data;
	int ret;
	SOCKET listen_socket = INVALID_SOCKET, client_socket;
	struct addrinfo* result = NULL, * ptr = NULL, hints;
	char recvbuf[BUF_LEN] = { 0 };
	char sendbuf[BUF_LEN] = { 0 };
	char* mark = recvbuf;

	// Initialize Winsock
	ret = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (ret != 0) {
		printf("WSAStartup failed: %d\n", ret);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	ret = getaddrinfo(NULL, PORT, &hints, &result);
	if (ret != 0) {
		printf("getaddrinfo failed: %d\n", ret);
		WSACleanup();
		return 1;
	}
	listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (listen_socket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	ret = bind(listen_socket, result->ai_addr, (int)result->ai_addrlen);
	if (ret == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(listen_socket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(listen_socket);
		WSACleanup();
		return 1;
	}

	cout << "Waiting for client to connect..." << endl;
	// Accept a client socket
	client_socket = accept(listen_socket, NULL, NULL);
	if (client_socket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(listen_socket);
		WSACleanup();
		return 1;
	}
	cout << "Client connected!" << endl;

	while (1) {
		ret = recv(client_socket, mark, BUF_LEN - (mark - recvbuf), 0);
		if (ret > 0) {
			// recv doesn't seem to write null byte, ensure string has proper ending
			mark[ret] = 0;
			printf("Bytes received: %d\n", ret);
			mark += ret;
		}
		else if (ret < 0) {
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(client_socket);
			WSACleanup();
			return 1;
		}
		mark -= handle_command(recvbuf, sendbuf);
		if (strlen(sendbuf)) {
			ret = send(client_socket, sendbuf, strlen(sendbuf), 0);
			if (ret == SOCKET_ERROR) {
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(client_socket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", ret);
		}
		if (kill) {
			printf("Goodbye\n");
			exit(0);
		}
	}
	return 0;
}

int handle_command(char* in, char* out) {
	string s_in = string(in);
	size_t end = s_in.find_first_of("\n");
	if (end == string::npos) {
		out[0] = 0;
		return 0;
	}
	string cmd = s_in.substr(0, end);
	string result;
	string new_in = s_in.substr(end + 1, s_in.size() - cmd.size() - 1);

	printf("Recieved cmd: %s\n", cmd.c_str());
	printf("cmd size: %d\n", cmd.size());
	result = parse_and_invoke(cmd);
	sprintf_s(out, BUF_LEN, "\n%s\n%s", result.c_str(), PROMPT);
	printf("Responding with: %s\n", out);
	strcpy_s(in, BUF_LEN, new_in.c_str());
	return cmd.size() + 1;
}

string parse_and_invoke(string cmd) {
	istringstream iss(cmd);
	vector<string> results(istream_iterator<string>{iss},
		istream_iterator<string>());
	int idx;
	int val;
	if (!results.size()) {
		return string("");
	}
	else if (results[0] == string("num_widgets")) {
		return to_string(NUM_WIDGETS);
	}
	else if (results[0] == string("get_widget")) {
		if (results.size() < 2) {
			return string("ERROR: get_widget requires 1 argument");
		}
		try {
			idx = stoi(results[1]);
		}
		catch (...) {
			return string("ERROR: could not parse idx");
		}
		if(idx >= NUM_WIDGETS) {
			return string("ERROR: widget idx out of range");
		}
		return to_string(widgets[idx]);
	}
	else if (results[0] == string("set_widget")) {
		if (results.size() < 3) {
			return string("ERROR: set_widget requires 2 arguments");
		}
		try {
			idx = stoi(results[1]);
		}
		catch (...) {
			return string("ERROR: could not parse idx");
		}
		try {
			val = stoi(results[2]);
		}
		catch (...) {
			return string("ERROR: could not parse val");
		}
		if(idx >= NUM_WIDGETS) {
			return string("ERROR: widget idx out of range");
		}
		widgets[idx] = val;
		return string("complete");
	}
	else if (results[0] == string("kill")) {
		kill = true;
		return string("Killing server...");
	}
	else {
		return string("ERROR: cmd not recognized");
	}

}

