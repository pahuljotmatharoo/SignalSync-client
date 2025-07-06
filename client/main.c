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

int main() {

	//just initalize winsockets
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (r != 0) {
		fprintf(stderr, "WSAStartup failed: %d\n", r);
		return 1;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in server;

	int status = connect_to_server(&server, &sock);

	if (status == SOCKET_ERROR) {
		fprintf(stderr, "connect(): %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	printf("Connected!\n");
	printf("Enter a username!: ");
	char username[50];

	//get user input for username
	if (!get_user_input((username), 50)) {
		return 0;
	}

	//send the username to the server
	send(sock, &username, sizeof username, 0);

	char input[50];

	//create the recieve thread
	DWORD pThreadID;
	HANDLE call_thread = create_thread(recieving, &sock, &pThreadID);
	CloseHandle(call_thread);

	while (1) {
		//input
		printf("Command: ");
		fflush(stdout);

		if (!(get_user_input((input), 50)))
			break;

		// exit loop if user typed "Exit"
		if (strcmp(input, "Exit") == 0) {
			MsgHeader* hdr = malloc(sizeof(MsgHeader));
			send_inital_msg(hdr, sock, MSG_EXIT);
			break;
		}

		//if we want to send
		if (strcmp(input, "Send") == 0) {

			MsgHeader* hdr = malloc(sizeof(MsgHeader));

			send_inital_msg(hdr, sock, MSG_SEND);

			if (!send_to_user(sock)) 
				continue;
			
		}
		if (strcmp(input, "List") == 0) {
			MsgHeader* hdr = malloc(sizeof(MsgHeader));
			send_inital_msg(hdr, sock, MSG_LIST);
			printf("Sent command info Successfully! \n");
			//it jumps to the other thread now
		}
	}
	closesocket(sock);
	//clean up winsockets
	WSACleanup();
}