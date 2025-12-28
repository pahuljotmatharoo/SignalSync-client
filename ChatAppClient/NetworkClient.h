#pragma once
#include <string>
#include <WinSock2.h>

constexpr auto MSG_SEND = 1;
constexpr auto MSG_LIST = 2;
constexpr auto MSG_EXIT = 3;
constexpr auto USER_EXIT = 4;
constexpr auto ROOM_CREATE = 5;
constexpr auto ROOM_MSG = 6;
constexpr auto ROOM_LIST = 7;
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

class Network {
private:
	SOCKET m_sockid;
	WSADATA m_wsaData;
public:
	Network() : m_sockid{ 0 }, m_wsaData{ 0 } {}
	~Network();
	SOCKET getSockID() { return m_sockid; }
	void setSockID(const SOCKET sockid) { m_sockid = sockid; }
	void setWsaData(const WSADATA wsadata) { m_wsaData = wsadata; }
	WSADATA getWsaData() { return m_wsaData; }
	int serverConnect(std::string username);
	int16_t serverConnectHelper(const uint16_t port);

	//im unable to move this to the other file....
	template <typename T>
	T* recvMethod() {
		T* msg = new T;
		char* ptr = reinterpret_cast<char*>(msg);
		std::size_t total = 0;

		while (total < static_cast<unsigned long long>(sizeof(T))) {
			std::size_t recvBytes = recv(getSockID(), ptr + total, sizeof(T) - static_cast<size_t>(total), 0);
			total += recvBytes;
		}

		return msg;
	};

	std::size_t sendInitMsg(int constant);
	char* recvUser();
	std::size_t sendUserMsg(std::string message, std::string username);
	std::size_t sendGroupMsg(std::string t_message, std::string t_groupname);
	std::size_t sendGroupName(std::string group_name);
};