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

//BUGS: Not selecting a user before sending file leads to a crash


ChattingWindow::ChattingWindow(QWidget* parent) : QMainWindow(parent), m_lastPressedUser(nullptr), m_threadStop(false), m_thread(&ChattingWindow::threadFunction, this),
                                                    m_lastPressedGroup(nullptr), m_messageFont("Montserrat", 14), m_titleFont("Montserrat", 25), 
                                                    m_userOrGroup(UserB), m_buttonAddGroupFont("Montserrat", 8), m_buttonFont("Montserrat", 10)
{
    initUI();

    initEncryptMap();
}

ChattingWindow::~ChattingWindow() {
    destroyFiles();
    destroyUserMessages();
    destroyGroupMessages();
    m_network.sendInitMsg(MSG_EXIT);
    ::shutdown(m_network.getSockID(), SD_BOTH); // send shut down to socket, should get us out of the while loop
    m_threadStop = true; // thread safe atomic
    m_thread.join();
}

void ChattingWindow::threadFunction() {
    int type = 0;
    while (!m_threadStop) {
        std::size_t recvData = recv(m_network.getSockID(), reinterpret_cast<char*>(&type), sizeof(int), 0);
        if(recvData > 0) {
            switch (type) {
                case MSG_SEND: {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    MsgRecvUser* recvStruct = m_network.recvMethod<MsgRecvUser>();
                    if (recvStruct == nullptr) { continue; }
                    addMessage(recvStruct->message, recvStruct->user_from);
                    delete recvStruct;
                    break;
                }
                case MSG_LIST: {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    List* list = m_network.recvMethod<List>();
                    if (list == nullptr) { continue; }
                    list->size = ntohl(list->size);
                    addUsers(list->arr, list->size);
                    delete list;
                    break;
                }
                case USER_EXIT: {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    char* username = m_network.recvUser();
                    if (username == nullptr) { continue; }
                    removeUsers(username, USERNAME_LENGTH);
                    delete[] username;
                    break;
                }
                case ROOM_CREATE: {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    RecvGroupName* groupName = m_network.recvMethod<RecvGroupName>();
                    if (groupName == nullptr) { continue; }
                    addGroup(groupName->groupName);
                    delete groupName;
                    break;
                }
                case ROOM_MSG: {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    MsgRecvGroup* recvGrpMsg = m_network.recvMethod<MsgRecvGroup>();
                    if (recvGrpMsg == nullptr) { continue; }
                    addMessage_group(recvGrpMsg->message, recvGrpMsg->user_from, recvGrpMsg->group_name);
                    delete recvGrpMsg;
                    break;
                }
                case ROOM_LIST: {
                    std::lock_guard<std::mutex> lock(m_mutex);
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
                case FILE_C: {
                    std::lock_guard<std::mutex> lock(m_mutex);
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
                    File* recvFile = new File(QString::fromStdString(userString), fileData, *sizeFile);
                    processFileRecv(recvFile, filenameString);
                    delete sizeFile;
                    delete userFrom;
                }
            }
        }
        else {
            break;
        }
    }
}

void ChattingWindow::destroyFiles() {
    for (auto itr = m_files.begin(); itr != m_files.end(); ++itr) {
        delete itr->second;
    }
}

void ChattingWindow::destroyUserMessages() {
    for (auto itr = m_messages.begin(); itr != m_messages.end(); ++itr) {
        delete itr->second;
    }
}

void ChattingWindow::destroyGroupMessages() {
    for (auto itr = m_groupMessages.begin(); itr != m_groupMessages.end(); ++itr) {
        delete itr->second;
    }
}

void ChattingWindow::processFileRecv(File* recvFile, std::string fileName) {
    QMetaObject::invokeMethod(this, [=] { this->addFileButtonToScreen(recvFile, fileName); }, Qt::QueuedConnection);
}

void ChattingWindow::downloadFile() {
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select a directory"), "/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (m_files[btn]->downloadFile(btn->text().toStdString(), dirName.toStdString())) {
        sendError("File download successfully!");
    }
    else {
        sendError("Unable to download file!");
    }
}

QPushButton* ChattingWindow::createAndStyleFileButton(std::string& fileName) {
    QPushButton* button = new QPushButton(this);

    button->setText(QString::fromStdString(fileName));
    button->setMinimumSize(205, 40);
    button->setStyleSheet(m_defaultButtonStylesheet);
    button->setIcon(QIcon("download.png"));

    button->hide();

    connect(button, &QPushButton::clicked, this, &ChattingWindow::downloadFile);


    return button;
}


void ChattingWindow::addFileButtonToScreen(File* recvFile, std::string fileName) {
    QPushButton* button = createAndStyleFileButton(fileName);

    if (button == nullptr) { return; }

    //definately have a method that does this
    m_ui.chatLayout->addWidget(button, 0, Qt::AlignLeft);

    m_ui.scrollArea->verticalScrollBar()->setValue(m_ui.scrollArea->verticalScrollBar()->maximum());

    QTimer::singleShot(0, this, [=]() {
        m_ui.scrollArea->ensureWidgetVisible(button);
        });

    if (m_lastPressedUser != nullptr && recvFile->getUserFrom() == m_lastPressedUser->text()) {
        button->show();
    }

    m_files.insert(std::make_pair(button, recvFile));
}

std::pair<QPushButton*, QString> ChattingWindow::createAndStyleGroupButton() {
    if (m_Groups.size() >= 10) {
        sendError("Already at a maximum number of Groups!");
        QString empty{ "" };
        return { nullptr, empty};
    }

    QPushButton* group = new QPushButton(this);

    QString groupName = QString("Group %1").arg(m_Groups.size() + 1);

    group->setText(groupName);
    group->setMinimumSize(205, 40);
    group->setStyleSheet(m_defaultButtonStylesheet);

    m_Groups.insert(std::make_pair(groupName, group));

    m_ui.userLayout->addWidget(group, 0, Qt::AlignCenter | Qt::AlignTop);

    return { group, groupName };
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

            if (fileInfo.size() > MAX_FILE_SIZE) {
                sendError("File too large! Must be less than 5 MB");
                return;
            }

            if (m_selfUsername == m_usernameToSend.toStdString().substr(4)) {
                file.close();
                return;
            }   
            else {
                m_network.sendFile(&content, m_usernameToSend.toStdString(), fileName.toStdString());
            }
            file.close();
        }
    }
}

