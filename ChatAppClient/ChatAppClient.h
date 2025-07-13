#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ChatAppClient.h"

class ChatAppClient : public QMainWindow
{
    Q_OBJECT

private:
    Ui::ChatAppClientClass ui;
    struct Impl;
    Impl* impl_;
    QString username;
public:
    ChatAppClient(QWidget* parent = nullptr);
    ~ChatAppClient();
    void throw_connection_error();
    //void set_WSADATA(WSADATA wsData);

private slots: // Declare slots in this section
    void on_pushButton_clicked();
    void on_lineEdit_textEdited(const QString& text);
};

