#pragma once
#include <iostream>
#include "RecvUserMessage.h"
namespace SignalSync {
	class RecvGroupMessage : public RecvUserMessage {
	protected:
		std::string m_group;
	public:
		RecvGroupMessage(const RecvUserMessage& t_usermessage, std::string t_group) : RecvUserMessage{ t_usermessage.getUsername(), t_usermessage.getMessage() }, m_group { t_group } {}
		std::string getGroupName() const { return m_group; }
		void setGroupName(std::string group_name) { m_group = group_name; }
	};
}