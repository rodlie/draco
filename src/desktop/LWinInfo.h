/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
# Available under the 3-clause BSD license
# See the LICENSE file for full details
#
*/

//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================

#ifndef DESKTOP_WINDOW_INFO_H
#define DESKTOP_WINDOW_INFO_H

// Qt includes
#include <QString>
#include <QPixmap>
#include <QIcon>
#include <QPainter>

// libLumina includes
#include <LuminaX11.h>
#include <LuminaXDG.h>

// Local includes
//#include "Globals.h" //For the STATES enumeration definition
//#include "LSession.h"

class LWinInfo
{
private:
    WId window;
    LXCB::WINDOWVISIBILITY cstate; // current window state

public:
    LWinInfo(WId id = 0)
    {
        window = id;
        cstate = LXCB::IGNORE; // make sure this gets updates with the first "status" call
    }
    ~LWinInfo(){};

    // The current window ID
    WId windowID()
    {
        return window;
    }

    // Information Retrieval
    // Don't cache these results because they can change regularly
    QString  text();
    QIcon icon(bool &noicon);
    QString Class();
    LXCB::WINDOWVISIBILITY status(bool update = false);
};

#endif
