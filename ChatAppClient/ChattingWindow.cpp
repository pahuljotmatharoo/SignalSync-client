﻿#include <winsock2.h>
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
constexpr auto MSG_SEND = 1;
constexpr auto MSG_LIST = 2;
constexpr auto MSG_EXIT = 3;
constexpr auto OTHER_USER = true;
constexpr auto CURR_USER = false;
//just need to ensure that the scroll area doesn't condense the messages down

struct ChattingWindow::Impl {
    SOCKET sock{ INVALID_SOCKET };
    ~Impl() {
        if (sock != INVALID_SOCKET)
            closesocket(sock);
        //WSACleanup();
    }
};

ChattingWindow::ChattingWindow(QWidget* parent) : QMainWindow(parent), username{ "" }, impl_(new Impl()), last_pressed(nullptr)
{
    ui.setupUi(this);
    
    //we're just creating a layout for scrolling and out vertical layout
    QWidget* contents = ui.scrollArea->takeWidget();
    QVBoxLayout* layout = new QVBoxLayout(contents);

    layout->setContentsMargins(0, 0, 0, 0);
    contents->setLayout(layout);
    ui.scrollArea->setWidget(contents);

    ui.verticalLayout = layout;
    layout->setSpacing(5);
    layout->setSizeConstraint(QLayout::SetMinimumSize);

    //we're just creating a layout for scrolling and out vertical layout
    QWidget* contents_users = ui.scrollArea_2->takeWidget();
    QVBoxLayout* layout_users = new QVBoxLayout(contents_users);

    layout_users->setContentsMargins(0, 0, 0, 0);
    contents_users->setLayout(layout_users);
    ui.scrollArea_2->setWidget(contents_users);

    ui.verticalLayout_2 = layout_users;
    layout_users->setSpacing(5);
    layout_users->setSizeConstraint(QLayout::SetMinimumSize);

    default_button_stylesheet = 
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

    pressed_button_stylesheet = 
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
    Users = new std::unordered_map<QString, QPushButton*>();

    ui.verticalLayout_2->setContentsMargins(10, 5, 10, 0); // 10px left/right margins

    ui.verticalLayout->setContentsMargins(10, 10, 10, 10); // 10px left/right margins

    auto* l = static_cast<QVBoxLayout*>(ui.scrollAreaWidgetContents->layout());
    l->insertStretch(0, 1);

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
}

//this is function i use to interact with recieved messages (sort of acts as the middle man)
extern "C" void handle_message(void* window_ptr, char message[message_length], char username[username_length]) {
    static_cast<ChattingWindow*>(window_ptr)->addMessage(message, username);
}

//this is function i use to interact with recieved messages (sort of acts as the middle man)
extern "C" void handle_list_update(void* window_ptr, char users[max_users][username_length], uint32_t size) {
    static_cast<ChattingWindow*>(window_ptr)->addUsers(users, size);
}

extern "C" void handle_user_update(void* window_ptr, char user[username_length], uint32_t size) {
    static_cast<ChattingWindow*>(window_ptr)->removeUsers(user, size);
}

void ChattingWindow::thread_creator()
{
    auto* arg = new RecvParams{&(impl_->sock), this, handle_message, handle_list_update, handle_user_update};

    DWORD pThreadID;
    HANDLE call_thread = create_thread(recieving, arg, &pThreadID);
    CloseHandle(call_thread);
    return;
}

void ChattingWindow::setSOCKET(SOCKET newsock) {
    impl_->sock = newsock;
    thread_creator();
    return;
}

void ChattingWindow::setUsername(const QString& new_user)
{
    this->username = new_user;
    return;
}


void ChattingWindow::addMessage(char message[message_length], char username[username_length])
{
    std::string username_toadd(username);
    std::string message_toadd(message);

    (*Messages)[QString::fromStdString(username_toadd)].push_back(std::make_pair(OTHER_USER, message_toadd));

    //since this function will be called by recv thread, cannot create element here so queue it on main thread
    QMetaObject::invokeMethod(this, [=] { this->sendMessageToScreen(QString::fromStdString(message_toadd), username_toadd); }, Qt::QueuedConnection);
}

