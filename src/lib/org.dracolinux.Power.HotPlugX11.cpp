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

#include "org.dracolinux.Power.HotPlugX11.h"

HotPlug::HotPlug(QObject *parent) :
    QObject(parent)
  , _scanning(false)
{
    moveToThread(&t);
    t.start();
}

HotPlug::~HotPlug()
{
    _scanning = false;
    t.wait();
    t.quit();
}

void HotPlug::requestScan()
{
    QMetaObject::invokeMethod(this, "scan");
}

void HotPlug::scan()
{
    if (_scanning) { return; }
    _scanning = true;

    Display *dpy;
    if ((dpy = XOpenDisplay(NULL)) == NULL) { return; }

    XRRScreenResources *sr;
    XRROutputInfo *info;
    XEvent ev;

    getScreens(dpy);

    XRRSelectInput(dpy, DefaultRootWindow(dpy), RROutputChangeNotifyMask);
    XSync(dpy, 0);
    while(_scanning) {
        if (!XNextEvent(dpy, &ev)) {
            sr = XRRGetScreenResources(OCNE(&ev)->display, OCNE(&ev)->window);
            if (sr == NULL) { continue; }
            info = XRRGetOutputInfo(OCNE(&ev)->display, sr, OCNE(&ev)->output);
            if (info == NULL) {
                XRRFreeScreenResources(sr);
                continue;
            }
            QString screenName = info->name;
            bool screenConnected = false;
            if (info->connection == RR_Connected) { screenConnected = true; }
            emit status(screenName, screenConnected);
            XRRFreeScreenResources(sr);
            XRRFreeOutputInfo(info);
        }
    }
    XCloseDisplay(dpy);
}

void HotPlug::requestSetScan(bool scanning)
{
    QMetaObject::invokeMethod(this, "setScan", Q_ARG(bool, scanning));
}

void HotPlug::getScreens(Display *dpy)
{
    if (dpy == NULL) { return; }
    QMap<QString,bool> result = Screens::outputsDpy(dpy);
    emit found(result);
}

void HotPlug::setScan(bool scanning)
{
    _scanning = scanning;
}
