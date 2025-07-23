#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <stdbool.h>
#include "inital_message.h"
#include "data.h"


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
	client_list* temp = (client_list*)buf;
	size_t total = 0;
	while (total < len) {
		size_t r = recv(temp->sock, &(temp->a) + (total), len - total, 0);
		if (r < 0)  return -1;   // error
		if (r == 0)  return 0;   // peer closed
		total += r;
	}
	return total;
}

size_t send_inital_msg(SOCKET sock, int constant) {
	MsgHeader msg;
	msg.type = htonl(constant);
	msg.length = htonl((uint32_t)5);

	size_t x = send(sock, &msg, sizeof(msg), 0);
	return x;
}

size_t recv_exact_username(SOCKET sock, char* username, size_t len) {
	size_t total = 0;
	while (total < len) {
		size_t r = recv(sock, username + (total), len - total, 0);
		if (r < 0)  return -1;   // error
		if (r == 0)  return 0;   // peer closed
		total += r;
	}
	return total;
}

bool get_user_input(char* buffer, size_t size) {
	// read up to size-1 chars + '\n'
	if (!fgets(buffer, size, stdin))
		return false;  // EOF or error

	size_t len = strlen(buffer);
	// if we got the newline, strip it
	if (len > 0 && buffer[len - 1] == '\n') {
		buffer[--len] = '\0';
	}
	// if buffer is full (no '\n') then the user typed too many chars
	else if (len == size - 1) {
		printf("Message too long!\n");
		// flush the rest of the over-long line
		int c;
		while ((c = getchar()) != '\n' && c != EOF) {}
		return false;
	}

	return true;
}

