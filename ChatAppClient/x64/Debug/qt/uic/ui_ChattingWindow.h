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
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChattingWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *Username_input;
    QLabel *label;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QLineEdit *Message_input;
    QPushButton *sendButton;
    QMenuBar *menubar;
    QMenu *menuUser1;
    QMenu *menuUser2;

    void setupUi(QMainWindow *ChattingWindow)
    {
        if (ChattingWindow->objectName().isEmpty())
            ChattingWindow->setObjectName("ChattingWindow");
        ChattingWindow->resize(898, 623);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ChattingWindow->sizePolicy().hasHeightForWidth());
        ChattingWindow->setSizePolicy(sizePolicy);
        ChattingWindow->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        centralwidget = new QWidget(ChattingWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        Username_input = new QLineEdit(centralwidget);
        Username_input->setObjectName("Username_input");

        verticalLayout->addWidget(Username_input);

        label = new QLabel(centralwidget);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setEnabled(true);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 878, 470));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        Message_input = new QLineEdit(centralwidget);
        Message_input->setObjectName("Message_input");

        verticalLayout->addWidget(Message_input);

        sendButton = new QPushButton(centralwidget);
        sendButton->setObjectName("sendButton");

        verticalLayout->addWidget(sendButton);

        ChattingWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ChattingWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 898, 21));
        menuUser1 = new QMenu(menubar);
        menuUser1->setObjectName("menuUser1");
        menuUser2 = new QMenu(menubar);
        menuUser2->setObjectName("menuUser2");
        ChattingWindow->setMenuBar(menubar);

        menubar->addAction(menuUser1->menuAction());
        menubar->addAction(menuUser2->menuAction());

        retranslateUi(ChattingWindow);

        QMetaObject::connectSlotsByName(ChattingWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ChattingWindow)
    {
        ChattingWindow->setWindowTitle(QCoreApplication::translate("ChattingWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("ChattingWindow", "User to Send To:", nullptr));
        sendButton->setText(QCoreApplication::translate("ChattingWindow", "Send Message", nullptr));
        menuUser1->setTitle(QCoreApplication::translate("ChattingWindow", "User1", nullptr));
        menuUser2->setTitle(QCoreApplication::translate("ChattingWindow", "User2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChattingWindow: public Ui_ChattingWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATTINGWINDOW_H
