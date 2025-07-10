#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "ChattingWindow.h"
#include "thread_functions.h"
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

void ChattingWindow::thread_creator()
{
    DWORD pThreadID;
    HANDLE call_thread = create_thread(recieving, &(impl_->sock), &pThreadID);
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
