#pragma once
#include <string>
#include <WinSock2.h>
#include <QFileDialog>

constexpr auto MSG_SEND = 1;
constexpr auto MSG_LIST = 2;
constexpr auto MSG_EXIT = 3;
constexpr auto USER_EXIT = 4;
constexpr auto ROOM_CREATE = 5;
constexpr auto ROOM_MSG = 6;
constexpr auto ROOM_LIST = 7;
constexpr auto PNG_IMG = 8;
constexpr auto MESSAGE_LENGTH = 128;
constexpr auto MAX_USERS = 10;
constexpr auto USERNAME_LENGTH = 50;
constexpr auto MAXUSERS = 10;

struct MsgHeaderr {
	uint32_t type;
	uint32_t length;
};

struct MsgSend {
	char message[128];
	char user_to_send[50];
	uint32_t size_m;
	uint32_t size_u;
};

struct MsgRecvUser {
	char message[128];
	char user_from[50];
};

struct MsgRecvGroup {
	char message[128];
	char user_from[50];
	char group_name[50];
};

struct RecvGroupName {
	char groupName[50];
};

struct List { // can be User List or Group List
	uint32_t size;
	char arr[MAX_USERS][USERNAME_LENGTH];
};

struct PngSend {
	char* data;
	char *user_to_send;
	uint32_t size_m;
	uint32_t size_u;
};

struct PngRecv {
	char* data;
	char* user_to_send;
	char* filename_to_send;
	uint32_t size_m;
	uint32_t size_u;
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
	int16_t serverConnectHelper(const uint16_t port);

	//im unable to move this to the other file....
	template <typename T>
	T* recvMethod() {
		T* msg = new T;
		char* ptr = reinterpret_cast<char*>(msg);
		std::size_t total = 0;

		while (total < static_cast<std::size_t>(sizeof(T))) {
			std::size_t recvBytes = recv(getSockID(), ptr + total, sizeof(T) - (total), 0);
			total += recvBytes;
		}

		return msg;
	};
	char* recvPng(const size_t t_sizePng) const;
	uint32_t sendPngData(const PngSend* t_data) const;
	uint32_t sendUsername(const std::string& t_username, const std::size_t t_length) const;
	std::size_t sendInitMsg(const int& constant) const;
	std::size_t sendPngSize(const uint32_t& t_pngSize) const;
	void sendPng(const QByteArray* t_pngData, const std::string& t_user_to_send, const std::string& t_filename_to_send) const;
	char* recvUser() const;
	std::size_t sendMsg(const std::string& t_message, const std::string& t_username, const int& t_constant) const;
	std::size_t sendGroupName(const std::string& group_name) const;
};