/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
# Available under the 3-clause BSD license
# See the LICENSE file for full details
#
*/

#include "draco.h"
#include "org.dracolinux.Power.Tray.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("freedesktop");
    QCoreApplication::setOrganizationDomain("org");

    if (!QDBusConnection::sessionBus().registerService(Draco::powerSessionName())) {
        qWarning() << "Failed to register service" << Draco::powerSessionName();
        return 1;
    }

    SysTray tray(a.parent());
    return a.exec();
}
