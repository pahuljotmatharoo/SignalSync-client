#ifndef INITAL_MESSAGE_H
#define INITAL_MESSAGE_H
#include <stdint.h>   /* for uint32_t */
#include <winsock2.h> /* for SOCKET */
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint32_t type;    // message type
	uint32_t length;  // payload length
} MsgHeader;

size_t recv_exact_msg(void* buf, size_t len);
size_t recv_exact_list(void* buf, size_t len);
size_t send_inital_msg(SOCKET sock, int constant);
bool get_user_input(char* buffer, size_t size);

#ifdef __cplusplus
}
#endif

#endif // INITAL_MESSAGE_H
