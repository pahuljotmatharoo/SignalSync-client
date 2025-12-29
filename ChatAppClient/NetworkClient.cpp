#include <NetworkClient.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <QMessageBox>

Network::~Network() {
	closesocket(m_sockid);
	WSACleanup();
}

int Network::serverConnect(std::string t_username)
{
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (r != 0) {
		fprintf(stderr, "WSAStartup failed: %d\n", r);
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	setSockID(sock);

	int16_t status = serverConnectHelper(2520);

	if (status == SOCKET_ERROR) {
		int error = WSAGetLastError();
		fprintf(stderr, "connect(): %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return status;
	}

	setWsaData(wsaData);

	send(sock, t_username.c_str(), USERNAME_LENGTH, 0);

	return 0;
}

int16_t Network::serverConnectHelper(const uint16_t t_port)
{
	sockaddr_in t_server{};
	t_server.sin_family = AF_INET; //specify to use IPV4
	t_server.sin_port = htons(t_port);
#pragma warning(push)
#pragma warning(disable:4996)   // disable this function is deprecated warnings
	t_server.sin_addr.s_addr = inet_addr("74.12.132.67");
#pragma warning(pop)

	int status = connect(getSockID(), reinterpret_cast<sockaddr*>(&t_server), sizeof(t_server));
	return (static_cast<int16_t>(status));
}

std::size_t Network::sendInitMsg(int t_constant)
{
	MsgHeaderr msg = {0};
	msg.type = htonl(t_constant);
	msg.length = htonl(static_cast<uint32_t>(5));

	return send(getSockID(), reinterpret_cast<char*>(&msg), sizeof(msg), 0);
}

char* Network::recvUser()
{
	char* username = new char[USERNAME_LENGTH];
	std::size_t total = 0;

	while (total < static_cast<unsigned long long>(USERNAME_LENGTH)) {
		std::size_t recvBytes = recv(getSockID(), username + total, USERNAME_LENGTH - static_cast<size_t>(total), 0);
		total += recvBytes;
	}

	return username;
}

std::size_t Network::sendMsg(std::string t_message, std::string t_username, int constant)
{
	sendInitMsg(constant);

	MsgSend message_to_Send{0};

	memcpy(message_to_Send.message, t_message.c_str(), 128);
	memcpy(message_to_Send.user_to_send, t_username.c_str(), 50);
	message_to_Send.size_u = htonl(t_username.length());
	message_to_Send.size_m = htonl(t_message.length());

	int sent = send(getSockID(), reinterpret_cast<char*>(&message_to_Send), sizeof(MsgSend), 0);

	return static_cast<std::size_t>(sent);
}


std::size_t Network::sendGroupName(std::string t_group_name)
{
	sendInitMsg(ROOM_CREATE);

	char grpName[USERNAME_LENGTH];
	memcpy(grpName, t_group_name.c_str(), USERNAME_LENGTH);

	return send(getSockID(), grpName, USERNAME_LENGTH, 0);
}

