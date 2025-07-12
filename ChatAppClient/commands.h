#ifndef COMMANDS_H
#define COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

	bool send_to_user(SOCKET* sock, const char* temp, const char* user_to_send_to);

#ifdef __cplusplus
}
#endif

#endif
