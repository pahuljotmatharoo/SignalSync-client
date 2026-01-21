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
// BUG: Fix name sending of custom size instead of 50 byte cap
// feature: custom png name sending

ChattingWindow::ChattingWindow(QWidget* parent) : QMainWindow(parent), m_lastPressedUser(nullptr), m_threadStop(false),
                                                    m_lastPressedGroup(nullptr), m_messageFont("Montserrat", 14), m_titleFont("Montserrat", 25), 
                                                    m_userOrGroup(User), m_buttonAddGroupFont("Montserrat", 8), m_buttonFont("Montserrat", 10)
{
    initUI();

    initEncryptMap();

    m_thread = std::thread(&ChattingWindow::threadFunction, this);
}

ChattingWindow::~ChattingWindow() {
    destroyPngs();
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
                    MsgRecvUser* recvStruct = m_network.recvMethod<MsgRecvUser>();
                    if (recvStruct == nullptr) { continue; }
                    m_mutex.lock();
                    addMessage(recvStruct->message, recvStruct->user_from);
                    m_mutex.unlock();
                    delete recvStruct;
                    break;
                }
                case MSG_LIST: {
                    List* list = m_network.recvMethod<List>();
                    if (list == nullptr) { continue; }
                    list->size = ntohl(list->size);
                    m_mutex.lock();
                    addUsers(list->arr, list->size);
                    m_mutex.unlock();
                    delete list;
                    break;
                }
                case USER_EXIT: {
                    char* username = m_network.recvUser();
                    if (username == nullptr) { continue; }
                    m_mutex.lock();
                    removeUsers(username, USERNAME_LENGTH);
                    m_mutex.unlock();
                    delete[] username;
                    break;
                }
                case ROOM_CREATE: {
                    RecvGroupName* groupName = m_network.recvMethod<RecvGroupName>();
                    if (groupName == nullptr) { continue; }
                    m_mutex.lock();
                    addGroup(groupName->groupName);
                    m_mutex.unlock();
                    delete groupName;
                    break;
                }
                case ROOM_MSG: {
                    MsgRecvGroup* recvGrpMsg = m_network.recvMethod<MsgRecvGroup>();
                    if (recvGrpMsg == nullptr) { continue; }
                    m_mutex.lock();
                    addMessage_group(recvGrpMsg->message, recvGrpMsg->user_from, recvGrpMsg->group_name);
                    m_mutex.unlock();
                    delete recvGrpMsg;
                    break;
                }
                case ROOM_LIST: {
                    List* listGroup = m_network.recvMethod<List>();
                    if (listGroup == nullptr) { continue; }
                    listGroup->size = ntohl(listGroup->size);
                    if (listGroup->size > MAXUSERS) {
                        delete listGroup;
                        break;
                    }
                    m_mutex.lock();
                    addGroups(listGroup->arr, listGroup->size);
                    m_mutex.unlock();
                    delete listGroup;
                    break;
                }
                case PNG_IMG: {
                    uint32_t* sizePng = m_network.recvMethod<uint32_t>();
                    if (sizePng == nullptr) { continue; }
                    char* pngData = m_network.recvPng(*sizePng);
                    if (pngData == nullptr) { continue; }
                    char* userFrom = m_network.recvUser();
                    if (userFrom == nullptr) { continue; }
                    char* fileName = m_network.recvUser();
                    if (fileName == nullptr) { continue; }
                    m_mutex.lock();
                    std::string userString(userFrom);
                    std::string filenameString(fileName);
                    processPngRecv(*sizePng, pngData, userString, filenameString);
                    m_mutex.unlock();
                    delete sizePng;
                    delete userFrom;
                }
            }
        }
        else {
            break;
        }
    }
}

void ChattingWindow::destroyPngs() {
    for (auto itr = m_Pngs.begin(); itr != m_Pngs.end(); ++itr) {
        char* pngData = std::get<1>(itr->second);
        delete pngData;
    }
}

void ChattingWindow::processPngRecv(uint32_t sizePng, char* pngData, std::string userFrom, std::string fileName) {
    QMetaObject::invokeMethod(this, [=] { this->addPngButtonToScreen(sizePng, pngData, userFrom, fileName); }, Qt::QueuedConnection);
}

