#ifndef COMMANDS_H
#define COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

	bool send_to_user(SOCKET* sock, const char* temp, const char* user_to_send_to);
	bool send_chatroom_name(SOCKET* sock, const char* temp); //this will work for both username and chatroom name

#ifdef __cplusplus
}
#endif

#endif
