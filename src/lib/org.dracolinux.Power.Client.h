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

#ifndef POWER_CLIENT_H
#define POWER_CLIENT_H

#include <QDBusInterface>

class PowerClient
{
public:
    static double getBatteryLeft(QDBusInterface *iface);
    static bool hasBattery(QDBusInterface *iface);
    static bool onBattery(QDBusInterface *iface);
    static qlonglong timeToEmpty(QDBusInterface *iface);
    static bool canHibernate(QDBusInterface *iface);
    static bool canSuspend(QDBusInterface *iface);
    static bool canRestart(QDBusInterface *iface);
    static bool canPowerOff(QDBusInterface *iface);
    static bool lidIsPresent(QDBusInterface *iface);
    static void lockScreen(QDBusInterface *iface);
    static void hibernate(QDBusInterface *iface);
    static void suspend(QDBusInterface *iface);
    static bool restart(QDBusInterface *iface);
    static bool poweroff(QDBusInterface *iface);
};

#endif // POWER_CLIENT_H
