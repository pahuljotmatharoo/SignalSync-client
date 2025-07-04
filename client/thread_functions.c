#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string.h>
#include "data.h"
#include "thread_functions.h"
#include "inital_message.h"
#define MSG_SEND 1
#define MSG_LIST 2
#define MSG_EXIT 3

//use the HANDLE return value to close the thread
// Returns the new thread handle, and writes its ID into *pThreadID.
HANDLE create_thread(LPTHREAD_START_ROUTINE func,
	LPVOID             param,
	LPDWORD            pThreadID)
{
	return CreateThread(
		NULL,      // default security
		0,         // default stack
		func,      // start function
		param,     // argument to it
		0,         // run immediately
		pThreadID  // caller’s DWORD*
	);
}

DWORD WINAPI recieving(LPVOID arg) {
	int* socket = (int*)arg;
	int first_rec;
	int type = 0;
	while ((first_rec = recv(*(socket), &(type), sizeof(type), 0)) > 0) {

		if (type == MSG_SEND) {
			data_to_recieve data;
			data.sock = *socket;
			size_t x = recv_exact_msg(&(data), 128);
			if (x <= 0) {
				printf("Received no data! \n");
				continue;
			}
			data.a.message[127] = '\0';
			printf("Received message: %s \n", data.a.message);
			type = INT_MAX;
			continue;
		}

		else if (type == MSG_LIST) {
			client_list temp;
			temp.a.size = 0;
			temp.sock = *socket;
			int x = recv_exact_list(&(temp), 504);
			if (x <= 0) {
				printf("Received no data! \n");
				continue;
			}
			for (int i = 0; i < temp.a.size; i++) {
				printf("%s \n", temp.a.arr[i]);
			}
			type = INT_MAX;
			continue;
		}
	}
	return NULL;
}
