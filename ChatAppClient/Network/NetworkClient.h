#pragma once
#include <string>
#include <WinSock2.h>
#include <QFileDialog>
#include "RecvGroupMessage.h"
#include "../Assets/File.h"
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
		FILE_GROUP = 9,
		USER_JOIN = 10,
		FILE_DOWNLOAD = 11
	};

	constexpr auto MESSAGE_LENGTH = 128;
	constexpr auto MAX_USERS = 10;
	constexpr auto USERNAME_LENGTH = 50;
	constexpr auto MAXUSERS = 10;
	constexpr auto MAX_FILE_SIZE = 5000000;

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
		int recvAll(T* t_storage, int bytes) {
			std::size_t total = 0;

			while (total < bytes) {
				std::size_t recvBytes = recv(getSockID(), reinterpret_cast<char*>(t_storage) + total, bytes - total, 0);
				total += recvBytes;
			}
			
			return total;
		}

		std::pair<std::string, std::string> recvFile();
		std::tuple<char*, char*, char*, char*, uint32_t> recvFileGroup();
		uint32_t recvSize();
		char* recvString();
		std::pair<std::vector<std::string>, uint32_t> recvList();
		RecvUserMessage recvUserMessage();
		RecvGroupMessage recvGroupMessage();
		File downloadFileFromServer(std::string username, std::string filename);
		void startDownloadFile(std::string filename);


		uint32_t sendFileData(const char* t_data, uint32_t size);
		uint32_t sendUsername(const std::string& t_username);
		std::size_t sendInitMsg(const NetworkRequest& t_constant);
		std::size_t sendSize(uint32_t size);
		int sendFile(const QByteArray* t_fileData, const std::string& t_user_to_send, const std::string& t_filename_to_send, NetworkRequest constant);
		std::size_t sendMsg(const std::string& t_message, const std::string& t_username, const NetworkRequest& constant);
		std::size_t sendGroupName(const std::string& group_name);

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