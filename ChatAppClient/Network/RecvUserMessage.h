namespace SignalSync {
    class RecvUserMessage {
    private:
        std::string message;
        std::string username;
    public:
        RecvUserMessage() : message{""}, username{""} {}
        RecvUserMessage(std::string t_message, std::string t_username) : message{ t_message }, username{ t_username } {}

        RecvUserMessage(const RecvUserMessage& other) {
            message = other.getMessage();
            username = other.getUsername();
        }

        RecvUserMessage(RecvUserMessage&& other) noexcept {
            message = other.getMessage();
            username = other.getUsername();

            other.setMessage("");
            other.setUsername("");
        }

        std::string getMessage() const { return message; }
        std::string getUsername() const { return username; }

        void setMessage(std::string msg) { message = msg; }
        void setUsername(std::string user) { username = user; }
    };
}