#include "TheGame_Client.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TheGame_Client window;
    window.show();
    return app.exec();
}
