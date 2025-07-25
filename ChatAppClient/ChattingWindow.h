﻿// tell windows.h *not* to drag in the old winsock.h
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif

// you must include win32 socket types in the header if you use SOCKET publicly
#include <winsock2.h>
#include <ws2tcpip.h>
#include <QMainWindow>
#include "ui_ChattingWindow.h"
#include <QHBoxLayout>
constexpr auto message_length = 128;
constexpr auto username_length = 50;
constexpr auto max_users = 10;

class ChattingWindow : public QMainWindow {
    Q_OBJECT
private:
    Ui::ChattingWindow ui;
    QString default_button_stylesheet;
    QString pressed_button_stylesheet;
    QString username;
    QString username_to_send;
    QString message_to_send;
    //should get deleted itself
    QPushButton* last_pressed;
    struct Impl;
    Impl* impl_;
    std::unordered_map<QString, std::vector<std::pair<bool, std::string>>>* Messages;
    std::unordered_map<QString, QPushButton*>* Users;
public:
    explicit ChattingWindow(QWidget* parent = nullptr);
    ~ChattingWindow();
    void thread_creator();
    void setSOCKET(SOCKET sock);
    void setUsername(const QString& new_user);
    void addMessage(char message[message_length], char username[username_length]);
    void sendMessageToScreen(const QString& message, const std::string& username);
    void sendUserToScreen(QString username);
    void addUsers(char users[max_users][username_length], uint32_t size);
    void removeUsers(char user[username_length], uint32_t size);
    void removeUserfromScreen(const QString& user);
private slots:
    void on_sendButton_clicked();
    void on_Username_input_textEdited(const QString& text);
    void on_Message_input_textEdited(const QString& text);
    void onUserClick();
};