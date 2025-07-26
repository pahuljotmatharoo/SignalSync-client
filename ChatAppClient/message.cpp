// messagewidget.cpp
#include "message.h"
#include "ui_message_r.h"   // generated from message.ui
#include <qboxlayout.h>
#include <qtimer.h>
#include <QAbstractTextDocumentLayout>

QSize MessageWidget::sizeHint() const {
    // forward the hint from your layout
    if (layout()) return layout()->sizeHint();
    return QWidget::sizeHint();
}


MessageWidget::MessageWidget(const QString& message, QWidget* parent)
    : QWidget(parent), ui(new Ui::Message_chat)
{
    setSizePolicy({ QSizePolicy::Expanding, QSizePolicy::Preferred });
    ui->setupUi(this);
    ui->message_recv->setText(message);
}


MessageWidget::~MessageWidget()
{
    delete ui;
}
