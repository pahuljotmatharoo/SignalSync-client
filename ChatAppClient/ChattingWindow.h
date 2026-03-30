#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#pragma once
#include <thread>
#include <mutex>
#include <type_traits>
#include <semaphore>
#include <functional>
#include <utility>
#include <queue>
#include "ChatAppClient.h"
#include "./Messages/Message Templates/message.h"
#include "./Messages/Message Templates/message_s.h"
#include "./Network/NetworkClient.h"
#include "./Assets/File.h"
#include "./Messages/User Messages/UserMessage.h"
#include "./Messages/Group Messages/GroupMessage.h"
#include "./Assets/LockGuard.h"
#include "./Assets/SemaphoreLockGuard.h"
#include "./Assets/HttpRequest.h"
#include "./Assets/UnqiuePtr.h"
namespace Ui { class ChattingWindow; }

namespace SignalSync {
    constexpr bool OTHER_USER = true;
    constexpr bool CURR_USER = false;
    constexpr bool UserB = true;
    constexpr bool GroupB = false;
    constexpr int MAX_THREADS = 5;

    class ChattingWindow : public QMainWindow {
        Q_OBJECT
    private:
        Ui::ChattingWindow* m_ui;
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
        std::unordered_map<QString, UniquePtr<UserMessage>> m_messages;
        std::unordered_map<QString, UniquePtr<GroupMessage>> m_groupMessages;
        std::unordered_map<QString, QPushButton*> m_Users;
        std::unordered_map<QString, QPushButton*> m_Groups;
        std::unordered_map<QChar, QChar> m_encryptMap;
        std::unordered_map <QPushButton*, File> m_filesUsers;
        std::unordered_map<QPushButton*, std::pair<File, QString>> m_filesGroup;
        Network m_network;
        std::thread m_thread;
        std::vector<std::thread> m_threadPool;
        std::queue<std::function<void()>> m_functionQueue;
        std::mutex m_queueMutex;
        std::binary_semaphore m_groupSemaphore;
        std::binary_semaphore m_generalSemaphore;
        std::counting_semaphore<INT_MAX> m_queueSemaphore;
        std::atomic<bool> m_threadStop;
    public:
        explicit ChattingWindow(QWidget* parent = nullptr);
        ~ChattingWindow();
        void setSelfUser(const QString t_username) { m_selfUsername = t_username; }
        void setApiKey(const std::string t_apiKey) { m_apiKey = t_apiKey; }
        std::string getApiKey() { return m_apiKey; }
        void setNetwork(Network& t_network) { m_network = t_network; };
        void networkMessageRecv();
        void networkUserExit();
        void networkRoomCreateRecv();
        void networkRoomMessageRecv();
        void networkRoomListRecv();
        void networkFileRecv();
        void networkFileGroupRecv();
        void networkUserListRecv();
        void addMessage(MsgRecvUser* recvStruct);
        void addMessage_group(std::string message_toadd, const std::string username_toadd, const std::string group_toadd);
        void sendMessageToScreenRecv(const QString& message, const QString& user, bool type);
        void sendMessageToScreenSend(const QString& message);
        void sendUserToScreen(const QString username);
        void addUsers(List* list);
        void removeUsers(const std::string user, uint32_t size);
        void removeUserfromScreen(const QString& user);
        void addGroup(const std::string group);
        void addGroups(char users[MAXUSERS][USERNAME_LENGTH], uint32_t size);
        void addGrouptoScreen(const QString group_name);
        void removeAllChatItemsFromScreen();
        void initEncryptMap();
        void initUI();
        void encrypt(QString& message);
        void networkThreadFunction();
        std::pair<QPushButton*, QString> createAndStyleGroupButton();
        QPushButton* createAndStyleButton(const QString& name);
        QPushButton* createAndStyleFileButton(const std::string& fileName);
        void userOrGroupSelect(std::unordered_map<QString, QPushButton*>& hide, std::unordered_map<QString, QPushButton*>& show, QPushButton*& lastPressedButton) const;
        void addFileButtonToScreenUser(const QString t_userFrom, char* t_data, const uint32_t t_size, const std::string& fileName);
        void addFileButtonToScreenGroup(const QString t_userFrom, char* t_data, const uint32_t t_size, const std::string& fileName, const std::string& groupName);
        void downloadUserFile();
        void downloadGroupFile();
        void processFileRecvUser(const QString t_userFrom, char* t_data, const uint32_t t_size, const std::string& fileName);
        void processFileRecvGroup(const QString t_userFrom, char* t_data, const uint32_t t_size, const std::string& fileName, const std::string& groupName);
        void threadShutdown();
        void displayUserMessages(const QString& t_user_name);
        void displayGroupMessages(const QString& t_group_name);
        void displayFileButtons(const QString& user_or_group_name);
        void displayMessages(UserMessage& t_messages, const QString& user_or_group_name, bool user_or_group);
        void notificationPassUser(const QString user_from);
        void notificationPassGroup(const QString& user_from);
        void notificationUser(const QString& user_from);
        void notificationGroup(const QString& group_from);
        void createIfGroupMissing(const QString& group_name);
        void addWidgetToLayout(QWidget* widget, Qt::Alignment alignment);
        QString findNewGroupName();
        void initStyles();
        void initButtons();
        void initLayout();
        void initContentLayout();
        void freeMessageStruct(MsgRecvUser* t_msg);
        void enqueue(std::function<void()> func);
        void dequeue();
        void waitingThreadFunction();
        void initThreads();
        QString decrypt(const std::string message);
        void freeListGroup(std::pair<char**, uint32_t>& list_group);
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
}