#ifndef INITIAL_MESSAGE_H
#define INITIAL_MESSAGE_H
#include <stdint.h>   /* for uint32_t */
#include <winsock2.h> /* for SOCKET */



typedef struct {
	uint32_t type;    // message type
	uint32_t length;  // payload length
} MsgHeader;

size_t recv_exact_msg(void* buf, size_t len);
size_t recv_exact_list(void* buf, size_t len);
size_t send_inital_msg(MsgHeader* msg, SOCKET sock, int constant);

#endif // INITAL_MESSAGE_H
