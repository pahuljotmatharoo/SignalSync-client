#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string.h>
#include <stdint.h>
#include "thread_functions.h"
#include "inital_message.h"
#include "data.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")   // make sure the Winsock library is linked
#define MSG_SEND 1
#define MSG_LIST 2
#define MSG_EXIT 3
//so now we are able to send to a user using their IP, lets make this a user name now

int main() {

	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (r != 0) {
		fprintf(stderr, "WSAStartup failed: %d\n", r);
		return 1;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in server;
	server.sin_family = AF_INET; //specify to use IPV4
	server.sin_port = htons(2520); // port is 2520
#pragma warning(push)
#pragma warning(disable:4996)   // disable “this function is deprecated” warnings
	server.sin_addr.s_addr = inet_addr("192.168.2.61"); //server address
#pragma warning(pop)


	int status = connect(sock, (struct sockaddr*)&server, sizeof server);

	if (status == SOCKET_ERROR) {
		fprintf(stderr, "connect(): %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	printf("Connected!\n");
	printf("Enter a username!: ");
	char username[50];
	if (fgets(username, sizeof username, stdin)) {

		size_t len = strlen(username);

		/*if (len > 128) {
			printf("Message too long!");
		}*/
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
		if (!fgets(input, sizeof input, stdin))
			break;

		size_t n = strlen(input);
		if (n > 0 && input[n - 1] == '\n')
			input[n - 1] = '\0';

		// exit loop if user typed "Exit"
		if (strcmp(input, "Exit") == 0) {
			MsgHeader* hdr = malloc(sizeof(MsgHeader));
			send_inital_msg(hdr, sock, MSG_EXIT);
			break;
		}

		//if we want to send
		if (strcmp(input, "Send") == 0) {

			data_to_send* data = malloc(sizeof(*data));
			memset(data, 0, sizeof(*data));
			data->sock = sock;

			MsgHeader* hdr = malloc(sizeof(MsgHeader));

			//i can make this a common function
			send_inital_msg(hdr, sock, MSG_SEND);

			printf("Enter message to send: ");
			fflush(stdout);

			//char temp[128] = { "hello i'm a person!" };
			char temp[128];
			char ip[11];
			memset(temp, '\0', sizeof(temp));

			if (fgets(temp, sizeof temp, stdin)) {
				//	// strlen gives you the number of chars *before* the first '\0'
				size_t len = strlen(temp);
				//	// if the last stored char is '\n', overwrite it with '\0'
				if (len > 128) {
					printf("Message too long!");
					continue;
				}
			}
			strcpy_s(data->a.message, 128, temp);

			printf("Enter IP: ");
			fflush(stdout);

			if (fgets(ip, sizeof ip, stdin)) {
				//	// strlen gives you the number of chars *before* the first '\0'
				size_t len = strlen(ip);
				//	// if the last stored char is '\n', overwrite it with '\0'
				if (len > 12) {
					printf("Message too long!");
					continue;
				}
			}

			int i = atoi(ip);

			data->a.ip = i;

			int sent = send(data->sock, &(data->a), 132, 0);

			if (sent > 0) {
				printf("Sent %d Successfully! \n", sent);
			}
			free(data);
		}
		if (strcmp(input, "List") == 0) {

			MsgHeader* hdr = malloc(sizeof(MsgHeader));
			send_inital_msg(hdr, sock, MSG_LIST);
			printf("Sent command info Successfully! \n");
			//it jumps to the other thread now
		}
	}
	closesocket(sock);
	WSACleanup();
}