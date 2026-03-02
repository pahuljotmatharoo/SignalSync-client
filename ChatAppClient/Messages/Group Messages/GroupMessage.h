#pragma once
#include <QString>
#include <unordered_map>
class GroupMessage {
	private:
		QString m_groupName;
		std::unordered_map<QString, UserMessage*> m_messages;
	public:
		GroupMessage(QString t_name) : m_groupName(t_name) {}
		~GroupMessage() {
			for (auto itr = m_messages.begin(); itr != m_messages.end(); itr++) {
				delete itr->second;
			}
		}
		std::unordered_map<QString, UserMessage*>& getMessages() {
			return m_messages;
		};

		void addMessage(QString t_username, bool curr_or_user, QString t_message) {
			if (m_messages.find(t_username) == m_messages.end()) {
				m_messages[t_username] = new UserMessage(t_username);
			}
			m_messages[t_username]->addMessage((std::make_pair(curr_or_user, t_message.toStdString())));
		};

		void setName(QString t_name) { m_groupName = t_name; }
		QString getName() const { return m_groupName; }
};
