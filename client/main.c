#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string.h>
#include <stdint.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")   // make sure the Winsock library is linked
#define MSG_SEND 1
#define MSG_LIST 2
#define MSG_EXIT 3

//TODO

//issues - recieving
//type = 0 even though List is called (happens after doing Send)
//even though message has data, nothing is printed

//create seperate files and git repo for client

size_t recv_exact_msg(void* buf, size_t len);
size_t recv_exact_list(void* buf, size_t len);

//have multiple recieves which are picking up the wrong info, need to correct this

//elementary data structures
typedef struct data_r {
	char message[128];
} data_r;

typedef struct data_s {
	char message[128];
	int ip;
} data_s;

typedef struct list {
	int size;
	int arr[10];
} list;

//actual data structures to use
typedef struct data_send {
	data_s a;
	int sock;
} data_to_send;

typedef struct data_recieve {
	data_r a;
	int sock;
} data_to_recieve;

typedef struct client_list {
	list a;
	int sock;
} client_list;

typedef struct {
	uint32_t type;    // message type
	uint32_t length;  // payload length
} MsgHeader;


//it the same sort of idea as POSIX, the input is a void pointer
DWORD WINAPI sendingg(LPVOID arg) {
	data_to_send* data = (data_to_send*)arg;

	data->a.ip = 1057138880;

	int sent = send(data->sock, &(data->a), 132, 0);

	if (sent > 0) {
		printf("Sent %d Successfully! \n", sent);
	}
	free(data);
	return sent;
}

//this thread is always running parallel

//issues -
//type = 0 even though List is called (happens after doing Send)
//even though message has data, nothing is printed

DWORD WINAPI recieving(LPVOID arg) {
	int* socket = (int*) arg;
	int first_rec;
	int type = INT_MAX;
	while ((first_rec = recv(*(socket), &(type), sizeof(type), MSG_WAITALL)) > 0) {

		if (type == MSG_SEND) {
			data_to_recieve data;
			data.sock = *socket;
			size_t x = recv_exact_msg(&(data), 128);
			if (x <= 0) {
				printf("Received no data! \n");
				continue;
			}
			data.a.message[127] = '\0';
			printf("Received message: \"%s\"\n", data.a.message);
			type = INT_MAX;
			continue;
		}

		else if (type == MSG_LIST) {
			client_list temp;
			temp.a.size = 0;
			temp.sock = *socket;
			int x = recv_exact_list(&(temp), 44);
			if (x <= 0) {
				printf("Received no data! \n");
				continue;
			}
			for (int i = 0; i < temp.a.size; i++) {
				printf("%d \n", temp.a.arr[i]);
			}
			type = INT_MAX;
			continue;
		}
	}
	return NULL;
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

size_t recv_exact_msg(void* buf, size_t len) {
	data_to_recieve* temp = (data_to_recieve*)buf;
	char* p = &(temp->a.message);
	size_t total = 0;
	while (total < len) {
		size_t r = recv(temp->sock, p + total, len - total, 0);
		if (r < 0)  return -1;   // error
		if (r == 0)  return 0;   // peer closed
		total += r;
	}
	return total;
}

size_t recv_exact_list(void* buf, size_t len) {
	client_list* temp = (client_list*) buf;
	size_t total = 0;
	while (total < len) {
		size_t r = recv(temp->sock, &(temp->a) + (total), len - total, 0);
		if (r < 0)  return -1;   // error
		if (r == 0)  return 0;   // peer closed
		total += r;
	}
	return total;
}

size_t send_inital_msg(MsgHeader *msg, SOCKET sock, int constant) {
	msg->type = htonl(constant);
	msg->length = htonl((uint32_t)5);
	size_t x = send(sock, msg, sizeof (*msg), 0); // first we send the info about what we are sending
	//size_t y = send(sock, payload, payload_len, 0); // then we send the thing
	free(msg);
	return x;
}

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

	DWORD            pThreadID;
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

			char* sending = "Sending";

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

			DWORD threadId;
			HANDLE hThread = create_thread(sendingg, data, &threadId);
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
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