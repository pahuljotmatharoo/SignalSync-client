#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ChatAppClient.h"
#include <NetworkClient.h>

class ChatAppClient : public QMainWindow
{
    Q_OBJECT

private:
    Ui::ChatAppClientClass ui;
    QString username;
    Network m_network;
public:
    ChatAppClient(QWidget* parent = nullptr);
    void send_error(const QString& error_message);
private slots: // Declare slots in this section
    void on_pushButton_clicked();
    void on_lineEdit_textEdited(const QString& text);
};

