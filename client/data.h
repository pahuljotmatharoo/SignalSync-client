#pragma once
typedef struct data_send {
	char message[128];
	int ip;
	int sock;
} data_to_send;

typedef struct data_recieve {
	char message[128];
	int sock;
} data_to_recieve;