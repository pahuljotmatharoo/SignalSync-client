#include "../Network/NetworkClient.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <QMessageBox>
namespace SignalSync {
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
			closesocket(sock);
			WSACleanup();
			return status;
		}

		setWsaData(wsaData);

		send(sock, t_username.c_str(), USERNAME_LENGTH, 0);

		return 0;
	}

	int16_t Network::serverConnectHelper(const uint16_t t_port) const {
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
		uint32_t currentPointer{ 0 };
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

	std::size_t Network::sendInitMsg(const NetworkRequest& t_constant) const {
		MsgHeaderr msg = { 0 };
		msg.type = htonl(static_cast<u_long>(t_constant));
		msg.length = htonl(static_cast<uint32_t>(5));

		return send(getSockID(), reinterpret_cast<char*>(&msg), sizeof(msg), 0);
	}

	std::size_t Network::sendSize(const uint32_t& size) const {
		return send(m_sockid, reinterpret_cast<const char*>(&size), sizeof(uint32_t), 0);
	}

	int Network::sendFile(const QByteArray* t_fileData, const std::string& t_user_to_send, const std::string& t_filename_to_send, NetworkRequest constant) const {
		const char* pngData = t_fileData->constData();
		FileSend png{ 0 };
		png.user_to_send = const_cast<char*>(t_user_to_send.c_str());
		png.size_m = static_cast<uint32_t>(t_fileData->size());
		png.size_u = t_user_to_send.length();
		png.data = const_cast<char*>(pngData);
		if (sendInitMsg(constant) == -1) { return -1; }
		if (sendSize(htonl(png.size_m)) == -1) { return -1; };
		if (sendFileData(&png) == -1) { return -1; };
		if (sendUsername(t_user_to_send, USERNAME_LENGTH) == -1) { return -1; };
		if (sendUsername(t_filename_to_send, USERNAME_LENGTH) == -1) { return -1; };
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

	std::size_t Network::sendMsg(const std::string& t_message, const std::string& t_username, const NetworkRequest& t_constant) const {
		MsgSend message_to_Send{ 0 };

		if (sendInitMsg(t_constant) <= 0) { return 0; }
		if (sendSize(static_cast<uint32_t>(htonl(t_message.length() + 1))) <= 0) { return 0; }

		memcpy(message_to_Send.message, t_message.c_str(), t_message.length());

		if (send(getSockID(), message_to_Send.message, t_message.length() + 1, 0) <= 0) { return 0; }

		if (sendSize(static_cast<uint32_t>(htonl(t_username.length() + 1))) <= 0) { return 0; }

		memcpy(message_to_Send.user_to_send, t_username.c_str(), 50);
		if (send(getSockID(), message_to_Send.user_to_send, t_username.length() + 1, 0) <= 0) { return 0; }

		return 1;
	}


	std::size_t Network::sendGroupName(const std::string& t_group_name) const {
		sendInitMsg(NetworkRequest::ROOM_CREATE);

		char grpName[USERNAME_LENGTH];
		memcpy(grpName, t_group_name.c_str(), USERNAME_LENGTH);

		return send(getSockID(), grpName, USERNAME_LENGTH, 0);
	}
}