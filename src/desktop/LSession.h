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
//  Copyright (c) 2012-2015, Ken Moore
//  Available under the 3-clause BSD license
//===========================================

#ifndef DESKTOP_SESSION_H
#define DESKTOP_SESSION_H

#include <QApplication>
#include <QDebug>
#include <QString>
#include <QX11Info>
#include <QEvent>
#include <QTranslator>
#include <QSettings>
#include <QProxyStyle>
#include <QDesktopWidget>
#include <QList>
#include <QThread>
#include <QThread>
#include <QUrl>
#include <QClipboard>
#ifndef NO_DBUS
#include <QDBusInterface>
#endif
//#include "powerkit.h"
#include "AppMenu.h"
//#include "SettingsMenu.h"
#include "SystemWindow.h"
#include "LDesktop.h"
#include "LuminaX11.h"
//#include "LuminaSingleApplication.h"
#include "LIconCache.h"

// SYSTEM TRAY STANDARD DEFINITIONS
#define SYSTEM_TRAY_REQUEST_DOCK 0
#define SYSTEM_TRAY_BEGIN_MESSAGE 1
#define SYSTEM_TRAY_CANCEL_MESSAGE 2

class LSession : public QApplication
{
    Q_OBJECT
public:
    LSession(int &argc, char **argv);
    ~LSession();

    // Functions to be called during startup
    void setupFallbackDesktop(QSettings *dset);
    void setupSession();

    // Public System Tray Functions
    QList<WId> currentTrayApps(WId visualTray);
    bool registerVisualTray(WId);
    void unregisterVisualTray(WId);

    // Public start menu buttons
    bool registerStartButton(QString ID);
    void unregisterStartButton(QString ID);

    // Special functions for XCB event filter parsing only
    // DO NOT USE MANUALLY
    void RootSizeChange();
    void WindowPropertyEvent();
    void WindowPropertyEvent(WId);
    void SysTrayDockRequest(WId);
    void WindowClosedEvent(WId);
    void WindowConfigureEvent(WId);
    void WindowDamageEvent(WId);
    void WindowSelectionClearEvent(WId);

    // System Access
    // Return a pointer to the current session
    static LSession* handle()
    {
        return static_cast<LSession*>(LSession::instance());
    }

    static void LaunchApplication(QString cmd);
    QFileInfoList DesktopFiles();

    QRect screenGeom(int num);

    AppMenu* applicationMenu();
    void systemWindow();
    //SettingsMenu* settingsMenu();

    LXCB *XCB; //class for XCB usage

    QSettings* sessionSettings();
    QSettings* DesktopPluginSettings();

    // Keep track of which non-desktop window should be treated as active
    WId activeWindow(); // This will return the last active window if a desktop element is currently active

    // Temporarily change the session locale (nothing saved between sessions)
    void switchLocale(QString localeCode);

    // REMOVE?
    // Window Adjustment Routine (due to Fluxbox not respecting _NET_WM_STRUT)
    void adjustWindowGeom(WId win, bool maximize = false);

private:
    // WMProcess *WM;
    QList<LDesktop*> DESKTOPS;
    QFileSystemWatcher *watcher;
    QTimer *screenTimer;
    QRect screenRect;
    bool xchange; //flag for when the x11 session was adjusted

    // Internal variable for global usage
    AppMenu *appmenu;
    //SettingsMenu *settingsmenu;
    SystemWindow *sysWindow;
    QTranslator *currTranslator;
    QSettings *sessionsettings, *DPlugSettings;
    bool cleansession;
    // QList<QRect> savedScreens;

    // System Tray Variables
    WId SystemTrayID, VisualTrayID;
    int TrayDmgEvent, TrayDmgError;
    QList<WId> RunningTrayApps;
    bool TrayStopping;

    //Start Button Variables
    QString StartButtonID;

    // Task Manager Variables
    WId lastActiveWin;
    QList<WId> RunningApps;
    QList<WId> checkWin;
    QFileInfoList desktopFiles;

    void CleanupSession();
    bool ignoreClipboard; // flag for (handle/store)Clipboard

    bool startupApps;

    //PowerKit *pm;
#ifndef NO_DBUS
    QDBusInterface *pm;
#endif

public slots:
    void StartLogout();
    void StartShutdown();
    void StartSuspend(bool hibernate = false);
    void StartReboot();
    void lockScreen();
    void reloadIconTheme();

    bool canShutdown();
    bool canReboot();
    bool canSuspend();
    bool canHibernate();

private slots:
    void NewCommunication(QStringList);
    void launchStartupApps(); //used during initialization
    void watcherChange(QString);
    void screensChanged();
    void screenResized(int);
    void checkWindowGeoms();

    // System Tray Functions
    void startSystemTray();
    void stopSystemTray(bool detachall = false);
    void attachTrayWindow(WId);
    void removeTrayWindow(WId);

    // Internal simplification functions
    void refreshWindowManager();
    void updateDesktops();
    void registerDesktopWindows();

    void SessionEnding();

    // Clipboard
    void handleClipboard(QClipboard::Mode mode);
    void storeClipboard(QString text, QClipboard::Mode mode);

signals:
    // System Tray Signals
    void VisualTrayAvailable(); //new Visual Tray Plugin can be registered
    void TrayListChanged(); //Item added/removed from the list
    void TrayIconChanged(WId); //WinID of Tray App

    // Start Button signals
    void StartButtonAvailable();
    void StartButtonActivated();

    // Task Manager Signals
    void WindowListEvent(WId);
    void WindowListEvent();

    // General Signals
    void LocaleChanged();
    void IconThemeChanged();
    void DesktopConfigChanged();
    void SessionConfigChanged();
    void FavoritesChanged();
    void DesktopFilesChanged();
    void MediaFilesChanged();
    void WorkspaceChanged();
};

#endif
