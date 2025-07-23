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
#define USER_EXIT 4

//need to remove the magic numbers

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
	RecvParams* funct_arg = (RecvParams*) arg;
	int first_rec;
	int type = 0;
	while ((first_rec = recv(*(funct_arg->sock), &(type), sizeof(type), 0)) > 0) {

		if (type == MSG_SEND) {
			data_to_recieve data;
			data.sock = *(funct_arg->sock);
			size_t x = recv_exact_msg(&(data), sizeof (data_r));
			if (x <= 0) {
				printf("Received no data! \n");
				continue;
			}
			data.a.message[127] = '\0';
			funct_arg->on_message(funct_arg->window_ptr, data.a.message, data.a.username);
			type = INT_MAX;
		}

		else if (type == MSG_LIST) {
			client_list data;
			data.sock = *(funct_arg->sock);
			int x = recv_exact_list(&(data), 504);
			if (x <= 0) {
				printf("Received no data! \n");
				continue;
			}
			data.a.size = ntohl(data.a.size);
			funct_arg->on_list(funct_arg->window_ptr, data.a.arr, data.a.size);
			type = INT_MAX;
		}
		else if (type == USER_EXIT) {
			char username[50];
			int x = recv_exact_username(*(funct_arg->sock), username, 50);
			funct_arg->on_user(funct_arg->window_ptr, username, 50);
		}
	}
	free(arg);
	return NULL;
}
