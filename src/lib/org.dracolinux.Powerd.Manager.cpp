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

#include "org.dracolinux.Powerd.Manager.h"
#include "org.dracolinux.Powerd.Manager.RTC.h"
#include "org.dracolinux.Powerd.Manager.Backlight.h"
#include "org.dracolinux.Powerd.Manager.CPU.h"

#include <QDebug>

Manager::Manager(QObject *parent) : QObject(parent)
{
}

bool Manager::SetWakeAlarm(const QString &alarm)
{
    qDebug() << "Try to set RTC wake alarm" << alarm;
    QDateTime date = QDateTime::fromString(alarm, "yyyy-MM-dd HH:mm:ss");
    if (date.isNull() || !date.isValid()) { return false; }
    return PowerRtc::setAlarm(date);
}

bool Manager::SetDisplayBacklight(const QString &device, int value)
{
    qDebug() << "Try to set DISPLAY backlight" << device << value;
    if (!PowerBacklight::canAdjustBrightness(device)) { return false; }
    int light = value;
    if (light>PowerBacklight::getMaxBrightness(device)) { light = PowerBacklight::getMaxBrightness(device); }
    else if (light<0) { light = 0; }
    return PowerBacklight::setCurrentBrightness(device, light);
}

bool Manager::SetCpuGovernor(const QString &gov)
{
    qDebug() << "Try to set CPU governor" << gov;
    return PowerCpu::setGovernor(gov);
}

bool Manager::SetCpuFrequency(const QString &freq)
{
    qDebug() << "Try to set CPU frequency" << freq;
    return  PowerCpu::setFrequency(freq);
}

