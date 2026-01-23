#include <NetworkClient.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <QMessageBox>

Network::~Network() {
	closesocket(m_sockid);
	WSACleanup();
}

int Network::serverConnect(std::string t_username) {
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

int16_t Network::serverConnectHelper(const uint16_t t_port) {
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

char* Network::recvFile(const size_t t_sizeFile) const {
	char* pngData = new char[t_sizeFile];
	std::size_t total = 0;

	while (total < t_sizeFile) {
		std::size_t recvBytes = recv(getSockID(), pngData + total, t_sizeFile - (total), 0);
		total += recvBytes;
	}
	return pngData;
}

uint32_t Network::sendFileData(const FileSend* t_data) const {
	uint32_t currentPointer {0};
	while (currentPointer < t_data->size_m) {
		int sent = send(m_sockid, (t_data->data) + currentPointer, t_data->size_m - currentPointer, 0);
		currentPointer += sent;
	}
	return currentPointer;
}

uint32_t Network::sendUsername(const std::string& t_username, const std::size_t t_length) const {
	uint32_t currentPointer{ 0 };
	const char* userbuf = t_username.c_str();
	while (currentPointer < t_length) {
		int sent = send(m_sockid, (userbuf)+currentPointer, t_length - currentPointer, 0);
		currentPointer += sent;
	}
	return currentPointer;
}

std::size_t Network::sendInitMsg(const int& t_constant) const {
	MsgHeaderr msg = {0};
	msg.type = htonl(t_constant);
	msg.length = htonl(static_cast<uint32_t>(5));

	return send(getSockID(), reinterpret_cast<char*>(&msg), sizeof(msg), 0);
}

std::size_t Network::sendFileSize(const uint32_t& t_fileSize) const {
	return send(m_sockid, reinterpret_cast<const char*>(&t_fileSize), sizeof(uint32_t), 0); // QSize = 8 bytes
}

void Network::sendFile(const QByteArray* t_fileData, const std::string& t_user_to_send, const std::string& t_filename_to_send) const {
	const char* pngData = t_fileData->constData();
	FileSend png{ 0 };
	png.user_to_send = const_cast<char*>(t_user_to_send.c_str());
	png.size_m = static_cast<uint32_t>(t_fileData->size());
	png.size_u = t_user_to_send.length();
	png.data = const_cast<char*>(pngData);
	sendInitMsg(FILE_C);
	sendFileSize(png.size_m);
	sendFileData(&png);
	sendUsername(t_user_to_send, USERNAME_LENGTH);
	sendUsername(t_filename_to_send, USERNAME_LENGTH);
}

char* Network::recvUser() const {
	char* username = new char[USERNAME_LENGTH];
	std::size_t total = 0;

	while (total < static_cast<unsigned long long>(USERNAME_LENGTH)) {
		std::size_t recvBytes = recv(getSockID(), username + total, USERNAME_LENGTH - static_cast<size_t>(total), 0);
		total += recvBytes;
	}

	return username;
}

std::size_t Network::sendMsg(const std::string& t_message, const std::string& t_username, const int& t_constant) const {
	sendInitMsg(t_constant);

	MsgSend message_to_Send{0};

	memcpy(message_to_Send.message, t_message.c_str(), 128);
	memcpy(message_to_Send.user_to_send, t_username.c_str(), 50);
	message_to_Send.size_u = htonl(t_username.length());
	message_to_Send.size_m = htonl(t_message.length());

	int sent = send(getSockID(), reinterpret_cast<char*>(&message_to_Send), sizeof(MsgSend), 0);

	return static_cast<std::size_t>(sent);
}


std::size_t Network::sendGroupName(const std::string& t_group_name) const {
	sendInitMsg(ROOM_CREATE);

	char grpName[USERNAME_LENGTH];
	memcpy(grpName, t_group_name.c_str(), USERNAME_LENGTH);

	return send(getSockID(), grpName, USERNAME_LENGTH, 0);
}

