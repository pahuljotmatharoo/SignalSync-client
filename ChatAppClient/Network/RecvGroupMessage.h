#include <iostream>
#include "RecvUserMessage.h"
#pragma once
namespace SignalSync {
	class RecvGroupMessage {
	private:
		RecvUserMessage m_usermessage;
		std::string m_group;
	public:
		RecvGroupMessage(RecvUserMessage& t_usermessage, std::string t_group) : m_group{ t_group } {
			setUserMessage(std::move(t_usermessage));
		}
		std::string getGroupName() const { return m_group; }
		std::string getUsername() const { return m_usermessage.getUsername(); }
		std::string getMessage() const { return m_usermessage.getMessage(); }
		void setUserMessage(RecvUserMessage&& t_usermessage) { 
			m_usermessage.setMessage(t_usermessage.getMessage());
			m_usermessage.setUsername(t_usermessage.getUsername());
		}
		void setGroupName(std::string group_name) { m_group = group_name; }
	};
}