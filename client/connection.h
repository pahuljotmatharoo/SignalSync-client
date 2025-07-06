#ifndef CONNECTION_H
#define CONNECTION_H
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

int connect_to_server(struct sockaddr_in* server, SOCKET* sock);

#endif CONNECTION_H
