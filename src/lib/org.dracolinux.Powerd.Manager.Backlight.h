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

#ifndef POWER_BACKLIGHT_H
#define POWER_BACKLIGHT_H

#include <QString>

class PowerBacklight
{
public:
    static const QString getDevice();
    static bool canAdjustBrightness(const QString &device);
    static bool canAdjustBrightness();
    static int getMaxBrightness(const QString &device);
    static int getMaxBrightness();
    static int getCurrentBrightness(const QString &device);
    static int getCurrentBrightness();
    static bool setCurrentBrightness(const QString &device, int value);
    static bool setCurrentBrightness(int value);
};

#endif // POWER_BACKLIGHT_H
