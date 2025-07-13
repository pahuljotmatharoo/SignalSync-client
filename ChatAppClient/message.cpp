// messagewidget.cpp
#include "message.h"
#include "ui_message.h"   // generated from message.ui

MessageWidget::MessageWidget(const QString& message, QWidget* parent): ui(new Ui::Message_chat)
{
    ui->setupUi(this);    // -> creates ui->label for you
    ui->message_recv->setText(message);
}

MessageWidget::~MessageWidget()
{
    delete ui;
}
