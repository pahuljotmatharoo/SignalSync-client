#pragma once
#include <string>
#include <WinSock2.h>
#include <QFileDialog>
namespace SignalSync {

	enum class NetworkRequest {
		MSG_SEND = 1,
		MSG_LIST = 2,
		MSG_EXIT = 3,
		USER_EXIT = 4,
		ROOM_CREATE = 5,
		ROOM_MSG = 6,
		ROOM_LIST = 7,
		FILE_USER = 8,
		FILE_GROUP = 9
	};

	constexpr auto MESSAGE_LENGTH = 128;
	constexpr auto MAX_USERS = 10;
	constexpr auto USERNAME_LENGTH = 50;
	constexpr auto MAXUSERS = 10;
	constexpr auto MAX_FILE_SIZE = 5000000;

	struct MsgHeaderr {
		uint32_t type;
		uint32_t length;
	};

	struct MsgRecvUser {
		int message_size;
		char* message;
		int username_size;
		char* username;
	};

	struct MsgRecvGroup {
		int message_size;
		char* message;
		int username_size;
		char* username;
		int group_size;
		char* group;
	};

	struct List { // can be User List or Group List
		uint32_t size;
		char arr[MAX_USERS][USERNAME_LENGTH];
	};

	class Network {
	private:
		SOCKET m_sockid;
		WSADATA m_wsaData;
	public:
		Network() : m_sockid{ 0 }, m_wsaData{ 0 } {}
		Network(SOCKET t_sockid, WSADATA t_wsaData) : m_sockid{ t_sockid }, m_wsaData{ t_wsaData } {}
		~Network();
		SOCKET getSockID() const { return m_sockid; }
		void setSockID(const SOCKET sockid) { m_sockid = sockid; }
		void setWsaData(const WSADATA wsadata) { m_wsaData = wsadata; }
		WSADATA getWsaData() const { return m_wsaData; }
		int serverConnect(std::string username);
		int16_t serverConnectHelper(const uint16_t port) const;

		template <typename T>
		T* recvMethod() {
			T* msg = new T;

			if constexpr (std::is_same_v<T, MsgRecvUser> || std::is_same_v<T, MsgRecvGroup>) {
				uint32_t size_message = recvSize();
				msg->message = new char[size_message + 1];
				recvAll<char>(msg->message, size_message);
				msg->message[size_message] = '\0';

				uint32_t size_username = recvSize();
				msg->username = new char[size_username + 1];
				recvAll<char>(msg->username, size_username);
				msg->username[size_username] = '\0';
			}
			else {
				recvAll<T>(msg, sizeof(T));
			}

			if constexpr (std::is_same_v<T, MsgRecvGroup>) {
				uint32_t size_group = recvSize();
				msg->group = new char[size_group + 1];
				recvAll<char>(msg->group, size_group);
				msg->group[size_group] = '\0';
			}

			return msg;
		};

		template <typename T>
		int recvAll(T* t_storage, int bytes) {
			std::size_t total = 0;

			while (total < bytes) {
				std::size_t recvBytes = recv(getSockID(), reinterpret_cast<char*>(t_storage) + total, bytes - total, 0);
				total += recvBytes;
			}
			
			return total;
		}

		std::tuple<char*, char*, char*, uint32_t> recvFile();
		std::tuple<char*, char*, char*, char*, uint32_t> recvFileGroup();
		uint32_t sendFileData(const char* t_data, uint32_t size);
		uint32_t sendUsername(const std::string& t_username);
		std::size_t sendInitMsg(const NetworkRequest& t_constant);
		std::size_t sendSize(uint32_t size);
		int sendFile(const QByteArray* t_fileData, const std::string& t_user_to_send, const std::string& t_filename_to_send, NetworkRequest constant);
		char* recvUser();
		uint32_t recvSize();
		char* recvString();
		std::size_t sendMsg(const std::string& t_message, const std::string& t_username, const NetworkRequest& constant);
		std::size_t sendGroupName(const std::string& group_name);
		std::pair<std::vector<std::string>, uint32_t> recvGroupList();

		template <typename T>
		std::size_t sendAll(T* t_storage, int bytes) {
			std::size_t total = 0;

			while (total < bytes) {
				std::size_t recvBytes = send(getSockID(), reinterpret_cast<const char*>(t_storage) + total, bytes - total, 0);
				total += recvBytes;
			}
			
			return total;
		}
	};
}