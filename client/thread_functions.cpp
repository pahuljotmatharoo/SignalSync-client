#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string.h>
#include "data.h"
#include "thread_functions.h"

DWORD WINAPI sendingg(LPVOID arg) {
	data_to_send* data = (data_to_send*)arg;
	//this is to let the server know that we are doing a send to it
	const char* sending = "Sending";
	int sentt = send(data->sock, sending, strlen(sending), 0);
	printf("Sent command info Successfully! \n");

	//if (fgets(data->message, sizeof(data->message), stdin)) {
	//	// strip trailing newline, if present
	//	size_t len = strlen(data->message);
	//	if (len > 0 && data->message[len - 1] == '\n') {
	//		data->message[len - 1] = '\0';
	//	}
	//}
	char arr[6] = "hello";
	for (int i = 0; i < 6; i++) {
		data->message[i] = arr[i];
	}

	data->ip = 1057138880;

	int sent = send(data->sock, (const char*)data, 136, 0);

	if (sent > 0) {
		printf("Sent %d Successfully! \n", sent);
	}
	free(data);
	return sentt;
}

DWORD WINAPI recieving(LPVOID arg) {
	data_to_recieve* data = (data_to_recieve*)arg;
	int n = 0;
	while ((n = recv(data->sock, data->message, sizeof(data->message) - 1, 0)) > 0) {
		data->message[n] = '\0';
		printf("Received: %s\n", data->message);
		if (n == SOCKET_ERROR) {
			fprintf(stderr, "recv error: %d\n", WSAGetLastError());
		}
	}
	free(arg);
	return n;
}

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