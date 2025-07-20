/********************************************************************************
** Form generated from reading UI file 'message_r.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGE_R_H
#define UI_MESSAGE_R_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Message_chat
{
public:
    QLineEdit *message_recv;

    void setupUi(QWidget *Message_chat)
    {
        if (Message_chat->objectName().isEmpty())
            Message_chat->setObjectName("Message_chat");
        Message_chat->resize(172, 43);
        Message_chat->setStyleSheet(QString::fromUtf8(""));
        message_recv = new QLineEdit(Message_chat);
        message_recv->setObjectName("message_recv");
        message_recv->setGeometry(QRect(0, 0, 171, 41));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush1);
        QBrush brush2(QColor(255, 255, 255, 128));
        brush2.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush2);
#endif
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush1);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush2);
#endif
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush1);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush2);
#endif
        message_recv->setPalette(palette);
        message_recv->setStyleSheet(QString::fromUtf8("/* bubble background */\n"
"    background-color: rgb(0, 0, 0);\n"
"    /* light border */\n"
"    border: 1px solid #DADADA;\n"
"    /* round most corners */\n"
"    border-top-left-radius: 20px;\n"
"    border-top-right-radius: 20px;\n"
"    border-bottom-right-radius: 20px;\n"
"    /* leave bottom-left un-rounded for a little \342\200\234tail\342\200\235 effect */\n"
"    border-bottom-left-radius: 0px;\n"
"    /* give some breathing room around the text */\n"
"    padding: 6px 12px;\n"
"    /* text styling */\n"
"    color: rgb(255, 255, 255);\n"
"    font: 11pt \"Segoe UI\";"));
        message_recv->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        message_recv->setCursorMoveStyle(Qt::CursorMoveStyle::VisualMoveStyle);

        retranslateUi(Message_chat);

        QMetaObject::connectSlotsByName(Message_chat);
    } // setupUi

    void retranslateUi(QWidget *Message_chat)
    {
        Message_chat->setWindowTitle(QCoreApplication::translate("Message_chat", "Form", nullptr));
        message_recv->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Message_chat: public Ui_Message_chat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGE_R_H
