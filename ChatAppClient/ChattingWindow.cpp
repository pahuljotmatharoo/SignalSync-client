#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "ChattingWindow.h"
#include "thread_functions.h"
#include "data.h"
#include "commands.h"
#include <inital_message.h>
#include <QHBoxLayout>
#include <message.h>
#include <qscrollbar.h>
#include <qlayout.h>
#include <qtimer.h>
#include <QMessageBox>
constexpr auto MSG_SEND = 1;
constexpr auto MSG_LIST = 2;
constexpr auto MSG_EXIT = 3;
constexpr auto OTHER_USER = true;
constexpr auto CURR_USER = false;
constexpr auto User = true;
constexpr auto Group = false;

//Add dynamically resizing UI

//I did change username -> ourUsername, might have caused some issues 

struct ChattingWindow::Impl {
    SOCKET sock{ INVALID_SOCKET };
    ~Impl() {
        if (sock != INVALID_SOCKET)
            closesocket(sock);
        //WSACleanup();
    }
};

ChattingWindow::ChattingWindow(QWidget* parent) : QMainWindow(parent), ourUsername{ "" }, impl_(new Impl()), lastPressedUser(nullptr), lastPressedGroup(nullptr), messageFont("Montserrat", 14), titleFont("Montserrat", 18), UserOrGroup(User)
{
    ui.setupUi(this);
    //we're just creating a layout for scrolling and out vertical layout
    QWidget* contents = ui.scrollArea->takeWidget();
    QVBoxLayout* layout = new QVBoxLayout(contents);

    layout->setContentsMargins(0, 0, 0, 0);
    contents->setLayout(layout);
    ui.scrollArea->setWidget(contents);

    ui.chatLayout = layout;
    layout->setSpacing(5);
    layout->setSizeConstraint(QLayout::SetMinimumSize);

    //we're just creating a layout for scrolling and out vertical layout
    QWidget* contents_users = ui.scrollArea_2->takeWidget();
    QVBoxLayout* layout_users = new QVBoxLayout(contents_users);

    layout_users->setContentsMargins(0, 0, 0, 0);
    contents_users->setLayout(layout_users);
    ui.scrollArea_2->setWidget(contents_users);

    ui.userLayout = layout_users;
    layout_users->setSpacing(5);
    layout_users->setSizeConstraint(QLayout::SetMinimumSize);

    defaultButtonStylesheet = 
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

    pressedButtonStylesheet = 
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

    Messages = new std::unordered_map<QString, std::vector<std::pair<bool, std::string>>>();
    groupMessages = new std::unordered_map<QString, std::vector<std::pair<bool, std::pair<QString, std::string>>>>();
    Users = new std::unordered_map<QString, QPushButton*>();
    Groups = new std::unordered_map<QString, QPushButton*>();

    ui.userLayout->setContentsMargins(10, 5, 10, 0); // 10px left/right margins

    ui.chatLayout->setContentsMargins(10, 10, 10, 10); // 10px left/right margins

    auto* l = static_cast<QVBoxLayout*>(ui.scrollAreaWidgetContents->layout());
    l->insertStretch(0, 1);

    ui.chatLayout->addStretch(1);  // Push all bubbles to top

    ui.title_label->setFont(titleFont);

    ui.chatLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    ui.addGroup->hide();
}

ChattingWindow::~ChattingWindow()
{
    send_inital_msg(impl_->sock, MSG_EXIT);

    delete impl_;
    impl_ = nullptr;

    delete Messages;
    Messages = nullptr;

    delete Users;
    Users = nullptr;

    delete Groups;
    Groups = nullptr;
}




//------------------------------------------------ ELEMENTARY FUNCTIONS (SETTERS) ---------------------------------------------------------------------
void ChattingWindow::setSOCKET(SOCKET newsock) {
    impl_->sock = newsock;
    thread_creator();
    return;
}

void ChattingWindow::setUsername(const QString& new_user)
{
    this->ourUsername = new_user;
    return;
}

