#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include "ChatAppClient.h"
#include <QMessageBox>
#include "ChattingWindow.h"

ChatAppClient::ChatAppClient(QWidget *parent): QMainWindow(parent), m_username(""), m_password(""), m_http("localhost:8080")
{
    ui.setupUi(this);
}

void ChatAppClient::on_registerButton_clicked() {
    if (m_http.registerToServer(m_username.toStdString(), m_password.toStdString()) == 0) { sendError("Login information incorrect!"); return; }
    sendError("Registered Successfully!");
}

void ChatAppClient::on_deleteButton_clicked() {
    if (m_http.deleteToServer(m_username.toStdString(), m_password.toStdString()) == 0) { sendError("Login information incorrect!"); return; }
    sendError("Deleted Successfully!");
}

void ChatAppClient::on_usernameEdit_textEdited(const QString &text) {
    m_username = text;
}

void ChatAppClient::on_passwordEdit_textEdited(const QString& text) {
    m_password = text;
}

void ChatAppClient::sendError(const QString& error_message) {
    QMessageBox msgBox;
    msgBox.setText(error_message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    return;
}

void ChatAppClient::on_connectButton_clicked() {

    const std::string userStd = m_username.toStdString();

    if (userStd.length() >= 50) {
        sendError("Username too long! Cannot be longer than 50 characters.");
        return;
    }

    if (userStd.length() <= 4) {
        sendError("Username too short! Cannot be shorter than 5 characters.");
        return;
    }

    if (m_http.connectToServer(m_username.toStdString(), m_password.toStdString()) == 0) { sendError("Login information incorrect!"); return; }

    if (m_network.serverConnect(userStd) == SOCKET_ERROR) { sendError("Cannot Connect to Server!"); return; }

    QMessageBox::information(this,tr("Logged in!"),tr("Logged in as %1").arg(m_username));

    auto* window = new ChattingWindow;
    window->setNetwork(this->m_network);
    window->setSelfUser(m_username);

    this->close();

    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}
