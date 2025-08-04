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
    QString ourUsername; // username of this instance
    QString usernameToSend;
    QString messageToSend;
    QString groupToSend;
    //should get deleted itself
    QPushButton* lastPressedUser; // basically current button pressed (last pressed as we need to see if we had pressed a button before, and which)
    QPushButton* lastPressedGroup; // basically current button pressed (last pressed as we need to see if we had pressed a button before, and which)
    QFont messageFont;
    QFont titleFont;
    struct Impl;
    Impl* impl_;
    std::unordered_map<QString, std::vector<std::pair<bool, std::string>>>* Messages; // Name of other user, <Who sent it, The Message>
    std::unordered_map<QString, std::vector<std::pair<bool, std::pair<QString, std::string> > > >* groupMessages; // Name of Group, <Who sent it <The user who sent it, The Message>>
    std::unordered_map<QString, QPushButton*>* Users; // <Name of User, Button addr>
    std::unordered_map<QString, QPushButton*>* Groups;  // <Name of Group, Button addr> 
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
    void addGroups(char users[maxUsers][usernameLength], uint32_t size);
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