void ChattingWindow::downloadPng() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    std::string fileName = btn->text().toStdString();
    auto& png = m_Pngs[btn];
    char* pngArr = std::get<1>(png);
    uint32_t pngSize = std::get<2>(png);

    std::ofstream outputFile(fileName, std::ios::binary);
    outputFile.write(pngArr, pngSize);
    outputFile.close();
}

QPushButton* ChattingWindow::createAndStylePngButton(std::string& fileName) {
    QPushButton* button = new QPushButton(this);

    button->setText(QString::fromStdString(fileName));
    button->setMinimumSize(205, 40);
    button->setStyleSheet(m_defaultButtonStylesheet);
    button->setIcon(QIcon("download.png"));

    button->hide();

    connect(button, &QPushButton::clicked, this, &ChattingWindow::downloadPng);


    return button;
}


void ChattingWindow::addPngButtonToScreen(uint32_t sizePng, char* pngData, const std::string userFrom, std::string fileName) {
    QPushButton* button = createAndStylePngButton(fileName);

    //definately have a method that does this
    m_ui.chatLayout->addWidget(button, 0, Qt::AlignLeft);

    m_ui.scrollArea->verticalScrollBar()->setValue(m_ui.scrollArea->verticalScrollBar()->maximum());

    QTimer::singleShot(0, this, [=]() {
        m_ui.scrollArea->ensureWidgetVisible(button);
        });

    if (m_lastPressedUser != nullptr && QString::fromStdString(userFrom) == m_lastPressedUser->text()) {
        button->show();
    }

    m_Pngs.insert(std::make_pair(button, std::make_tuple(QString::fromStdString(userFrom), pngData, sizePng)));
}

std::pair<QPushButton*, QString> ChattingWindow::createAndStyleGroupButton() {
    if (m_Groups.size() >= 10) {
        send_error("Already at a maximum number of Groups!");
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
//------------------------------------------------ ELEMENTARY FUNCTIONS (SETTERS) ---------------------------------------------------------------------
void ChattingWindow::on_Message_input_textEdited(const QString& text) {
    m_messageToSend = text;
    return;
}
//----------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------ BUTTON CLICKED FUNCTIONS---------------------------------------------------------------------

void ChattingWindow::on_fileButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Select one or more files to open", "/home", "PNG Images (*.png)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray content = file.readAll();
            QFileInfo fileInfo(file);
            QString fileName = fileInfo.fileName();
            m_network.sendPng(&content, m_usernameToSend.toStdString(), fileName.toStdString());
            file.close();
        }
    }
}

void ChattingWindow::on_addGroup_clicked() {
    auto buttonAndName = createAndStyleGroupButton(); // its a refernce

    if (buttonAndName.first == nullptr) { return; }

    connect(buttonAndName.first, &QPushButton::clicked, this, &ChattingWindow::onGroupClick);
    std::string name_to_sendStd = buttonAndName.second.toStdString();

    m_network.sendGroupName(name_to_sendStd);
}

void ChattingWindow::userOrGroupSelect(std::unordered_map<QString, QPushButton*> &hide, std::unordered_map<QString, QPushButton*> &show, QPushButton*& lastPressedButton) {
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
    m_userOrGroup = Group;

    m_ui.currUsers_label->setText("Current Groups");

    userOrGroupSelect(m_Users, m_Groups, m_lastPressedUser);
}

//can't really simplify this i think as it is tied to the button itself
void ChattingWindow::on_userList_clicked() {
    m_ui.addGroup->hide();
    m_userOrGroup = User;

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
    auto& vec_msg = (m_Messages)[m_usernameToSend];

    displayAllUserMessages<std::vector<std::pair<bool, std::string>>>(vec_msg, m_usernameToSend);
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
    auto& vec_msg = (m_groupMessages)[m_groupToSend];

    displayAllUserMessages<std::vector<std::pair<bool, std::pair<QString, std::string>>>>(vec_msg, m_groupToSend);
}