void ChattingWindow::on_addGroup_clicked() {
    auto buttonAndName = createAndStyleGroupButton();

    if (buttonAndName.first == nullptr) { return; }

    connect(buttonAndName.first, &QPushButton::clicked, this, &ChattingWindow::onGroupClick);

    GroupMessage* groupMsg = new GroupMessage(buttonAndName.second);

    m_groupMessages.insert(std::make_pair(buttonAndName.second, groupMsg));

    m_network.sendGroupName(buttonAndName.second.toStdString());
}

void ChattingWindow::userOrGroupSelect(std::unordered_map<QString, QPushButton*> &hide, std::unordered_map<QString, QPushButton*> &show, QPushButton*& lastPressedButton) const {
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

//can't really simplify this i think as it is tied to the button itself
void ChattingWindow::on_groupChat_clicked() {
    m_ui.addGroup->show();
    m_userOrGroup = GroupB;

    m_ui.currUsers_label->setText("Current Groups");

    userOrGroupSelect(m_Users, m_Groups, m_lastPressedUser);
}

//can't really simplify this i think as it is tied to the button itself
void ChattingWindow::on_userList_clicked() {
    m_ui.addGroup->hide();
    m_userOrGroup = UserB;

    m_ui.currUsers_label->setText("Current Users");

    userOrGroupSelect(m_Groups, m_Users, m_lastPressedGroup);
}

//can't really simplify this i think as it is tied to the button itself
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

    m_ui.username_label->setText(clickedButton->text());
    m_usernameToSend = clickedButton->text();
    auto& vec_msg = (m_messages)[m_usernameToSend];

    displayUserMessages(m_usernameToSend);
}

//can't really simplify this i think as it is tied to the button itself
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

    //remove all the items in the current chat layout
    removeAllChatItemsFromScreen();

    m_ui.username_label->setText(clickedButton->text());
    m_groupToSend = clickedButton->text();

    displayGroupMessages(m_groupToSend);
}

void ChattingWindow::displayUserMessages(const QString& t_user_name) {
    UserMessage* user_messages = m_messages[t_user_name];
    if (user_messages == nullptr) { return; }

    displayMessages(user_messages, t_user_name, UserB);

    displayFileButtons(t_user_name);
}

