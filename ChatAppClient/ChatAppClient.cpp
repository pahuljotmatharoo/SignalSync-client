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

void ChatAppClient::send_error(const QString& error_message)
{
    QMessageBox msgBox;
    msgBox.setText(error_message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    return;
}

void ChatAppClient::on_pushButton_clicked() {

    const std::string userStd = username.toStdString();

    if (userStd.length() >= 50) {
        send_error("Username too long! Cannot be longer than 50 characters.");
        return;
    }

    const char* userCStr = userStd.c_str();;

    SOCKET sock = main_connect(userCStr);

    //if we cannot connect to the server
    if (sock == SOCKET_ERROR) {
        this->close();
        send_error("Couldn't connect to server.");
        return;
    }

    this->impl_->sock = sock;

    QMessageBox::information(this,tr("Logged in!"),tr("Logged in as %1").arg(username));

    auto* w = new ChattingWindow;
    w->setUsername(this->username);
    w->setSOCKET((this->impl_->sock));

    this->close();

    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show(); 
}
