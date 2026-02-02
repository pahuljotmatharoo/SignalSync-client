#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include "ChatAppClient.h"
#include <QMessageBox>
#include "ChattingWindow.h"

ChatAppClient::ChatAppClient(QWidget *parent): QMainWindow(parent), username{""}, m_http("localhost:8080")
{
    ui.setupUi(this);
}

void ChatAppClient::on_lineEdit_textEdited(const QString &text)
{
    username = text;
}

void ChatAppClient::sendError(const QString& error_message)
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
        sendError("Username too long! Cannot be longer than 50 characters.");
        return;
    }

    if (userStd.length() <= 4) {
        sendError("Username too short! Cannot be shorter than 5 characters.");
        return;
    }

    int status = m_network.serverConnect(userStd);

    if (status == SOCKET_ERROR) { QMessageBox::information(this, tr("Error!"), tr("Cannot connect to server"));  return; }

    QMessageBox::information(this,tr("Logged in!"),tr("Logged in as %1").arg(username));

    auto* w = new ChattingWindow;
    w->setNetwork(this->m_network);
    w->setSelfUser(username);

    this->close();

    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show(); 
}
