#include "ChatAppClient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SignalSync::ChatAppClient window;
    window.show();
    return app.exec();
}
