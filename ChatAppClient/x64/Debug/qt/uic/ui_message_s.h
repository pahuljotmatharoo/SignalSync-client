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
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Message_chat_s
{
public:
    QLabel *message_recv;

    void setupUi(QWidget *Message_chat_s)
    {
        if (Message_chat_s->objectName().isEmpty())
            Message_chat_s->setObjectName("Message_chat_s");
        Message_chat_s->resize(202, 51);
        message_recv = new QLabel(Message_chat_s);
        message_recv->setObjectName("message_recv");
        message_recv->setGeometry(QRect(0, 0, 202, 51));
        message_recv->setStyleSheet(QString::fromUtf8("background-color: #FFFFFF;     /* white bubble */\n"
"    border: 1px solid #E0E0E0;     /* light gray outline */\n"
"    border-radius: 12px;           /* round corners */\n"
"    color: #212121;                /* dark text */\n"
"    font-size: 16px;\n"
"qproperty-wordWrap: true;\n"
"/* wrap once the text exceeds this width */\n"
"    min-width: 80px;     /* never get narrower than 80px */\n"
"    min-height: 24px;    /* never shorter than a line of text */\n"
"    max-width: 200px;    /* wrap beyond ~200px */"));
        message_recv->setWordWrap(true);

        retranslateUi(Message_chat_s);

        QMetaObject::connectSlotsByName(Message_chat_s);
    } // setupUi

    void retranslateUi(QWidget * Message_chat_s)
    {
        Message_chat_s->setWindowTitle(QCoreApplication::translate("Message_chat_s", "Message_chat_s", nullptr));
        message_recv->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Message_chat_s : public Ui_Message_chat_s {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGE_S_H
