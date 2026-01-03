#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <ws2tcpip.h>
#include <windows.h>
#include <QHBoxLayout>
#include <qscrollbar.h>
#include <qlayout.h>
#include <qtimer.h>
#include <QMessageBox>
#include "message.h"
#include "message_s.h"
#include "ChattingWindow.h"

constexpr auto OTHER_USER = true;
constexpr auto CURR_USER = false;
constexpr auto User = true;
constexpr auto Group = false;

ChattingWindow::ChattingWindow(QWidget* parent) : QMainWindow(parent), m_lastPressedUser(nullptr), 
                                                    m_lastPressedGroup(nullptr), m_messageFont("Montserrat", 14), m_titleFont("Montserrat", 25), m_userOrGroup(User), m_buttonAddGroupFont("Montserrat", 8), m_buttonFont("Montserrat", 10)
{
    initUI();

    initEncryptMap();

    m_thread = std::thread(&ChattingWindow::threadFunction, this);
    m_thread.detach();
}

ChattingWindow::~ChattingWindow()
{
    m_network.sendInitMsg(MSG_EXIT);
}

void ChattingWindow::threadFunction()
{
    int type = 0;
    std::size_t recvData;
    while ( (recvData = recv(m_network.getSockID(), reinterpret_cast<char*>(&type), sizeof(int), 0)) > 0) {
        switch (type) {
            case MSG_SEND: {
                MsgRecvUser* recvStruct = m_network.recvMethod<MsgRecvUser>();;
                addMessage(recvStruct->message, recvStruct->user_from);
                delete recvStruct;
                break;
            }
            case MSG_LIST: {
                List* list = m_network.recvMethod<List>();
                list->size = ntohl(list->size);
                addUsers(list->arr, list->size);
                delete list;
                break;
            }
            case USER_EXIT: {
                char* username = m_network.recvUser();
                removeUsers(username, USERNAME_LENGTH);
                delete[] username;
                break;
            }
            case ROOM_CREATE: {
                RecvGroupName* groupName = m_network.recvMethod<RecvGroupName>();
                addGroup(groupName->groupName);
                delete groupName;
                break;
            }
            case ROOM_MSG: {
                MsgRecvGroup* recvGrpMsg = m_network.recvMethod<MsgRecvGroup>();
                addMessage_group(recvGrpMsg->message, recvGrpMsg->user_from, recvGrpMsg->group_name);
                delete recvGrpMsg;
                break;
            }
            case ROOM_LIST: {
                List* listGroup = m_network.recvMethod<List>();
                listGroup->size = ntohl(listGroup->size);
                if (listGroup->size > MAXUSERS) {
                    delete listGroup;
                    break;
                }
                addGroups(listGroup->arr, listGroup->size);
                delete listGroup;
                break;
            }
        }
    }
}

//------------------------------------------------ ELEMENTARY FUNCTIONS (SETTERS) ---------------------------------------------------------------------

void ChattingWindow::on_Message_input_textEdited(const QString& text)
{
    m_messageToSend = text;
    return;
}
//----------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------ BUTTON CLICKED FUNCTIONS---------------------------------------------------------------------
void ChattingWindow::on_addGroup_clicked()
{
    if (m_Groups.size() >= 10) {
        send_error("Already at a maximum number of Groups!");
        return;
    }

    QPushButton* group = new QPushButton(this);

    QString groupName = QString("Group %1").arg(m_Groups.size() + 1);

    group->setText(groupName);
    group->setMinimumSize(205, 40);
    group->setStyleSheet(m_defaultButtonStylesheet);

    m_Groups.insert(std::make_pair(groupName, group));

    m_ui.userLayout->addWidget(group, 0, Qt::AlignCenter | Qt::AlignTop);

    connect(group, &QPushButton::clicked, this, &ChattingWindow::onGroupClick);

    std::string name_to_sendStd = groupName.toStdString();
    const char* name_to_sendCStr = name_to_sendStd.c_str();;

    m_network.sendGroupName(name_to_sendStd);
}

