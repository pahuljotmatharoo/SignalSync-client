class RecvUserMessage {
private:
	char* message;
	char* username;
public:
    RecvUserMessage() : message{ nullptr }, username{ nullptr } {}
    RecvUserMessage(char* t_message, char* t_username) : message{t_message}, username{t_username} {}
    ~RecvUserMessage() {
        if(message != nullptr) {
            delete[] message;
        }
        if(username != nullptr) {
            delete[] username;
        }
    }

    RecvUserMessage(const RecvUserMessage& other) {
        message = other.getMessage();
        username = other.getUsername();
    }

    RecvUserMessage(RecvUserMessage&& other) {
        message = other.getMessage();
        username = other.getUsername();

        other.setMessage(nullptr);
        other.setUsername(nullptr);
    }

    char* getMessage() const { return message; }
    char* getUsername() const { return username; }

    void setMessage(char* msg) { message = msg; }
    void setUsername(char* user) { username = user; }
}