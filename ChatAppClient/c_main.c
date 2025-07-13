#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "thread_functions.h"
#include "inital_message.h"
#include "data.h"
#include "commands.h"
#include "connection.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")   // make sure the Winsock library is linked
#define MSG_SEND 1
#define MSG_LIST 2
#define MSG_EXIT 3

SOCKET main_connect(const char* user) {

	//just initalize winsockets
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (r != 0) {
		fprintf(stderr, "WSAStartup failed: %d\n", r);
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in server;

	int status = connect_to_server(&server, &sock);

	if (status == SOCKET_ERROR) {
		fprintf(stderr, "connect(): %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return status;
	}

	printf("Connected!\n");
	printf("Enter a username!: ");
	char username[50];

	strcpy_s(username, 50, user);

	//send the username to the server
	send(sock, username, sizeof username, 0);

	return sock;
}