//weird behaviour where its encrypted once then unencrypted...
void ChattingWindow::on_sendButton_clicked() {
    QString messageCopy = m_messageToSend; // unencrypted message
    encrypt(messageCopy);
    if (m_userOrGroup == User) {
        std::string username_to_sendStd = m_usernameToSend.toStdString();

        std::string message_to_sendStd = messageCopy.toStdString(); // the one we are going to store in our vector (unencrypted)

        if (message_to_sendStd.length() >= 128) {
            send_error("Message is too long! Cannot be longer than 128 characters.");
            return;
        }

        if (message_to_sendStd.length() == 0) {
            send_error("Message is empty! Cannot send empty message!.");
            return;
        }

        if (m_selfUsername == username_to_sendStd.substr(4)) {
            sendMessageToScreenSend(m_messageToSend);
            return;
        }

        (m_Messages)[m_usernameToSend].push_back(std::make_pair(CURR_USER, m_messageToSend.toStdString()));

        m_network.sendMsg(message_to_sendStd, username_to_sendStd, MSG_SEND);

        sendMessageToScreenSend(m_messageToSend);
    }

    else {
        std::string group_to_sendStd = m_groupToSend.toStdString(); // group name not coming correctly
        std::string message_to_sendStd = messageCopy.toStdString(); // the one we are going to store in our vector (unencrypted)

        if (message_to_sendStd.length() >= 128) {
            send_error("Message is too long! Cannot be longer than 128 characters.");
            return;
        }

        if (message_to_sendStd.length() == 0) {
            send_error("Message is empty! Cannot send empty message!.");
            return;
        }

        (m_groupMessages)[m_groupToSend].push_back(std::make_pair(CURR_USER, std::make_pair(m_selfUsername, m_messageToSend.toStdString())));

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

    (m_groupMessages)[QString::fromStdString(group)].push_back(std::make_pair(OTHER_USER, std::make_pair(QString::fromStdString(username_toadd), message_toadd)));

    //we'll be able to display right away to screen, needs to run on the gui thread (main thread)
    QMetaObject::invokeMethod(this, [=] { this->sendMessageToScreenRecv(QString::fromStdString(username_toadd + " : " + message_toadd), QString::fromStdString(group_toadd), Group); }, Qt::QueuedConnection);
}

void ChattingWindow::addMessage(char message[MESSAGE_LENGTH], char username[USERNAME_LENGTH]) {
    std::string username_toadd(username);

    std::string message_toadd(message);
    QString message_r = QString::fromStdString(message_toadd);
    encrypt(message_r); //unencrypt the message
    message_toadd = message_r.toStdString();

    (m_Messages)[QString::fromStdString(username_toadd)].push_back(std::make_pair(OTHER_USER, message_toadd));

    //we'll be able to display right away to screen, since this function will be called by recv thread, cannot create element here so queue it on main thread
    QMetaObject::invokeMethod(this, [=] { this->sendMessageToScreenRecv(QString::fromStdString(message_toadd), QString::fromStdString(username_toadd), User); }, Qt::QueuedConnection);
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

void ChattingWindow::addGrouptoScreen(const QString& group_n) {
    QPushButton* group = new QPushButton(this);

    m_Groups.insert(std::make_pair(group_n, group));

    group->setText(group_n);
    group->setMinimumSize(205, 40);
    group->setStyleSheet(m_defaultButtonStylesheet);

    if (m_userOrGroup == User) {
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
        m_Messages.erase(user);
        //remove from the map as well
    }
    return;
}

//check check where we check if we are on the correct person, only then send to screen
void ChattingWindow::sendMessageToScreenRecv(const QString& message, const QString& user, bool type) {
    if (!m_lastPressedGroup && !m_lastPressedUser) { return; }
    if (type == User) {
        if (m_lastPressedGroup) { return; }
        else if (m_lastPressedUser && m_lastPressedUser->text() != user) { return; }
    }
    if (type == Group) {
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

    if (m_userOrGroup == Group) {
        user->hide();
    }

    m_Users.insert(std::make_pair(username, user));

    connect(user, &QPushButton::clicked, this, &ChattingWindow::onUserClick);

    return;
}

void ChattingWindow::send_error(const QString& error_message) {
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

    m_ui.chatLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    m_ui.addGroup->hide();
}