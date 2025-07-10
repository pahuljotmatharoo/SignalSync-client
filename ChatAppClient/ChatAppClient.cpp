#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include "ChatAppClient.h"
#include "c_main.h"
#include <QMessageBox>
#include "ChattingWindow.h"

struct ChatAppClient::Impl {
    SOCKET sock {INVALID_SOCKET};
};

ChatAppClient::ChatAppClient(QWidget *parent): QMainWindow(parent), username{""}, impl_(new Impl())
{
    ui.setupUi(this);
}

ChatAppClient::~ChatAppClient()
{
    delete impl_;
    impl_ = nullptr;
}

void ChatAppClient::on_lineEdit_textEdited(const QString &text)
{
    username = text;
}

void ChatAppClient::on_pushButton_clicked() {
    // make sure you’re getting the final text
    const QString user = username;
    std::string userStd = username.toStdString();
    const char* userCStr = userStd.c_str();;

    SOCKET sock = main_connect(userCStr);
    this->impl_->sock = sock;

    QMessageBox::information(this,
        tr("Logged in as %1"),
        tr("Hello, %1!").arg(user));

    auto* w = new ChattingWindow;

    w->setUsername(this->username);
    w->setSOCKET((this->impl_->sock));
    this->close();

    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show(); 
}
