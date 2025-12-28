#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#pragma once
#include "ui_ChattingWindow.h"
#include <NetworkClient.h>
#include <thread>
constexpr auto MAXUSERS = 10;

class ChattingWindow : public QMainWindow {
    Q_OBJECT
private:
    Ui::ChattingWindow ui;
    bool UserOrGroup; // lets us know whether the user is on userlist or grouplist
    QString defaultButtonStylesheet;
    QString pressedButtonStylesheet;
    QString sendMessageStylesheet;
    QString recvMessageStylesheet;
    QString ourUsername; // username of this instance
    QString usernameToSend;
    QString messageToSend;
    QString groupToSend;
    //should get deleted itself
    QPushButton* lastPressedUser; // basically current button pressed (last pressed as we need to see if we had pressed a button before, and which)
    QPushButton* lastPressedGroup; // basically current button pressed (last pressed as we need to see if we had pressed a button before, and which)
    QFont messageFont;
    QFont titleFont;
    QFont button_addGroup_Font;
    QFont buttonFont;
    std::unordered_map<QString, std::vector<std::pair<bool, std::string>>> m_Messages; // Name of other user, <Who sent it, The Message>
    std::unordered_map<QString, std::vector<std::pair<bool, std::pair<QString, std::string> > > > m_groupMessages; // Name of Group, <Who sent it <The user who sent it, The Message>>
    std::unordered_map<QString, QPushButton*> m_Users; // <Name of User, Button addr>
    std::unordered_map<QString, QPushButton*> m_Groups;  // <Name of Group, Button addr> 
    std::unordered_map<QChar, QChar> m_encryptMap;
    Network m_network;
    std::thread m_thread;
public:
    explicit ChattingWindow(QWidget* parent = nullptr);
    ~ChattingWindow();
    void send_error(const QString& error_message);
    void setNetwork(Network& t_network) { m_network = t_network; };
    void setUsername(const QString& new_user);
    void addMessage(char message[MESSAGE_LENGTH], char username[USERNAME_LENGTH]);
    void addMessage_group(char message[MESSAGE_LENGTH], char username[USERNAME_LENGTH], char group[USERNAME_LENGTH]);
    void sendMessageToScreen(const QString& message, const std::string& username, bool user);
    void sendUserToScreen(QString username);
    void addUsers(char users[MAXUSERS][USERNAME_LENGTH], uint32_t size);
    void removeUsers(char user[USERNAME_LENGTH], uint32_t size);
    void removeUserfromScreen(const QString& user);
    void addGroup(const char group[USERNAME_LENGTH]);
    void addGroups(char users[MAXUSERS][USERNAME_LENGTH], uint32_t size);
    void addGrouptoScreen(const QString& user);
    void removeMessagesFromScreen();
    void initEncryptMap();
    void initUI();
    void encrypt(QString& message);
    void threadFunction();
private slots:
    void on_sendButton_clicked();
    void on_Message_input_textEdited(const QString& text);
    void on_groupChat_clicked();
    void on_userList_clicked();
    void onUserClick();
    void onGroupClick();
    void on_addGroup_clicked();
};