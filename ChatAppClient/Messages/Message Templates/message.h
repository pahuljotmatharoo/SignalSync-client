// messagewidget.h
#include <QWidget>

namespace Ui {
    class Message_chat;    // forward‐declared class generated from message.ui
}

class MessageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MessageWidget(const QString& message, QWidget* parent = nullptr);
    ~MessageWidget();
    QSize sizeHint() const;
private:
    Ui::Message_chat* ui;   // owns all the controls from your .ui file
};
