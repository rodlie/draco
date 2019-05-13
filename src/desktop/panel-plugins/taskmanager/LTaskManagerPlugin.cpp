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
#include "LTaskManagerPlugin.h"
#include "LSession.h"

LTaskManagerPlugin::LTaskManagerPlugin(QWidget *parent, QString id, bool horizontal) : LPPlugin(parent, id, horizontal){
  timer = new QTimer(this);
	timer->setSingleShot(true);
	timer->setInterval(10); // 1/100 second
	connect(timer, SIGNAL(timeout()), this, SLOT(UpdateButtons()) ); 
  usegroups = true; //backwards-compatible default value
  showText = true;
  if(id.contains("-nogroups")){ usegroups = false; }
  connect(LSession::handle(), SIGNAL(WindowListEvent()), this, SLOT(checkWindows()) );
  connect(LSession::handle(), SIGNAL(WindowListEvent(WId)), this, SLOT(UpdateButton(WId)) );
  this->layout()->setContentsMargins(0,0,0,0);
  QTimer::singleShot(0,this, SLOT(UpdateButtons()) ); //perform an initial sync
  //QTimer::singleShot(100,this, SLOT(OrientationChange()) ); //perform an initial sync
}

LTaskManagerPlugin::~LTaskManagerPlugin(){

}

//==============
//    PRIVATE SLOTS
//==============
void LTaskManagerPlugin::UpdateButtons(){
    qDebug() << "update task buttons";
  updating = QDateTime::currentDateTime(); //global time stamp
  QDateTime ctime = updating; //current thread time stamp

  //Get the current window list
  QList<WId> winlist = LSession::handle()->XCB->WindowList();
  // Ignore the windows which don't want to be listed
  for (int i = 0; i < winlist.length(); i++) {
    QList<LXCB::WINDOWSTATE> states = LSession::handle()->XCB->WM_Get_Window_States(winlist[i]);
    for (int j = 0; j < states.length(); j++) {
      if (states[j] == LXCB::S_SKIP_TASKBAR) {
        // Skip taskbar window
        winlist.removeAt(i);
        i--;
        break;
      }
    }
  }
  //Do not change the status of the previously active window if it just changed to a non-visible window
  //WId activeWin = LSession::handle()->XCB->ActiveWindow();
  //bool skipActive = !winlist.contains(activeWin);
  //qDebug() << "Update Buttons:" << winlist;
  if(updating > ctime){ return; } //another thread kicked off already - stop this one
  //Now go through all the current buttons first
  for(int i=0; i<BUTTONS.length(); i++){

      // show/hide text
      BUTTONS[i]->setShowText(showText);
    //Get the windows managed in this button
    QList<WId> WI = BUTTONS[i]->windows();
    bool updated=false;
    if(updating > ctime){ return; } //another thread kicked off already - stop this one
    //Loop over all the windows for this button
    for(int w=0; w<WI.length(); w++){
      if(updating > ctime){ return; } //another thread kicked off already - stop this one
      if( winlist.contains( WI[w] ) ){
        //Still current window - update it later
	winlist.removeAll(WI[w] ); //remove this window from the list since it is done
      }else{
	//Window was closed - remove it
	if(WI.length()==1){
	  //Remove the entire button
	  //qDebug() << "Window Closed: Remove Button" ;
	  this->layout()->takeAt(i); //remove from the layout
	  BUTTONS.takeAt(i)->deleteLater();
	  i--;
	  updated = true; //prevent updating a removed button
	  break; //break out of the button->window loop
	}else{
      //qDebug() << "Window Closed: Remove from button:" << WI[w].windowID() << "Button:" << w;
	  BUTTONS[i]->rmWindow(WI[w]); // one of the multiple windows for the button
	  WI.removeAt(w); //remove this window from the list
	  w--;
	}
	updated=true; //button already changed
      }
      if(updating > ctime){ return; } //another thread kicked off already - stop this one
    }
    if(!updated){
      //qDebug() << "Update Button:" << i;
      if(updating > ctime){ return; } //another thread kicked off already - stop this one
      //if(!skipActive || !BUTTONS[i]->isActive()){
        QTimer::singleShot(1,BUTTONS[i], SLOT(UpdateButton()) ); //keep moving on
      //}
    }
  }
  //Now go through the remaining windows
  for(int i=0; i<winlist.length(); i++){
    //New windows, create buttons for each (add grouping later)
    if(updating > ctime){ return; } //another thread kicked off already - stop this one
    //Check for a button that this can just be added to
    QString ctxt = LSession::handle()->XCB->WindowClass(winlist[i]);
    bool found = false;
    for(int b=0; b<BUTTONS.length(); b++){
      if(updating > ctime){ return; } //another thread kicked off already - stop this one
      if(BUTTONS[b]->classname()== ctxt && usegroups){
	//This adds a window to an existing group
        found = true;
    //qDebug() << "Add Window to Button:" << b;
	BUTTONS[b]->addWindow(winlist[i]);
	break;
      }
    }
    if(!found){
      if(updating > ctime){ return; } //another thread kicked off already - stop this one
      //No group, create a new button
      //qDebug() << "New Button";
      LTaskButton *but = new LTaskButton(this, usegroups);
        but->addWindow( winlist[i] );
	if(this->layout()->direction()==QBoxLayout::LeftToRight){
	    but->setIconSize(QSize(this->height(), this->height()));
        but->setToolButtonStyle(showText?Qt::ToolButtonTextBesideIcon:Qt::ToolButtonIconOnly);
	}else{
	    but->setIconSize(QSize(this->width(), this->width()));
	    but->setToolButtonStyle(Qt::ToolButtonIconOnly);
	}
      this->layout()->addWidget(but);
      connect(but, SIGNAL(MenuClosed()), this, SIGNAL(MenuClosed()));
      BUTTONS << but;
    }
  }
}

void LTaskManagerPlugin::UpdateButton(WId win){
  for(int i=0; i<BUTTONS.length(); i++){
    if(BUTTONS[i]->windows().contains(win)){
      qDebug() << "Update Task Manager Button (single window ping)";
      QTimer::singleShot(0,BUTTONS[i], SLOT(UpdateButton()) );
      break;
    }
  }
}

void LTaskManagerPlugin::checkWindows(){
    timer->start();
}

void LTaskManagerPlugin::settingsChange(QSettings *settings, const QString &prefix)
{
    if (!settings) { return; }
    qWarning() << "TASK! settings changed" << prefix;
    showText = settings->value(QString("%1taskmanagerText").arg(prefix), true).toBool();
    UpdateButtons();
}
