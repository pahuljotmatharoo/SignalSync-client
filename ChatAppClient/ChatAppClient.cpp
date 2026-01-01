#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include "ChatAppClient.h"
#include <QMessageBox>
#include "ChattingWindow.h"

ChatAppClient::ChatAppClient(QWidget *parent): QMainWindow(parent), username{""}
{
    ui.setupUi(this);
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

    int status = m_network.serverConnect(userStd);

    if (status == SOCKET_ERROR) { return; }

    QMessageBox::information(this,tr("Logged in!"),tr("Logged in as %1").arg(username));

    auto* w = new ChattingWindow;
    w->setNetwork(this->m_network);
    w->setSelfUser(username);

    this->close();

    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show(); 
}
