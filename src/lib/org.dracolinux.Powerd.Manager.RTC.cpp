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

#include "org.dracolinux.Powerd.Manager.RTC.h"

#ifdef Q_OS_LINUX
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#define RTC_DEV "/dev/rtc"
#endif

bool PowerRtc::setAlarm(const QDateTime &date)
{
#ifdef Q_OS_LINUX
    if (!date.isValid() || date.isNull()) { return false; }

    int fd, result = 0;
    struct rtc_time rtc;

    fd = open(RTC_DEV, O_RDONLY);
    if (fd ==  -1) { return false; }

    result = ioctl(fd, RTC_UIE_OFF, 0);
    if (result == -1) {
        close(fd);
        return false;
    }

    result = ioctl(fd, RTC_RD_TIME, &rtc);
    if (result == -1) {
        close(fd);
        return false;
    }

    rtc.tm_year = date.date().year();
    rtc.tm_mon = date.date().month();
    rtc.tm_mday = date.date().day();
    rtc.tm_hour = date.time().hour();
    rtc.tm_min = date.time().minute();
    rtc.tm_sec = date.time().second();

    result = ioctl(fd, RTC_ALM_SET, &rtc);
    if (result == -1) {
        close(fd);
        return false;
    }

    result = ioctl(fd, RTC_AIE_ON, 0);
    if (result == -1) {
        close(fd);
        return false;
    }

    close(fd);
    return true;
#else
    return false;
#endif
}
