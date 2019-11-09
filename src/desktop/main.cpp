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
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================

#ifndef NO_DBUS
#include <QDBusConnection>
#include <QDBusInterface>
#endif

#include "LSession.h"
#include "LuminaXDG.h"
#include "draco.h"

int main(int argc, char ** argv)
{
#ifndef NO_DBUS
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
#else
    // WE NEED TO DO SOMETHING HERE!
#endif

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
