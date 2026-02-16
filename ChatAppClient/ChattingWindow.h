#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#pragma once
#include <thread>
#include <mutex>
#include <type_traits>
#include <semaphore>
#include "ChatAppClient.h"
#include "message.h"
#include "message_s.h"
#include "ui_ChattingWindow.h"
#include "NetworkClient.h"
#include "File.h"
#include "UserMessage.h"
#include "GroupMessage.h"

constexpr bool OTHER_USER = true;
constexpr bool CURR_USER = false;
constexpr bool UserB = true;
constexpr bool GroupB = false;

class ChattingWindow : public QMainWindow {
    Q_OBJECT
private:
    Ui::ChattingWindow m_ui;
    bool m_userOrGroup; // lets us know whether the user is on userlist or grouplist
    HTTPRequest m_http;
    std::string m_apiKey;
    QString m_defaultButtonStylesheet;
    QString m_pressedButtonStylesheet;
    QString m_sendMessageStylesheet;
    QString m_recvMessageStylesheet;
    QString m_recvNotificationStylesheet;
    QString m_usernameToSend;
    QString m_messageToSend;
    QString m_groupToSend;
    QString m_selfUsername;
    QPushButton* m_lastPressedUser; // basically current button pressed (last pressed as we need to see if we had pressed a button before, and which)
    QPushButton* m_lastPressedGroup; // basically current button pressed (last pressed as we need to see if we had pressed a button before, and which)
    QFont m_messageFont;
    QFont m_titleFont;
    QFont m_buttonAddGroupFont;
    QFont m_buttonFont;
    std::unordered_map<QString, UserMessage*> m_messages;
    std::unordered_map<QString, GroupMessage*> m_groupMessages;
    std::unordered_map<QString, QPushButton*> m_Users;
    std::unordered_map<QString, QPushButton*> m_Groups;
    std::unordered_map<QChar, QChar> m_encryptMap;
    std::unordered_map<QPushButton*, File*> m_files;
    Network m_network;
    std::thread m_thread;
    std::mutex m_mutex;
    std::binary_semaphore m_groupSemaphore;
    std::binary_semaphore m_generalSemaphore;
    std::atomic<bool> m_threadStop;
public:
    explicit ChattingWindow(QWidget* parent = nullptr);
    ~ChattingWindow();
    void setSelfUser(const QString t_username) { m_selfUsername = t_username; }
    void setApiKey(const std::string t_apiKey) { m_apiKey = t_apiKey; }
    void setNetwork(Network& t_network) { m_network = t_network; };
    void addMessage(char message[MESSAGE_LENGTH], char username[USERNAME_LENGTH]);
    void addMessage_group(char message[MESSAGE_LENGTH], char username[USERNAME_LENGTH], char group[USERNAME_LENGTH]);
    void sendMessageToScreenRecv(const QString& message, const QString& user, bool type);
    void sendMessageToScreenSend(const QString& message);
    void sendUserToScreen(const QString& username);
    void addUsers(char users[MAXUSERS][USERNAME_LENGTH], uint32_t size);
    void removeUsers(char user[USERNAME_LENGTH], uint32_t size);
    void removeUserfromScreen(const QString& user);
    void addGroup(const char group[USERNAME_LENGTH]);
    void addGroups(char users[MAXUSERS][USERNAME_LENGTH], uint32_t size);
    void addGrouptoScreen(const QString user);
    void removeAllChatItemsFromScreen();
    void initEncryptMap();
    void initUI();
    void encrypt(QString& message);
    void threadFunction();
    std::pair<QPushButton*, QString> createAndStyleGroupButton();
    QPushButton* createAndStyleButton(const QString& name);
    QPushButton* createAndStyleFileButton(const std::string& fileName);
    void userOrGroupSelect(std::unordered_map<QString, QPushButton*> &hide, std::unordered_map<QString, QPushButton*> &show, QPushButton*& lastPressedButton) const;
    void addFileButtonToScreen(File* recvFile, const std::string& fileName);
    void downloadFile();
    void processFileRecv(File* recvFile, const std::string& fileName);
    void destroyFiles();
    void destroyUserMessages();
    void destroyGroupMessages();
    void threadShutdown();
    void displayUserMessages(const QString& t_user_name);
    void displayGroupMessages(const QString& t_group_name);
    void displayFileButtons(const QString& user_or_group_name);
    void displayMessages(UserMessage* t_messages, const QString& user_or_group_name, bool user_or_group);
    void notificationPassUser(const QString& user_from);
    void notificationPassGroup(const QString& user_from);
    void notificationUser(const QString& user_from);
    void notificationGroup(const QString& group_from);
    void createIfGroupMissing(const QString& group_name);
    template <typename T>
    void addWidgetToLayout(const T& widget, Qt::Alignment alignment) {
        m_ui.chatLayout->addWidget(widget, 0, alignment);

        m_ui.scrollArea->verticalScrollBar()->setValue(m_ui.scrollArea->verticalScrollBar()->maximum());

        QTimer::singleShot(0, this, [=]() { m_ui.scrollArea->ensureWidgetVisible(widget); });
    }
    QString findNewGroupName();
    void initStyles();
    void initButtons();
    void initLayout();
    void initContentLayout();
private slots:
    void on_sendButton_clicked();
    void on_fileButton_clicked();
    void on_Message_input_textEdited(const QString& text);
    void on_groupChat_clicked();
    void on_userList_clicked();
    void onUserClick();
    void onGroupClick();
    void on_addGroup_clicked();
};