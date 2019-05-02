/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
# Available under the 3-clause BSD license
# See the LICENSE file for full details
#
*/

//===========================================
//  Lumina-DE source code
//  Copyright (c) 2016, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================

#include <QDebug>
#include <QCoreApplication>
#include <QProcess>
#include <QString>
#include <QX11Info>
#include <QDBusConnection>
#include <QDBusInterface>
#include <unistd.h>

#include "draco.h"
#include "session.h"
#include "LUtils.h"
#include "LDesktopUtils.h"
#include "LuminaXDG.h"

int findAvailableSession()
{
    int num = 0;
    while (QFile::exists(QString("/tmp/.X11-unix/X%1").arg(num))) { num++; }
    return num;
}

int main(int argc, char ** argv)
{
    // check for required binaries
    if (!LUtils::isValidBinary(Draco::desktopApp().toUtf8())) {
        qWarning("Desktop manager not found!");
        return 1;
    }
    if (!LUtils::isValidBinary(Draco::powerApp().toUtf8())) {
        qWarning() << "Power manager not found!";
        return 1;
    }
    /*if (!LUtils::isValidBinary("qtfm-tray")) {
        qWarning() << "Storage manager not found!";
        return 1;
    }
    if (!LUtils::isValidBinary(Draco::launcherApp().toUtf8())) {
        qWarning("Application launcher not found!");
        return 1;
    }*/
    if(!LUtils::isValidBinary(Draco::windowManager().toUtf8())){
        qWarning("Window manager not found!");
        return 1;
    }

    // check for dbus
    /*if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning("Cannot connect to the D-Bus session bus!");
        return 1;
    }*/

    // check for running desktop
    QDBusInterface session(Draco::desktopSessionName(),
                           Draco::desktopSessionPath(),
                           Draco::desktopSessionName(),
                           QDBusConnection::sessionBus());
    if (session.isValid()) {
        qWarning("A desktop session is already running!");
        return 1;
    }

    // Check configs
    Draco::themeEngineCheckConf();

    // Start X11 if needed
    QString disp = QString(getenv("DISPLAY")).simplified();
    if (disp.isEmpty()) {
        qDebug() << "No X11 session detected: will try to start one...";
        // No X session found. Go ahead and re-init this binary within an xinit call
        QString prog = QString(argv[0]).section("/", -1);
        LUtils::isValidBinary(prog); //will adjust the path to be absolute
        QStringList args; args << prog << "--" << ":" + QString::number(findAvailableSession());
        //if(LUtils::isValidBinary("x11vnc")){ args << "--" << "-listen" << "tcp"; } //need to be able to VNC into this session
        return QProcess::execute("xinit", args);
    }
    qDebug() << "Starting the desktop on current X11 session:" << disp;

    // Configure X11 monitors if needed
    if (LUtils::isValidBinary(Draco::xconfig().toUtf8())) {
        qDebug() << " - Resetting monitor configuration to last-used settings";
        QProcess::execute(QString("%1 --reset-monitors").arg(Draco::xconfig()));
    }

    // Setup any initialization values
    LXDG::setEnvironmentVars();
    setenv("DESKTOP_SESSION", DESKTOP_APP_NAME, 1);
    setenv("XDG_CURRENT_DESKTOP", DESKTOP_APP_NAME, 1);

    // Startup the session
    QApplication a(argc, argv);
    setenv("QT_QPA_PLATFORMTHEME", "qt5ct", 1); // make sure this is after the QApplication - not actually using the theme plugin for **this** process
    LSession sess;
    sess.start();
    return a.exec();
}
