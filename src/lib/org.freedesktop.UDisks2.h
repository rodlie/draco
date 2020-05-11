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

#ifndef UDISKS2_H
#define UDISKS2_H

#include <QStringList>

#define DBUS_SERVICE "org.freedesktop.UDisks2"
#define DBUS_PATH "/org/freedesktop/UDisks2"
#define DBUS_OBJMANAGER "org.freedesktop.DBus.ObjectManager"
#define DBUS_PROPERTIES "org.freedesktop.DBus.Properties"
#define DBUS_INTROSPECTABLE "org.freedesktop.DBus.Introspectable"
#define DBUS_DEVICE_ADDED "InterfacesAdded"
#define DBUS_DEVICE_REMOVED "InterfacesRemoved"

class UDisks2
{
public:
    static const QString getDrivePath(QString path);
    static bool hasPartition(QString path);
    static const QString getFileSystem(QString path);
    static bool isRemovable(QString path);
    static bool isOptical(QString path);
    static bool hasMedia(QString path);
    static bool hasOpticalMedia(QString path);
    static bool canEject(QString path);
    static bool opticalMediaIsBlank(QString path);
    static int opticalDataTracks(QString path);
    static int opticalAudioTracks(QString path);
    static const QString getMountPointOptical(QString path);
    static const QString getMountPoint(QString path);
    static const QString getDeviceName(QString path);
    static const QString getDeviceLabel(QString path);
    static const QString mountDevice(QString path);
    static const QString mountOptical(QString path);
    static const QString unmountDevice(QString path);
    static const QString unmountOptical(QString path);
    static const QString ejectDevice(QString path);
    static const QStringList getDevices();
};

#endif // UDISKS2_H
