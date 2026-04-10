#include "../Network/NetworkClient.h"

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

	std::pair<std::string, std::string> Network::recvFile() {
		std::string username = recvString();
		std::string filename = recvString();
		return { username, filename };
	}

	File Network::downloadFileFromServer(std::string username, std::string filename) {
		uint32_t size_file = recvSize();
		char* pngData = new char[size_file];
		recvAll<char>(pngData, size_file);
		return File(QString::fromStdString(username), pngData, size_file);
	}

	void Network::startDownloadFile(std::string filename) {
		sendInitMsg(NetworkRequest::FILE_DOWNLOAD);
		sendUsername(filename);
	}

	std::vector<std::unordered_map<std::string, std::vector<RecvUserMessage>>> Network::recvUserChatlogs() { // custom class?
		std::vector<std::unordered_map<std::string, std::vector<RecvUserMessage>>> return_type;
		std::unordered_map<std::string, std::vector<RecvUserMessage>> temp;
		std::string current_user_recv{ "" };
		while (1) {
			std::string recieved_string = recvString();
			if (recieved_string == "FILE") {
				current_user_recv = recvString();
				if (temp.size() > 0) {
					return_type.push_back(temp);
					temp.clear();
				}
			}
			else if(recieved_string == "END") {
				break;
			}
			else { // only want to recieve the username here
				std::string username = recvString();
				RecvUserMessage message(recieved_string, username);
				temp[current_user_recv].push_back(message);
			}
		}
		return_type.push_back(temp);
		return return_type;
	}

	std::tuple<std::string, std::string, std::string> Network::recvFileGroup() { // username, filename, groupname
		auto [username, filename] = recvFile();
		std::string group_name = recvString();
		return std::tuple(username, filename, group_name);
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
		uint32_t msg{ 0 };
		msg = htonl(static_cast<u_long>(t_constant));

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

	std::string Network::recvString() {
		uint32_t size = recvSize();

		char* text = new char[size + 1]();
		recvAll<char>(text, size);
		text[size] = '\0';

		std::string ret_val(text);

		delete[] text;
		return ret_val;
	}

	uint32_t Network::recvSize() {
		uint32_t size{ 0 };
		recvAll<uint32_t>(&size, sizeof(uint32_t));
		return ntohl(size);
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

	std::pair<std::vector<std::string>, uint32_t> SignalSync::Network::recvList() {
		uint32_t size = recvSize();
		std::vector<std::string> group_list(size);
		for (int i{ 0 }; i < size; i++) {
			std::string username = recvString();
			group_list[i] = username;
		}
		return { group_list, size };
	}

	std::size_t Network::sendGroupName(const std::string& t_group_name) {
		sendInitMsg(NetworkRequest::ROOM_CREATE);

		return sendUsername(t_group_name);
	}

	RecvUserMessage SignalSync::Network::recvUserMessage() {
		std::string message = recvString();
		std::string username = recvString();
		return RecvUserMessage(message, username);
	}

	RecvGroupMessage Network::recvGroupMessage() {
		RecvUserMessage user_message = recvUserMessage();
		std::string group_name = recvString();
		return RecvGroupMessage(user_message, group_name);
	}
}