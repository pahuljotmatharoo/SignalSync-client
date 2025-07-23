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
#include <QtWidgets/QGroupBox>
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
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_3;
    QLabel *label_2;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QLineEdit *Message_input;
    QPushButton *sendButton;
    QLabel *label;
    QLabel *label_3;
    QGroupBox *groupBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QMenuBar *menubar;

    void setupUi(QMainWindow *ChattingWindow)
    {
        if (ChattingWindow->objectName().isEmpty())
            ChattingWindow->setObjectName("ChattingWindow");
        ChattingWindow->resize(1010, 744);
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
        verticalLayoutWidget->setGeometry(QRect(0, 160, 231, 561));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        scrollArea_2 = new QScrollArea(verticalLayoutWidget);
        scrollArea_2->setObjectName("scrollArea_2");
        scrollArea_2->setEnabled(true);
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName("scrollAreaWidgetContents_3");
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 227, 557));
        label_2 = new QLabel(scrollAreaWidgetContents_3);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(40, 0, 151, 31));
        QFont font1;
        font1.setPointSize(18);
        label_2->setFont(font1);
        scrollArea_2->setWidget(scrollAreaWidgetContents_3);

        verticalLayout_2->addWidget(scrollArea_2);

        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(230, 60, 781, 661));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(5);
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
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 777, 594));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        Message_input = new QLineEdit(layoutWidget);
        Message_input->setObjectName("Message_input");
        Message_input->setEnabled(true);

        verticalLayout->addWidget(Message_input);

        sendButton = new QPushButton(layoutWidget);
        sendButton->setObjectName("sendButton");
        QFont font2;
        font2.setPointSize(12);
        sendButton->setFont(font2);

        verticalLayout->addWidget(sendButton);

        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(230, 20, 371, 31));
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("text-align: center;"));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(0, 10, 131, 41));
        QFont font3;
        font3.setPointSize(20);
        label_3->setFont(font3);
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(0, 60, 231, 101));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(0, 10, 231, 24));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(0, 40, 231, 24));
        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(0, 70, 231, 24));
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
        label_2->setText(QCoreApplication::translate("ChattingWindow", "Current Users", nullptr));
        sendButton->setText(QCoreApplication::translate("ChattingWindow", "Send Message", nullptr));
        label->setText(QCoreApplication::translate("ChattingWindow", "Temp User Name", nullptr));
        label_3->setText(QCoreApplication::translate("ChattingWindow", "SignalSync", nullptr));
        groupBox->setTitle(QString());
        pushButton->setText(QCoreApplication::translate("ChattingWindow", "PushButton", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ChattingWindow", "PushButton", nullptr));
        pushButton_3->setText(QCoreApplication::translate("ChattingWindow", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChattingWindow: public Ui_ChattingWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATTINGWINDOW_H