void ChattingWindow::addUsers(char users[max_users][username_length], uint32_t size) {
    //just goes through the list of users when its updated from the server end, and adds any new ones.
    //better logic will be implemented later from server side soon
    for (std::size_t i = 0; i < size; i++) {
        std::string username(users[i]);
        QString usernamee = QString::fromStdString(username);

        if ((*Users).find(usernamee) == (*Users).end()) {
            QMetaObject::invokeMethod(this, [=] { this->sendUserToScreen(QString::fromStdString(username)); }, Qt::QueuedConnection);
        }
    }
    return;
}

void ChattingWindow::removeUsers(char user[username_length], uint32_t size)
{
    std::string user_to_remove(user);
    QMetaObject::invokeMethod(this, [=] { this->removeUserfromScreen(QString::fromStdString(user_to_remove)); }, Qt::QueuedConnection);
    return;
}

void ChattingWindow::removeUserfromScreen(const QString& user)
{
    if ((*Users)[user]) {
        if (last_pressed == (*Users)[user]) {
            last_pressed = nullptr;
        }
        ui.verticalLayout_2->removeWidget((*Users)[user]);
        (*Users)[user]->hide();
        (*Users)[user]->deleteLater();
        (*Users)[user] = nullptr;
        Users->erase(user);
        //remove from the map as well
    }
    return;
}

void ChattingWindow::sendMessageToScreen(const QString& message, const std::string &username)
{
    if (last_pressed && last_pressed->text() == QString::fromStdString(username)) {
        auto* bubble = new MessageWidget(message, this);

        ui.verticalLayout->addWidget(bubble, 0, Qt::AlignLeft | Qt::AlignTop);

        ui.scrollArea->verticalScrollBar()->setValue(ui.scrollArea->verticalScrollBar()->maximum());

        QTimer::singleShot(0, this, [=]() {
            ui.scrollArea->ensureWidgetVisible(bubble);
            });

    }
    return;
}

void ChattingWindow::sendUserToScreen(QString username) {
    QPushButton* user = new QPushButton(this);

    user->setText(username);
    user->setMinimumSize(205, 40);
    user->setStyleSheet(default_button_stylesheet);

    ui.verticalLayout_2->addWidget(user, 0, Qt::AlignLeft | Qt::AlignTop);

    Users->insert(std::make_pair(username, user));

    connect(user, &QPushButton::clicked, this, &ChattingWindow::onUserClick);

    return;
}

void ChattingWindow::on_Username_input_textEdited(const QString& text)
{
    username_to_send = text;
    return;
}

void ChattingWindow::on_Message_input_textEdited(const QString& text)
{
    message_to_send = text;
    return;
}

void ChattingWindow::onUserClick()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());

    if (last_pressed == clickedButton) {
        return;
    }
    
    if (last_pressed) {
        last_pressed->setStyleSheet(default_button_stylesheet);
    }

    clickedButton->setStyleSheet(pressed_button_stylesheet);
    last_pressed = clickedButton;

    QLayoutItem* item;
    while ((item = ui.verticalLayout->takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        if (widget != nullptr) {
            widget->deleteLater();
        }
        delete item;
    }

    ui.label->setText(clickedButton->text());
    username_to_send = clickedButton->text();
    auto vec_msg = (*Messages)[username_to_send];

    for (std::size_t i = 0; i < vec_msg.size(); i++) {
        auto* bubble = new MessageWidget(QString::fromStdString(vec_msg[i].second), this);
        bubble->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        bubble->adjustSize();

        vec_msg[i].first ? ui.verticalLayout->addWidget(bubble, 0, Qt::AlignLeft) : ui.verticalLayout->addWidget(bubble, 0, Qt::AlignRight);

        QTimer::singleShot(0, this, [=]() {
            ui.scrollArea->ensureWidgetVisible(bubble);
            });
    }
    return;
}

void ChattingWindow::on_sendButton_clicked()
{
    std::string username_to_sendStd = username_to_send.toStdString();
    const char* username_to_sendCStr = username_to_sendStd.c_str();;

    std::string message_to_sendStd = message_to_send.toStdString();
    const char* message_to_sendCStr = message_to_sendStd.c_str();;

    send_to_user(&impl_->sock, message_to_sendCStr, username_to_sendCStr);

    auto* bubble = new MessageWidget(message_to_send, this);

    ui.verticalLayout->addWidget(bubble, 0, Qt::AlignRight | Qt::AlignTop);

    ui.scrollArea->verticalScrollBar()->setValue(ui.scrollArea->verticalScrollBar()->maximum());

    (*Messages)[QString::fromStdString(username_to_sendStd)].push_back(std::make_pair(CURR_USER, message_to_sendStd));
    return;
}