void ChattingWindow::displayGroupMessages(const QString& t_group_name) {
    GroupMessage* group_messages = m_groupMessages[t_group_name];

    auto& vec_m = group_messages->getMessages();

    for (auto itr = vec_m.begin(); itr != vec_m.end(); ++itr) {
        displayMessages(itr->second, t_group_name, GroupB);
    }
}

void ChattingWindow::displayFileButtons(const QString& user_or_group_name) {
    auto itr = m_files.begin();
    for (std::size_t i{ 0 }; i < m_files.size() && itr != m_files.end(); i++) {
        if (itr->second->getUserFrom() == user_or_group_name) { itr->first->show(); itr++; }
    }
}

void ChattingWindow::displayMessages(UserMessage* t_messages, const QString& user_or_group_name, bool user_or_group) {
    auto& vec = t_messages->getMessages();

    for (int i{ 0 }; i < vec.size(); i++) {
        if (vec[i].first == CURR_USER) { sendMessageToScreenSend(QString::fromStdString(vec[i].second)); }
        else { sendMessageToScreenRecv(QString::fromStdString(vec[i].second), user_or_group_name, user_or_group); };
    }
}

void ChattingWindow::on_sendButton_clicked() {
    QString messageCopy = m_messageToSend;
    encrypt(messageCopy);
    if (m_userOrGroup == UserB) {
        std::string username_to_sendStd = m_usernameToSend.toStdString();

        std::string message_to_sendStd = messageCopy.toStdString(); // the one we are going to store in our vector (unencrypted)

        if (message_to_sendStd.length() >= 128) {
            sendError("Message is too long! Cannot be longer than 128 characters.");
            return;
        }

        if (message_to_sendStd.length() == 0) {
            sendError("Message is empty! Cannot send empty message!.");
            return;
        }

        if (m_selfUsername == username_to_sendStd.substr(4)) {
            sendMessageToScreenSend(m_messageToSend);
            return;
        }

        if ((m_messages).find(m_usernameToSend) == m_messages.end() || m_messages[m_usernameToSend] == nullptr) {
            UserMessage* userMsg = new UserMessage(QString::fromStdString(username_to_sendStd));
            m_messages[m_usernameToSend] = userMsg;
        }

        (m_messages)[m_usernameToSend]->addMessage((std::make_pair(CURR_USER, m_messageToSend.toStdString())));

        m_network.sendMsg(message_to_sendStd, username_to_sendStd, MSG_SEND);

        sendMessageToScreenSend(m_messageToSend);
    }

    else {
        std::string group_to_sendStd = m_groupToSend.toStdString(); // group name not coming correctly
        std::string message_to_sendStd = messageCopy.toStdString(); // the one we are going to store in our vector (unencrypted)

        if (message_to_sendStd.length() >= 128) {
            sendError("Message is too long! Cannot be longer than 128 characters.");
            return;
        }

        if (message_to_sendStd.length() == 0) {
            sendError("Message is empty! Cannot send empty message!.");
            return;
        }
        m_groupMessages[m_groupToSend]->addMessage(m_selfUsername, CURR_USER, m_selfUsername + " : " + m_messageToSend);

        m_network.sendMsg(message_to_sendStd, group_to_sendStd, ROOM_MSG);

        sendMessageToScreenSend(m_selfUsername + " : " + m_messageToSend);
    }
}

void ChattingWindow::addGroup(const char group[USERNAME_LENGTH]) {
    std::string group_toadd(group);

    QMetaObject::invokeMethod(this, [=] { this->addGrouptoScreen(QString::fromStdString(group_toadd)); }, Qt::QueuedConnection);
}

void ChattingWindow::addMessage_group(char message[MESSAGE_LENGTH], char username[USERNAME_LENGTH], char group[USERNAME_LENGTH]) {
    std::string username_toadd(username);
    std::string message_toadd(message);
    QString message_r = QString::fromStdString(message_toadd);
    encrypt(message_r); //unencrypt the message
    message_toadd = message_r.toStdString();
    std::string group_toadd(group);

    m_groupMessages[QString::fromStdString(group_toadd)]->addMessage(QString::fromStdString(username_toadd), OTHER_USER, QString::fromStdString(username_toadd) + ":" + message_r);

    //we'll be able to display right away to screen, needs to run on the gui thread (main thread)
    QMetaObject::invokeMethod(this, [=] { this->sendMessageToScreenRecv(QString::fromStdString(username_toadd + " : " + message_toadd), QString::fromStdString(group_toadd), GroupB); }, Qt::QueuedConnection);
}

