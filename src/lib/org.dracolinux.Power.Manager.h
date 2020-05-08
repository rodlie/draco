/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com>
# All rights reserved.
#
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
#
*/

#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include <QObject>
#include <QStringList>
#include <QMap>
#include <QDBusInterface>
#include <QDBusObjectPath>
#include <QTimer>
#include <QDateTime>
#include <QDBusUnixFileDescriptor>

#include "org.dracolinux.Power.Device.h"

#define CONSOLEKIT_SERVICE "org.freedesktop.ConsoleKit"
#define CONSOLEKIT_PATH "/org/freedesktop/ConsoleKit/Manager"
#define CONSOLEKIT_MANAGER "org.freedesktop.ConsoleKit.Manager"

#define LOGIND_SERVICE "org.freedesktop.login1"
#define LOGIND_PATH "/org/freedesktop/login1"
#define LOGIND_MANAGER "org.freedesktop.login1.Manager"
#define LOGIND_DOCKED "Docked"

#define UPOWER_PATH "/org/freedesktop/UPower"
#define UPOWER_MANAGER "org.freedesktop.UPower"
#define UPOWER_DEVICES "/org/freedesktop/UPower/devices/"
#define UPOWER_DOCKED "IsDocked"
#define UPOWER_LID_IS_PRESENT "LidIsPresent"
#define UPOWER_LID_IS_CLOSED "LidIsClosed"
#define UPOWER_ON_BATTERY "OnBattery"
#define UPOWER_NOTIFY_RESUME "NotifyResume"
#define UPOWER_NOTIFY_SLEEP "NotifySleep"

#define PK_PREPARE_FOR_SUSPEND "PrepareForSuspend"
#define PK_PREPARE_FOR_SLEEP "PrepareForSleep"
#define PK_CAN_RESTART "CanReboot"
#define PK_RESTART "Reboot"
#define PK_CAN_POWEROFF "CanPowerOff"
#define PK_POWEROFF "PowerOff"
#define PK_CAN_SUSPEND "CanSuspend"
#define PK_SUSPEND_ALLOWED "SuspendAllowed"
#define PK_SUSPEND "Suspend"
#define PK_CAN_HIBERNATE "CanHibernate"
#define PK_HIBERNATE_ALLOWED "HibernateAllowed"
#define PK_HIBERNATE "Hibernate"
#define PK_CAN_HYBRIDSLEEP "CanHybridSleep"
#define PK_HYBRIDSLEEP "HybridSleep"
#define PK_NO_BACKEND "No backend available."
#define PK_NO_ACTION "Action no available."

#define DBUS_OK_REPLY "yes"
#define DBUS_FAILED_CONN "Failed D-Bus connection."
#define DBUS_OBJECT_MANAGER "org.freedesktop.DBus.ObjectManager"

#define DBUS_INTROSPECTABLE "org.freedesktop.DBus.Introspectable"
#define DBUS_JOBS "%1/jobs"
#define DBUS_DEVICE_ADDED "DeviceAdded"
#define DBUS_DEVICE_REMOVED "DeviceRemoved"
#define DBUS_DEVICE_CHANGED "DeviceChanged"

#define XSCREENSAVER "xscreensaver-command -deactivate"
#define XSCREENSAVER_LOCK "xscreensaver-command -lock"

#define TIMEOUT_CHECK 60000

class Power : public QObject
{
    Q_OBJECT

public:
    enum PKBackend {
        PKNoBackend,
        PKConsoleKit,
        PKLogind,
        PKUPower
    };

    enum PKAction {
        PKNoAction,
        PKRestartAction,
        PKPowerOffAction,
        PKSuspendAction,
        PKHibernateAction,
        PKHybridSleepAction
    };

    enum PKMethod {
        PKNoMethod,
        PKCanRestart,
        PKCanPowerOff,
        PKCanSuspend,
        PKCanHibernate,
        PKCanHybridSleep,
        PKSuspendAllowed,
        PKHibernateAllowed
    };

    explicit Power(QObject *parent = nullptr);
    ~Power();
    QMap<QString, Device*> getDevices();

private:
    QMap<QString, Device*> devices;
    QMap<quint32,QString> ssInhibitors;
    QMap<quint32,QString> pmInhibitors;

    QDBusInterface *upower;
    QDBusInterface *logind;
    QDBusInterface *ckit;
    QDBusInterface *pmd;

    QTimer timer;

    bool wasDocked;
    bool wasLidClosed;
    bool wasOnBattery;

    bool wakeAlarm;
    QDateTime wakeAlarmDate;

    QScopedPointer<QDBusUnixFileDescriptor> suspendLock;

    int suspendWakeupBattery;
    int suspendWakeupAC;

    bool lockScreenOnSuspend;
    bool lockScreenOnResume;

signals:
    void Update();
    void UpdatedDevices();
    void LidClosed();
    void LidOpened();
    void SwitchedToBattery();
    void SwitchedToAC();
    void PrepareForSuspend();
    void PrepareForResume();
    void DeviceWasRemoved(const QString &path);
    void DeviceWasAdded(const QString &path);
    void UpdatedInhibitors();

private slots:
    bool availableService(const QString &service,
                          const QString &path,
                          const QString &interface);
    bool availableAction(const PKMethod &method,
                         const PKBackend &backend);
    QString executeAction(const PKAction &action,
                          const PKBackend &backend);

    QStringList find();
    void setup();
    void check();
    void scan();

    void deviceAdded(const QDBusObjectPath &obj);
    void deviceAdded(const QString &path);
    void deviceRemoved(const QDBusObjectPath &obj);
    void deviceRemoved(const QString &path);
    void deviceChanged();
    void handleDeviceChanged(const QString &device);
    void handleResume();
    void handleSuspend();
    void handlePrepareForSuspend(bool prepare);
    void clearDevices();
    void handleNewInhibitScreenSaver(const QString &application,
                                     const QString &reason,
                                     quint32 cookie);
    void handleNewInhibitPowerManagement(const QString &application,
                                         const QString &reason,
                                         quint32 cookie);
    void handleDelInhibitScreenSaver(quint32 cookie);
    void handleDelInhibitPowerManagement(quint32 cookie);
    
    bool registerSuspendLock();
    void setWakeAlarmFromSettings();

public slots:
    bool HasConsoleKit();
    bool HasLogind();
    bool HasUPower();
    bool hasPMD();
    bool hasWakeAlarm();

    bool CanRestart();
    bool CanPowerOff();
    bool CanSuspend();
    bool CanHibernate();
    bool CanHybridSleep();

    QString Restart();
    QString PowerOff();
    QString Suspend();
    QString Hibernate();
    QString HybridSleep();
    bool setWakeAlarm(const QDateTime &date);
    void clearWakeAlarm();

    bool IsDocked();
    bool LidIsPresent();
    bool LidIsClosed();
    bool OnBattery();
    double BatteryLeft();
    void LockScreen();
    bool HasBattery();
    qlonglong TimeToEmpty();
    qlonglong TimeToFull();
    void UpdateDevices();
    void UpdateBattery();
    void UpdateConfig();
    QStringList ScreenSaverInhibitors();
    QStringList PowerManagementInhibitors();
    QMap<quint32, QString> GetInhibitors();
    const QDateTime getWakeAlarm();
    void releaseSuspendLock();
    void setSuspendWakeAlarmOnBattery(int value);
    void setSuspendWakeAlarmOnAC(int value);
    void setLockScreenOnSuspend(bool lock);
    void setLockScreenOnResume(bool lock);
    bool setDisplayBacklight(QString const &device, int value);
};

#endif // POWER_MANAGER_H
