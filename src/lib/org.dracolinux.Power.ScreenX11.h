/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
# Available under the 3-clause BSD license
# See the LICENSE file for full details
#
*/
#ifndef SCREENS_H
#define SCREENS_H

#include <QMap>
#include <QString>

#include <X11/extensions/Xrandr.h>

class Screens
{
public:
    static QMap<QString,bool> outputsDpy(Display *dpy);
    static QMap<QString,bool> outputs();
    static QString internalDpy(Display *dpy);
    static QString internal();
};

#endif // SCREENS_H