void ChattingWindow::on_Message_input_textEdited(const QString& text)
{
    messageToSend = text;
    return;
}
//----------------------------------------------------------------------------------------------------------------------------------------------






//------------------------------------------------ BUTTON CLICKED FUNCTIONS---------------------------------------------------------------------
void ChattingWindow::on_addGroup_clicked()
{
    QPushButton* group = new QPushButton(this);

    QString groupName = QString("Group %1").arg(Groups->size() + 1);

    group->setText(groupName);
    group->setMinimumSize(205, 40);
    group->setStyleSheet(defaultButtonStylesheet);

    Groups->insert(std::make_pair(groupName, group));

    ui.userLayout->addWidget(group, 0, Qt::AlignLeft | Qt::AlignTop);

    connect(group, &QPushButton::clicked, this, &ChattingWindow::onGroupClick);

    std::string name_to_sendStd = groupName.toStdString();
    const char* name_to_sendCStr = name_to_sendStd.c_str();;

    send_chatroom_name(&impl_->sock, name_to_sendCStr);
}

void ChattingWindow::on_groupChat_clicked()
{
    ui.addGroup->show();
    UserOrGroup = Group;


    //hide all the users
    if (Users->size() != 0) {
        for (auto itr = Users->begin(); itr != Users->end(); itr++) {
            itr->second->hide();
        }
    }

    //show all the groups
    if (Groups->size() != 0) {
        for (auto itr = Groups->begin(); itr != Groups->end(); itr++) {
            itr->second->show();
        }
    }

    if (lastPressedUser) {
        lastPressedUser->setStyleSheet(defaultButtonStylesheet);
        lastPressedUser = nullptr;
    }
}

void ChattingWindow::on_userList_clicked()
{
    ui.addGroup->hide();
    UserOrGroup = User;

    if (Groups->size() != 0) {
        for (auto itr = Groups->begin(); itr != Groups->end(); itr++) {
            itr->second->hide();
        }
    }

    if (Users->size() != 0) {
        for (auto itr = Users->begin(); itr != Users->end(); itr++) {
            itr->second->show();
        }
    }

    if (lastPressedGroup) {
        lastPressedGroup->setStyleSheet(defaultButtonStylesheet);
        lastPressedGroup = nullptr;
    }
}

//onUserClick and onGroupClick are too similar 
void ChattingWindow::onUserClick()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());

    if (lastPressedUser == clickedButton) {
        return;
    }

    if (lastPressedUser) {
        lastPressedUser->setStyleSheet(defaultButtonStylesheet);
    }

    clickedButton->setStyleSheet(pressedButtonStylesheet);
    lastPressedUser = clickedButton;

    QLayoutItem* item;
    while ((item = ui.chatLayout->takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        if (widget != nullptr) {
            widget->deleteLater();
        }
        delete item;
    }

    ui.username_label->setText(clickedButton->text());
    usernameToSend = clickedButton->text();
    auto& vec_msg = (*Messages)[usernameToSend];

    for (std::size_t i = 0; i < vec_msg.size(); i++) {
        auto* bubble = new QLabel(QString::fromStdString(vec_msg[i].second), this);
        bubble->setWordWrap(true);
        bubble->setMaximumWidth(500);  // Adjust to how wide you want chat bubbles
        bubble->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

        vec_msg[i].first ? ui.chatLayout->addWidget(bubble, 0, Qt::AlignLeft) : ui.chatLayout->addWidget(bubble, 0, Qt::AlignRight);

        QTimer::singleShot(0, this, [=]() {
            ui.scrollArea->ensureWidgetVisible(bubble);
            });
    }
    return;
}

