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

#include "org.dracolinux.Powerd.Manager.Backlight.h"
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QDebug>

const QString PowerBacklight::getDevice()
{
    QString path = "/sys/class/backlight";
    QDirIterator it(path, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString foundDir = it.next();
        if (foundDir.startsWith(QString("%1/radeon").arg(path))) {
            return foundDir;
        } else if (foundDir.startsWith(QString("%1/intel").arg(path))) {
            return foundDir;
        } else if (foundDir.startsWith(QString("%1/acpi").arg(path))) {
            return foundDir;
        }
    }
    return QString();
}

bool PowerBacklight::canAdjustBrightness(const QString &device)
{
    QFileInfo backlight(QString("%1/brightness").arg(device));
    if (backlight.isWritable()) { return true; }
    return false;
}

bool PowerBacklight::canAdjustBrightness()
{
    return canAdjustBrightness(getDevice());
}

int PowerBacklight::getMaxBrightness(const QString &device)
{
    int result = 0;
    QFile backlight(QString("%1/max_brightness").arg(device));
    if (backlight.open(QIODevice::ReadOnly)) {
        result = backlight.readAll().trimmed().toInt();
        backlight.close();
    }
    return result;
}

int PowerBacklight::getMaxBrightness()
{
    return getMaxBrightness(getDevice());
}

int PowerBacklight::getCurrentBrightness(const QString &device)
{
    int result = 0;
    QFile backlight(QString("%1/brightness").arg(device));
    if (backlight.open(QIODevice::ReadOnly)) {
        result = backlight.readAll().trimmed().toInt();
        backlight.close();
    }
    return result;
}

int PowerBacklight::getCurrentBrightness()
{
    return getCurrentBrightness(getDevice());
}

bool PowerBacklight::setCurrentBrightness(const QString &device, int value)
{
    if (!canAdjustBrightness(device)) { return false; }
    QFile backlight(QString("%1/brightness").arg(device));
    if (backlight.open(QIODevice::WriteOnly|QIODevice::Truncate)) {
        QTextStream out(&backlight);
        if (value<1) { value = 1; }
        out << QString::number(value);
        backlight.close();
        if (value == getCurrentBrightness(device)) { return true;}
    }
    return false;
}

bool PowerBacklight::setCurrentBrightness(int value)
{
    return setCurrentBrightness(getDevice(), value);
}
