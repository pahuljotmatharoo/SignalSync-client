#include "../Network/NetworkClient.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <QMessageBox>
//#include "dotenv.h"
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

		sendUsername(t_username);

		return 0;
	}

	int16_t Network::serverConnectHelper(const uint16_t t_port) const {
		sockaddr_in t_server{};
		//auto& dotenv = dotenv::env.load_dotenv();

		t_server.sin_family = AF_INET; //specify to use IPV4
		t_server.sin_port = htons(t_port);
#pragma warning(push)
#pragma warning(disable:4996)   // disable this function is deprecated warnings
		t_server.sin_addr.s_addr = inet_addr("");
#pragma warning(pop)

		int status = connect(getSockID(), reinterpret_cast<sockaddr*>(&t_server), sizeof(t_server));
		return (static_cast<int16_t>(status));
	}

	std::tuple<char*, char*, char*> Network::recvFile(const size_t t_sizeFile) {
		char* pngData = new char[t_sizeFile];
		recvAll<char>(pngData, t_sizeFile);
		char* userFrom = recvUser();
		char* fileName = recvUser();
		return { pngData, userFrom, fileName };
	}

	std::tuple<char*, char*, char*, char*, uint32_t*> Network::recvFileGroup() {
		uint32_t* temp_size_file = recvMethod<uint32_t>();
		*temp_size_file = ntohl(*temp_size_file);
		auto tup = recvFile(*temp_size_file);
		char* group_name = recvUser();
		return {std::get<0>(tup), std::get<1>(tup), std::get<2>(tup), group_name, temp_size_file };
	}

	uint32_t Network::sendFileData(const char* t_data, uint32_t size) {
		return sendAll<const char>(t_data, size);
	}

	uint32_t Network::sendUsername(const std::string& t_username) {
		sendSize(t_username.length() + 1);
		const char* userbuf = t_username.c_str();
		return sendAll<const char>(userbuf, t_username.length() + 1);;
	}

	std::size_t Network::sendInitMsg(const NetworkRequest& t_constant) {
		MsgHeaderr msg = { 0 };
		msg.type = htonl(static_cast<u_long>(t_constant));
		msg.length = htonl(static_cast<uint32_t>(5));

		return send(getSockID(), reinterpret_cast<char*>(&msg), sizeof(msg), 0);
	}

	std::size_t Network::sendSize(uint32_t size) {
		size = htonl(size);
		return sendAll<uint32_t>(&size, sizeof(uint32_t));
	}

	int Network::sendFile(const QByteArray* t_fileData, const std::string& t_user_to_send, const std::string& t_filename_to_send, NetworkRequest constant) {
		const char* pngData = t_fileData->constData();
		uint32_t file_size = static_cast<uint32_t>(t_fileData->size());
		uint32_t user_size = t_user_to_send.length();

		if (sendInitMsg(constant) <= 0) { return -1; }

		if (sendSize(file_size) <= 0) { return -1; };

		if (sendFileData(pngData, file_size) <= 0) { return -1; };

		if (sendUsername(t_user_to_send) <= 0) { return -1; };

		if (sendUsername(t_filename_to_send) <= 0) { return -1; };

		return 1;
	}

	char* Network::recvUser() {
		int size{ 0 };
		recvAll<int>(&size, sizeof(int));
		size = ntohl(size);
		char* username = new char[size]();
		int ret = recvAll<char>(username, size);
		return username;
	}

	std::size_t Network::sendMsg(const std::string& t_message, const std::string& t_username, const NetworkRequest& t_constant) {
		const char* message_to_send = t_message.c_str();
		const char* user_to_send = t_username.c_str();

		if (sendInitMsg(t_constant) <= 0) { return 0; }

		if (sendSize(t_message.length() + 1) <= 0) { return 0; }

		if (sendAll<const char>(message_to_send, t_message.length() + 1) <= 0) { return 0; }

		if (sendUsername(t_username) <= 0) { return 0; }

		return 1;
	}

	std::pair<std::vector<std::string>, uint32_t> SignalSync::Network::recvGroupList() {
		int size{ 0 };
		recvAll<int>(&size, sizeof(int));
		size = ntohl(size);
		std::vector<std::string> group_list(size);
		for (int i{ 0 }; i < size; i++) {
			char* username = recvUser();
			group_list[i] = std::string(username);
			delete[] username;
		}
		return { group_list, size };
	}

	std::size_t Network::sendGroupName(const std::string& t_group_name) {
		sendInitMsg(NetworkRequest::ROOM_CREATE);

		return sendUsername(t_group_name);
	}
}