void ChattingWindow::on_groupChat_clicked()
{
    m_ui.addGroup->show();
    m_userOrGroup = Group;

    m_ui.currUsers_label->setText("Current Groups");

    //hide all the users
    if (m_Users.size() != 0) {
        for (auto itr = m_Users.begin(); itr != m_Users.end(); itr++) {
            itr->second->hide();
        }
    }

    //show all the groups
    if (m_Groups.size() != 0) {
        for (auto itr = m_Groups.begin(); itr != m_Groups.end(); itr++) {
            itr->second->show();
        }
    }

    if (m_lastPressedUser) {
        m_lastPressedUser->setStyleSheet(m_defaultButtonStylesheet);
        m_lastPressedUser = nullptr;
    }
}

void ChattingWindow::on_userList_clicked()
{
    m_ui.addGroup->hide();
    m_userOrGroup = User;

    m_ui.currUsers_label->setText("Current Users");

    if (m_Groups.size() != 0) {
        for (auto itr = m_Groups.begin(); itr != m_Groups.end(); itr++) {
            itr->second->hide();
        }
    }

    if (m_Users.size() != 0) {
        for (auto itr = m_Users.begin(); itr != m_Users.end(); itr++) {
            itr->second->show();
        }
    }

    if (m_lastPressedGroup) {
        m_lastPressedGroup->setStyleSheet(m_defaultButtonStylesheet);
        m_lastPressedGroup = nullptr;
    }
}

//onUserClick and onGroupClick are too similar 
void ChattingWindow::onUserClick()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());

    if (m_lastPressedUser == clickedButton) {
        return;
    }

    if (m_lastPressedUser) {
        m_lastPressedUser->setStyleSheet(m_defaultButtonStylesheet);
    }

    clickedButton->setStyleSheet(m_pressedButtonStylesheet);
    m_lastPressedUser = clickedButton;

    removeMessagesFromScreen();

    m_ui.username_label->setText(clickedButton->text());
    m_usernameToSend = clickedButton->text();
    auto& vec_msg = (m_Messages)[m_usernameToSend];

    for (std::size_t i = 0; i < vec_msg.size(); i++) {

        if (vec_msg[i].first == CURR_USER) {
            auto* bubble = new MessageWidget_s(QString::fromStdString(vec_msg[i].second), this);
            m_ui.chatLayout->addWidget(bubble, 0, Qt::AlignRight);
            QTimer::singleShot(0, this, [=]() {
                m_ui.scrollArea->ensureWidgetVisible(bubble);
                });
        }
        else {
            auto* bubble = new MessageWidget(QString::fromStdString(vec_msg[i].second), this);
            m_ui.chatLayout->addWidget(bubble, 0, Qt::AlignLeft);
            QTimer::singleShot(0, this, [=]() {
                m_ui.scrollArea->ensureWidgetVisible(bubble);
                });
        }
    }
    return;
}

void ChattingWindow::onGroupClick()
{
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
    removeMessagesFromScreen();

    m_ui.username_label->setText(clickedButton->text());
    m_groupToSend = clickedButton->text();
    auto& vec_msg = (m_groupMessages)[m_groupToSend];

    for (std::size_t i = 0; i < vec_msg.size(); i++) {
        auto* bubble = new MessageWidget(vec_msg[i].second.first + ": " + QString::fromStdString(vec_msg[i].second.second), this);
        if(vec_msg[i].first == CURR_USER) {
            auto* bubble = new MessageWidget_s(vec_msg[i].second.first + ": " + QString::fromStdString(vec_msg[i].second.second), this);
            m_ui.chatLayout->addWidget(bubble, 0, Qt::AlignRight);
            QTimer::singleShot(0, this, [=]() {
                m_ui.scrollArea->ensureWidgetVisible(bubble);
                });
        }
        else {
            auto* bubble = new MessageWidget(vec_msg[i].second.first + ": " + QString::fromStdString(vec_msg[i].second.second), this);
            m_ui.chatLayout->addWidget(bubble, 0, Qt::AlignLeft);
            QTimer::singleShot(0, this, [=]() {
                m_ui.scrollArea->ensureWidgetVisible(bubble);
                });
        }

    }
    return;
}

