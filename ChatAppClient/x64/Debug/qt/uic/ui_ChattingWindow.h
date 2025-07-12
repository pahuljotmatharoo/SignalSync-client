/********************************************************************************
** Form generated from reading UI file 'ChattingWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATTINGWINDOW_H
#define UI_CHATTINGWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChattingWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QPushButton *sendButton;
    QLineEdit *Username_input;
    QLabel *label_2;
    QLineEdit *Message_input;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ChattingWindow)
    {
        if (ChattingWindow->objectName().isEmpty())
            ChattingWindow->setObjectName("ChattingWindow");
        ChattingWindow->resize(800, 600);
        centralwidget = new QWidget(ChattingWindow);
        centralwidget->setObjectName("centralwidget");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(220, 210, 91, 31));
        sendButton = new QPushButton(centralwidget);
        sendButton->setObjectName("sendButton");
        sendButton->setGeometry(QRect(320, 370, 101, 24));
        Username_input = new QLineEdit(centralwidget);
        Username_input->setObjectName("Username_input");
        Username_input->setGeometry(QRect(330, 210, 151, 24));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(200, 280, 101, 16));
        Message_input = new QLineEdit(centralwidget);
        Message_input->setObjectName("Message_input");
        Message_input->setGeometry(QRect(310, 280, 371, 24));
        ChattingWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ChattingWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        ChattingWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ChattingWindow);
        statusbar->setObjectName("statusbar");
        ChattingWindow->setStatusBar(statusbar);

        retranslateUi(ChattingWindow);

        QMetaObject::connectSlotsByName(ChattingWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ChattingWindow)
    {
        ChattingWindow->setWindowTitle(QCoreApplication::translate("ChattingWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("ChattingWindow", "User to Send To:", nullptr));
        sendButton->setText(QCoreApplication::translate("ChattingWindow", "Send Message", nullptr));
        label_2->setText(QCoreApplication::translate("ChattingWindow", "Message to Send:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChattingWindow: public Ui_ChattingWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATTINGWINDOW_H
