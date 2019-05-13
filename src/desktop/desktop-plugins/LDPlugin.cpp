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
//  Copyright (c) 2014-2015, Ken Moore
//  Available under the 3-clause BSD license
//===========================================
#include "LDPlugin.h"

#include "LSession.h"
#include <LuminaXDG.h>

LDPlugin::LDPlugin(QWidget *parent, QString id) : QFrame(parent){
  PLUGID=id;
  prefix = id.replace("/","_")+"/";
  //qDebug() << "ID:" << PLUGID << prefix;
  settings = LSession::handle()->DesktopPluginSettings();
  //Setup the plugin system control menu
  menu = new QMenu(this);
  contextM = 0;
  setupMenu();
  //Setup the internal timer for when to start/stop drag events
  dragTimer = new QTimer(this);
    dragTimer->setSingleShot(true);
    dragTimer->setInterval(500); //1/2 second to show the plugin menu
    connect(dragTimer, SIGNAL(timeout()), this, SLOT(showPluginMenu()));
  //Use plugin-specific values for stylesheet control (applauncher, desktopview, etc...)
  this->setObjectName(id.section("---",0,0).section("::",0,0));
  this->setContextMenuPolicy(Qt::CustomContextMenu);
  this->setMouseTracking(false); //only catch mouse movement events if the mouse is clicked/held on the plugin
  connect(QApplication::instance(), SIGNAL(LocaleChanged()), this, SLOT(LocaleChange()) );
  connect(QApplication::instance(), SIGNAL(IconThemeChanged()), this, SLOT(ThemeChange()) );
  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showPluginMenu()) );
}

void LDPlugin::setupMenu(){
  menu->clear();
  menu->setTitle(tr("Modify Item"));
  menu->setIcon(LXDG::findIcon("preferences-desktop-icons","") );
  //SPECIAL CONTEXT MENU OPTIONS FOR PARTICULAR PLUGIN TYPES
  /*if(PLUGID.startsWith("applauncher::")){
    menu->addAction( LXDG::findIcon("quickopen",""), tr("Launch Item"), this, SIGNAL(PluginActivated()) );
    menu->addSeparator();
  }*/
  //General Options
  menu->addAction( LXDG::findIcon("transform-move",""), tr("Start Moving Item"), this, SLOT(slotStartMove()) );
  menu->addAction( LXDG::findIcon("transform-scale",""), tr("Start Resizing Item"), this, SLOT(slotStartResize()) );
  menu->addSeparator();
  menu->addAction( LXDG::findIcon("zoom-in",""), tr("Increase Item Sizes"), this, SIGNAL(IncreaseIconSize()) );
  menu->addAction( LXDG::findIcon("zoom-out",""), tr("Decrease Item Sizes"), this, SIGNAL(DecreaseIconSize()) );
  //menu->addSeparator();
  //menu->addAction( LXDG::findIcon("edit-delete",""), tr("Remove Item"), this, SLOT(slotRemovePlugin()) );
}

void LDPlugin::showPluginMenu(){
  emit CloseDesktopMenu();
  //Double check which menu should be shown
  if(this->contextMenu()!=0){
    //Got a special context menu for this plugin - need to layer them together
    if(!this->contextMenu()->actions().contains(menu->menuAction())){
      this->contextMenu()->addSeparator();
      this->contextMenu()->addMenu(menu);
    }
    this->contextMenu()->popup( QCursor::pos() );
  }else{
    menu->popup( QCursor::pos() );
  }
}