void ChattingWindow::onGroupClick()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());

    if (lastPressedGroup == clickedButton) {
        return;
    }

    if (lastPressedGroup) {
        lastPressedGroup->setStyleSheet(defaultButtonStylesheet);
    }
    clickedButton->setStyleSheet(pressedButtonStylesheet);

    lastPressedGroup = clickedButton;

    //remove all the items in the current chat layout
    QLayoutItem* item;
    while ((item = ui.chatLayout->takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        if (widget != nullptr) {
            widget->deleteLater();
        }
        delete item;
    }

    ui.username_label->setText(clickedButton->text());
    groupToSend = clickedButton->text();
    auto& vec_msg = (*groupMessages)[groupToSend];

    for (std::size_t i = 0; i < vec_msg.size(); i++) {
        auto* bubble = new QLabel(vec_msg[i].second.first + " : " + QString::fromStdString(vec_msg[i].second.second), this);
        bubble->setWordWrap(true);
        bubble->setMaximumWidth(500);  // Adjust to how wide you want chat bubbles
        bubble->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

        vec_msg[i].first ? ui.chatLayout->addWidget(bubble, 0, Qt::AlignLeft) : ui.chatLayout->addWidget(bubble, 0, Qt::AlignRight);

        QTimer::singleShot(0, this, [=]() {
            ui.scrollArea->ensureWidgetVisible(bubble);
            });
    }
    return;
}

void ChattingWindow::on_sendButton_clicked()
{
    if (UserOrGroup == User) {
        std::string username_to_sendStd = usernameToSend.toStdString();
        const char* username_to_sendCStr = username_to_sendStd.c_str();;

        std::string message_to_sendStd = messageToSend.toStdString();
        const char* message_to_sendCStr = message_to_sendStd.c_str();

        if (message_to_sendStd.length() >= 128) {
            send_error("Message is too long! Cannot be longer than 128 characters.");
            return;
        }

        if (message_to_sendStd.length() == 0) {
            send_error("Message is empty! Cannot send empty message!.");
            return;
        }

        send_to_user(&impl_->sock, message_to_sendCStr, username_to_sendCStr, UserOrGroup);

        auto* bubble = new QLabel(messageToSend, this);
        bubble->setWordWrap(true);
        bubble->setMaximumWidth(500);  // Adjust to how wide you want chat bubbles
        bubble->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        bubble->setFont(messageFont);

        ui.chatLayout->addWidget(bubble, 0, Qt::AlignRight);

        ui.scrollArea->verticalScrollBar()->setValue(ui.scrollArea->verticalScrollBar()->maximum());

        QTimer::singleShot(0, this, [=]() {
            ui.scrollArea->ensureWidgetVisible(bubble);
            });

        (*Messages)[usernameToSend].push_back(std::make_pair(CURR_USER, message_to_sendStd));
    }

    else {
        std::string group_to_sendStd = groupToSend.toStdString();
        const char* group_to_sendCStr = group_to_sendStd.c_str();;

        std::string message_to_sendStd = messageToSend.toStdString();
        const char* message_to_sendCStr = message_to_sendStd.c_str();

        if (message_to_sendStd.length() >= 128) {
            send_error("Message is too long! Cannot be longer than 128 characters.");
            return;
        }

        if (message_to_sendStd.length() == 0) {
            send_error("Message is empty! Cannot send empty message!.");
            return;
        }

        send_to_user(&impl_->sock, message_to_sendCStr, group_to_sendCStr, UserOrGroup);

        auto* bubble = new QLabel(ourUsername + " : " + messageToSend, this);
        bubble->setWordWrap(true);
        bubble->setMaximumWidth(500);  // Adjust to how wide you want chat bubbles
        bubble->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        bubble->setFont(messageFont);

        ui.chatLayout->addWidget(bubble, 0, Qt::AlignRight);

        ui.scrollArea->verticalScrollBar()->setValue(ui.scrollArea->verticalScrollBar()->maximum());

        QTimer::singleShot(0, this, [=]() {
            ui.scrollArea->ensureWidgetVisible(bubble);
            });

        (*groupMessages)[groupToSend].push_back(std::make_pair(CURR_USER, std::make_pair(ourUsername, message_to_sendStd)));
    }
    return;
}

//----------------------------------------------------------------------------------------------------------------------------------------------







//------------------------------------------------ C TO C++ FUNCTIONS ---------------------------------------------------------------------

