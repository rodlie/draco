//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================

#include <QDBusConnection>
#include <QDBusInterface>

#include "LSession.h"
#include "LuminaXDG.h"
#include "common.h"

int main(int argc, char ** argv)
{
    // Check for dbus
    if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning("Cannot connect to the D-Bus session bus.");
        return 1;
    }

    // Check for running desktop
    QDBusInterface session(Draco::desktopSessionName(),
                           Draco::desktopSessionPath(),
                           Draco::desktopSessionName(),
                           QDBusConnection::sessionBus());
    if (session.isValid()) {
        qWarning("A desktop session is already running");
        return 1;
    }

    // Register desktop session
    if (!QDBusConnection::sessionBus().registerService(Draco::desktopSessionName())) {
        qWarning() << QDBusConnection::sessionBus().lastError().message();
        return 1;
    }

    // Setup any pre-QApplication initialization values
    LXDG::setEnvironmentVars();
    setenv("DESKTOP_SESSION", DESKTOP_APP_NAME, 1);
    setenv("XDG_CURRENT_DESKTOP", DESKTOP_APP_NAME, 1);
    setenv("QT_QPA_PLATFORMTHEME", "qt5ct", 1);
    unsetenv("QT_AUTO_SCREEN_SCALE_FACTOR"); // causes pixel-specific scaling issues with the desktop - turn this on after-the-fact for other apps

    Draco::checkConfigs();

    // Startup the session
    LSession a(argc, argv);
    a.setupSession();
    return a.exec();
}