void ChattingWindow::addMessage(char message[MESSAGE_LENGTH], char username[USERNAME_LENGTH]) {
    std::string username_toadd(username);

    std::string message_toadd(message);
    QString message_r = QString::fromStdString(message_toadd);
    encrypt(message_r); //unencrypt the message
    message_toadd = message_r.toStdString();

    if (m_messages.find(QString::fromStdString(username_toadd)) != m_messages.end()) {
        m_messages[QString::fromStdString(username_toadd)]->addMessage(std::make_pair(OTHER_USER, message_toadd));
    }

    else {
        UserMessage* user_msg = new UserMessage(QString::fromStdString(username_toadd));
        m_messages[QString::fromStdString(username_toadd)] = user_msg;
        user_msg->addMessage(std::make_pair(OTHER_USER, message_toadd));
    }

    //we'll be able to display right away to screen, since this function will be called by recv thread, cannot create element here so queue it on main thread
    QMetaObject::invokeMethod(this, [=] { this->sendMessageToScreenRecv(QString::fromStdString(message_toadd), QString::fromStdString(username_toadd), UserB); }, Qt::QueuedConnection);
}

void ChattingWindow::addUsers(char users[MAXUSERS][USERNAME_LENGTH], uint32_t size) {
    //just goes through the list of users when its updated from the server end, and adds any new ones.
    //better logic will be implemented later from server side soon
    for (std::size_t i{0}; i < size; i++) {
        std::string username(users[i]);
        QString usernamee = QString::fromStdString(username);

        //since this function will be called by recv thread, cannot create element here so queue it on main thread
        if ((m_Users).find(usernamee) == (m_Users).end()) {
            QMetaObject::invokeMethod(this, [=] { this->sendUserToScreen(usernamee); }, Qt::QueuedConnection);
        }
    }
    return;
}

void ChattingWindow::addGroups(char groups[MAXUSERS][USERNAME_LENGTH], uint32_t size) {
    //just goes through the list of users when its updated from the server end, and adds any new ones.
    //better logic will be implemented later from server side soon
    for (std::size_t i{0}; i < size; i++) {
        std::string group(groups[i]);
        QString groupp = QString::fromStdString(group);

        //since this function will be called by recv thread, cannot create element here so queue it on main thread
        if ((m_Groups).find(groupp) == (m_Groups).end()) {
            QMetaObject::invokeMethod(this, [=] { this->addGrouptoScreen(groupp); }, Qt::QueuedConnection);
        }
    }
    return;
}

void ChattingWindow::removeUsers(char user[USERNAME_LENGTH], uint32_t size) {
    std::string user_to_remove(user);
    QMetaObject::invokeMethod(this, [=] { this->removeUserfromScreen(QString::fromStdString(user_to_remove)); }, Qt::QueuedConnection);
    return;
}

void ChattingWindow::addGrouptoScreen(const QString group_n) {
    QPushButton* group = new QPushButton(this);

    GroupMessage* groupMsg = new GroupMessage(group_n);

    m_Groups.insert(std::make_pair(group_n, group));
    m_groupMessages.insert(std::make_pair(group_n, groupMsg));

    group->setText(group_n);
    group->setMinimumSize(205, 40);
    group->setStyleSheet(m_defaultButtonStylesheet);

    if (m_userOrGroup == UserB) {
        group->hide();
    }

    m_ui.userLayout->addWidget(group, 0, Qt::AlignCenter | Qt::AlignTop);
    connect(group, &QPushButton::clicked, this, &ChattingWindow::onGroupClick);
}

void ChattingWindow::removeUserfromScreen(const QString& user) {
    if ((m_Users)[user]) {
        if (m_lastPressedUser == (m_Users)[user]) {
            m_lastPressedUser = nullptr;
            //need to remove messages 
            removeAllChatItemsFromScreen();
            m_ui.username_label->setText("Select a User to talk to!");
        }
        m_ui.userLayout->removeWidget((m_Users)[user]);
        (m_Users)[user]->hide();
        (m_Users)[user]->deleteLater();
        (m_Users)[user] = nullptr;
        m_Users.erase(user);
        delete m_messages[user];
        m_messages.erase(user);
        //remove from the map as well
    }
    return;
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

    auto* bubble = new MessageWidget (message, this);

    m_ui.chatLayout->addWidget(bubble, 0, Qt::AlignLeft);

    m_ui.scrollArea->verticalScrollBar()->setValue(m_ui.scrollArea->verticalScrollBar()->maximum());

    QTimer::singleShot(0, this, [=]() {
       m_ui.scrollArea->ensureWidgetVisible(bubble);
    });
}

