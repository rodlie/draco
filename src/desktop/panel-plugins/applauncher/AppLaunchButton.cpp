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
//  Copyright (c) 2015, Ken Moore
//  Available under the 3-clause BSD license
//===========================================
#include "AppLaunchButton.h"
#include "LSession.h"

#include <LuminaXDG.h>
#include <LUtils.h>
#include <QInputDialog>
#include <LFileInfo.h>

#include "draco.h"

AppLaunchButtonPlugin::AppLaunchButtonPlugin(QWidget *parent, QString id, bool horizontal) : LPPlugin(parent, id, horizontal){
  button = new QToolButton(this);
    button->setAutoRaise(true);
    button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    appfile = id.section("---",0,0).section("::",1,1);
    if(!QFile::exists(appfile) && appfile.endsWith(".desktop")){
      //might be a relative path - try to find the file
      appfile = LUtils::AppToAbsolute(appfile.section("/",-1) );
    }
    if(!QFile::exists(appfile)){ appfile.clear(); }
    connect(button, SIGNAL(clicked()), this, SLOT(AppClicked()));
    this->layout()->setContentsMargins(0,0,0,0);
    this->layout()->addWidget(button);

  QTimer::singleShot(0,this, SLOT(OrientationChange())); //Update icons/sizes
}

AppLaunchButtonPlugin::~AppLaunchButtonPlugin(){

}

void AppLaunchButtonPlugin::updateButtonVisuals()
{
    QIcon icon;
    QString tooltip = tr("Click to assign an application");
    LFileInfo info(appfile);
    //qDebug() << "UPDATE BUTTON VISUALS" << appfile << info.iconfile() << info.isDesktopFile();
    if (info.iconfile().isEmpty()) {
        if (!info.XDG()->icon.isEmpty()) { icon = LXDG::findIcon(info.XDG()->icon, "application-x-executable"); }
    } else {
        icon = LXDG::findIcon(info.iconfile(), "application-x-executable");
    }
    if (info.isDesktopFile()) {
        if (icon.isNull()) { icon = LXDG::findIcon("application-x-executable"); }
        tooltip = QString(tr("Launch %1")).arg(info.XDG()->name);
    } else if (info.exists()) {
        if (icon.isNull()) { icon = LXDG::findIcon("application-x-executable"); }
        tooltip = QString(tr("Open %1")).arg(appfile.section("/",-1));
    } else {
        icon =  LXDG::findIcon("task-attention", "application-x-executable");
    }
    button->setIcon( icon );
    button->setToolTip(tooltip);
}

// ========================
//    PRIVATE FUNCTIONS
// ========================
void AppLaunchButtonPlugin::AppClicked(){
  if(appfile.isEmpty()){
    //No App File selected
    QList<XDGDesktop*> apps = LSession::handle()->applicationMenu()->currentAppHash()->value("All");
    QStringList names;
    for(int i=0; i<apps.length(); i++){ names << apps[i]->name; }
    bool ok = false;
    QString app = QInputDialog::getItem(this, tr("Select Application"), tr("Name:"), names, 0, false, &ok);
    if(!ok || names.indexOf(app)<0){ return; } //cancelled
    appfile = apps[ names.indexOf(app) ]->filePath;
    //Still need to find a way to set this value persistently
    // --- perhaps replace the plugin in the desktop settings file with the new path?
    // --- "applauncher::broken---<something>"  -> "applauncher::fixed---<something>" ?
    QTimer::singleShot(0,this, SLOT(updateButtonVisuals()));
  }else{
    LSession::LaunchApplication(QString("%1 \"%2\"").arg(Draco::launcherApp()).arg(appfile));
  }
}
