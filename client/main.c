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

	//char* sending = "Init";

	//int to_loop = send(sock, sending, strlen(sending), 0);

	char input[50];

	printf("Command: \n");

	//create the recieve thread
	DWORD pThreadID;
	HANDLE call_thread = create_thread(recieving, &sock, &pThreadID);
	CloseHandle(call_thread);


	while (1) {
		//input
		if (scanf_s("%49s", input, (unsigned)sizeof(input)) != 1) {
			// error or EOF
			break;
		}

		// exit loop if user typed "Exit"
		if (strcmp(input, "Exit") == 0) {
			MsgHeader* hdr = malloc(sizeof(MsgHeader));
			send_inital_msg(hdr, sock, MSG_EXIT);
			break;
		}

		//if we want to send
		if (strcmp(input, "Send") == 0) {

			data_to_send* data = malloc(sizeof(*data));
			memset(data, 0, sizeof (*data));
			data->sock = sock;

			MsgHeader* hdr = malloc(sizeof(MsgHeader));

			//i can make this a common function
			send_inital_msg(hdr, sock, MSG_SEND);

			/*printf("Enter message to send: ");
			fflush(stdout);*/

			//if (fgets(data->message, sizeof data->message, stdin)) {
			//	// strlen gives you the number of chars *before* the first '\0'
			//	size_t len = strlen(data->message);
			//	// if the last stored char is '\n', overwrite it with '\0'
			//	if (len > 0 && data->message[len - 1] == '\n') {
			//		data->message[len - 1] = '\0';
			//	}
			//}

			char temp[128] = { "hello i'm a person!" };
			//can prolly use strcpy here instead of this loop
			for (int i = 0; i < 19; i++) {
				data->a.message[i] = temp[i];
			}

			data->a.ip = 1057138880;

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