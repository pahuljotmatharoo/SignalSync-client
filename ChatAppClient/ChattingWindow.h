// tell windows.h *not* to drag in the old winsock.h
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif

// you must include win32 socket types in the header if you use SOCKET publicly
#include <winsock2.h>
#include <ws2tcpip.h>
#include <QMainWindow>
#include "ui_ChattingWindow.h"

class ChattingWindow : public QMainWindow {
    Q_OBJECT
private:
    Ui::ChattingWindow ui;
    QString username;
    QString username_to_send;
    QString message_to_send;
    struct Impl;
    Impl* impl_;
    std::unordered_map<std::string, std::vector<std::string>> Messages;
    std::unordered_set<std::string> Users;
public:
    explicit ChattingWindow(QWidget* parent = nullptr);
    ~ChattingWindow();
    void thread_creator();
    void setSOCKET(SOCKET sock);
    void setUsername(const QString& new_user);
    void addMessage(char message[128], char username[50]);
    void addUsers(char users[10][50], uint32_t size);
private slots:
    void on_sendButton_clicked();
    void on_Username_input_textEdited(const QString& text); // this is for 
    void on_Message_input_textEdited(const QString& text);
};