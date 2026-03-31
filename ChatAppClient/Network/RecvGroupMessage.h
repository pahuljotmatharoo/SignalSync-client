#include <iostream>
#include "RecvUserMessage.h"
#pragma once
namespace SignalSync {
	class RecvGroupMessage {
	private:
		RecvUserMessage m_usermessage;
		char* m_group;
	public:
		RecvGroupMessage(RecvUserMessage& t_usermessage, char* t_group) : m_group{ t_group } {
			setUserMessage(std::move(t_usermessage));
		}
		~RecvGroupMessage() { 
			if (m_group) {
				delete[] m_group;
				m_group = nullptr;
			}
		}
		char* getGroupName() const { return m_group; }
		char* getUsername() const { return m_usermessage.getUsername(); }
		char* getMessage() const { return m_usermessage.getMessage(); }
		void setUserMessage(RecvUserMessage&& t_usermessage) { 
			m_usermessage.setMessage(t_usermessage.getMessage());
			m_usermessage.setUsername(t_usermessage.getUsername());

			t_usermessage.setMessage(nullptr);
			t_usermessage.setUsername(nullptr);
		}
		void setGroupName(char* group_name) { m_group = group_name; }
	};
}