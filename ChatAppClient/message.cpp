// messagewidget.cpp
#include "message.h"
#include "ui_message_r.h"   // generated from message.ui
#include <qboxlayout.h>

QSize MessageWidget::sizeHint() const {
    // forward the hint from your layout
    if (layout()) return layout()->sizeHint();
    return QWidget::sizeHint();
}


MessageWidget::MessageWidget(const QString& message, QWidget* parent): ui(new Ui::Message_chat)
{
    ui->setupUi(this);
    ui->message_recv->setText(message);
    ui->message_recv->setReadOnly(true);        // since it’s a bubble

    setMinimumSize(200, 50);
}

MessageWidget::~MessageWidget()
{
    delete ui;
}
