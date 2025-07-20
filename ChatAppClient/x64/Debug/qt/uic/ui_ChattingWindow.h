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
    QLineEdit *Message_input;
    QPushButton *sendButton;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QLabel *label;
    QLabel *label_2;
    QMenuBar *menubar;

    void setupUi(QMainWindow *ChattingWindow)
    {
        if (ChattingWindow->objectName().isEmpty())
            ChattingWindow->setObjectName("ChattingWindow");
        ChattingWindow->resize(1010, 743);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ChattingWindow->sizePolicy().hasHeightForWidth());
        ChattingWindow->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(16);
        ChattingWindow->setFont(font);
        ChattingWindow->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        ChattingWindow->setTabShape(QTabWidget::TabShape::Rounded);
        centralwidget = new QWidget(ChattingWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(0, 50, 231, 591));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(230, 50, 781, 661));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(layoutWidget);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setEnabled(true);
        scrollArea->setFrameShape(QFrame::Shape::StyledPanel);
        scrollArea->setFrameShadow(QFrame::Shadow::Raised);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 777, 592));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        Message_input = new QLineEdit(layoutWidget);
        Message_input->setObjectName("Message_input");
        Message_input->setEnabled(true);

        verticalLayout->addWidget(Message_input);

        sendButton = new QPushButton(layoutWidget);
        sendButton->setObjectName("sendButton");
        QFont font1;
        font1.setPointSize(12);
        sendButton->setFont(font1);

        verticalLayout->addWidget(sendButton);

        scrollArea_2 = new QScrollArea(centralwidget);
        scrollArea_2->setObjectName("scrollArea_2");
        scrollArea_2->setGeometry(QRect(0, 50, 231, 594));
        scrollArea_2->setStyleSheet(QString::fromUtf8("QScrollArea { border: 1px solid #555555; background-color: #2b2b2b;}\n"
""));
        scrollArea_2->setFrameShape(QFrame::Shape::VLine);
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName("scrollAreaWidgetContents_2");
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 229, 592));
        scrollArea_2->setWidget(scrollAreaWidgetContents_2);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(230, 0, 371, 41));
        QFont font2;
        label->setFont(font2);
        label->setStyleSheet(QString::fromUtf8("text-align: center;"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(50, 20, 151, 31));
        QFont font3;
        font3.setPointSize(18);
        label_2->setFont(font3);
        ChattingWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ChattingWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1010, 21));
        ChattingWindow->setMenuBar(menubar);

        retranslateUi(ChattingWindow);

        QMetaObject::connectSlotsByName(ChattingWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ChattingWindow)
    {
        ChattingWindow->setWindowTitle(QCoreApplication::translate("ChattingWindow", "MainWindow", nullptr));
        sendButton->setText(QCoreApplication::translate("ChattingWindow", "Send Message", nullptr));
        label->setText(QString());
        label_2->setText(QCoreApplication::translate("ChattingWindow", "Current Users", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChattingWindow: public Ui_ChattingWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATTINGWINDOW_H
