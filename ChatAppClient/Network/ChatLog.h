#pragma once
#include <unordered_map>
#include <string>
#include "Network/RecvUserMessage.h"
namespace SignalSync {
	class ChatLog {
	private:
		std::unordered_map<std::string, std::vector<RecvUserMessage>> m_logs;
		std::string m_user;
	public:
		void addMessage(RecvUserMessage&& message);
		size_t size() { return m_logs.size(); }
		std::string getUsername() const { return m_user; }
		void setUsername(std::string username) { m_user = username; }
		void clearMap();
		std::unordered_map<std::string, std::vector<RecvUserMessage>>::iterator begin() {
			return m_logs.begin();
		}
		std::unordered_map<std::string, std::vector<RecvUserMessage>>::iterator end() {
			return m_logs.end();
		}
		std::unordered_map<std::string, std::vector<RecvUserMessage>>::const_iterator begin() const {
			return m_logs.begin();
		}
		std::unordered_map<std::string, std::vector<RecvUserMessage>>::const_iterator end() const {
			return m_logs.end();
		}
	};
}