//this is function i use to interact with recieved messages (sort of acts as the middle man)
extern "C" void handle_message(void* window_ptr, char message[messageLength], char username[usernameLength]) {
    static_cast<ChattingWindow*>(window_ptr)->addMessage(message, username);
}

extern "C" void handle_group_message(void* window_ptr, char message[messageLength], char username[usernameLength], char group[usernameLength]) {
    static_cast<ChattingWindow*>(window_ptr)->addMessage_group(message, username, group);
}

extern "C" void handle_group_creation(void* window_ptr, char group[usernameLength]) {
    static_cast<ChattingWindow*>(window_ptr)->addGroup(group);
}

extern "C" void handle_group_list_update(void* window_ptr, char groups[maxUsers][usernameLength], uint32_t size) {
    static_cast<ChattingWindow*>(window_ptr)->addGroups(groups, size);
}

//this is function i use to interact with recieved messages (sort of acts as the middle man)
extern "C" void handle_list_update(void* window_ptr, char users[maxUsers][usernameLength], uint32_t size) {
    static_cast<ChattingWindow*>(window_ptr)->addUsers(users, size);
}

extern "C" void handle_user_update(void* window_ptr, char user[usernameLength], uint32_t size) {
    static_cast<ChattingWindow*>(window_ptr)->removeUsers(user, size);
}

//----------------------------------------------------------------------------------------------------------------------------------------------








//-------------------------------------------------------------------- ADDING/REMOVING TO DATA STRUCTURE FUNCTIONS ---------------------------------------------------------------------

void ChattingWindow::addGroup(const char group[usernameLength])
{
    std::string group_toadd(group);

    QMetaObject::invokeMethod(this, [=] { this->addGrouptoScreen(QString::fromStdString(group_toadd)); }, Qt::QueuedConnection);
}

void ChattingWindow::addMessage_group(char message[messageLength], char username[usernameLength], char group[usernameLength])
{
    std::string username_toadd(username);
    std::string message_toadd(message);
    std::string group_toadd(group);

    (*groupMessages)[QString::fromStdString(group)].push_back(std::make_pair(OTHER_USER, std::make_pair(QString::fromStdString(username_toadd), message_toadd)));

    //we'll be able to display right away to screen, needs to run on the gui thread (main thread)
    QMetaObject::invokeMethod(this, [=] { this->sendMessageToScreen(QString::fromStdString(username_toadd + " : " + message_toadd), group_toadd, false); }, Qt::QueuedConnection);
}

void ChattingWindow::addMessage(char message[messageLength], char username[usernameLength])
{
    std::string username_toadd(username);
    std::string message_toadd(message);

    (*Messages)[QString::fromStdString(username_toadd)].push_back(std::make_pair(OTHER_USER, message_toadd));

    //we'll be able ti display right away to screen, since this function will be called by recv thread, cannot create element here so queue it on main thread
    QMetaObject::invokeMethod(this, [=] { this->sendMessageToScreen(QString::fromStdString(message_toadd), username_toadd, true); }, Qt::QueuedConnection);
}

void ChattingWindow::addUsers(char users[maxUsers][usernameLength], uint32_t size) {
    //just goes through the list of users when its updated from the server end, and adds any new ones.
    //better logic will be implemented later from server side soon
    for (std::size_t i = 0; i < size; i++) {
        std::string username(users[i]);
        QString usernamee = QString::fromStdString(username);

        //since this function will be called by recv thread, cannot create element here so queue it on main thread
        if ((*Users).find(usernamee) == (*Users).end()) {
            QMetaObject::invokeMethod(this, [=] { this->sendUserToScreen(usernamee); }, Qt::QueuedConnection);
        }
    }
    return;
}

void ChattingWindow::addGroups(char groups[maxUsers][usernameLength], uint32_t size) {
    //just goes through the list of users when its updated from the server end, and adds any new ones.
    //better logic will be implemented later from server side soon
    for (std::size_t i = 0; i < size; i++) {
        std::string group(groups[i]);
        QString groupp = QString::fromStdString(group);

        //since this function will be called by recv thread, cannot create element here so queue it on main thread
        if ((*Groups).find(groupp) == (*Groups).end()) {
            QMetaObject::invokeMethod(this, [=] { this->addGrouptoScreen(groupp); }, Qt::QueuedConnection);
        }
    }
    return;
}

