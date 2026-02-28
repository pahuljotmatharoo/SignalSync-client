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
#include "message.h"
#include "message_s.h"
#include "ui_ChattingWindow.h"
#include "NetworkClient.h"
#include "File.h"
#include "UserMessage.h"
#include "GroupMessage.h"
#include "LockGuard.h"

constexpr bool OTHER_USER = true;
constexpr bool CURR_USER = false;
constexpr bool UserB = true;
constexpr bool GroupB = false;
constexpr int MAX_THREADS = 5;

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
    std::unordered_map<QPushButton*, File*> m_filesUsers;
    std::unordered_map<QPushButton*, std::pair<File*, QString>> m_filesGroup;
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
    void setNetwork(Network& t_network) { m_network = t_network; };
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
    void userOrGroupSelect(std::unordered_map<QString, QPushButton*> &hide, std::unordered_map<QString, QPushButton*> &show, QPushButton*& lastPressedButton) const;
    void addFileButtonToScreenUser(File* recvFile, const std::string& fileName);
    void addFileButtonToScreenGroup(File* recvFile, const std::string& fileName, const std::string& groupName);
    void downloadUserFile();
    void downloadGroupFile();
    void processFileRecvUser(File* recvFile, const std::string& fileName);
    void processFileRecvGroup(File* recvFile, const std::string& fileName, const std::string& groupName);
    void destroyUserFiles();
    void destroyGroupFiles();
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

    template <class F, class... Args>
    void enqueue(F&& f, Args&&... args) // accepts args and function as universal reference (could be l-val or r-val)
    {
        // idek
        auto task = [fn = std::forward<F>(f), tp = std::make_tuple(std::forward<Args>(args)...)]() mutable // std::forward here to ensure it works with both r-val and l-val (otherwise compiler will assume its l-val)
            { 
                std::apply( [&](auto&&... xs) { std::invoke(fn, std::forward<decltype(xs)>(xs)...); }, tp );
            };
        m_queueMutex.lock();
        m_functionQueue.push(std::move(task));
        m_queueMutex.unlock();
        m_queueSemaphore.release();
    }

    void dequeue();
    void waitingThreadFunction();
    void initThreads();
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