void ChattingWindow::on_sendButton_clicked()
{
    if (m_userOrGroup == User) {
        std::string username_to_sendStd = m_usernameToSend.toStdString();

        //check to see if sending to ourselves
        std::string username_copy = username_to_sendStd;
        username_copy.erase(0, 4);
        if (username_copy == m_selfUsername) {
            username_to_sendStd = username_copy;
        }

        const char* username_to_sendCStr = username_to_sendStd.c_str();

        QString messageCopy = m_messageToSend; // temp copy of the message for user display side
        std::string message_to_sendStd = messageCopy.toStdString(); // the one we are going to store in our vector (unencrypted)
        (m_Messages)[m_usernameToSend].push_back(std::make_pair(CURR_USER, message_to_sendStd));

        long size = messageCopy.length();

        //now we can do the message encryption of the message sending to server
        //encrypt(messageToSend);
        message_to_sendStd = m_messageToSend.toStdString();
        const char* message_to_sendCStr = message_to_sendStd.c_str();

        if (message_to_sendStd.length() >= 128) {
            send_error("Message is too long! Cannot be longer than 128 characters.");
            return;
        }

        if (message_to_sendStd.length() == 0) {
            send_error("Message is empty! Cannot send empty message!.");
            return;
        }

        m_network.sendMsg(message_to_sendStd, username_to_sendStd, MSG_SEND);

        auto* bubble = new MessageWidget_s(messageCopy, this);

        m_ui.chatLayout->addWidget(bubble, 0, Qt::AlignRight);

        m_ui.scrollArea->verticalScrollBar()->setValue(m_ui.scrollArea->verticalScrollBar()->maximum());

        QTimer::singleShot(0, this, [=]() {
            m_ui.scrollArea->ensureWidgetVisible(bubble);
            });

    }

    else {
        std::string group_to_sendStd = m_groupToSend.toStdString(); // group name not coming correctly
        const char* group_to_sendCStr = group_to_sendStd.c_str();

        QString messageCopy = m_messageToSend; // temp copy of the message for user display side
        std::string message_to_sendStd = messageCopy.toStdString(); // the one we are going to store in our vector (unencrypted)
        (m_groupMessages)[m_groupToSend].push_back(std::make_pair(CURR_USER, std::make_pair(m_selfUsername, message_to_sendStd)));

        //now we can do the message encryption of the message sending to server
        //encrypt(messageToSend);
        message_to_sendStd = m_messageToSend.toStdString();
        const char* message_to_sendCStr = message_to_sendStd.c_str();

        if (message_to_sendStd.length() >= 128) {
            send_error("Message is too long! Cannot be longer than 128 characters.");
            return;
        }

        if (message_to_sendStd.length() == 0) {
            send_error("Message is empty! Cannot send empty message!.");
            return;
        }

        m_network.sendMsg(message_to_sendStd, group_to_sendStd, ROOM_MSG);

        auto* bubble = new MessageWidget_s(m_selfUsername + " : " + m_messageToSend, this);

        m_ui.chatLayout->addWidget(bubble, 0, Qt::AlignRight);

        m_ui.scrollArea->verticalScrollBar()->setValue(m_ui.scrollArea->verticalScrollBar()->maximum());

        QTimer::singleShot(0, this, [=]() {
            m_ui.scrollArea->ensureWidgetVisible(bubble);
            });

        (m_groupMessages)[m_groupToSend].push_back(std::make_pair(CURR_USER, std::make_pair(m_selfUsername, message_to_sendStd)));
    }
    return;
}

//----------------------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------- ADDING/REMOVING TO DATA STRUCTURE FUNCTIONS ---------------------------------------------------------------------

void ChattingWindow::addGroup(const char group[USERNAME_LENGTH])
{
    std::string group_toadd(group);

    QMetaObject::invokeMethod(this, [=] { this->addGrouptoScreen(QString::fromStdString(group_toadd)); }, Qt::QueuedConnection);
}

void ChattingWindow::addMessage_group(char message[MESSAGE_LENGTH], char username[USERNAME_LENGTH], char group[USERNAME_LENGTH])
{
    std::string username_toadd(username);
    std::string message_toadd(message);
    QString message_r = QString::fromStdString(message_toadd);
    //encrypt(message_r); //unencrypt the message
    message_toadd = message_r.toStdString();
    std::string group_toadd(group);

    (m_groupMessages)[QString::fromStdString(group)].push_back(std::make_pair(OTHER_USER, std::make_pair(QString::fromStdString(username_toadd), message_toadd)));

    //we'll be able to display right away to screen, needs to run on the gui thread (main thread)
    QMetaObject::invokeMethod(this, [=] { this->sendMessageToScreen(QString::fromStdString(username_toadd + " : " + message_toadd), group_toadd, false); }, Qt::QueuedConnection);
}

