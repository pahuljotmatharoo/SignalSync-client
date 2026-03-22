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
    ChattingWindow::ChattingWindow(QWidget* parent) : QMainWindow(parent), m_lastPressedUser(nullptr), m_threadStop(false), m_thread(&ChattingWindow::networkThreadFunction, this),
        m_lastPressedGroup(nullptr), m_messageFont("Montserrat", 14), m_titleFont("Montserrat", 25),
        m_userOrGroup(UserB), m_buttonAddGroupFont("Montserrat", 8), m_buttonFont("Montserrat", 10), m_usernameToSend(""), m_groupSemaphore(1), m_generalSemaphore(1), m_queueSemaphore(0),
        m_http("localhost:8080"), m_threadPool(MAX_THREADS), m_ui(new ::Ui::ChattingWindow)
    {
        initUI();
        initEncryptMap();
        initThreads();
    }

    ChattingWindow::~ChattingWindow() { threadShutdown(); }

    void ChattingWindow::networkThreadFunction() {
        NetworkRequest type{};
        while (!m_threadStop) {
            std::size_t recvData = recv(m_network.getSockID(), reinterpret_cast<char*>(&type), sizeof(NetworkRequest), 0);
            if (recvData > 0) {
                switch (type) {
                case NetworkRequest::MSG_SEND: {
                    MsgRecvUser* recvStruct = m_network.recvMethod<MsgRecvUser>();
                    if (recvStruct == nullptr) { continue; }
                    //m_functionQueue.push([=]()->void{addMessage(recvStruct);});
                    enqueue(&ChattingWindow::addMessage, this, recvStruct);
                    std::string user_from(recvStruct->username);
                    notificationPassUser(QString::fromStdString(user_from));
                    break;
                }
                case NetworkRequest::MSG_LIST: {
                    List* list = m_network.recvMethod<List>();
                    if (list == nullptr) { continue; }
                    list->size = ntohl(list->size);
                    enqueue(&ChattingWindow::addUsers, this, list);
                    break;
                }
                case NetworkRequest::USER_EXIT: {
                    char* username = m_network.recvUser();
                    if (username == nullptr) { continue; }
                    enqueue(&ChattingWindow::removeUsers, this, std::string(username), 50);
                    delete[] username;
                    break;
                }
                case NetworkRequest::ROOM_CREATE: {
                    char* groupName = m_network.recvUser();
                    if (groupName == nullptr) { continue; }
                    std::string group_name(groupName);
                    enqueue(&ChattingWindow::addGroup, this, group_name);
                    delete[] groupName;
                    break;
                }
                case NetworkRequest::ROOM_MSG: {
                    MsgRecvGroup* recvGrpMsg = m_network.recvMethod<MsgRecvGroup>();
                    if (recvGrpMsg == nullptr) { continue; }
                    std::string group_name(recvGrpMsg->group);
                    enqueue(&ChattingWindow::addMessage_group, this, std::string(recvGrpMsg->message), std::string(recvGrpMsg->username), group_name);
                    notificationPassGroup(QString::fromStdString(group_name));
                    delete recvGrpMsg;
                    break;
                }
                case NetworkRequest::ROOM_LIST: { // ? wtf am i doing here lol
                    List* listGroup = m_network.recvMethod<List>();
                    if (listGroup == nullptr) { continue; }
                    listGroup->size = ntohl(listGroup->size);
                    if (listGroup->size > MAXUSERS) {
                        delete listGroup;
                        break;
                    }
                    delete listGroup;
                    break;
                }
                case NetworkRequest::FILE_USER: {
                    uint32_t* sizeFile = m_network.recvMethod<uint32_t>();
                    if (sizeFile == nullptr) { continue; }
                    char* fileData = m_network.recvFile(*sizeFile);
                    if (fileData == nullptr) { continue; }
                    char* userFrom = m_network.recvUser();
                    if (userFrom == nullptr) { continue; }
                    char* fileName = m_network.recvUser();
                    if (fileName == nullptr) { continue; }
                    std::string userString(userFrom);
                    std::string filenameString(fileName);
                    //File* recvFile = new File(QString::fromStdString(userString), fileData, *sizeFile);
                    enqueue(&ChattingWindow::processFileRecvUser, this, QString::fromStdString(userString), fileData, *sizeFile, filenameString);
                    delete sizeFile;
                    delete userFrom;
                    delete fileName;
                    break;
                }
                case NetworkRequest::FILE_GROUP: {
                    uint32_t* sizeFile = m_network.recvMethod<uint32_t>();
                    if (sizeFile == nullptr) { continue; }
                    char* fileData = m_network.recvFile(*sizeFile);
                    if (fileData == nullptr) { continue; }
                    char* userFrom = m_network.recvUser();
                    if (userFrom == nullptr) { continue; }
                    char* fileName = m_network.recvUser();
                    if (fileName == nullptr) { continue; }
                    char* groupName = m_network.recvUser();
                    if (groupName == nullptr) { continue; }
                    std::string userString(userFrom);
                    std::string filenameString(fileName);
                    std::string groupNameString(groupName);
                    //File* recvFile = new File(QString::fromStdString(userString), fileData, *sizeFile);
                    enqueue(&ChattingWindow::processFileRecvGroup, this, QString::fromStdString(userString), fileData, *sizeFile, filenameString, groupNameString);
                    delete sizeFile;
                    delete userFrom;
                    delete fileName;
                    delete groupName;
                    break;
                }
                }
            }
            else {
                break;
            }
        }
    }

    void ChattingWindow::initThreads() {
        for (std::thread& t : m_threadPool) {
            t = std::thread(&ChattingWindow::waitingThreadFunction, this);
        }
    }

    void ChattingWindow::dequeue() {
        m_functionQueue.pop();
    }

    void ChattingWindow::waitingThreadFunction() {
        while (!m_threadStop) {
            m_queueSemaphore.acquire();
            if (m_threadStop) {
                break;
            }
            std::function<void()> task;
            {
                std::lock_guard<std::mutex> lock(m_queueMutex);
                if (m_functionQueue.size() > 0) {
                    task = std::move(m_functionQueue.front());
                    dequeue();
                }
            }
            if (task) {
                task();
            }
        }
    }

    void ChattingWindow::threadShutdown() {
        m_network.sendInitMsg(NetworkRequest::MSG_EXIT);
        ::shutdown(m_network.getSockID(), SD_BOTH);
        m_threadStop = true;
        m_thread.join();
        m_queueSemaphore.release(MAX_THREADS);
        for (auto& t : m_threadPool) {
            t.join();
        }
    }

    void ChattingWindow::processFileRecvUser(const QString t_userFrom, char* t_data, const uint32_t t_size, const std::string& fileName) {
        QMetaObject::invokeMethod(this, [=] { this->addFileButtonToScreenUser(t_userFrom, t_data, t_size, fileName); }, Qt::QueuedConnection);
    }

    void ChattingWindow::processFileRecvGroup(const QString t_userFrom, char* t_data, const uint32_t t_size, const std::string& fileName, const std::string& groupName) {
        QMetaObject::invokeMethod(this, [=] { this->addFileButtonToScreenGroup(t_userFrom, t_data, t_size  ,fileName, groupName); }, Qt::QueuedConnection);
    }

    void ChattingWindow::freeMessageStruct(MsgRecvUser* t_msg) {
        delete[]t_msg->message;
        delete[]t_msg->username;
    }


    //these are the same basically, fix
    void ChattingWindow::downloadUserFile() {
        QString dirName = QFileDialog::getExistingDirectory(this, tr("Select a directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        QPushButton* btn = qobject_cast<QPushButton*>(sender());
        if (m_filesUsers[btn].downloadFile(btn->text().toStdString(), dirName.toStdString())) {
            ChatAppClient::sendError("File download successfully!");
        }
        else {
            ChatAppClient::sendError("Unable to download file!");
        }
    }

    void ChattingWindow::downloadGroupFile() {
        QString dirName = QFileDialog::getExistingDirectory(this, tr("Select a directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        QPushButton* btn = qobject_cast<QPushButton*>(sender());
        if (m_filesGroup[btn].first.downloadFile(btn->text().toStdString(), dirName.toStdString())) {
            ChatAppClient::sendError("File download successfully!");
        }
        else {
            ChatAppClient::sendError("Unable to download file!");
        }
    }

    QPushButton* ChattingWindow::createAndStyleFileButton(const std::string& fileName) {
        QPushButton* button = new QPushButton(this);

        button->setText(QString::fromStdString(fileName));
        button->setMinimumSize(205, 40);
        button->setStyleSheet(m_defaultButtonStylesheet);
        button->setIcon(QIcon("./Icons/download.png"));

        button->hide();

        return button;
    }

    //yeah literally same function... (but theres no real good way to make this DRY)
    void ChattingWindow::addFileButtonToScreenUser(const QString t_userFrom, char* t_data, const uint32_t t_size, const std::string& fileName) {
        QPushButton* file_button = createAndStyleFileButton(fileName);

        if (m_lastPressedUser != nullptr && t_userFrom == m_lastPressedUser->text()) {
            file_button->show();
        }

        m_filesUsers.emplace(file_button, File(t_userFrom, t_data, t_size));

        connect(file_button, &QPushButton::clicked, this, &ChattingWindow::downloadUserFile);

        addWidgetToLayout(file_button, Qt::AlignLeft);

        notificationPassUser(t_userFrom);
    }

    void ChattingWindow::addFileButtonToScreenGroup(const QString t_userFrom, char* t_data, const uint32_t t_size, const std::string& fileName, const std::string& groupName) {
        QPushButton* file_button = createAndStyleFileButton(fileName);

        if (m_lastPressedGroup != nullptr && groupName == m_lastPressedGroup->text()) {
            file_button->show();
        }

        m_filesGroup.emplace(file_button, std::make_pair(File(t_userFrom, t_data, t_size), QString::fromStdString(groupName)));

        connect(file_button, &QPushButton::clicked, this, &ChattingWindow::downloadGroupFile);

        addWidgetToLayout(file_button, Qt::AlignLeft);

        notificationPassGroup(QString::fromStdString(groupName));
    }

    QPushButton* ChattingWindow::createAndStyleButton(const QString& name) {
        QPushButton* button = new QPushButton(this);
        button->setText(name);
        button->setMinimumSize(205, 40);
        button->setStyleSheet(m_defaultButtonStylesheet);
        return button;
    }

    std::pair<QPushButton*, QString> ChattingWindow::createAndStyleGroupButton() {
        if (m_Groups.size() >= 10) {
            ChatAppClient::sendError("Already at a maximum number of Groups!");
            return { nullptr, "" };
        }

        QString group_name = findNewGroupName();

        QPushButton* group = createAndStyleButton(group_name);

        {
            LockGuard guard(m_generalSemaphore);
            m_Groups.insert(std::make_pair(group_name, group));
        }

        m_ui->userLayout->addWidget(group, 0, Qt::AlignCenter | Qt::AlignTop);

        return { group, group_name };
    }

    QString ChattingWindow::findNewGroupName() {
        size_t curr_map_size = m_Groups.size();
        QString group_name = QString("Group %1").arg(++curr_map_size);

        while (m_Groups.find(group_name) != m_Groups.end()) {
            group_name = QString("Group %1").arg(++curr_map_size);
        }

        return group_name;
    }

    void ChattingWindow::on_Message_input_textEdited(const QString& text) {
        m_messageToSend = text;
    }

    void ChattingWindow::on_fileButton_clicked() {
        QString fileName = QFileDialog::getOpenFileName(this, "Select one or more files to open", "/home");

        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly)) {
                QByteArray content = file.readAll();
                QFileInfo fileInfo(file);

                QString fileName = fileInfo.fileName();

                if (m_usernameToSend == "" && m_groupToSend == "") {
                    ChatAppClient::sendError("Select a user!");
                    file.close();
                    return;
                }

                if (fileInfo.size() > MAX_FILE_SIZE) {
                    ChatAppClient::sendError("File too large! Must be less than 5 MB");
                    return;
                }
                if (m_groupToSend == "" && m_selfUsername == m_usernameToSend.toStdString().substr(4)) {
                    file.close(); // sending file to self (not allowed as of right now)
                    return;
                }
                else {
                    if (m_lastPressedUser) {
                        if (m_network.sendFile(&content, m_usernameToSend.toStdString(), fileName.toStdString(), NetworkRequest::FILE_USER) == -1) {
                            ChatAppClient::sendError("Cannot send file successfully");
                            file.close();
                            //processFileRecvUser()
                            return;
                        }
                    }
                    else if (m_lastPressedGroup) {
                        if (m_network.sendFile(&content, m_groupToSend.toStdString(), fileName.toStdString(), NetworkRequest::FILE_GROUP) == -1) {
                            ChatAppClient::sendError("Cannot send file successfully");
                            file.close();
                            //processFileRecvGroup()
                            return;
                        }
                    }
                }
                file.close();
            }
        }
    }

    void ChattingWindow::on_addGroup_clicked() {
        auto buttonAndName = createAndStyleGroupButton();

        if (buttonAndName.first == nullptr) { return; }

        connect(buttonAndName.first, &QPushButton::clicked, this, &ChattingWindow::onGroupClick);

        {
            LockGuard guard(m_generalSemaphore);
            m_groupMessages.insert(std::make_pair(buttonAndName.second, UniquePtr<GroupMessage>(GroupMessage(buttonAndName.second))));
        }

        if (m_network.sendGroupName(buttonAndName.second.toStdString()) == -1) {
            ChatAppClient::sendError("Cannot add group successfully");
        };
    }

    void ChattingWindow::userOrGroupSelect(std::unordered_map<QString, QPushButton*>& hide, std::unordered_map<QString, QPushButton*>& show, QPushButton*& lastPressedButton) const {
        if (hide.size() != 0) {
            for (auto itr = hide.begin(); itr != hide.end(); itr++) {
                itr->second->hide();
            }
        }

        if (show.size() != 0) {
            for (auto itr = show.begin(); itr != show.end(); itr++) {
                itr->second->show();
            }
        }

        if (lastPressedButton) {
            lastPressedButton->setStyleSheet(m_defaultButtonStylesheet);
            lastPressedButton = nullptr;
        }
    }

    void ChattingWindow::on_groupChat_clicked() {
        m_ui->addGroup->show();
        m_userOrGroup = GroupB;

        m_ui->currUsers_label->setText("Current Groups");

        userOrGroupSelect(m_Users, m_Groups, m_lastPressedUser);
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


    void ChattingWindow::onGroupClick() {
        QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());

        if (m_lastPressedGroup == clickedButton) {
            return;
        }

        if (m_lastPressedGroup) {
            m_lastPressedGroup->setStyleSheet(m_defaultButtonStylesheet);
        }

        clickedButton->setStyleSheet(m_pressedButtonStylesheet);

        m_lastPressedGroup = clickedButton;


        removeAllChatItemsFromScreen();

        m_ui->username_label->setText(clickedButton->text());
        m_groupToSend = clickedButton->text();

        displayGroupMessages(m_groupToSend);
    }

    void ChattingWindow::displayUserMessages(const QString& t_user_name) {
        UserMessage& user_messages = *m_messages[t_user_name];
        //if (user_messages == nullptr) { return; }

        displayMessages(user_messages, t_user_name, UserB);

        displayFileButtons(t_user_name);
    }

    void ChattingWindow::displayGroupMessages(const QString& t_group_name) {
        GroupMessage& group_messages = *m_groupMessages[t_group_name];

        auto& vec_m = group_messages.getMessages();

        for (auto itr = vec_m.begin(); itr != vec_m.end(); ++itr) {
            displayMessages(*(itr->second), t_group_name, GroupB);
        }

        for (auto itr = m_filesGroup.begin(); itr != m_filesGroup.end(); itr++) {
            if (itr->second.second == t_group_name) {
                itr->first->show();
            }
        }
    }

    void ChattingWindow::displayFileButtons(const QString& user_or_group_name) {
        auto itr = m_filesUsers.begin();
        for (std::size_t i{ 0 }; i < m_filesUsers.size() && itr != m_filesUsers.end(); i++) {
            if (itr->second.getUserFrom() == user_or_group_name) { itr->first->show(); itr++; }
        }
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

    void ChattingWindow::notificationPassUser(const QString user_from) {
        {
            LockGuard guard(m_generalSemaphore);
            if (m_Users.find(user_from) == m_Users.end() || m_lastPressedUser == m_Users[user_from]) {
                return;
            }
        }
        QMetaObject::invokeMethod(this, [=] { this->notificationUser(user_from); }, Qt::QueuedConnection);
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

    void ChattingWindow::notificationUser(const QString& user_from) {
        LockGuard guard(m_generalSemaphore);
        m_Users[user_from]->setStyleSheet(m_recvNotificationStylesheet);
    }

    void ChattingWindow::notificationGroup(const QString& group_from) {
        LockGuard guard(m_generalSemaphore);
        m_Groups[group_from]->setStyleSheet(m_recvNotificationStylesheet);
    }

    void ChattingWindow::addWidgetToLayout(QWidget* widget, Qt::Alignment alignment) {
        m_ui->chatLayout->addWidget(widget, 0, alignment);

        auto* sb = m_ui->scrollArea->verticalScrollBar();
        sb->setValue(sb->maximum());

        QTimer::singleShot(0, this, [this, widget]() {
            m_ui->scrollArea->ensureWidgetVisible(widget);
            });
    }

    void ChattingWindow::createIfGroupMissing(const QString& group_name) {
        QPushButton* group_button = createAndStyleButton(group_name);

        {
            LockGuard guard(m_generalSemaphore);
            m_Groups.insert(std::make_pair(group_name, group_button));
        }

        if (m_userOrGroup == UserB) {
            group_button->hide();
        }

        connect(group_button, &QPushButton::clicked, this, &ChattingWindow::onGroupClick);
        m_ui->userLayout->addWidget(group_button, 0, Qt::AlignCenter | Qt::AlignTop);
    }

    void ChattingWindow::on_sendButton_clicked() {
        QString messageCopy = m_messageToSend;
        encrypt(messageCopy);
        if (m_userOrGroup == UserB) {
            std::string username_to_sendStd = m_usernameToSend.toStdString();

            std::string message_to_sendStd = messageCopy.toStdString(); // the one we are going to store in our vector (unencrypted)

            if (message_to_sendStd.length() >= 128) {
                ChatAppClient::sendError("Message is too long! Cannot be longer than 128 characters.");
                return;
            }

            if (message_to_sendStd.length() == 0) {
                ChatAppClient::sendError("Message is empty! Cannot send empty message!.");
                return;
            }

            if (m_selfUsername == username_to_sendStd.substr(4)) {
                sendMessageToScreenSend(m_messageToSend);
                return;
            }

            {
                LockGuard guard(m_generalSemaphore);
                if ((m_messages).find(m_usernameToSend) == m_messages.end()) {
                    m_messages.emplace(m_usernameToSend, UniquePtr<UserMessage>(UserMessage(QString::fromStdString(username_to_sendStd))));
                }
                m_messages[m_usernameToSend]->addMessage((std::make_pair(CURR_USER, m_messageToSend.toStdString())));
            }

            if (m_network.sendMsg(message_to_sendStd, username_to_sendStd, NetworkRequest::MSG_SEND) == -1) {
                ChatAppClient::sendError("Connection Lost! Please reconnect!");
                std::exit(1);
            }

            sendMessageToScreenSend(m_messageToSend);
        }

        else {
            std::string group_to_sendStd = m_groupToSend.toStdString();
            std::string message_to_sendStd = messageCopy.toStdString();

            if (message_to_sendStd.length() >= 128) {
                ChatAppClient::sendError("Message is too long! Cannot be longer than 128 characters.");
                return;
            }

            if (message_to_sendStd.length() == 0) {
                ChatAppClient::sendError("Message is empty! Cannot send empty message!.");
                return;
            }

            {
                LockGuard guard(m_generalSemaphore);
                m_groupMessages[m_groupToSend]->addMessage(m_selfUsername, CURR_USER, m_selfUsername + " : " + m_messageToSend);
            }

            if (m_network.sendMsg(message_to_sendStd, group_to_sendStd, NetworkRequest::ROOM_MSG) == -1) {
                ChatAppClient::sendError("Connection Lost! Please reconnect!");
                std::exit(1);
            }

            sendMessageToScreenSend(m_selfUsername + " : " + m_messageToSend);
        }
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

    void ChattingWindow::addMessage(MsgRecvUser* recvStruct) {
        std::string username_toadd(recvStruct->username);
        std::string message_toadd(recvStruct->message);

        message_toadd = decrypt(message_toadd).toStdString();

        {
            LockGuard guard(m_generalSemaphore);
            if (m_messages.find(QString::fromStdString(username_toadd)) == m_messages.end()) {
                m_messages.emplace(QString::fromStdString(username_toadd), UniquePtr<UserMessage>(UserMessage(QString::fromStdString(username_toadd))));
            }
            m_messages[QString::fromStdString(username_toadd)]->addMessage(std::make_pair(OTHER_USER, message_toadd));
        }

        freeMessageStruct(recvStruct);

        QMetaObject::invokeMethod(this, [=] { this->sendMessageToScreenRecv(QString::fromStdString(message_toadd), QString::fromStdString(username_toadd), UserB); }, Qt::QueuedConnection);
    }

    void ChattingWindow::addUsers(List* list) {
        for (std::size_t i{ 0 }; i < list->size; i++) {
            QString username = QString::fromStdString(std::string(list->arr[i]));
            LockGuard guard(m_generalSemaphore);
            if ((m_Users).find(QString::fromStdString(std::string(list->arr[i]))) == (m_Users).end()) {
                QMetaObject::invokeMethod(this, [=] { this->sendUserToScreen(username); }, Qt::QueuedConnection);
            }
        }
        delete list;
    }

    void ChattingWindow::addGroups(char groups[MAXUSERS][USERNAME_LENGTH], uint32_t size) {
        for (std::size_t i{ 0 }; i < size; i++) {
            QString group = QString::fromStdString(std::string(groups[i]));
            LockGuard guard(m_generalSemaphore);
            if ((m_Groups).find(group) == (m_Groups).end()) {
                QMetaObject::invokeMethod(this, [=] { this->addGroup(std::string(groups[i])); }, Qt::QueuedConnection);
            }
        }
    }

    void ChattingWindow::removeUsers(const std::string user, uint32_t size) {
        QMetaObject::invokeMethod(this, [=] { this->removeUserfromScreen(QString::fromStdString(std::string(user))); }, Qt::QueuedConnection);
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

    //check check where we check if we are on the correct person, only then send to screen
    void ChattingWindow::sendMessageToScreenRecv(const QString& message, const QString& user, bool type) {
        if (!m_lastPressedGroup && !m_lastPressedUser) { return; }
        if (type == UserB) {
            if (m_lastPressedGroup) { return; }
            else if (m_lastPressedUser && m_lastPressedUser->text() != user) { return; }
        }
        if (type == GroupB) {
            if (m_lastPressedUser) { return; }
            else if (m_lastPressedGroup && m_lastPressedGroup->text() != user) { return; }
        }

        addWidgetToLayout(new MessageWidget(message, this), Qt::AlignLeft);
    }

    void ChattingWindow::sendMessageToScreenSend(const QString& message) {
        addWidgetToLayout(new MessageWidget_s(message, this), Qt::AlignRight);
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

    void ChattingWindow::removeAllChatItemsFromScreen() {
        for (int i = 0; i < m_ui->chatLayout->count(); ++i) {
            QLayoutItem* item = m_ui->chatLayout->itemAt(i);
            QWidget* w = nullptr;
            if (item != nullptr) { w = item->widget(); }
            if (!w) continue;

            if (qobject_cast<QPushButton*>(w)) {
                w->hide();
            }
            else {
                m_ui->chatLayout->removeWidget(w);
                w->deleteLater();
                --i;
            }
        }
    }

    void ChattingWindow::initEncryptMap() {
        QString uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        QString lowercase = "abcdefghijklmnopqrstuvwxyz";
        QString digits = "0123456789";
        QString symbols = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

        // Uppercase mapping (A <-> Z, B <-> Y, etc.)
        for (int i = 0; i < 26; i++) {
            (m_encryptMap)[uppercase[i]] = uppercase[25 - i];
        }

        // Lowercase mapping (a <-> z, b <-> y, etc.)
        for (int i = 0; i < 26; i++) {
            (m_encryptMap)[lowercase[i]] = lowercase[25 - i];
        }

        // Digit mapping (0 <-> 9, 1 <-> 8, etc.)
        for (int i = 0; i < 10; i++) {
            (m_encryptMap)[digits[i]] = digits[9 - i];
        }

        // Symbol mapping (reverse order)
        int symLen = symbols.length();
        for (int i = 0; i < symLen; i++) {
            (m_encryptMap)[symbols[i]] = symbols[symLen - 1 - i];
        }
    }

    void ChattingWindow::encrypt(QString& message) {
        for (std::size_t i = 0; i < message.length(); i++) {
            if (message[i] == ' ') {
                continue;
            }
            message[i] = (m_encryptMap)[message[i]];
        }
    }

    QString ChattingWindow::decrypt(const std::string message) {
        QString message_r = QString::fromStdString(message);
        encrypt(message_r);
        return message_r;
    }

    void ChattingWindow::initStyles() {
        m_defaultButtonStylesheet =
            "QPushButton {"
            " background-color: #4CAF50;"
            " color: white;"
            " border: none;"
            " padding: 10px;"
            " font-size: 16px;"
            " border-radius: 5px;"
            " }"
            " QPushButton:hover {"
            " background-color: #45a049;"
            " }";

        m_pressedButtonStylesheet =
            "QPushButton {"
            " background-color: #1E90FF;"
            " color: white;"
            " border: none;"
            " padding: 10px;"
            " font-size: 16px;"
            " border-radius: 5px;"
            " }"
            " QPushButton:hover {"
            " background-color: #055cb0;"
            " }";

        m_recvNotificationStylesheet =
            "QPushButton {"
            " background-color: #FF6B6B;"
            " color: white;"
            " border: none;"
            " padding: 10px;"
            " font-size: 16px;"
            " border-radius: 5px;"
            " }"
            " QPushButton:hover {"
            " background-color: #055cb0;"
            " }";

        m_sendMessageStylesheet =
            " background-color: #4CAF50;"
            " color:#212121;"
            " border:1px solid #E0E0E0;"
            " border-radius:12px;"
            " font-size: 16px;";

        m_recvMessageStylesheet =
            " background-color: #555555;"
            " color:#212121;"
            " border:1px solid #E0E0E0;"
            " border-radius:12px;"
            " font-size: 16px;";
    }

    void ChattingWindow::initButtons() {
        m_ui->chatLayout->addStretch(1);

        m_ui->title_label->setFont(m_titleFont);

        m_ui->addGroup->setFont(m_buttonAddGroupFont);
        m_ui->userList->setFont(m_buttonFont);
        m_ui->groupChat->setFont(m_buttonFont);
        m_ui->Message_input->setFont(m_buttonFont);
        m_ui->sendButton->setIcon(QIcon("./Icons/icon.png"));
        m_ui->sendButton->setIconSize(QSize(45, 37));

        m_ui->fileButton->setIcon(QIcon("./Icons/fileupload.png"));
        m_ui->fileButton->setIconSize(QSize(45, 37));

        m_ui->chatLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        m_ui->addGroup->hide();
    }

    void ChattingWindow::initLayout() {
        QWidget* contents = m_ui->scrollArea->takeWidget();
        QVBoxLayout* layout = new QVBoxLayout(contents);

        layout->setContentsMargins(0, 0, 0, 0);
        contents->setLayout(layout);
        m_ui->scrollArea->setWidget(contents);

        m_ui->chatLayout = layout;
        layout->setSpacing(5);
        layout->setSizeConstraint(QLayout::SetMinimumSize);
    }

    void ChattingWindow::initContentLayout() {
        //we're just creating a layout for scrolling and out vertical layout
        QWidget* contents_users = m_ui->scrollArea_2->takeWidget();
        QVBoxLayout* layout_users = new QVBoxLayout(contents_users);

        layout_users->setContentsMargins(0, 0, 0, 0);
        contents_users->setLayout(layout_users);
        m_ui->scrollArea_2->setWidget(contents_users);

        m_ui->userLayout = layout_users;
        layout_users->setSpacing(5);
        layout_users->setSizeConstraint(QLayout::SetMinimumSize);

        m_ui->userLayout->setContentsMargins(10, 5, 10, 0); // 10px left/right margins

        m_ui->chatLayout->setContentsMargins(10, 10, 10, 10); // 10px left/right margins

        auto* l = static_cast<QVBoxLayout*>(m_ui->scrollAreaWidgetContents->layout());
        l->insertStretch(0, 1);
    }

    void ChattingWindow::initUI() {
        m_ui->setupUi(this);
        initStyles();
        initButtons();
        initLayout();
        initContentLayout();
    }
}