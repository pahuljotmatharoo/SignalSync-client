#pragma once

#include <QtWidgets/QMainWindow>
#include "NetworkClient.h"
#include "HttpRequest.h"

namespace Ui { class ChatAppClientClass; }

class ChatAppClient : public QMainWindow
{
    Q_OBJECT

private:
    Ui::ChatAppClientClass* ui;
    std::string m_apiKey;
    QString m_username;
    QString m_password;
    Network m_network;
    HTTPRequest m_http;
public:
    ChatAppClient(QWidget* parent = nullptr);
    ~ChatAppClient();
    static void sendError(const QString& error_message);
    static std::string generateAPIKey(int length);
private slots: // Declare slots in this section
    void on_connectButton_clicked();
    void on_registerButton_clicked();
    void on_deleteButton_clicked();
    void on_usernameEdit_textEdited(const QString& text);
    void on_passwordEdit_textEdited(const QString& text);
};

