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
    ui->setupUi(this);

    // Keep only visual styling — remove width/height limits from the stylesheet
    ui->message_recv->setStyleSheet(R"(
        background-color: #2e2e2e; color:#E0E0E0; border:1px solid #E0E0E0; border-radius:12px;
        font-size:15px; font-family: "Montserrat";
    )");

    ui->message_recv->setWordWrap(true);
    ui->message_recv->setText(message);

    // Let width cap control wrapping; height becomes the minimum needed
    ui->message_recv->setMaximumWidth(500);
    ui->message_recv->setMaximumHeight(80);
    ui->message_recv->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    // Put it in a layout (critical for height-for-width to work)
    auto* ly = new QHBoxLayout(this);
    ly->setContentsMargins(0, 0, 0, 0);
    ly->addWidget(ui->message_recv);
}




MessageWidget::~MessageWidget()
{
    delete ui;
}
