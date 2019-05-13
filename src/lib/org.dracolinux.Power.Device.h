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

#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QDBusInterface>

#define UPOWER_SERVICE "org.freedesktop.UPower"
#define DBUS_PROPERTIES "org.freedesktop.DBus.Properties"
#define DBUS_DEVICE "Device"
#define DBUS_CHANGED "Changed"

class Device : public QObject
{
    Q_OBJECT

public:
    enum DeviceType {
        DeviceUnknown,
        DeviceLinePower,
        DeviceBattery,
        DeviceUps,
        DeviceMonitor,
        DeviceMouse,
        DeviceKeyboard,
        DevicePda,
        DevicePhone
    };
    explicit Device(const QString block,
                    QObject *parent = NULL);
    QString name;
    QString path;
    QString model;
    DeviceType type;
    bool isRechargable;
    bool isPresent;
    double percentage;
    bool online;
    bool hasPowerSupply;
    bool isBattery;
    bool isAC;
    QString vendor;
    QString nativePath;
    double capacity;
    double energy;
    double energyFullDesign;
    double energyFull;
    double energyEmpty;
    qlonglong timeToEmpty;
    qlonglong timeToFull;

private:
    QDBusInterface *dbus;
    QDBusInterface *dbusp;

signals:
    void deviceChanged(const QString &devicePath);

private slots:
    void updateDeviceProperties();
public slots:
    void update();
    void updateBattery();
};

#endif // DEVICE_H
