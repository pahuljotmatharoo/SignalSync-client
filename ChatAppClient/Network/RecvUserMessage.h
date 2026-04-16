#pragma once
namespace SignalSync {
    class RecvUserMessage {
    protected:
        std::string m_message;
        std::string m_username;
    public:
        RecvUserMessage() : m_message{""}, m_username{""} {}
        RecvUserMessage(std::string t_message, std::string t_username) : m_message{ t_message }, m_username{ t_username } {}

        RecvUserMessage(const RecvUserMessage& other) {
            m_message = other.getMessage();
            m_username = other.getUsername();
        }

        RecvUserMessage(RecvUserMessage&& other) noexcept {
            m_message = other.getMessage();
            m_username = other.getUsername();

            other.setMessage("");
            other.setUsername("");
        }

        std::string getMessage() const { return m_message; }
        std::string getUsername() const { return m_username; }

        void setMessage(std::string msg) { m_message = msg; }
        void setUsername(std::string user) { m_username = user; }
    };
}