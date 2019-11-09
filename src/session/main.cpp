/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com>
# All rights reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
#
# This file incorporates work covered by the following copyright and
# permission notice:
#
# Lumina Desktop Environment (https://lumina-desktop.org)
# Copyright (c) 2012-2017, Ken Moore (moorekou@gmail.com)
# All rights reserved
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the organization nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
*/

//===========================================
//  Lumina-DE source code
//  Copyright (c) 2016, Ken Moore
//  Available under the 3-clause BSD license
//===========================================

#include <QDebug>
#include <QCoreApplication>
#include <QProcess>
#include <QString>
#include <QX11Info>
#ifndef NO_DBUS
#include <QDBusConnection>
#include <QDBusInterface>
#endif
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
    /*if (!LUtils::isValidBinary(Draco::powerApp().toUtf8())) {
        qWarning() << "Power manager not found!";
        return 1;
    }
    if (!LUtils::isValidBinary(Draco::storageApp().toUtf8())) {
        qWarning() << "Storage manager not found!";
        return 1;
    }*/
    if (!LUtils::isValidBinary(Draco::launcherApp().toUtf8())) {
        qWarning("Application launcher not found!");
        return 1;
    }
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
#ifndef NO_DBUS
    QDBusInterface session(Draco::desktopSessionName(),
                           Draco::desktopSessionPath(),
                           Draco::desktopSessionName(),
                           QDBusConnection::sessionBus());
    if (session.isValid()) {
        qWarning("A desktop session is already running!");
        return 1;
    }
#else
    // WE NEED TO DO SOMETHING HERE!
#endif

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
