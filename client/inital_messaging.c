#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
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

size_t send_inital_msg(MsgHeader* msg, SOCKET sock, int constant) {
	msg->type = htonl(constant);
	msg->length = htonl((uint32_t)5);
	size_t x = send(sock, msg, sizeof(*msg), 0); // first we send the info about what we are sending
	//size_t y = send(sock, payload, payload_len, 0); // then we send the thing
	free(msg);
	return x;
}