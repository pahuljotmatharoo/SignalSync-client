#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "ChattingWindow.h"
#include "thread_functions.h"
#include "data.h"
#include "commands.h"
#include <inital_message.h>
#define MSG_SEND 1
#define MSG_LIST 2
#define MSG_EXIT 3

struct ChattingWindow::Impl {
    SOCKET sock{ INVALID_SOCKET };
    ~Impl() {
        if (sock != INVALID_SOCKET)
            closesocket(sock);
        //WSACleanup();
    }
};

ChattingWindow::ChattingWindow(QWidget* parent) : QMainWindow(parent), username{ "" }, impl_(new Impl())
{
    ui.setupUi(this);
}

ChattingWindow::~ChattingWindow()
{
    send_inital_msg(impl_->sock, MSG_EXIT);
    delete impl_;
    impl_ = nullptr;
}

//this is function i use to interact with recieved messages (sort of acts as the middle man)
extern "C" void handle_message(void* window_ptr, char message[128], char username[50]) {
    static_cast<ChattingWindow*>(window_ptr)->addMessage(message, username);
}

//this is function i use to interact with recieved messages (sort of acts as the middle man)
extern "C" void handle_list_update(void* window_ptr, char users[10][50], uint32_t size) {
    static_cast<ChattingWindow*>(window_ptr)->addUsers(users, size);
}

void ChattingWindow::thread_creator()
{
    auto* arg = new RecvParams{&(impl_->sock), this, handle_message, handle_list_update};

    DWORD pThreadID;
    HANDLE call_thread = create_thread(recieving, arg, &pThreadID);
    CloseHandle(call_thread);
    return;
}

void ChattingWindow::setSOCKET(SOCKET newsock) {
    impl_->sock = newsock;
    thread_creator();
    return;
}

void ChattingWindow::setUsername(const QString& new_user)
{
    this->username = new_user;
    return;
}

void ChattingWindow::addMessage(char message[128], char username[50])
{
    std::string username_toadd(username);
    std::string message_toadd(message);

    Messages[username_toadd].push_back(message_toadd);
    return;
}

void ChattingWindow::addUsers(char users[10][50], uint32_t size) {
    //just goes through the list of users when its updated from the server end, and adds any new ones.
    //better logic will be implemented later from server side soon
    for (std::size_t i = 0; i < size; i++) {
        std::string username(users[i]);
        if (Users.find(username) == Users.end()) {
            Users.insert(username);
        }
    }
    return;
}

void ChattingWindow::on_Username_input_textEdited(const QString& text)
{
    username_to_send = text;
}

void ChattingWindow::on_Message_input_textEdited(const QString& text)
{
    message_to_send = text;
}

//implement this
void ChattingWindow::on_sendButton_clicked()
{
    std::string username_to_sendStd = username_to_send.toStdString();
    const char* username_to_sendCStr = username_to_sendStd.c_str();;

    std::string message_to_sendStd = message_to_send.toStdString();
    const char* message_to_sendCStr = message_to_sendStd.c_str();;

    send_to_user(&impl_->sock, message_to_sendCStr, username_to_sendCStr);
}