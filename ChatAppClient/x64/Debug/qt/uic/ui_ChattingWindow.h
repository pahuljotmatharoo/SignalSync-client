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
#include <QtWidgets/QFrame>
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
    QFrame *frame;
    QLabel *username_label;
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

    void setupUi(QMainWindow *ChattingWindow)
    {
        if (ChattingWindow->objectName().isEmpty())
            ChattingWindow->setObjectName("ChattingWindow");
        ChattingWindow->resize(1295, 920);
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
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(10, 320, 261, 531));
        userLayout = new QVBoxLayout(verticalLayoutWidget);
        userLayout->setObjectName("userLayout");
        userLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea_2 = new QScrollArea(verticalLayoutWidget);
        scrollArea_2->setObjectName("scrollArea_2");
        scrollArea_2->setEnabled(true);
        scrollArea_2->setStyleSheet(QString::fromUtf8("/* ===== scrollArea_2 & viewport ===== */\n"
"QScrollArea#scrollArea_2 {\n"
"    border: 1px solid #C0C0C0;   /* subtle outline */\n"
"    border-radius: 12px;         /* round the outer border */\n"
"    background-color: #ECE5DD;\n"
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
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName("scrollAreaWidgetContents_3");
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 251, 521));
        scrollArea_2->setWidget(scrollAreaWidgetContents_3);

        userLayout->addWidget(scrollArea_2);

        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(280, 10, 1001, 51));
        frame->setStyleSheet(QString::fromUtf8("border-radius: 5px;\n"
"border: 1px solid #C0C0C0;   /* subtle outline */"));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        username_label = new QLabel(frame);
        username_label->setObjectName("username_label");
        username_label->setGeometry(QRect(0, 0, 1001, 51));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Arial")});
        font1.setPointSize(23);
        username_label->setFont(font1);
        username_label->setStyleSheet(QString::fromUtf8("text-align: center;"));
        currUsers_label = new QLabel(centralwidget);
        currUsers_label->setObjectName("currUsers_label");
        currUsers_label->setGeometry(QRect(70, 280, 151, 31));
        QFont font2;
        font2.setPointSize(18);
        currUsers_label->setFont(font2);
        title_label = new QLabel(centralwidget);
        title_label->setObjectName("title_label");
        title_label->setGeometry(QRect(50, 10, 181, 61));
        QFont font3;
        font3.setPointSize(24);
        title_label->setFont(font3);
        title_label->setStyleSheet(QString::fromUtf8("border-radius: 5px;\n"
"border: 1px solid #C0C0C0;   /* subtle outline */\n"
"padding: 5px;"));
        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setEnabled(true);
        scrollArea->setGeometry(QRect(280, 74, 1001, 781));
        scrollArea->setAutoFillBackground(false);
        scrollArea->setStyleSheet(QString::fromUtf8("/* ===== ScrollArea & viewport ===== */\n"
"QScrollArea {\n"
"    border: 1px solid #C0C0C0;   /* subtle outline */\n"
"    border-radius: 12px;         /* round the outer border */\n"
"    background-color: #ECE5DD;\n"
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
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 993, 773));
        scrollAreaWidgetContents->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        verticalLayoutWidget_2 = new QWidget(scrollAreaWidgetContents);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(10, 10, 1021, 751));
        chatLayout = new QVBoxLayout(verticalLayoutWidget_2);
        chatLayout->setSpacing(20);
        chatLayout->setObjectName("chatLayout");
        chatLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea->setWidget(scrollAreaWidgetContents);
        Message_input = new QLineEdit(centralwidget);
        Message_input->setObjectName("Message_input");
        Message_input->setEnabled(true);
        Message_input->setGeometry(QRect(280, 860, 881, 51));
        Message_input->setStyleSheet(QString::fromUtf8("border: 1px solid #C0C0C0;   /* subtle outline */\n"
"    border-radius: 12px;         /* round the outer border */\n"
"    padding: 5px;"));
        sendButton = new QPushButton(centralwidget);
        sendButton->setObjectName("sendButton");
        sendButton->setGeometry(QRect(1160, 860, 121, 51));
        QFont font4;
        font4.setPointSize(12);
        sendButton->setFont(font4);
        sendButton->setStyleSheet(QString::fromUtf8(" border: 1px solid #C0C0C0;   /* subtle outline */\n"
" border-radius: 12px;         /* round the outer border */\n"
"padding: 5px;"));
        userList = new QPushButton(centralwidget);
        userList->setObjectName("userList");
        userList->setGeometry(QRect(80, 150, 111, 31));
        groupChat = new QPushButton(centralwidget);
        groupChat->setObjectName("groupChat");
        groupChat->setGeometry(QRect(80, 190, 111, 31));
        addGroup = new QPushButton(centralwidget);
        addGroup->setObjectName("addGroup");
        addGroup->setGeometry(QRect(110, 860, 80, 24));
        ChattingWindow->setCentralWidget(centralwidget);

        retranslateUi(ChattingWindow);

        QMetaObject::connectSlotsByName(ChattingWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ChattingWindow)
    {
        ChattingWindow->setWindowTitle(QCoreApplication::translate("ChattingWindow", "SignalSync", nullptr));
        username_label->setText(QCoreApplication::translate("ChattingWindow", "Select a User to talk to!", nullptr));
        currUsers_label->setText(QCoreApplication::translate("ChattingWindow", "Current Users", nullptr));
        title_label->setText(QCoreApplication::translate("ChattingWindow", "SignalSync", nullptr));
        Message_input->setPlaceholderText(QCoreApplication::translate("ChattingWindow", "Type a message...", nullptr));
        sendButton->setText(QCoreApplication::translate("ChattingWindow", "Send Message", nullptr));
        userList->setText(QCoreApplication::translate("ChattingWindow", "User List", nullptr));
        groupChat->setText(QCoreApplication::translate("ChattingWindow", "Group Chat", nullptr));
        addGroup->setText(QCoreApplication::translate("ChattingWindow", "Add ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChattingWindow: public Ui_ChattingWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATTINGWINDOW_H
