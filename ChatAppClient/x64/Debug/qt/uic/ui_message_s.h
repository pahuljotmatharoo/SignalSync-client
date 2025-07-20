/********************************************************************************
** Form generated from reading UI file 'message_s.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGE_S_H
#define UI_MESSAGE_S_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Message_chat_s
{
public:
    QLineEdit *message_send;

    void setupUi(QWidget *Message_chat_s)
    {
        if (Message_chat_s->objectName().isEmpty())
            Message_chat_s->setObjectName("Message_chat_s");
        Message_chat_s->resize(123, 41);
        message_send = new QLineEdit(Message_chat_s);
        message_send->setObjectName("message_send");
        message_send->setGeometry(QRect(0, 0, 121, 41));
        message_send->setStyleSheet(QString::fromUtf8("/* bubble background */\n"
"    background-color: rgb(0, 0, 0);\n"
"    /* light border */\n"
"    border: 1px solid #DADADA;\n"
"    /* round most corners */\n"
"    border-top-left-radius: 20px;\n"
"    border-top-right-radius: 20px;\n"
"    border-bottom-right-radius: 0px;\n"
"    /* leave bottom-left un-rounded for a little \342\200\234tail\342\200\235 effect */\n"
"    border-bottom-left-radius: 20px;\n"
"    /* give some breathing room around the text */\n"
"    padding: 6px 12px;\n"
"    /* text styling */\n"
"    color: rgb(255, 255, 255);\n"
"    font: 11pt \"Segoe UI\";"));
        message_send->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTop|Qt::AlignmentFlag::AlignTrailing);

        retranslateUi(Message_chat_s);

        QMetaObject::connectSlotsByName(Message_chat_s);
    } // setupUi

    void retranslateUi(QWidget *Message_chat_s)
    {
        Message_chat_s->setWindowTitle(QCoreApplication::translate("Message_chat_s", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Message_chat_s: public Ui_Message_chat_s {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGE_S_H
