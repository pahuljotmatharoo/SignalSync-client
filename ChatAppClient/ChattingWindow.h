#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#pragma once
#include "ui_ChattingWindow.h"
#include <NetworkClient.h>
#include <thread>

class ChattingWindow : public QMainWindow {
    Q_OBJECT
private:
    Ui::ChattingWindow m_ui;
    bool m_userOrGroup; // lets us know whether the user is on userlist or grouplist
    QString m_defaultButtonStylesheet;
    QString m_pressedButtonStylesheet;
    QString m_sendMessageStylesheet;
    QString m_recvMessageStylesheet;
    QString m_usernameToSend;
    QString m_messageToSend;
    QString m_groupToSend;
    QString m_selfUsername;
    //should get deleted itself
    QPushButton* m_lastPressedUser; // basically current button pressed (last pressed as we need to see if we had pressed a button before, and which)
    QPushButton* m_lastPressedGroup; // basically current button pressed (last pressed as we need to see if we had pressed a button before, and which)
    QFont m_messageFont;
    QFont m_titleFont;
    QFont m_buttonAddGroupFont;
    QFont m_buttonFont;
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
    void setSelfUser(const QString t_username) { m_selfUsername = t_username; }
    void setNetwork(Network& t_network) { m_network = t_network; };
    void send_error(const QString& error_message);
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