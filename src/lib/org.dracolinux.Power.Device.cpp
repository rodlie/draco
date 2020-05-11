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

#include "org.dracolinux.Power.Device.h"

#include <QDBusConnection>
#include <QStringList>

#define PROP_CHANGED "PropertiesChanged"
#define PROP_DEV_MODEL "Model"
#define PROP_DEV_CAPACITY "Capacity"
#define PROP_DEV_IS_RECHARGE "IsRechargeable"
#define PROP_DEV_PRESENT "IsPresent"
#define PROP_DEV_PERCENT "Percentage"
#define PROP_DEV_ENERGY_FULL_DESIGN "EnergyFullDesign"
#define PROP_DEV_ENERGY_FULL "EnergyFull"
#define PROP_DEV_ENERGY_EMPTY "EnergyEmpty"
#define PROP_DEV_ENERGY "Energy"
#define PROP_DEV_ONLINE "Online"
#define PROP_DEV_POWER_SUPPLY "PowerSupply"
#define PROP_DEV_TIME_TO_EMPTY "TimeToEmpty"
#define PROP_DEV_TIME_TO_FULL "TimeToFull"
#define PROP_DEV_TYPE "Type"
#define PROP_DEV_VENDOR "Vendor"
#define PROP_DEV_NATIVEPATH "NativePath"

PowerDevice::PowerDevice(const QString block, QObject *parent)
    : QObject(parent)
    , path(block)
    , isRechargable(false)
    , isPresent(false)
    , percentage(0)
    , online(false)
    , hasPowerSupply(false)
    , isBattery(false)
    , isAC(false)
    , capacity(0)
    , energy(0)
    , energyFullDesign(0)
    , energyFull(0)
    , energyEmpty(0)
    , dbus(nullptr)
    , dbusp(nullptr)
{
    QDBusConnection system = QDBusConnection::systemBus();
    dbus = new QDBusInterface(UPOWER_SERVICE,
                              path,
                              QString("%1.%2").arg(UPOWER_SERVICE).arg(DBUS_DEVICE),
                              system,
                              parent);
    system.connect(dbus->service(),
                   dbus->path(),
                   QString("%1.%2").arg(UPOWER_SERVICE).arg(DBUS_DEVICE),
                   DBUS_CHANGED,
                   this,
                   SLOT(updateDeviceProperties()));
    dbusp = new QDBusInterface(UPOWER_SERVICE,
                               path,
                               DBUS_PROPERTIES,
                               system,
                               parent);
    system.connect(dbusp->service(),
                   dbusp->path(),
                   DBUS_PROPERTIES,
                   PROP_CHANGED,
                   this,
                   SLOT(updateDeviceProperties()));
    if (name.isEmpty()) { name = path.split("/").takeLast(); }
    updateDeviceProperties();
}

// get device properties
void PowerDevice::updateDeviceProperties()
{
    if (!dbus->isValid()) { return; }

    model = dbus->property(PROP_DEV_MODEL).toString();
    capacity =  dbus->property(PROP_DEV_CAPACITY).toDouble();
    isRechargable =  dbus->property(PROP_DEV_IS_RECHARGE).toBool();
    isPresent =  dbus->property(PROP_DEV_PRESENT).toBool();
    percentage =  dbus->property(PROP_DEV_PERCENT).toDouble();
    energyFullDesign = dbus->property(PROP_DEV_ENERGY_FULL_DESIGN).toDouble();
    energyFull = dbus->property(PROP_DEV_ENERGY_FULL).toDouble();
    energyEmpty = dbus->property(PROP_DEV_ENERGY_EMPTY).toDouble();
    energy = dbus->property(PROP_DEV_ENERGY).toDouble();
    online = dbus->property(PROP_DEV_ONLINE).toBool();
    hasPowerSupply = dbus->property(PROP_DEV_POWER_SUPPLY).toBool();
    timeToEmpty = dbus->property(PROP_DEV_TIME_TO_EMPTY).toLongLong();
    timeToFull = dbus->property(PROP_DEV_TIME_TO_FULL).toLongLong();
    type = (PowerDeviceType)dbus->property(PROP_DEV_TYPE).toUInt();

    if (type == PowerDeviceBattery) { isBattery = true; }
    else {
        isBattery = false;
        if (type == PowerDeviceLinePower) { isAC = true; }
        else { isAC = false; }
    }

    vendor = dbus->property(PROP_DEV_VENDOR).toString();
    nativePath = dbus->property(PROP_DEV_NATIVEPATH).toString();

    emit deviceChanged(path);
}

void PowerDevice::update()
{
    updateDeviceProperties();
}

void PowerDevice::updateBattery()
{
    percentage =  dbus->property(PROP_DEV_PERCENT).toDouble();
    timeToEmpty = dbus->property(PROP_DEV_TIME_TO_EMPTY).toLongLong();
    timeToFull = dbus->property(PROP_DEV_TIME_TO_FULL).toLongLong();
}