void ChattingWindow::removeUsers(char user[usernameLength], uint32_t size)
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

    Groups->insert(std::make_pair(group_n, group));

    group->setText(group_n);
    group->setMinimumSize(205, 40);
    group->setStyleSheet(defaultButtonStylesheet);

    if (UserOrGroup == User) {
        group->hide();
    }

    ui.userLayout->addWidget(group, 0, Qt::AlignLeft | Qt::AlignTop);
    connect(group, &QPushButton::clicked, this, &ChattingWindow::onGroupClick);
}

void ChattingWindow::removeUserfromScreen(const QString& user)
{
    if ((*Users)[user]) {
        if (lastPressedUser == (*Users)[user]) {
            lastPressedUser = nullptr;
        }
        ui.userLayout->removeWidget((*Users)[user]);
        (*Users)[user]->hide();
        (*Users)[user]->deleteLater();
        (*Users)[user] = nullptr;
        Users->erase(user);
        //remove from the map as well
    }
    return;
}

void ChattingWindow::sendMessageToScreen(const QString& message, const std::string &username, bool user)
{
    if (user && lastPressedUser && lastPressedUser->text() == QString::fromStdString(username)) {

        auto* bubble = new QLabel (message, this);
        bubble->setWordWrap(true);
        bubble->setMaximumWidth(500);  // Adjust to how wide you want chat bubbles
        bubble->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        bubble->setFont(messageFont);

        ui.chatLayout->addWidget(bubble, 0, Qt::AlignLeft);

        ui.scrollArea->verticalScrollBar()->setValue(ui.scrollArea->verticalScrollBar()->maximum());

        QTimer::singleShot(0, this, [=]() {
            ui.scrollArea->ensureWidgetVisible(bubble);
            });

    }

    else if (!user && lastPressedGroup && lastPressedGroup->text() == QString::fromStdString(username)) {
        auto* bubble = new QLabel(message, this);
        bubble->setWordWrap(true);
        bubble->setMaximumWidth(500);  // Adjust to how wide you want chat bubbles
        bubble->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        bubble->setFont(messageFont);

        ui.chatLayout->addWidget(bubble, 0, Qt::AlignLeft);

        ui.scrollArea->verticalScrollBar()->setValue(ui.scrollArea->verticalScrollBar()->maximum());

        QTimer::singleShot(0, this, [=]() {
            ui.scrollArea->ensureWidgetVisible(bubble);
            });
    }

    return;
}

void ChattingWindow::sendUserToScreen(QString username) {
    QPushButton* user = new QPushButton(this);

    if(username == ourUsername) {
        user->setText("You: " + username);
    }
    else {
        user->setText(username);
    }
    user->setMinimumSize(205, 40);
    user->setStyleSheet(defaultButtonStylesheet);

    ui.userLayout->addWidget(user, 0, Qt::AlignLeft | Qt::AlignTop);

    if (UserOrGroup == Group) {
        user->hide();
    }

    Users->insert(std::make_pair(username, user));

    connect(user, &QPushButton::clicked, this, &ChattingWindow::onUserClick);

    return;
}

//----------------------------------------------------------------------------------------------------------------------------------------------






//-------------------------------------------------------------------- MISCELLANEOUS ---------------------------------------------------------------------


void ChattingWindow::thread_creator()
{
    auto* arg = new RecvParams{ &(impl_->sock), this, handle_message, handle_group_message, handle_group_list_update, handle_group_creation, handle_list_update, handle_user_update };

    DWORD pThreadID;
    HANDLE call_thread = create_thread(recieving, arg, &pThreadID);
    CloseHandle(call_thread);
    return;
}

void ChattingWindow::send_error(const QString& error_message)
{
    QMessageBox msgBox;
    msgBox.setText(error_message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    return;
}

//----------------------------------------------------------------------------------------------------------------------------------------------