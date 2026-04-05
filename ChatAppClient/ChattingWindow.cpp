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

// Some sort of race conditions with 3 users, check

// TODO: Ensure c-style string sizes are all + 1 before being sent to server

namespace SignalSync {
    ChattingWindow::ChattingWindow(QWidget* parent) : QMainWindow(parent), m_lastPressedUser(nullptr), m_threadStop(false), m_thread(&ChattingWindow::networkThreadFunction, this),
        m_lastPressedGroup(nullptr), m_messageFont("Montserrat", 14), m_titleFont("Montserrat", 25),
        m_fileDownloadDone(0), m_userOrGroup(UserB), m_buttonAddGroupFont("Montserrat", 8), m_buttonFont("Montserrat", 10), m_usernameToSend(""), m_groupSemaphore(1), m_generalSemaphore(1), m_queueSemaphore(0), 
        m_fileDownloadStart(0), m_http("localhost:8080"), m_threadPool(MAX_THREADS), m_ui(new ::Ui::ChattingWindow)
    {
        initUI();
        initEncryptMap();
        initThreads();
    }

    ChattingWindow::~ChattingWindow() {
        threadShutdown(); 
    }
    
    void ChattingWindow::networkThreadFunction() {
        NetworkRequest type{};
        while (!m_threadStop) {
            std::size_t recvData = recv(m_network.getSockID(), reinterpret_cast<char*>(&type), sizeof(NetworkRequest), 0);
            if (recvData > 0) {
                switch (type) {
                    case NetworkRequest::MSG_SEND: {
                        networkMessageRecv();
                        break;
                    }
                    case NetworkRequest::MSG_LIST: {
                        networkUserListRecv();
                        break;
                    }
                    case NetworkRequest::USER_EXIT: {
                        networkUserExit();
                        break;
                    }
                    case NetworkRequest::ROOM_CREATE: {
                        networkRoomCreateRecv();
                        break;
                    }
                    case NetworkRequest::ROOM_MSG: {
                        networkRoomMessageRecv();
                        break;
                    }
                    case NetworkRequest::ROOM_LIST: {
                        networkRoomListRecv();
                        break;
                    }
                    case NetworkRequest::FILE_USER: {
                        networkFileRecv();
                        break;
                    }
                    case NetworkRequest::FILE_GROUP: {
                        networkFileGroupRecv();
                        break;
                    }
                    case NetworkRequest::USER_JOIN: {
                        networkUserJoinRecv();
                        break;
                    }
                    case NetworkRequest::FILE_DOWNLOAD: {
                        networkDownloadFile();
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        }
    }

    void ChattingWindow::enqueue(std::function<void()> func) {
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            m_functionQueue.push(func);
        }
        m_queueSemaphore.release();
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
                    //task = std::move(m_functionQueue.front());
                    task = m_functionQueue.front();
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

    void ChattingWindow::networkMessageRecv() {
        RecvUserMessage recv_msg = m_network.recvUserMessage();
        std::string username (recv_msg.getUsername());
        std::string message(recv_msg.getMessage());
        enqueue([=]()->void {addMessage(username, message); });
        notificationPassUser(QString::fromStdString(username));
    }

    void SignalSync::ChattingWindow::networkUserExit() {
        char* username = m_network.recvString();
        if (username == nullptr) { 
            return;
        }
        std::string username_str(username);
        enqueue([=]()->void {removeUsers(username_str); });
        delete[] username;
    }

    void SignalSync::ChattingWindow::networkRoomCreateRecv() {
        char* groupName = m_network.recvString();
        if (groupName == nullptr) { 
            return;
        }
        std::string group_name(groupName);
        enqueue([=]()->void {addGroup(group_name); });
        delete[] groupName;
    }

    void ChattingWindow::networkRoomMessageRecv() {
        RecvGroupMessage group_message = m_network.recvGroupMessage();
        std::string group_name(group_message.getGroupName());
        std::string username(group_message.getUsername());
        std::string message(group_message.getMessage());
        enqueue([=]()->void {addMessage_group(message, username, group_name); });
        notificationPassGroup(QString::fromStdString(group_name));
    }

    void ChattingWindow::networkRoomListRecv() {
        std::pair<std::vector<std::string>, uint32_t> list_group = m_network.recvList();

        if (list_group.first.size() == 0) {
            return;
        }

        for (const auto& group_name : list_group.first) {
            addGroup(group_name);
        }
    }

    void ChattingWindow::networkFileRecv() {
        auto recv_file = m_network.recvFile();
        enqueue([=]()->void {processFileRecvUser(recv_file.second, recv_file.first); });
    }

    void ChattingWindow::networkFileGroupRecv() {
        auto recv_file = m_network.recvFileGroup();

        std::string user(std::get<0>(recv_file));
        std::string filename(std::get<1>(recv_file));
        std::string group_name(std::get<2>(recv_file));

        enqueue([=]()->void {processFileRecvGroup(filename, user, group_name); });
    }

    void ChattingWindow::networkUserListRecv() {
        std::pair<std::vector<std::string>, uint32_t> list_users = m_network.recvList();
        for (const std::string& user : list_users.first) {
            enqueue([=]()->void {addUser(user); });
        }
    }

    void ChattingWindow::networkUserJoinRecv() {
        char* username = m_network.recvString();
        std::string username_s(username);
        enqueue([=]()->void {addUser(username_s); });
        delete[] username;
    }

    void ChattingWindow::networkDownloadFile() {
        m_fileDownloadStart.acquire();
        File file = m_network.downloadFileFromServer(m_selfUsername.toStdString(), m_fileToDownload);
        m_downloaded = std::move(file);
        m_fileDownloadDone.release();
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

    void ChattingWindow::displayFileButtons(const QString& user_or_group_name) {
        auto itr = m_filesUsers.begin();
        for (std::size_t i{ 0 }; i < m_filesUsers.size() && itr != m_filesUsers.end(); i++) {
            if (itr->second.second == user_or_group_name) { itr->first->show(); itr++; }
        }
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