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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatAppClientClass
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QLineEdit *lineEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ChatAppClientClass)
    {
        if (ChatAppClientClass->objectName().isEmpty())
            ChatAppClientClass->setObjectName("ChatAppClientClass");
        ChatAppClientClass->resize(600, 400);
        centralWidget = new QWidget(ChatAppClientClass);
        centralWidget->setObjectName("centralWidget");
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(250, 220, 80, 24));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(220, 140, 151, 24));
        ChatAppClientClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ChatAppClientClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        ChatAppClientClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ChatAppClientClass);
        mainToolBar->setObjectName("mainToolBar");
        ChatAppClientClass->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ChatAppClientClass);
        statusBar->setObjectName("statusBar");
        ChatAppClientClass->setStatusBar(statusBar);

        retranslateUi(ChatAppClientClass);

        QMetaObject::connectSlotsByName(ChatAppClientClass);
    } // setupUi

    void retranslateUi(QMainWindow *ChatAppClientClass)
    {
        ChatAppClientClass->setWindowTitle(QCoreApplication::translate("ChatAppClientClass", "ChatAppClient", nullptr));
        pushButton->setText(QCoreApplication::translate("ChatAppClientClass", "Connect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatAppClientClass: public Ui_ChatAppClientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATAPPCLIENT_H
