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

    if (!QDBusConnection::systemBus().registerService(Draco::powerdSessionName())) {
        qWarning() << "Failed to register service" << QDBusConnection::systemBus().lastError().message();
        return 1;
    }
    qDebug() << "Registered service" << Draco::powerdSessionName();

    Manager man;
    if (!QDBusConnection::systemBus().registerObject(Draco::powerdSessionPath(),
                                                     &man,
                                                     QDBusConnection::ExportAllContents)) {
        qWarning() << "Failed to register path" << Draco::powerdSessionPath() << QDBusConnection::systemBus().lastError().message();
        return 1;
    }
    qDebug() << "Registered service path" << Draco::powerdSessionPath();

    if (!QDBusConnection::systemBus().registerObject(Draco::powerdSessionFullPath(),
                                                     &man,
                                                     QDBusConnection::ExportAllContents)) {
        qWarning() << "Failed to register full path" << Draco::powerdSessionFullPath() << QDBusConnection::systemBus().lastError();
        return 1;
    }
    qDebug() << "Registered service full path" << Draco::powerdSessionFullPath();

    return a.exec();
}
