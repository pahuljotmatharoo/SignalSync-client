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

bool send_to_user(SOCKET sock)
{
	printf("Enter message to send: ");
	fflush(stdout);

	data_to_send* data = (data_to_send*) malloc(sizeof(*data));

	data->sock = sock;

	char temp[128];
	char user_to_send_to[50];
	memset(temp, '\0', sizeof(temp));

	bool send_input = get_user_input(temp, 128);
	if (!send_input) {
		printf("Too long!");
		return false;
	}

	strcpy_s(data->a.message, 128, temp);

	printf("Enter username to send to: ");
	fflush(stdout);

	bool send_user = get_user_input(user_to_send_to, 50);
	if (!send_user) {
		printf("Too long!");
		return false;
	}

	strcpy_s(data->a.user_to_send, 50, (user_to_send_to));

	int sent = send(data->sock, &(data->a), 178, 0);

	if (sent > 0) {
		printf("Sent %d Successfully! \n", sent);
	}
	free(data);
	return true;
}