void ChattingWindow::sendMessageToScreenSend(const QString& message) {
    auto* bubble = new MessageWidget_s(message, this);

    m_ui.chatLayout->addWidget(bubble, 0, Qt::AlignRight);

    m_ui.scrollArea->verticalScrollBar()->setValue(m_ui.scrollArea->verticalScrollBar()->maximum());

    QTimer::singleShot(0, this, [=]() {
        m_ui.scrollArea->ensureWidgetVisible(bubble);
        });
}

void ChattingWindow::sendUserToScreen(QString username) {
    QPushButton* user = new QPushButton(this);

    if(username == m_selfUsername) {
        user->setText("You:" + username);
    }
    else {
        user->setText(username);
    }
    user->setMinimumSize(205, 40);
    user->setStyleSheet(m_defaultButtonStylesheet);

    m_ui.userLayout->addWidget(user, 0, Qt::AlignCenter | Qt::AlignTop);

    if (m_userOrGroup == GroupB) {
        user->hide();
    }
    m_messages[username] = new UserMessage(username);
    m_Users.insert(std::make_pair(username, user));

    connect(user, &QPushButton::clicked, this, &ChattingWindow::onUserClick);

    return;
}

void ChattingWindow::sendError(const QString& error_message) {
    QMessageBox msgBox;
    msgBox.setText(error_message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    return;
}

void ChattingWindow::removeAllChatItemsFromScreen() {
    for (int i = 0; i < m_ui.chatLayout->count(); ++i) {
        QLayoutItem* item = m_ui.chatLayout->itemAt(i);
        QWidget* w = nullptr;
        if (item != nullptr) { w = item->widget(); }
        if (!w) continue;

        if (qobject_cast<QPushButton*>(w)) {
            w->hide();              // stays in layout, can be shown later
        }
        else {
            m_ui.chatLayout->removeWidget(w);
            w->deleteLater();
            --i;
        }
    }
}



//should only run once
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

void ChattingWindow::initUI() {
    m_ui.setupUi(this);
    //we're just creating a layout for scrolling and out vertical layout
    QWidget* contents = m_ui.scrollArea->takeWidget();
    QVBoxLayout* layout = new QVBoxLayout(contents);

    layout->setContentsMargins(0, 0, 0, 0);
    contents->setLayout(layout);
    m_ui.scrollArea->setWidget(contents);

    m_ui.chatLayout = layout;
    layout->setSpacing(5);
    layout->setSizeConstraint(QLayout::SetMinimumSize);

    //we're just creating a layout for scrolling and out vertical layout
    QWidget* contents_users = m_ui.scrollArea_2->takeWidget();
    QVBoxLayout* layout_users = new QVBoxLayout(contents_users);

    layout_users->setContentsMargins(0, 0, 0, 0);
    contents_users->setLayout(layout_users);
    m_ui.scrollArea_2->setWidget(contents_users);

    m_ui.userLayout = layout_users;
    layout_users->setSpacing(5);
    layout_users->setSizeConstraint(QLayout::SetMinimumSize);

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

    m_ui.userLayout->setContentsMargins(10, 5, 10, 0); // 10px left/right margins

    m_ui.chatLayout->setContentsMargins(10, 10, 10, 10); // 10px left/right margins

    auto* l = static_cast<QVBoxLayout*>(m_ui.scrollAreaWidgetContents->layout());
    l->insertStretch(0, 1);

    m_ui.chatLayout->addStretch(1);  // Push all bubbles to top

    m_ui.title_label->setFont(m_titleFont);

    m_ui.addGroup->setFont(m_buttonAddGroupFont);
    m_ui.userList->setFont(m_buttonFont);
    m_ui.groupChat->setFont(m_buttonFont);
    m_ui.Message_input->setFont(m_buttonFont);
    m_ui.sendButton->setIcon(QIcon("icon.png"));
    m_ui.sendButton->setIconSize(QSize(45, 37));

    m_ui.fileButton->setIcon(QIcon("fileupload.png"));
    m_ui.fileButton->setIconSize(QSize(45, 37));

    m_ui.chatLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    m_ui.addGroup->hide();
}