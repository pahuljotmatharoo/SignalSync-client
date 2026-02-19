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
    QVBoxLayout *userLayout;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_3;
    QLabel *currUsers_label;
    QLabel *title_label;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *chatLayout;
    QLineEdit *Message_input;
    QPushButton *sendButton;
    QPushButton *userList;
    QPushButton *groupChat;
    QPushButton *addGroup;
    QLabel *username_label;
    QPushButton *fileButton;

    void setupUi(QMainWindow *ChattingWindow)
    {
        if (ChattingWindow->objectName().isEmpty())
            ChattingWindow->setObjectName("ChattingWindow");
        ChattingWindow->resize(1077, 680);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Ignored);
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
        centralwidget->setStyleSheet(QString::fromUtf8("background-color: #1e1e1e;\n"
"    color: #dddddd;"));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(10, 250, 261, 371));
        userLayout = new QVBoxLayout(verticalLayoutWidget);
        userLayout->setObjectName("userLayout");
        userLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea_2 = new QScrollArea(verticalLayoutWidget);
        scrollArea_2->setObjectName("scrollArea_2");
        scrollArea_2->setEnabled(true);
        scrollArea_2->setStyleSheet(QString::fromUtf8("/* ===== scrollArea_2 & viewport ===== */\n"
"QScrollArea#scrollArea_2 {\n"
"    border: 1px solid #3a3a3a;   /* subtle outline */\n"
"    border-radius: 8px;         /* round the outer border */\n"
"    background-color: #3a3a3a;\n"
"    padding: 3px;\n"
"}\n"
"\n"
"\n"
"/* ===== Vertical ScrollBar for scrollArea_2 ===== */\n"
"QScrollArea#scrollArea_2 QScrollBar:vertical {\n"
"    background: transparent;\n"
"    width: 6px;\n"
"    margin: 0;\n"
"}\n"
"QScrollArea#scrollArea_2 QScrollBar::handle:vertical {\n"
"    background: rgba(0,0,0,0.2);\n"
"    min-height: 20px;\n"
"    border-radius: 3px;\n"
"}\n"
"QScrollArea#scrollArea_2 QScrollBar::add-line,\n"
"QScrollArea#scrollArea_2 QScrollBar::sub-line,\n"
"QScrollArea#scrollArea_2 QScrollBar::add-page,\n"
"QScrollArea#scrollArea_2 QScrollBar::sub-page {\n"
"    height: 0;\n"
"}\n"
""));
        scrollArea_2->setWidgetResizable(true);
        scrollArea_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName("scrollAreaWidgetContents_3");
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 251, 361));
        scrollArea_2->setWidget(scrollAreaWidgetContents_3);

        userLayout->addWidget(scrollArea_2);

        currUsers_label = new QLabel(centralwidget);
        currUsers_label->setObjectName("currUsers_label");
        currUsers_label->setGeometry(QRect(46, 210, 191, 33));
        QFont font1;
        font1.setPointSize(18);
        font1.setBold(true);
        currUsers_label->setFont(font1);
        currUsers_label->setStyleSheet(QString::fromUtf8("font-weight: bold;"));
        currUsers_label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        title_label = new QLabel(centralwidget);
        title_label->setObjectName("title_label");
        title_label->setGeometry(QRect(42, 10, 191, 61));
        QFont font2;
        font2.setBold(true);
        title_label->setFont(font2);
        title_label->setStyleSheet(QString::fromUtf8("font-size: 28px;\n"
"    font-weight: bold;\n"
"    letter-spacing: 1px;\n"
"    color: #ffffff;\n"
"    padding: 10px;\n"
"    border: 2px solid #3a3a3a;\n"
"    border-radius: 8px;\n"
"    background-color: #2a2a2a;\n"
"    qproperty-alignment: AlignCenter;"));
        title_label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setEnabled(true);
        scrollArea->setGeometry(QRect(280, 74, 781, 551));
        scrollArea->setAutoFillBackground(false);
        scrollArea->setStyleSheet(QString::fromUtf8("/* ===== ScrollArea & viewport ===== */\n"
"QScrollArea {\n"
"    border: 1px solid #3a3a3a;   /* subtle outline */\n"
"    border-radius: 8px;         /* round the outer border */\n"
"    background-color: #3a3a3a;\n"
"    padding: 3px;\n"
"}\n"
"\n"
"/* ===== Vertical ScrollBar ===== */\n"
"QScrollBar:vertical {\n"
"    background: transparent;\n"
"    width: 6px;\n"
"    margin: 0;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background: rgba(0,0,0,0.2);\n"
"    min-height: 20px;\n"
"    border-radius: 3px;\n"
"}\n"
"QScrollBar::add-line, QScrollBar::sub-line,\n"
"QScrollBar::add-page, QScrollBar::sub-page {\n"
"    height: 0px;\n"
"}\n"
"\n"
""));
        scrollArea->setFrameShape(QFrame::Shape::Panel);
        scrollArea->setFrameShadow(QFrame::Shadow::Plain);
        scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContentsOnFirstShow);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 773, 543));
        scrollAreaWidgetContents->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        verticalLayoutWidget_2 = new QWidget(scrollAreaWidgetContents);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(10, 10, 751, 521));
        chatLayout = new QVBoxLayout(verticalLayoutWidget_2);
        chatLayout->setSpacing(20);
        chatLayout->setObjectName("chatLayout");
        chatLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea->setWidget(scrollAreaWidgetContents);
        Message_input = new QLineEdit(centralwidget);
        Message_input->setObjectName("Message_input");
        Message_input->setEnabled(true);
        Message_input->setGeometry(QRect(281, 630, 661, 41));
        Message_input->setStyleSheet(QString::fromUtf8("padding: 6px;\n"
"    border: 1px solid #444;\n"
"    border-radius: 4px;\n"
"    background-color: #2c2c2c;\n"
"    color: #ccc;\n"
"    selection-background-color: #0078d7;"));
        sendButton = new QPushButton(centralwidget);
        sendButton->setObjectName("sendButton");
        sendButton->setGeometry(QRect(1005, 630, 55, 41));
        QFont font3;
        font3.setPointSize(12);
        font3.setBold(true);
        sendButton->setFont(font3);
        sendButton->setStyleSheet(QString::fromUtf8("QPushButton#sendButton {\n"
"    background-color: #0078d7;\n"
"    padding: 3px 7px;\n"
"    border-radius: 5px;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#sendButton:hover {\n"
"    background-color: #2892ff;\n"
"}\n"
"\n"
"QPushButton#sendButton:pressed {\n"
"    background-color: #005fa3;\n"
"}\n"
""));
        userList = new QPushButton(centralwidget);
        userList->setObjectName("userList");
        userList->setGeometry(QRect(40, 100, 191, 31));
        userList->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        groupChat = new QPushButton(centralwidget);
        groupChat->setObjectName("groupChat");
        groupChat->setGeometry(QRect(40, 140, 191, 31));
        groupChat->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        addGroup = new QPushButton(centralwidget);
        addGroup->setObjectName("addGroup");
        addGroup->setGeometry(QRect(50, 630, 191, 31));
        addGroup->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        username_label = new QLabel(centralwidget);
        username_label->setObjectName("username_label");
        username_label->setGeometry(QRect(280, 20, 771, 51));
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Arial")});
        font4.setPointSize(23);
        font4.setBold(true);
        username_label->setFont(font4);
        username_label->setStyleSheet(QString::fromUtf8("font-weight: bold;"));
        username_label->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        fileButton = new QPushButton(centralwidget);
        fileButton->setObjectName("fileButton");
        fileButton->setGeometry(QRect(950, 630, 51, 41));
        fileButton->setStyleSheet(QString::fromUtf8("QPushButton#fileButton {\n"
"    background-color: #0078d7;\n"
"    padding: 3px 7px;\n"
"    border-radius: 5px;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#fileButton:hover {\n"
"    background-color: #2892ff;\n"
"}\n"
"\n"
"QPushButton#fileButton:pressed {\n"
"    background-color: #005fa3;\n"
"}\n"
""));
        ChattingWindow->setCentralWidget(centralwidget);

        retranslateUi(ChattingWindow);

        QMetaObject::connectSlotsByName(ChattingWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ChattingWindow)
    {
        ChattingWindow->setWindowTitle(QCoreApplication::translate("ChattingWindow", "SignalSync", nullptr));
        currUsers_label->setText(QCoreApplication::translate("ChattingWindow", "Current Users", nullptr));
        title_label->setText(QCoreApplication::translate("ChattingWindow", "SignalSync", nullptr));
        Message_input->setPlaceholderText(QCoreApplication::translate("ChattingWindow", "Type a message...", nullptr));
        sendButton->setText(QString());
        userList->setText(QCoreApplication::translate("ChattingWindow", "User List", nullptr));
        groupChat->setText(QCoreApplication::translate("ChattingWindow", "Group Chat", nullptr));
        addGroup->setText(QCoreApplication::translate("ChattingWindow", "Add Group", nullptr));
        username_label->setText(QCoreApplication::translate("ChattingWindow", "Select a User to talk to!", nullptr));
        fileButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ChattingWindow: public Ui_ChattingWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATTINGWINDOW_H
