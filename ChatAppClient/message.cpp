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

    // 1) Install a real layout on the wrapper so it computes a sizeHint:
    auto* h = new QHBoxLayout(this);
    h->setContentsMargins(0, 0, 0, 0);
    h->addWidget(ui->message_recv);

    // 2) Give the wrapper a size policy that respects its contents:
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    // 3) Let Qt compute your preferred size now that you have a layout:
    adjustSize();
}

MessageWidget::~MessageWidget()
{
    delete ui;
}
