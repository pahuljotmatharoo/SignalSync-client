#include "message_s.h"
#include "ui_message_s.h"
#include <qboxlayout.h>

QSize MessageWidget_s::sizeHint() const {
    // forward the hint from your layout
    if (layout()) return layout()->sizeHint();
    return QWidget::sizeHint();
}

MessageWidget_s::MessageWidget_s(const QString& message, QWidget* parent): ui(new Ui::Message_chat_s)
{
        ui->setupUi(this);
        ui->message_send->setText(message);
        ui->message_send->setReadOnly(true);

        setMinimumSize(200, 50);
}

MessageWidget_s::~MessageWidget_s()
{
	delete ui;
}

