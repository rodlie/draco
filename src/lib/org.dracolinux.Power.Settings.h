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

#ifndef POWER_SETTINGS_H
#define POWER_SETTINGS_H

#include <QVariant>
#include <QString>

class PowerSettings
{
public:
    static void setValue(const QString &type, const QVariant &value);
    static const QVariant getValue(const QString &type);
    static bool isValid(const QString &type);
    static void saveDefault();
    static const QString getConf();
    static const QString getDir();
};

#endif // POWER_SETTINGS_H
