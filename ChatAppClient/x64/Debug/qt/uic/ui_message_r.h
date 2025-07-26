/********************************************************************************
** Form generated from reading UI file 'message_r.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGE_R_H
#define UI_MESSAGE_R_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <qboxlayout.h>

QT_BEGIN_NAMESPACE

class Ui_Message_chat
{
public:
    QLabel *message_recv;

    void setupUi(QWidget *Message_chat)
    {
        if (Message_chat->objectName().isEmpty())
            Message_chat->setObjectName("Message_chat");

        auto* mainLayout = new QVBoxLayout(Message_chat);

        message_recv = new QLabel(Message_chat);
        message_recv->setObjectName("message_recv");

        // wrapping + width bounds
        message_recv->setWordWrap(true);
        message_recv->setMinimumWidth(80);
        message_recv->setMinimumHeight(10);
        message_recv->setMaximumWidth(500);

        // allow it to stretch horizontally
        message_recv->setSizePolicy(
            QSizePolicy(QSizePolicy::Expanding,
                QSizePolicy::MinimumExpanding)
        );

        // bubble styling…
        message_recv->setStyleSheet(
            "background-color: #FFFFFF;"
            "border: 1px solid #E0E0E0;"
            "border-radius: 12px;"
            "padding: 8px 12px;"
            "font-size: 16px;"
            "color: #212121;"
        );

        mainLayout->addWidget(message_recv);
        //mainLayout->addStretch(1);

        retranslateUi(Message_chat);

        QMetaObject::connectSlotsByName(Message_chat);
    } // setupUi

    void retranslateUi(QWidget *Message_chat)
    {
        Message_chat->setWindowTitle(QCoreApplication::translate("Message_chat", "Form", nullptr));
        message_recv->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Message_chat: public Ui_Message_chat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGE_R_H
