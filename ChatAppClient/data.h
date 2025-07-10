#ifndef DATA_H
#define DATA_H
#include <stdint.h>

typedef struct data_r {
	char message[128];
} data_r;

typedef struct data_s {
	char message[128];
	char user_to_send[50];
} data_s;

typedef struct list {
	uint32_t size;
	char arr[10][50];
} list;

//actual data structures to use
typedef struct data_send {
	data_s a;
	SOCKET sock;
} data_to_send;

typedef struct data_recieve {
	data_r a;
	SOCKET sock;
} data_to_recieve;

typedef struct client_list {
	list a;
	SOCKET sock;
} client_list;

#endif