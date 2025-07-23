/********************************************************************************
** Form generated from reading UI file 'ChatAppClient.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATAPPCLIENT_H
#define UI_CHATAPPCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatAppClientClass
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QLineEdit *lineEdit;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QMainWindow *ChatAppClientClass)
    {
        if (ChatAppClientClass->objectName().isEmpty())
            ChatAppClientClass->setObjectName("ChatAppClientClass");
        ChatAppClientClass->resize(618, 443);
        centralWidget = new QWidget(ChatAppClientClass);
        centralWidget->setObjectName("centralWidget");
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(260, 330, 80, 24));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(200, 250, 201, 24));
        label = new QLabel(centralWidget);
        label->setObjectName("label");
        label->setGeometry(QRect(210, 40, 171, 51));
        QFont font;
        font.setPointSize(26);
        label->setFont(font);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(200, 230, 61, 16));
        ChatAppClientClass->setCentralWidget(centralWidget);

        retranslateUi(ChatAppClientClass);

        QMetaObject::connectSlotsByName(ChatAppClientClass);
    } // setupUi

    void retranslateUi(QMainWindow *ChatAppClientClass)
    {
        ChatAppClientClass->setWindowTitle(QCoreApplication::translate("ChatAppClientClass", "ChatAppClient", nullptr));
        pushButton->setText(QCoreApplication::translate("ChatAppClientClass", "Connect", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("ChatAppClientClass", "Enter a username", nullptr));
        label->setText(QCoreApplication::translate("ChatAppClientClass", "SignalSync", nullptr));
        label_2->setText(QCoreApplication::translate("ChatAppClientClass", "Username", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatAppClientClass: public Ui_ChatAppClientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATAPPCLIENT_H
