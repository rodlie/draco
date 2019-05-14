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

#include "org.dracolinux.Power.ScreenX11.h"

QMap<QString, bool> Screens::outputsDpy(Display *dpy)
{
    QMap<QString,bool> result;
    if (dpy == nullptr) { return result; }
    XRRScreenResources *sr;
    XRROutputInfo *info;
    sr = XRRGetScreenResourcesCurrent(dpy, DefaultRootWindow(dpy));
    if (sr) {
        for (int i = 0; i< sr->noutput;++i) {
            info = XRRGetOutputInfo(dpy, sr, sr->outputs[i]);
            if (info == nullptr) {
                XRRFreeOutputInfo(info);
                continue;
            }
            QString output = info->name;
            bool screenConnected = false;
            if (info->connection == RR_Connected) { screenConnected = true; }
            result[output] = screenConnected;
            XRRFreeOutputInfo(info);
        }
    }
    XRRFreeScreenResources(sr);
    return result;
}

QMap<QString, bool> Screens::outputs()
{
    QMap<QString, bool> result;
    Display *dpy;
    if ((dpy = XOpenDisplay(nullptr)) == nullptr) { return result; }
    result = outputsDpy(dpy);
    XCloseDisplay(dpy);
    return result;
}

QString Screens::internalDpy(Display *dpy)
{
    QString result;
    if (dpy == nullptr) { return result; }
    XRRScreenResources *sr;
    sr = XRRGetScreenResourcesCurrent(dpy, DefaultRootWindow(dpy));
    if (sr) {
        XRROutputInfo *info = XRRGetOutputInfo(dpy, sr, sr->outputs[0]);
        if (info) { result = info->name; }
        XRRFreeOutputInfo(info);
    }
    XRRFreeScreenResources(sr);
    return result;
}

QString Screens::internal()
{
    QString result;
    Display *dpy;
    if ((dpy = XOpenDisplay(nullptr)) == nullptr) { return result; }
    result = internalDpy(dpy);
    XCloseDisplay(dpy);
    return result;
}