void ChattingWindow::addMessage(char message[MESSAGE_LENGTH], char username[USERNAME_LENGTH])
{
    std::string username_toadd(username);

    std::string message_toadd(message);
    QString message_r = QString::fromStdString(message_toadd);
    //encrypt(message_r); //unencrypt the message
    message_toadd = message_r.toStdString();

    (m_Messages)[QString::fromStdString(username_toadd)].push_back(std::make_pair(OTHER_USER, message_toadd));

    //we'll be able to display right away to screen, since this function will be called by recv thread, cannot create element here so queue it on main thread
    QMetaObject::invokeMethod(this, [=] { this->sendMessageToScreen(QString::fromStdString(message_toadd), username_toadd, true); }, Qt::QueuedConnection);
}

void ChattingWindow::addUsers(char users[MAXUSERS][USERNAME_LENGTH], uint32_t size) {
    //just goes through the list of users when its updated from the server end, and adds any new ones.
    //better logic will be implemented later from server side soon
    for (std::size_t i = 0; i < size; i++) {
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
    for (std::size_t i = 0; i < size; i++) {
        std::string group(groups[i]);
        QString groupp = QString::fromStdString(group);

        //since this function will be called by recv thread, cannot create element here so queue it on main thread
        if ((m_Groups).find(groupp) == (m_Groups).end()) {
            QMetaObject::invokeMethod(this, [=] { this->addGrouptoScreen(groupp); }, Qt::QueuedConnection);
        }
    }
    return;
}

void ChattingWindow::removeUsers(char user[USERNAME_LENGTH], uint32_t size)
{
    std::string user_to_remove(user);
    QMetaObject::invokeMethod(this, [=] { this->removeUserfromScreen(QString::fromStdString(user_to_remove)); }, Qt::QueuedConnection);
    return;
}

//----------------------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------- ADDING/REMOVING TO SCREEN FUNCTIONS ---------------------------------------------------------------------
void ChattingWindow::addGrouptoScreen(const QString& group_n)
{
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

void ChattingWindow::removeUserfromScreen(const QString& user)
{
    if ((m_Users)[user]) {
        if (m_lastPressedUser == (m_Users)[user]) {
            m_lastPressedUser = nullptr;
            //need to remove messages 
            removeMessagesFromScreen();
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

void ChattingWindow::sendMessageToScreen(const QString& message, const std::string &username, bool user)
{
    if (user && m_lastPressedUser && m_lastPressedUser->text() == QString::fromStdString(username)) {

        auto* bubble = new MessageWidget (message, this);

        m_ui.chatLayout->addWidget(bubble, 0, Qt::AlignLeft);

        m_ui.scrollArea->verticalScrollBar()->setValue(m_ui.scrollArea->verticalScrollBar()->maximum());

        QTimer::singleShot(0, this, [=]() {
            m_ui.scrollArea->ensureWidgetVisible(bubble);
            });

    }

    else if (!user && m_lastPressedGroup && m_lastPressedGroup->text() == QString::fromStdString(username)) {
        auto* bubble = new MessageWidget(message, this);

        m_ui.chatLayout->addWidget(bubble, 0, Qt::AlignLeft);

        m_ui.scrollArea->verticalScrollBar()->setValue(m_ui.scrollArea->verticalScrollBar()->maximum());

        QTimer::singleShot(0, this, [=]() {
            m_ui.scrollArea->ensureWidgetVisible(bubble);
            });
    }

    return;
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

//----------------------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------- MISCELLANEOUS ---------------------------------------------------------------------

void ChattingWindow::send_error(const QString& error_message)
{
    QMessageBox msgBox;
    msgBox.setText(error_message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    return;
}

void ChattingWindow::removeMessagesFromScreen() {
    QLayoutItem* item;
    while ((item = m_ui.chatLayout->takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        if (widget != nullptr) {
            widget->deleteLater();
        }
        delete item;
    }
}

//should only run once
void ChattingWindow::initEncryptMap()
{
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

void ChattingWindow::encrypt(QString& message)
{
    for (std::size_t i = 0; i < message.length(); i++) {
        if (message[i] == ' ') {
            continue;
        }
        message[i] = (m_encryptMap)[message[i]];
    }
}

void ChattingWindow::initUI()
{
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
//----------------------------------------------------------------------------------------------------------------------------------------------