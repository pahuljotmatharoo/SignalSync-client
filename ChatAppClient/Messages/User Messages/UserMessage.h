#pragma once
class UserMessage {
	private:
		QString m_name;
		std::vector<std::pair<bool, std::string>> m_messages;

	public:
		UserMessage() {}
		UserMessage(QString t_name) : m_name(t_name) {}
		std::vector<std::pair<bool, std::string>>& getMessages() { return m_messages; }
		void addMessage(std::pair<bool, std::string> message) { m_messages.push_back(message); }
		void setName(QString t_name) { m_name = t_name; }
		QString getName() const { return m_name; }

};