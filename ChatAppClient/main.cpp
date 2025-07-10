#include "ChatAppClient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ChatAppClient window;
    window.show();
    return app.exec();
}
