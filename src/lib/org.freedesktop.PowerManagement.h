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

#ifndef POWERMANAGEMENT_H
#define POWERMANAGEMENT_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QTime>
#include <QString>

class PowerManagement : public QObject
{
    Q_OBJECT

public:
    explicit PowerManagement(QObject *parent = NULL);

private:
    QTimer timer;
    QMap<quint32, QTime> clients;

signals:
    void HasInhibitChanged(bool has_inhibit);
    void newInhibit(const QString &application,
                    const QString &reason,
                    quint32 cookie);
    void removedInhibit(quint32 cookie);

private slots:
    int randInt(int low, int high);
    quint32 genCookie();
    void checkForExpiredClients();
    bool canInhibit();
    void timeOut();

public slots:
    void SimulateUserActivity();
    quint32 Inhibit(const QString &application,
                    const QString &reason);
    void UnInhibit(quint32 cookie);
    bool HasInhibit();
};

#endif // POWERMANAGEMENT_H
