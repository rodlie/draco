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

#ifndef POWER_CPU_H
#define POWER_CPU_H

#include <QStringList>
#include <QDBusInterface>

#define LINUX_CPU_SYS "/sys/devices/system/cpu"
#define LINUX_CPU_DIR "cpufreq"
#define LINUX_CPU_FREQUENCIES "scaling_available_frequencies"
#define LINUX_CPU_FREQUENCY "scaling_cur_freq"
#define LINUX_CPU_FREQUENCY_MAX "scaling_max_freq"
#define LINUX_CPU_FREQUENCY_MIN "scaling_min_freq"
#define LINUX_CPU_GOVERNORS "scaling_available_governors"
#define LINUX_CPU_GOVERNOR "scaling_governor"
#define LINUX_CPU_SET_SPEED "scaling_setspeed"
#define LINUX_CPU_PSTATE "intel_pstate"
#define LINUX_CPU_PSTATE_STATUS "status"
#define LINUX_CPU_PSTATE_NOTURBO "no_turbo"
#define LINUX_CPU_PSTATE_MAX_PERF "max_perf_pct"
#define LINUX_CPU_PSTATE_MIN_PERF "min_perf_pct"

class PowerCpu
{
public:
    static int getTotal();

    static const QString getGovernor(int cpu);
    static const QStringList getGovernors();
    static const QStringList getAvailableGovernors();
    static bool governorExists(const QString &gov);
    static bool setGovernor(const QString &gov, int cpu);
    static bool setGovernor(const QString &gov);

    static const QString getFrequency(int cpu);
    static const QStringList getFrequencies();
    static const QStringList getAvailableFrequency();
    static bool frequencyExists(const QString &freq);
    static bool setFrequency(const QString &freq, int cpu);
    static bool setFrequency(const QString &freq);

    static bool hasPState();
    static bool hasPStateTurbo();
    static bool setPStateTurbo(bool turbo);
    static int getPStateMax();
    static int getPStateMin();
    static bool setPStateMax(int maxState);
    static bool setPStateMin(int minState);
};

#endif // POWER_CPU_H
