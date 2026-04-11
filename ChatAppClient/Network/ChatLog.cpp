#include "ChatLog.h"

namespace SignalSync {
	void ChatLog::addMessage(RecvUserMessage&& message) {
		m_logs[m_user].push_back(message);
	}
	void ChatLog::clearMap() {
		m_logs.clear();
	}
}