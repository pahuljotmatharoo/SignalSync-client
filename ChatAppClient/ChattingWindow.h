// tell windows.h *not* to drag in the old winsock.h
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <QMainWindow>
#include "ui_ChattingWindow.h"
#include <QHBoxLayout>
constexpr auto messageLength = 128;
constexpr auto usernameLength = 50;
constexpr auto maxUsers = 10;

class ChattingWindow : public QMainWindow {
    Q_OBJECT
private:
    Ui::ChattingWindow ui;
    bool UserOrGroup; // lets us know whether the user is on userlist or grouplist
    QString defaultButtonStylesheet;
    QString pressedButtonStylesheet;
    QString username;
    QString usernameToSend; // this also serves as the group name to send to...
    QString messageToSend;
    QString groupToSend;
    //should get deleted itself
    QPushButton* lastPressedUser;
    QPushButton* lastPressedGroup;
    QFont messageFont;
    QFont titleFont;
    struct Impl;
    Impl* impl_;
    std::unordered_map<QString, std::vector<std::pair<bool, std::string>>>* Messages;
    std::unordered_map<QString, std::vector<std::pair<bool, std::pair<QString, std::string> > > >* groupMessages;
    std::unordered_map<QString, QPushButton*>* Users;
    std::unordered_map<QString, QPushButton*>* Groups;
public:
    explicit ChattingWindow(QWidget* parent = nullptr);
    ~ChattingWindow();
    void send_error(const QString& error_message);
    void thread_creator();
    void setSOCKET(SOCKET sock);
    void setUsername(const QString& new_user);
    void addMessage(char message[messageLength], char username[usernameLength]);
    void addMessage_group(char message[messageLength], char username[usernameLength], char group[usernameLength]);
    void sendMessageToScreen(const QString& message, const std::string& username, bool user);
    void sendUserToScreen(QString username);
    void addUsers(char users[maxUsers][usernameLength], uint32_t size);
    void removeUsers(char user[usernameLength], uint32_t size);
    void removeUserfromScreen(const QString& user);
    void addGroup(const char group[usernameLength]);
    void addGrouptoScreen(const QString& user);
private slots:
    void on_sendButton_clicked();
    void on_Message_input_textEdited(const QString& text);
    void on_groupChat_clicked();
    void on_userList_clicked();
    void onUserClick();
    void onGroupClick();
    void on_addGroup_clicked();
};