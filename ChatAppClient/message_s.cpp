#include "message_s.h"
#include "ui_message_s.h"

MessageWidget_s::MessageWidget_s(const QString& message, QWidget* parent): ui(new Ui::Message_chat_s)
{
	ui->setupUi(this);    // -> creates ui->label for you
	ui->message_send->setText(message);
}

MessageWidget_s::~MessageWidget_s()
{
	delete ui;
}

