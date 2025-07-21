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
#include <message_s.h>
#include <qscrollbar.h>
#include <qlayout.h>
#include <qtimer.h>
constexpr auto MSG_SEND = 1;
constexpr auto MSG_LIST = 2;
constexpr auto MSG_EXIT = 3;
constexpr auto OTHER_USER = true;
constexpr auto CURR_USER = false;

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

    QString default_button =
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
    default_button_stylesheet = default_button;

    QString pressed_button =
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
    pressed_button_stylesheet = pressed_button;

    Messages = new std::unordered_map<QString, std::vector<std::pair<bool, std::string>>>();
    Users = new std::unordered_set<std::string>();
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
extern "C" void handle_message(void* window_ptr, char message[128], char username[50]) {
    static_cast<ChattingWindow*>(window_ptr)->addMessage(message, username);
}

//this is function i use to interact with recieved messages (sort of acts as the middle man)
extern "C" void handle_list_update(void* window_ptr, char users[10][50], uint32_t size) {
    static_cast<ChattingWindow*>(window_ptr)->addUsers(users, size);
}

void ChattingWindow::thread_creator()
{
    auto* arg = new RecvParams{&(impl_->sock), this, handle_message, handle_list_update};

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

void ChattingWindow::addMessage(char message[128], char username[50])
{
    std::string username_toadd(username);
    std::string message_toadd(message);

    (*Messages)[QString::fromStdString(username_toadd)].push_back(std::make_pair(OTHER_USER, message_toadd));

    //since this function will be called by recv thread, cannot create element here so queue it on main thread
    QMetaObject::invokeMethod(this, [=] { this->sendMessageToScreen(message);},Qt::QueuedConnection);
}

void ChattingWindow::addUsers(char users[10][50], uint32_t size) {
    //just goes through the list of users when its updated from the server end, and adds any new ones.
    //better logic will be implemented later from server side soon
    for (std::size_t i = 0; i < size; i++) {
        std::string username(users[i]);
        if ((*Users).find(username) == (*Users).end()) {
            (*Users).insert(username);
            QMetaObject::invokeMethod(this, [=] { this->sendUserToScreen(QString::fromStdString(username)); }, Qt::QueuedConnection);
        }
    }
}

void ChattingWindow::sendMessageToScreen(const QString& message)
{

    auto* bubble = new MessageWidget(message, this);
        
    // make bubble report a non-zero height
    bubble->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    bubble->adjustSize();

    ui.verticalLayout->addWidget(bubble, 0,Qt::AlignLeft);

    QTimer::singleShot(0, this, [=]() {
        ui.scrollArea->ensureWidgetVisible(bubble);
        });

    return;
}

void ChattingWindow::sendUserToScreen(QString username) {
    QPushButton* user = new QPushButton;
    user->setText(username);

    user->setStyleSheet(default_button_stylesheet);

    ui.verticalLayout_2->addWidget(user, 0, Qt::AlignLeft | Qt::AlignTop);
    connect(user, &QPushButton::clicked, this, &ChattingWindow::onUserClick);
    return;
}

void ChattingWindow::on_Username_input_textEdited(const QString& text)
{
    username_to_send = text;
}

void ChattingWindow::on_Message_input_textEdited(const QString& text)
{
    message_to_send = text;
}

void ChattingWindow::onUserClick()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    
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

    for (int i = 0; i < vec_msg.size(); i++) {
        auto* bubble = new MessageWidget(QString::fromStdString(vec_msg[i].second), this);
        bubble->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        bubble->adjustSize();

        vec_msg[i].first ? ui.verticalLayout->addWidget(bubble, 0, Qt::AlignLeft) : ui.verticalLayout->addWidget(bubble, 0, Qt::AlignRight);

        QTimer::singleShot(0, this, [=]() {
            ui.scrollArea->ensureWidgetVisible(bubble);
            });
    }
}

void ChattingWindow::on_sendButton_clicked()
{
    std::string username_to_sendStd = username_to_send.toStdString();
    const char* username_to_sendCStr = username_to_sendStd.c_str();;

    std::string message_to_sendStd = message_to_send.toStdString();
    const char* message_to_sendCStr = message_to_sendStd.c_str();;

    send_to_user(&impl_->sock, message_to_sendCStr, username_to_sendCStr);

    auto* bubble = new MessageWidget_s(message_to_send, this);

    // make bubble report a non-zero height
    bubble->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    bubble->adjustSize();

    ui.verticalLayout->addWidget(bubble, 0, Qt::AlignRight | Qt::AlignTop);

    (*Messages)[QString::fromStdString(username_to_sendStd)].push_back(std::make_pair(CURR_USER, message_to_sendStd));
    return;
}