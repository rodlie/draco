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

#ifndef HOTPLUG_H
#define HOTPLUG_H

#include <QObject>
#include <QThread>
#include <QMap>

#include "org.dracolinux.Power.ScreenX11.h"
#include <X11/extensions/Xrandr.h>

// fix X11 inc
#undef Bool
#undef None
#undef Status

#define OCNE(X) ((XRROutputChangeNotifyEvent*)X)

class HotPlug : public QObject
{
    Q_OBJECT

public:
    explicit HotPlug(QObject *parent = 0);
    ~HotPlug();

private:
    QThread t;
    bool _scanning;

signals:
    void status(QString display, bool connected);
    void found(QMap<QString,bool> devices);

public slots:
    void requestScan();
    void requestSetScan(bool scanning);
private slots:
    void scan();
    void getScreens(Display *dpy);
    void setScan(bool scanning);
};

#endif // HOTPLUG_H
