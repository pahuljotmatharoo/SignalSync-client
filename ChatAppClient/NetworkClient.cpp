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
	t_server.sin_addr.s_addr = inet_addr("");
#pragma warning(pop)

	int status = connect(getSockID(), reinterpret_cast<sockaddr*>(&t_server), sizeof(t_server));
	return (static_cast<int16_t>(status));
}

char* Network::recvPng(int sizePng) const
{
	char* pngData = new char[(sizePng)];
	std::size_t total = 0;

	while (total < static_cast<unsigned long long>(sizePng)) {
		std::size_t recvBytes = recv(getSockID(), pngData + total, sizePng - static_cast<size_t>(total), 0);
		total += recvBytes;
	}
	return pngData;
}

void Network::sendLargeFile(PngSend* t_data, long long size) const
{
	long long currentPointer = 0;
	while (currentPointer < t_data->size_m) {
		int sent = send(m_sockid, (t_data->data) + currentPointer, t_data->size_m - currentPointer, 0);
		currentPointer += sent;
	}

	currentPointer = 0;
	char userbuf[50];
	memset(userbuf, 0, sizeof(userbuf));
	strncpy_s(userbuf, t_data->user_to_send, sizeof(userbuf) - 1);
	while (currentPointer < 50) {
		int sent = send(m_sockid, (userbuf) + currentPointer, 50 - currentPointer, 0);
		currentPointer += sent;
	}
}

std::size_t Network::sendInitMsg(int t_constant) const
{
	MsgHeaderr msg = {0};
	msg.type = htonl(t_constant);
	msg.length = htonl(static_cast<uint32_t>(5));

	return send(getSockID(), reinterpret_cast<char*>(&msg), sizeof(msg), 0);
}

std::size_t Network::sendPngSize(long long t_pngSize) const
{
	return send(m_sockid, reinterpret_cast<const char*>(&t_pngSize), sizeof(long long), 0); // QSize = 8 bytes
}

void Network::sendPng(QByteArray* t_pngData, std::string user_to_send) const
{
	const char* pngData = t_pngData->constData();
	PngSend png{ 0 };
	png.user_to_send = const_cast<char*>(user_to_send.c_str());
	png.size_m = static_cast<uint32_t>(t_pngData->size());
	png.size_u = 50;
	png.data = const_cast<char*>(pngData);
	sendInitMsg(PNG_IMG);
	sendPngSize(static_cast<long long>(png.size_m));
	sendLargeFile(&png, static_cast<long long>(png.size_u) + png.size_m);
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

