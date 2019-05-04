#include "org.dracolinux.Storage.Tray.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SysTray tray(a.parent());
    return a.exec();
}
