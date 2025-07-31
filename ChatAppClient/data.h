#ifndef DATA_H
#define DATA_H
#define messageLength 128
#define usernameLength 50
#define maxUsers 10
#include <stdint.h>

//function def for the linker function between c and c++
typedef void (*message_callback_t)(void* ctx, char message[messageLength], char username[usernameLength]);
typedef void (*message_callback_t_list)(void* ctx, char users[maxUsers][usernameLength], uint32_t size);
typedef void (*message_callback_t_user)(void* ctx, char user[usernameLength], uint32_t size);

typedef struct data_r {
	char message[messageLength];
	char username[usernameLength];
} data_r;

typedef struct data_s {
	char message[messageLength];
	char user_to_send[usernameLength];
} data_s;

typedef struct list {
	uint32_t size;
	char arr[maxUsers][usernameLength];
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

typedef struct {
	SOCKET *sock;
	void* window_ptr;
	message_callback_t on_message;
	message_callback_t_list on_list;
	message_callback_t_user on_user;
} RecvParams;

#endif