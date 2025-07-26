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
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QLineEdit *Message_input;
    QPushButton *sendButton;
    QFrame *frame;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QMenuBar *menubar;

    void setupUi(QMainWindow *ChattingWindow)
    {
        if (ChattingWindow->objectName().isEmpty())
            ChattingWindow->setObjectName("ChattingWindow");
        ChattingWindow->resize(1025, 757);
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
        verticalLayoutWidget->setGeometry(QRect(0, 170, 231, 561));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
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
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 221, 551));
        scrollArea_2->setWidget(scrollAreaWidgetContents_3);

        verticalLayout_2->addWidget(scrollArea_2);

        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(240, 70, 781, 661));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(layoutWidget);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setEnabled(true);
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
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 771, 578));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        Message_input = new QLineEdit(layoutWidget);
        Message_input->setObjectName("Message_input");
        Message_input->setEnabled(true);
        Message_input->setStyleSheet(QString::fromUtf8("border: 1px solid #C0C0C0;   /* subtle outline */\n"
"    border-radius: 12px;         /* round the outer border */\n"
"    padding: 5px;"));

        verticalLayout->addWidget(Message_input);

        sendButton = new QPushButton(layoutWidget);
        sendButton->setObjectName("sendButton");
        QFont font1;
        font1.setPointSize(12);
        sendButton->setFont(font1);
        sendButton->setStyleSheet(QString::fromUtf8(" border: 1px solid #C0C0C0;   /* subtle outline */\n"
" border-radius: 12px;         /* round the outer border */\n"
"padding: 5px;"));

        verticalLayout->addWidget(sendButton);

        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(240, 10, 777, 51));
        frame->setStyleSheet(QString::fromUtf8("border-radius: 5px;\n"
"border: 1px solid #C0C0C0;   /* subtle outline */"));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 0, 777, 51));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Arial")});
        font2.setPointSize(23);
        label->setFont(font2);
        label->setStyleSheet(QString::fromUtf8("text-align: center;"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(40, 130, 151, 31));
        QFont font3;
        font3.setPointSize(18);
        label_2->setFont(font3);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(30, 10, 181, 61));
        QFont font4;
        font4.setPointSize(24);
        label_3->setFont(font4);
        label_3->setStyleSheet(QString::fromUtf8("border-radius: 5px;\n"
"border: 1px solid #C0C0C0;   /* subtle outline */\n"
"padding: 5px;"));
        ChattingWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ChattingWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1025, 21));
        ChattingWindow->setMenuBar(menubar);

        retranslateUi(ChattingWindow);

        QMetaObject::connectSlotsByName(ChattingWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ChattingWindow)
    {
        ChattingWindow->setWindowTitle(QCoreApplication::translate("ChattingWindow", "MainWindow", nullptr));
        sendButton->setText(QCoreApplication::translate("ChattingWindow", "Send Message", nullptr));
        label->setText(QCoreApplication::translate("ChattingWindow", "Select a User to talk to!", nullptr));
        label_2->setText(QCoreApplication::translate("ChattingWindow", "Current Users", nullptr));
        label_3->setText(QCoreApplication::translate("ChattingWindow", "SignalSync", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChattingWindow: public Ui_ChattingWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATTINGWINDOW_H
