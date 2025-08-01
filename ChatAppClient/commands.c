#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "commands.h"
#include "data.h"
#include "inital_message.h"
#include <corecrt_malloc.h>
#define MSG_SEND 1
#define MSG_LIST 2
#define MSG_EXIT 3
#define ROOM_CREATE 5
#define message_length 128
#define max_users 10
#define username_length 50

bool send_to_user(SOCKET* sock, const char* temp, const char* user_to_send_to)
{
	send_inital_msg(*sock, MSG_SEND);

	data_to_send* data = (data_to_send*) malloc(sizeof(*data));
	if (data == NULL) {
		//kill yourself
		return;
	}

	data->sock = *sock;

	strcpy_s(data->a.message, message_length, temp);

	strcpy_s(data->a.user_to_send, username_length, (user_to_send_to));

	int sent = send(data->sock, &(data->a), sizeof(data_s), 0);

	if (sent > 0) {
		printf("Sent %d Successfully! \n", sent);
	}

	free(data);
	return true;
}

bool send_chatroom_name(SOCKET* sock, const char* temp)
{
	send_inital_msg(*sock, ROOM_CREATE);
	char username[50];

	strcpy_s(username, 50, temp);

	int sent = send(*sock, username, sizeof(username), 0);
	
	return false;
}
