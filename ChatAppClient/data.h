#ifndef DATA_H
#define DATA_H
#define message_length 128
#define username_length 50
#define max_users 10
#include <stdint.h>

//function def for the linker function between c and c++
typedef void (*message_callback_t)(void* ctx, char message[message_length], char username[username_length]);
typedef void (*message_callback_t_list)(void* ctx, char users[max_users][username_length], uint32_t size);
typedef void (*message_callback_t_user)(void* ctx, char user[username_length], uint32_t size);

typedef struct data_r {
	char message[message_length];
	char username[username_length];
} data_r;

typedef struct data_s {
	char message[message_length];
	char user_to_send[username_length];
} data_s;

typedef struct list {
	uint32_t size;
	char arr[max_users][username_length];
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