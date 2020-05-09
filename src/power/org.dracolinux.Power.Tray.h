/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
#
*/

#ifndef SYSTRAY_H
#define SYSTRAY_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QIcon>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QSettings>
#include <QPainter>
#include <QMap>
#include <QMapIterator>
#include <QMap>
#include <QFileSystemWatcher>
#include <QEvent>
#include <QWheelEvent>
#include <QProcess>
#include <QMenu>
#include <QLabel>
#include <QFrame>
#include <QWidgetAction>
#include <QSlider>
#include <QCheckBox>

#include "org.freedesktop.PowerManagement.h"
#include "org.freedesktop.ScreenSaver.h"
#include "org.dracolinux.Power.ScreenX11.h"
#include "org.dracolinux.Power.Manager.h"

#include <X11/extensions/scrnsaver.h>
#include "org.dracolinux.Power.HotPlugX11.h"
#undef CursorShape
#undef Bool
#undef None
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#undef FontChange
#undef Expose
#undef FrameFeature
#undef Unsorted

#define XSCREENSAVER_RUN "xscreensaver -no-splash"

#define DEVICE_UUID Qt::UserRole+1
#define DEVICE_TYPE Qt::UserRole+2
#define MAX_WIDTH 150

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    enum WheelAction {
        WheelUp,
        WheelDown
    };
    TrayIcon(QObject *parent = nullptr)
        : QSystemTrayIcon(parent), wheel_delta(0) {}
    TrayIcon(const QIcon &icon, QObject *parent = nullptr)
        : QSystemTrayIcon(icon, parent), wheel_delta(0) {}
    bool event(QEvent *event);
signals:
    void wheel(TrayIcon::WheelAction action);
private:
    int wheel_delta;
};

class SysTray : public QObject
{
    Q_OBJECT

public:
    explicit SysTray(QObject *parent = nullptr);
    ~SysTray();

private:
    TrayIcon *tray;
    Power *man;
    PowerManagement *pm;
    ScreenSaver *ss;
    HotPlug *ht;
    bool wasLowBattery;
    bool wasVeryLowBattery;
    int lowBatteryValue;
    int critBatteryValue;
    bool hasService;
    int lidActionBattery;
    int lidActionAC;
    int criticalAction;
    int autoSuspendBattery;
    int autoSuspendAC;
    QTimer *timer;
    int timeouts;
    bool showNotifications;
    bool desktopSS;
    bool desktopPM;
    bool showTray;
    bool disableLidOnExternalMonitors;
    int autoSuspendBatteryAction;
    int autoSuspendACAction;
    QProcess *xscreensaver;
    bool startupScreensaver;
    //QMap<quint32,QString> ssInhibitors;
    //QMap<quint32,QString> pmInhibitors;
    QString internalMonitor;
    QFileSystemWatcher *watcher;
    bool lidWasClosed;
    QString backlightDevice;
    bool hasBacklight;
    bool backlightOnBattery;
    bool backlightOnAC;
    int backlightBatteryValue;
    int backlightACValue;
    bool backlightBatteryDisableIfLower;
    bool backlightACDisableIfHigher;
    bool warnOnLowBattery;
    bool warnOnVeryLowBattery;
    bool notifyOnBattery;
    bool notifyOnAC;
    bool backlightMouseWheel;
    bool ignoreKernelResume;
    QMap<QString, bool> monitors;
    bool monitorHotplugSupport;

    QMenu *powerMenu;
    QMenu *inhibitorsMenu;
    QActionGroup *inhibitorsGroup;
    QAction *actSettings;
    QLabel *labelBatteryStatus;
    QLabel *labelBatteryIcon;
    QFrame *menuFrame;
    QWidgetAction *menuHeader;
    QSlider *backlightSlider;
    QLabel *backlightLabel;
    QFileSystemWatcher *backlightWatcher;
    QLabel *cpuFreqLabel;

    QSlider *pstateMinSlider;
    QSlider *pstateMaxSlider;
    QCheckBox *pstateTurboCheckbox;

private slots:
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void checkDevices();
    void handleClosedLid();
    void handleOpenedLid();
    void handleOnBattery();
    void handleOnAC();
    void loadSettings();
    void registerService();
    void handleHasInhibitChanged(bool has_inhibit);
    void handleLow(double left);
    void handleVeryLow(double left);
    void handleCritical(double left);
    void drawBattery(double left);
    void timeout();
    int xIdle();
    void resetTimer();
    void handleDisplay(const QString &display, bool connected);
    void handleFoundDisplays(QMap<QString,bool> displays);
    void setInternalMonitor();
    bool internalMonitorIsConnected();
    bool externalMonitorIsConnected();
    void handleNewInhibitScreenSaver(const QString &application,
                                     const QString &reason,
                                     quint32 cookie);
    void handleNewInhibitPowerManagement(const QString &application,
                                         const QString &reason,
                                         quint32 cookie);
    void handleDelInhibitScreenSaver(quint32 cookie);
    void handleDelInhibitPowerManagement(quint32 cookie);
    void handleScreensaverFinished(int exitcode);
    void showMessage(const QString &title,
                     const QString &msg,
                     bool critical = false);
    void handleConfChanged(const QString &file);
    void disableHibernate();
    void disableSuspend();
    void handlePrepareForSuspend();
    void handlePrepareForResume();
    void switchInternalMonitor(bool toggle);
    void handleTrayWheel(TrayIcon::WheelAction action);
    void handleDeviceChanged(const QString &path);

    void populateMenu();
    void updateMenu();
    void updateBacklight(const QString &file);
    void handleBacklightSlider(int value);
    void getInhibitors();
    void openSettings();
    void getCpuFreq();

};

#endif // SYSTRAY_H
