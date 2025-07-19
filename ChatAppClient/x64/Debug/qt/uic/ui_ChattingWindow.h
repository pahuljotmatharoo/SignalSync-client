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
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChattingWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_2;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QLabel *label;
    QLineEdit *Message_input;
    QPushButton *sendButton;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QMenuBar *menubar;

    void setupUi(QMainWindow *ChattingWindow)
    {
        if (ChattingWindow->objectName().isEmpty())
            ChattingWindow->setObjectName("ChattingWindow");
        ChattingWindow->resize(1039, 727);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ChattingWindow->sizePolicy().hasHeightForWidth());
        ChattingWindow->setSizePolicy(sizePolicy);
        ChattingWindow->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        centralwidget = new QWidget(ChattingWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(0, 30, 291, 621));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(300, 10, 741, 701));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(layoutWidget);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setEnabled(true);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 737, 637));
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 0, 739, 16));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        Message_input = new QLineEdit(layoutWidget);
        Message_input->setObjectName("Message_input");
        Message_input->setEnabled(true);

        verticalLayout->addWidget(Message_input);

        sendButton = new QPushButton(layoutWidget);
        sendButton->setObjectName("sendButton");

        verticalLayout->addWidget(sendButton);

        scrollArea_2 = new QScrollArea(centralwidget);
        scrollArea_2->setObjectName("scrollArea_2");
        scrollArea_2->setGeometry(QRect(0, 30, 291, 621));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName("scrollAreaWidgetContents_2");
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 289, 619));
        scrollArea_2->setWidget(scrollAreaWidgetContents_2);
        ChattingWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ChattingWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1039, 21));
        ChattingWindow->setMenuBar(menubar);

        retranslateUi(ChattingWindow);

        QMetaObject::connectSlotsByName(ChattingWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ChattingWindow)
    {
        ChattingWindow->setWindowTitle(QCoreApplication::translate("ChattingWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("ChattingWindow", "Current User:", nullptr));
        sendButton->setText(QCoreApplication::translate("ChattingWindow", "Send Message", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChattingWindow: public Ui_ChattingWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATTINGWINDOW_H
