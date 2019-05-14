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

#include "org.dracolinux.Powerd.Manager.CPU.h"

#include <QFile>
#include <QTextStream>

int PowerCpu::getTotal()
{
    int counter = 0;
    while(1) {
        if (QFile::exists(QString("%1/cpu%2")
                          .arg(LINUX_CPU_SYS)
                          .arg(counter))) { counter++; }
        else { break; }
    }
    if (counter>0) { return counter; }
    return -1;
}

const QString PowerCpu::getGovernor(int cpu)
{
    QString result;
    QFile gov(QString("%1/cpu%2/%3/%4")
              .arg(LINUX_CPU_SYS)
              .arg(cpu)
              .arg(LINUX_CPU_DIR)
              .arg(LINUX_CPU_GOVERNOR));
    if (!gov.exists()) { return result; }
    if (gov.open(QIODevice::ReadOnly|QIODevice::Text)) {
        result = gov.readAll().trimmed();
        gov.close();
    }
    return result;
}

const QStringList PowerCpu::getGovernors()
{
    QStringList result;
    for (int i=0;i<getTotal();++i) {
        QString value = getGovernor(i);
        if (!value.isEmpty()) { result << value; }
    }
    return result;
}

const QStringList PowerCpu::getAvailableGovernors()
{
    QStringList result;
    QFile gov(QString("%1/cpu%2/%3/%4")
              .arg(LINUX_CPU_SYS)
              .arg(0)
              .arg(LINUX_CPU_DIR)
              .arg(LINUX_CPU_GOVERNORS));
    if (!gov.exists()) { return result; }
    if (gov.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QString value = gov.readAll().trimmed();
        result = value.split(" ", QString::SkipEmptyParts);
        gov.close();
    }
    return result;
}

bool PowerCpu::governorExists(const QString &gov)
{
    if (gov.isEmpty()) { return false; }
    return getAvailableGovernors().contains(gov);
}

bool PowerCpu::setGovernor(const QString &gov, int cpu)
{
    if (!governorExists(gov)) { return false; }
    QFile file(QString("%1/cpu%2/%3/%4")
              .arg(LINUX_CPU_SYS)
              .arg(cpu)
              .arg(LINUX_CPU_DIR)
              .arg(LINUX_CPU_GOVERNOR));
    if (file.open(QIODevice::WriteOnly|QIODevice::Truncate)) {
        QTextStream out(&file);
        out << gov;
        file.close();
        if (gov == getGovernor(cpu)) { return true;}
    }
    return false;
}

bool PowerCpu::setGovernor(const QString &gov)
{
    if (!governorExists(gov)) { return false; }
    bool failed = false;
    for (int i=0;i<getTotal();++i) {
        if (!setGovernor(gov, i)) { failed = true; }
    }
    if (failed) { return false; }
    return true;
}

const QString PowerCpu::getFrequency(int cpu)
{
    QString result;
    QFile freq(QString("%1/cpu%2/%3/%4")
              .arg(LINUX_CPU_SYS)
              .arg(cpu)
              .arg(LINUX_CPU_DIR)
              .arg(LINUX_CPU_FREQUENCY));
    if (!freq.exists()) { return result; }
    if (freq.open(QIODevice::ReadOnly|QIODevice::Text)) {
        result = freq.readAll().trimmed();
        freq.close();
    }
    return result;
}

const QStringList PowerCpu::getFrequencies()
{
    QStringList result;
    for (int i=0;i<getTotal();++i) {
        QString value = getFrequency(i);
        if (!value.isEmpty()) { result << value; }
    }
    return result;
}

const QStringList PowerCpu::getAvailableFrequency()
{
    QStringList result;
    QFile gov(QString("%1/cpu%2/%3/%4")
              .arg(LINUX_CPU_SYS)
              .arg(0)
              .arg(LINUX_CPU_DIR)
              .arg(LINUX_CPU_FREQUENCIES));
    if (!gov.exists()) { return result; }
    if (gov.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QString value = gov.readAll().trimmed();
        result = value.split(" ", QString::SkipEmptyParts);
        gov.close();
    }
    return result;
}

bool PowerCpu::frequencyExists(const QString &freq)
{
    if (freq.isEmpty()) { return false; }
    return getAvailableFrequency().contains(freq);
}

bool PowerCpu::setFrequency(const QString &freq, int cpu)
{
    if (!frequencyExists(freq)) { return false; }
    if (getGovernor(cpu) != "userspace") {
        if (!setGovernor("userspace", cpu)) { return false; }
    }
    QFile file(QString("%1/cpu%2/%3/%4")
              .arg(LINUX_CPU_SYS)
              .arg(cpu)
              .arg(LINUX_CPU_DIR)
              .arg(LINUX_CPU_SET_SPEED));
    if (file.open(QIODevice::WriteOnly|QIODevice::Truncate)) {
        QTextStream out(&file);
        out << freq;
        file.close();
        if (freq == getFrequency(cpu)) { return true;}
    }
    return false;
}

bool PowerCpu::setFrequency(const QString &freq)
{
    if (!frequencyExists(freq)) { return false; }
    bool failed = false;
    for (int i=0;i<getTotal();++i) {
        if (!setFrequency(freq, i)) { failed = true; }
    }
    if (failed) { return false; }
    return true;
}

bool PowerCpu::hasPState()
{
    return QFile::exists(QString("%1/%1")
                         .arg(LINUX_CPU_SYS)
                         .arg(LINUX_CPU_PSTATE));
}

bool PowerCpu::hasPStateTurbo()
{
    bool result = false;
    QFile turbo(QString("%1/%2/%3")
                .arg(LINUX_CPU_SYS)
                .arg(LINUX_CPU_PSTATE)
                .arg(LINUX_CPU_PSTATE_NOTURBO));
    if (!turbo.exists()) { return result; }
    if (turbo.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QString value = turbo.readAll().trimmed();
        turbo.close();
        if (value=="1") { result = false; }
        else if (value=="0") { result = true; }
    }
    return result;
}

bool PowerCpu::setPStateTurbo(bool turbo)
{
    if (!hasPState()) { return false; }
    QFile file(QString("%1/%2/%3")
              .arg(LINUX_CPU_SYS)
              .arg(LINUX_CPU_PSTATE)
              .arg(LINUX_CPU_PSTATE_NOTURBO));
    if (file.open(QIODevice::WriteOnly|QIODevice::Truncate)) {
        QTextStream out(&file);
        if (turbo) { out << "0"; }
        else { out << "1"; }
        file.close();
        if (turbo == hasPStateTurbo()) { return true; }
    }
    return false;
}

// TODO
int PowerCpu::getPStateMax()
{
    return 0;
}

// TODO
int PowerCpu::getPStateMin()
{
    return 0;
}

// TODO
bool PowerCpu::setPStateMax(int maxState)
{
    Q_UNUSED(maxState)
    return false;
}

// TODO
bool PowerCpu::setPStateMin(int minState)
{
    Q_UNUSED(minState)
    return false;
}
