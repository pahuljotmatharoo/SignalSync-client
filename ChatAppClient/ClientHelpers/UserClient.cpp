#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <QHBoxLayout>
#include <qscrollbar.h>
#include <qlayout.h>
#include <qtimer.h>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include <cstdlib>
#include "ChattingWindow.h"
#include "ui_ChattingWindow.h"

namespace SignalSync {

    void ChattingWindow::addMessage(std::string username_toadd, std::string message_toadd) {
        message_toadd = decrypt(message_toadd).toStdString();

        {
            LockGuard guard(m_generalSemaphore);
            if (m_messages.find(QString::fromStdString(username_toadd)) == m_messages.end()) {
                m_messages.emplace(QString::fromStdString(username_toadd), UniquePtr<UserMessage>(UserMessage(QString::fromStdString(username_toadd))));
            }
            m_messages[QString::fromStdString(username_toadd)]->addMessage(std::make_pair(OTHER_USER, message_toadd));
        }

        QMetaObject::invokeMethod(this, [=] { this->sendMessageToScreenRecv(QString::fromStdString(message_toadd), QString::fromStdString(username_toadd), UserB); }, Qt::QueuedConnection);
    }


    void ChattingWindow::addUser(const std::string username) {
        LockGuard guard(m_generalSemaphore);
        QString username_q = QString::fromStdString(username);
        if ((m_Users).find(username_q) == (m_Users).end()) {
            QMetaObject::invokeMethod(this, [=] { this->sendUserToScreen(username_q); }, Qt::QueuedConnection);
        }
    }

    void ChattingWindow::displayUserMessages(const QString& t_user_name) {
        UserMessage& user_messages = *m_messages[t_user_name];
        //if (user_messages == nullptr) { return; }

        displayMessages(user_messages, t_user_name, UserB);

        displayFileButtons(t_user_name);
    }

    void ChattingWindow::displayMessages(UserMessage& t_messages, const QString& user_or_group_name, bool user_or_group) {
        auto& vec = t_messages.getMessages();

        for (int i{ 0 }; i < vec.size(); i++) {
            if (vec[i].first == CURR_USER) {
                sendMessageToScreenSend(QString::fromStdString(vec[i].second));
            }
            else {
                sendMessageToScreenRecv(QString::fromStdString(vec[i].second), user_or_group_name, user_or_group);
            }
        }
    }

    void ChattingWindow::removeUsers(const std::string user) {
        QMetaObject::invokeMethod(this, [=] { this->removeUserfromScreen(QString::fromStdString(std::string(user))); }, Qt::QueuedConnection);
    }

    void ChattingWindow::processFileRecvUser(const std::string filename, const std::string username) {
        QMetaObject::invokeMethod(this, [=] { this->addFileButtonToScreenUser(filename, username); }, Qt::QueuedConnection);
    }

    void ChattingWindow::downloadUserFile() {
        QString dirName = QFileDialog::getExistingDirectory(this, tr("Select a directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        QPushButton* btn = qobject_cast<QPushButton*>(sender());

        m_fileToDownload = btn->text().toStdString();

        m_network.startDownloadFile(m_fileToDownload);

        // Pretty much using Producer-Consumer pattern
        m_fileDownloadStart.release();
        m_fileDownloadDone.acquire();

        if (m_downloaded.downloadFile(btn->text().toStdString(), dirName.toStdString())) {
            ChatAppClient::sendError("File download successfully!");
        }
        else {
            ChatAppClient::sendError("Unable to download file!");
        }
    }

    void ChattingWindow::addFileButtonToScreenUser(const std::string filename, const std::string username) {
        QPushButton* file_button = createAndStyleFileButton(filename);

        if (m_lastPressedUser != nullptr && QString::fromStdString(username) == m_lastPressedUser->text()) {
            file_button->show();
        }

        m_filesUsers.emplace(file_button, std::make_pair(filename, username));

        connect(file_button, &QPushButton::clicked, this, &ChattingWindow::downloadUserFile);

        addWidgetToLayout(file_button, Qt::AlignLeft);

        notificationPassUser(QString::fromStdString(username));
    }

    void ChattingWindow::on_userList_clicked() {
        m_ui->addGroup->hide();
        m_userOrGroup = UserB;

        m_ui->currUsers_label->setText("Current Users");

        userOrGroupSelect(m_Groups, m_Users, m_lastPressedGroup);
    }

    void ChattingWindow::onUserClick() {
        QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());

        if (m_lastPressedUser == clickedButton) {
            return;
        }

        if (m_lastPressedUser) {
            m_lastPressedUser->setStyleSheet(m_defaultButtonStylesheet);
        }

        clickedButton->setStyleSheet(m_pressedButtonStylesheet);
        m_lastPressedUser = clickedButton;

        removeAllChatItemsFromScreen();

        m_ui->username_label->setText(clickedButton->text());
        m_usernameToSend = clickedButton->text();

        displayUserMessages(m_usernameToSend);
    }

    void ChattingWindow::notificationPassUser(const QString user_from) {
        {
            LockGuard guard(m_generalSemaphore);
            if (m_Users.find(user_from) == m_Users.end() || m_lastPressedUser == m_Users[user_from]) {
                return;
            }
        }
        QMetaObject::invokeMethod(this, [=] { this->notificationUser(user_from); }, Qt::QueuedConnection);
    }

    void ChattingWindow::notificationUser(const QString& user_from) {
        LockGuard guard(m_generalSemaphore);
        m_Users[user_from]->setStyleSheet(m_recvNotificationStylesheet);
    }

    void ChattingWindow::removeUserfromScreen(const QString& user) {
        LockGuard guard(m_generalSemaphore);
        if ((m_Users)[user]) {
            if (m_lastPressedUser == (m_Users)[user]) {
                m_lastPressedUser = nullptr;
                removeAllChatItemsFromScreen();
                m_ui->username_label->setText("Select a User to talk to!");
            }
            m_ui->userLayout->removeWidget((m_Users)[user]);
            (m_Users)[user]->hide();
            (m_Users)[user]->deleteLater();
            (m_Users)[user] = nullptr;
            m_Users.erase(user);
            m_messages.erase(user);
        }
    }

    void ChattingWindow::sendUserToScreen(const QString username) {
        QPushButton* user = createAndStyleButton(username);

        if (username == m_selfUsername) {
            user->setText("You:" + username);
        }
        else {
            user->setText(username);
        }

        if (m_userOrGroup == GroupB) {
            user->hide();
        }

        {
            LockGuard guard(m_generalSemaphore);
            m_messages[username] = UniquePtr<UserMessage>(UserMessage(username));
            m_Users.insert(std::make_pair(username, user));
        }

        m_ui->userLayout->addWidget(user, 0, Qt::AlignCenter | Qt::AlignTop);

        connect(user, &QPushButton::clicked, this, &ChattingWindow::onUserClick);
    }
}