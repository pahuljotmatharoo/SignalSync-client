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

    void setupUi(QMainWindow *ChatAppClientClass)
    {
        if (ChatAppClientClass->objectName().isEmpty())
            ChatAppClientClass->setObjectName("ChatAppClientClass");
        ChatAppClientClass->resize(633, 411);
        centralWidget = new QWidget(ChatAppClientClass);
        centralWidget->setObjectName("centralWidget");
        centralWidget->setStyleSheet(QString::fromUtf8("background-color: #1e1e1e;\n"
"    color: #dddddd;"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(260, 290, 101, 31));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078d7;\n"
"    border: none;\n"
"    padding: 8px 16px;\n"
"    color: white;\n"
"    font-weight: bold;\n"
"    border-radius: 4px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #2892ff;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #005fa3;\n"
"}"));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(215, 230, 201, 31));
        lineEdit->setStyleSheet(QString::fromUtf8("padding: 6px;\n"
"    border: 1px solid #444;\n"
"    border-radius: 4px;\n"
"    background-color: #2c2c2c;\n"
"    color: #ccc;\n"
"    selection-background-color: #0078d7;"));
        label = new QLabel(centralWidget);
        label->setObjectName("label");
        label->setGeometry(QRect(210, 80, 211, 61));
        QFont font;
        font.setBold(true);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("font-size: 28px;\n"
"    font-weight: bold;\n"
"    letter-spacing: 1px;\n"
"    color: #ffffff;\n"
"    padding: 10px;\n"
"    border: 2px solid #3a3a3a;\n"
"    border-radius: 8px;\n"
"    background-color: #2a2a2a;\n"
"    qproperty-alignment: AlignCenter;"));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
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
    } // retranslateUi

};

namespace Ui {
    class ChatAppClientClass: public Ui_ChatAppClientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATAPPCLIENT_H
