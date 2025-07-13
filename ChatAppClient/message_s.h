// messagewidget.h
#include <QWidget>

namespace Ui {
    class Message_chat_s;    // forward‐declared class generated from message.ui
}

class MessageWidget_s : public QWidget
{
    Q_OBJECT

public:
    explicit MessageWidget_s(const QString& message, QWidget* parent = nullptr);

    ~MessageWidget_s();
private:
    Ui::Message_chat_s* ui;   // owns all the controls from your .ui file
};
