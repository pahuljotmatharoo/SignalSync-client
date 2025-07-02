#ifndef DATA_H
#define DATA_H
#include <stdint.h>

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

#endif