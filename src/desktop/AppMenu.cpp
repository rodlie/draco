/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com>
# All rights reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
#
# This file incorporates work covered by the following copyright and
# permission notice:
#
# Lumina Desktop Environment (https://lumina-desktop.org)
# Copyright (c) 2012-2017, Ken Moore (moorekou@gmail.com)
# All rights reserved
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the organization nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
*/

//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//===========================================

#include "AppMenu.h"
#include "LSession.h"
#include "LIconCache.h"
#include "draco.h"

extern LIconCache *ICONS;

AppMenu::AppMenu(QWidget* parent) : QMenu(parent)
{
    sysApps = new XDGDesktopList(this, true); // have this one automatically keep in sync
    APPS.clear();
    start(); // do the initial run during session init so things are responsive immediately.
    connect(QApplication::instance(), SIGNAL(LocaleChanged()), this, SLOT(watcherUpdate()) );
    connect(QApplication::instance(), SIGNAL(IconThemeChanged()), this, SLOT(watcherUpdate()) );
}

AppMenu::~AppMenu()
{
}

QHash<QString, QList<XDGDesktop*> >* AppMenu::currentAppHash()
{
    return &APPS;
}

void AppMenu::updateAppList()
{
    // Make sure the title/icon are updated as well (in case of locale/icon change)
    this->setTitle(tr("Applications"));
    this->setIcon( LXDG::findIcon("system-run","") );

    // Now update the lists
    this->clear();
    APPS.clear(); // NOTE: Don't delete these pointers - the pointers are managed by the sysApps class and these are just references to them

    if (LSession::handle()->sessionSettings()->value("AutomaticDesktopAppLinks",true).toBool() &&
        !lastHashUpdate.isNull() )
    {
        QString desktop = LUtils::standardDirectory(LUtils::Desktop);
        desktop.append("/");
        QStringList tmp = sysApps->removedApps;
        for (int i=0; i<tmp.length() && !desktop.isEmpty(); i++) {
            // Remove any old symlinks first
            QString filename = tmp[i].section("/",-1);
            if (QFileInfo(desktop+filename).isSymLink()){ QFile::remove(desktop+filename); }
        }
        tmp = sysApps->newApps;
        for(int i=0; i<tmp.length() && !desktop.isEmpty(); i++){
            XDGDesktop *desk = sysApps->files.value(tmp[i]);
            if(desk->isHidden || !desk->isValid(false) ){ continue; } // skip this one
            // Create a new symlink for this file if one does not exist
            QString filename = tmp[i].section("/",-1);
            if(!QFile::exists(desktop+filename) ){ QFile::link(tmp[i], desktop+filename); }
        }
    }
    QList<XDGDesktop*> allfiles = sysApps->apps(false,false); // only valid, non-hidden apps
    APPS = LXDG::sortDesktopCats(allfiles);
    APPS.insert("All", LXDG::sortDesktopNames(allfiles));
    lastHashUpdate = QDateTime::currentDateTime();

    // Now create the sub-menus
    QStringList cats = APPS.keys();
    cats.sort(); // make sure they are alphabetical
    for (int i=0; i<cats.length(); i++) {
        // Make sure they are translated and have the right icons
        QString name, icon;
        if (cats[i]=="All") { continue; } // skip this listing for the menu
        else if (cats[i] == "Multimedia") { name = tr("Multimedia"); icon = "applications-multimedia"; }
        else if (cats[i] == "Development") { name = tr("Development"); icon = "applications-development"; }
        else if (cats[i] == "Education") { name = tr("Education"); icon = "applications-science"; }
        else if (cats[i] == "Game") { name = tr("Games"); icon = "applications-games"; }
        else if (cats[i] == "Graphics") { name = tr("Graphics"); icon = "applications-graphics"; }
        else if (cats[i] == "Network") { name = tr("Network"); icon = "applications-internet"; }
        else if (cats[i] == "Office") { name = tr("Office"); icon = "applications-office"; }
        else if (cats[i] == "Science") { name = tr("Science"); icon = "applications-science"; }
        else if (cats[i] == "Settings") { name = tr("Settings"); icon = "preferences-system"; }
        else if (cats[i] == "System") { name = tr("System"); icon = "applications-system"; }
        else if (cats[i] == "Utility") { name = tr("Utility"); icon = "applications-utilities"; }
        else if (cats[i] == "Wine") { name = tr("Wine"); icon = "wine"; }
        else { name = tr("Other"); icon = "applications-other"; }

        QMenu *menu = new QMenu(name, this);
        //menu->setIcon( ICONS->loadIcon(icon) );
        menu->setIcon(LXDG::findIcon(icon,""));
        connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(launchApp(QAction*)) );
        QList<XDGDesktop*> appL = APPS.value(cats[i]);

        for (int a=0; a<appL.length(); a++) {
            if (appL[a]->actions.isEmpty()) {
                // Just a single entry point - no extra actions
                QAction *act = new QAction(appL[a]->name, this);
                //ICONS->loadIcon(act, appL[a]->icon);
                act->setIcon(LXDG::findIcon(appL[a]->icon, ""));
                act->setToolTip(appL[a]->comment);
                act->setWhatsThis(appL[a]->filePath);
                menu->addAction(act);
            } else {
                // This app has additional actions - make this a sub menu
                // - first the main menu/action
                QMenu *submenu = new QMenu(appL[a]->name, this);
                submenu->setIcon( /*ICONS->loadIcon(appL[a]->icon )*/ LXDG::findIcon(appL[a]->icon));
                // This is the normal behavior - not a special sub-action (although it needs to be at the top of the new menu)
                QAction *act = new QAction(appL[a]->name, this);
                //ICONS->loadIcon(act, appL[a]->icon);
                act->setIcon(LXDG::findIcon(appL[a]->icon, ""));
                act->setToolTip(appL[a]->comment);
                act->setWhatsThis(appL[a]->filePath);
                submenu->addAction(act);
                // Now add entries for every sub-action listed
                for (int sa=0; sa<appL[a]->actions.length(); sa++) {
                    QAction *sact = new QAction( appL[a]->actions[sa].name, this);
                    //if (ICONS->exists(appL[a]->actions[sa].icon)) { ICONS->loadIcon(sact, appL[a]->actions[sa].icon); }
                    //else { ICONS->loadIcon(sact, appL[a]->icon); }
                    sact->setIcon(LXDG::findIcon(appL[a]->icon, ""));
                    sact->setToolTip(appL[a]->comment);
                    sact->setWhatsThis("-action \""+appL[a]->actions[sa].ID+"\" \""+appL[a]->filePath+"\"");
                    submenu->addAction(sact);
                }
                menu->addMenu(submenu);
            }
        }
        this->addMenu(menu);
    }
    emit AppMenuUpdated();
}

void AppMenu::start()
{
    // Setup the watcher
    connect(sysApps, SIGNAL(appsUpdated()), this, SLOT(watcherUpdate()));
    sysApps->updateList();
    // Now fill the menu the first time
    updateAppList();
}

void AppMenu::watcherUpdate()
{
    updateAppList(); // Update the menu listings
}

void AppMenu::launchApp(QAction *act)
{
    QString appFile = act->whatsThis();
    qDebug() << "LAUNCH APP" << appFile;
    if (appFile.startsWith("-action")) {
        LSession::LaunchApplication(QString("%1 %2")
                                    .arg(Draco::launcherApp())
                                    .arg(appFile)); // already has quotes put in place properly
    } else {
        LSession::LaunchApplication(QString("%1 \"%2\"")
                                    .arg(Draco::launcherApp())
                                    .arg(appFile));
    }
}
