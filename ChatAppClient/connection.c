#include "connection.h"

int connect_to_server(struct sockaddr_in* server, SOCKET* sock)
{
	server->sin_family = AF_INET; //specify to use IPV4
	server->sin_port = htons(PORT); // port is 2520
#pragma warning(push)
#pragma warning(disable:4996)   // disable “this function is deprecated” warnings
	server->sin_addr.s_addr = inet_addr("SERVER IP"); //server address
#pragma warning(pop)

	int status = connect(*sock, server, (int)sizeof * server);
	return status;
}
