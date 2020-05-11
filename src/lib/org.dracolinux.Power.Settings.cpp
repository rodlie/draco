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

#include "org.dracolinux.Power.Settings.h"
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QDebug>

#include "power_def.h"
#include "draco.h"

void PowerSettings::setValue(const QString &type, const QVariant &value)
{
    QSettings settings(Draco::powerSettingsFile(), QSettings::IniFormat);
    settings.setValue(type, value);
    settings.sync();
}

const QVariant PowerSettings::getValue(const QString &type)
{
    QSettings settings(Draco::powerSettingsFile(), QSettings::IniFormat);
    return settings.value(type);
}

bool PowerSettings::isValid(const QString &type)
{
    QSettings settings(Draco::powerSettingsFile(), QSettings::IniFormat);
    return settings.value(type).isValid();
}

void PowerSettings::saveDefault()
{
    setValue(CONF_LID_BATTERY_ACTION, LID_BATTERY_DEFAULT);
    setValue(CONF_LID_AC_ACTION, LID_AC_DEFAULT);
    setValue(CONF_CRITICAL_BATTERY_ACTION, CRITICAL_DEFAULT);
    setValue(CONF_CRITICAL_BATTERY_TIMEOUT, CRITICAL_BATTERY);
    setValue(CONF_SUSPEND_BATTERY_TIMEOUT, AUTO_SLEEP_BATTERY);
    setValue(CONF_FREEDESKTOP_SS, true);
    setValue(CONF_FREEDESKTOP_PM, true);
    setValue(CONF_TRAY_NOTIFY, true);
    setValue(CONF_TRAY_SHOW, true);
    setValue(CONF_LID_DISABLE_IF_EXTERNAL, false);
    setValue(CONF_SUSPEND_BATTERY_ACTION, suspendSleep);
    setValue(CONF_SUSPEND_AC_ACTION, suspendNone);
    setValue(CONF_BACKLIGHT_BATTERY_ENABLE, false);
    setValue(CONF_BACKLIGHT_AC_ENABLE, false);
    setValue(CONF_BACKLIGHT_BATTERY_DISABLE_IF_LOWER, false);
    setValue(CONF_BACKLIGHT_AC_DISABLE_IF_HIGHER, false);
    setValue(CONF_WARN_ON_LOW_BATTERY, true);
    setValue(CONF_WARN_ON_VERYLOW_BATTERY, true);
    setValue(CONF_NOTIFY_ON_BATTERY, true);
    setValue(CONF_NOTIFY_ON_AC, true);
    setValue(CONF_BACKLIGHT_MOUSE_WHEEL, true);
    setValue(CONF_SUSPEND_LOCK_SCREEN, true);
    setValue(CONF_PSTATE_MAX_BATTERY, 100);
    setValue(CONF_PSTATE_MAX_AC, 100);
}

const QString PowerSettings::getConf()
{
    return Draco::powerSettingsFile();
}

const QString PowerSettings::getDir()
{
    return Draco::configDir();
}
