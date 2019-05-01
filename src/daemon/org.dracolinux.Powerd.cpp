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
#include "org.dracolinux.Powerd.Manager.h"

#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setOrganizationDomain("org");
    QCoreApplication::setApplicationName(DAEMON_APP_NAME);

    if (!QDBusConnection::systemBus().isConnected()) {
        qWarning("Cannot connect to the D-Bus system bus.");
        return 1;
    }

    if (!QDBusConnection::systemBus().registerService(Draco::daemonSessionName())) {
        qWarning() << QDBusConnection::systemBus().lastError().message();
        return 1;
    }

    Manager man;
    if (!QDBusConnection::systemBus().registerObject(Draco::daemonSessionPath(),
                                                     &man,
                                                     QDBusConnection::ExportAllContents)) {
        qWarning() << QDBusConnection::systemBus().lastError().message();
        return 1;
    }
    if (!QDBusConnection::systemBus().registerObject(Draco::daemonSessionFullPath(),
                                                     &man,
                                                     QDBusConnection::ExportAllContents)) {
        qWarning() << QDBusConnection::systemBus().lastError().message();
        return 1;
    }

    return a.exec();
}
