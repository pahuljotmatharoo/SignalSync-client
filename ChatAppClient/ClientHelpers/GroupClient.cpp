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

    void ChattingWindow::downloadGroupFile() {
        QString dir_name = QFileDialog::getExistingDirectory(this, tr("Select a directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        QPushButton* btn = qobject_cast<QPushButton*>(sender());

        std::string filename = btn->text().toStdString();
        const std::string& group_name = m_filesGroup[btn].getGroupName();

        m_network.startDownloadFile(filename, group_name, NetworkRequest::FILE_DOWNLOAD_GROUP);

        m_groupFileDownloaded = group_name;

        // Pretty much using Producer-Consumer pattern
        m_fileDownloadStart.release();
        m_fileDownloadDone.acquire();

        if (m_downloaded.downloadFile(btn->text().toStdString(), dir_name.toStdString())) {
            ChatAppClient::sendError("File download successfully!");
        }
        else {
            ChatAppClient::sendError("Unable to download file!");
        }
    }

    void ChattingWindow::addFileButtonToScreenGroup(const std::string filename, const std::string username, const std::string groupname) {
        QPushButton* file_button = createAndStyleFileButton(filename);

        if (m_lastPressedGroup != nullptr && groupname == m_lastPressedGroup->text()) {
            file_button->show();
        }

        m_filesGroup.emplace(file_button, FileGroup(filename, username, groupname));

        connect(file_button, &QPushButton::clicked, this, &ChattingWindow::downloadGroupFile);

        addWidgetToLayout(file_button, Qt::AlignLeft);

        notificationPassGroup(QString::fromStdString(groupname));
    }

    QString ChattingWindow::findNewGroupName() {
        size_t curr_map_size = m_Groups.size();
        QString group_name = QString("Group %1").arg(++curr_map_size);

        while (m_Groups.find(group_name) != m_Groups.end()) {
            group_name = QString("Group %1").arg(++curr_map_size);
        }

        return group_name;
    }

    void ChattingWindow::on_addGroup_clicked() {
        auto [button, name] = createAndStyleGroupButton();

        if (button == nullptr) { return; }

        connect(button, &QPushButton::clicked, this, &ChattingWindow::onGroupClick);

        {
            LockGuard guard(m_generalSemaphore);
            m_groupMessages.insert(std::make_pair(name, UniquePtr<GroupMessage>(GroupMessage(name))));
        }

        if (m_network.sendGroupName(name.toStdString()) == std::nullopt) {
            ChatAppClient::sendError("Cannot add group successfully");
        };
    }

    void ChattingWindow::on_groupChat_clicked() {
        m_ui->addGroup->show();
        m_userOrGroup = GroupB;

        m_ui->currUsers_label->setText("Current Groups");

        userOrGroupSelect(m_Users, m_Groups, m_lastPressedUser);
    }


    void ChattingWindow::onGroupClick() {
        QPushButton* clicked_button = qobject_cast<QPushButton*>(sender());

        if (m_lastPressedGroup == clicked_button) {
            return;
        }

        if (m_lastPressedGroup) {
            m_lastPressedGroup->setStyleSheet(m_defaultButtonStylesheet);
        }

        clicked_button->setStyleSheet(m_pressedButtonStylesheet);

        m_lastPressedGroup = clicked_button;


        removeAllChatItemsFromScreen();

        m_ui->username_label->setText(clicked_button->text());
        m_groupToSend = clicked_button->text();

        displayGroupMessages(m_groupToSend);
    }

    void ChattingWindow::displayGroupMessages(const QString& t_group_name) {
        GroupMessage& group_messages = *m_groupMessages[t_group_name];

        auto& vec_m = group_messages.getMessages();

        for (auto& [group_name, message] : vec_m) {
            displayMessages(*message, t_group_name, GroupB);
        }

        for (auto& [button, group_file] : m_filesGroup) {
            if (QString::fromStdString(group_file.getGroupName()) == t_group_name) { button->show(); }
        }
    }

    void ChattingWindow::notificationPassGroup(const QString& group_from) {
        {
            LockGuard guard(m_generalSemaphore);
            if (m_Groups.find(group_from) == m_Groups.end() || m_lastPressedGroup == m_Groups[group_from]) {
                return;
            }
        }
        QMetaObject::invokeMethod(this, [=] { this->notificationGroup(group_from); }, Qt::QueuedConnection);
    }

    void ChattingWindow::notificationGroup(const QString& group_from) {
        LockGuard guard(m_generalSemaphore);
        m_Groups[group_from]->setStyleSheet(m_recvNotificationStylesheet);
    }

    void ChattingWindow::addGroup(const std::string group_name_s) {

        QString group_name = QString::fromStdString(group_name_s);

        {
            LockGuard guard(m_generalSemaphore);
            m_groupMessages.insert(std::make_pair(group_name, UniquePtr<GroupMessage>(GroupMessage(group_name))));
        }

        QMetaObject::invokeMethod(this, [=] { this->addGrouptoScreen(group_name); }, Qt::QueuedConnection);
    }

    void ChattingWindow::addMessage_group(std::string message_toadd, const std::string username_toadd, const std::string group_toadd) {
        QString message_r = QString::fromStdString(message_toadd);
        encrypt(message_r);
        message_toadd = message_r.toStdString();

        {
            LockGuard guard(m_generalSemaphore);

            if (m_groupMessages.find(QString::fromStdString(group_toadd)) == m_groupMessages.end()) {
                m_groupMessages.insert(std::make_pair(QString::fromStdString(group_toadd), UniquePtr<GroupMessage>(GroupMessage(QString::fromStdString(group_toadd)))));
            }

            if (m_Groups.find(QString::fromStdString(group_toadd)) == m_Groups.end()) {
                QMetaObject::invokeMethod(this, [=] { this->createIfGroupMissing(QString::fromStdString(group_toadd)); }, Qt::QueuedConnection);
            }

            m_groupMessages[QString::fromStdString(group_toadd)]->addMessage(QString::fromStdString(username_toadd), OTHER_USER, QString::fromStdString(username_toadd) + ":" + message_r);

        }

        //we'll be able to display right away to screen, needs to run on the gui thread (main thread)
        QMetaObject::invokeMethod(this, [=] { this->sendMessageToScreenRecv(QString::fromStdString(username_toadd + " : " + message_toadd), QString::fromStdString(group_toadd), GroupB); }, Qt::QueuedConnection);
    }

    void ChattingWindow::addGrouptoScreen(const QString group_name) {
        QPushButton* group = createAndStyleButton(group_name);

        {
            LockGuard guard(m_generalSemaphore);
            m_Groups.insert(std::make_pair(group_name, group));
        }

        if (m_userOrGroup == UserB) {
            group->hide();
        }

        m_ui->userLayout->addWidget(group, 0, Qt::AlignCenter | Qt::AlignTop);
        connect(group, &QPushButton::clicked, this, &ChattingWindow::onGroupClick);
    }
}