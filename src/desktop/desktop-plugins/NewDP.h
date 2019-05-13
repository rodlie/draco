//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
//  This class is the interface to load all the different desktop plugins
//===========================================
#ifndef LUMINA_DESKTOP_NEW_DESKTOP_PLUGIN_H
#define LUMINA_DESKTOP_NEW_DESKTOP_PLUGIN_H

#include <QDebug>

#include "LDPlugin.h"
#include "applauncher/AppLauncherPlugin.h"

class NewDP{
public:
    static LDPlugin* createPlugin(QString plugin, QWidget* parent = Q_NULLPTR)
    {
        qDebug() << "Create Plugin:" << plugin;
        LDPlugin *plug = Q_NULLPTR;
        if (plugin.section("---",0,0).section("::",0,0) == "applauncher") {
            //This plugin can be pre-initialized to a file path after the "::" delimiter
            plug = new AppLauncherPlugin(parent, plugin);
        } else {
            qWarning() << "Invalid Desktop Plugin:"<<plugin << " -- Ignored";
        }
        return plug;
    }
};

#endif
