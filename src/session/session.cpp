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
//  Copyright (c) 2016, Ken Moore
//  Available under the 3-clause BSD license
//===========================================

#include "session.h"
#include "LUtils.h"
#include "LDesktopUtils.h"
#include "draco.h"

#include <QObject>
#include <QProcess>
#include <QProcessEnvironment>
#include <QDebug>
#include <QSettings>
#include <QDir>

void LSession::stopall(){
  stopping = true;
  for(int i=0; i<PROCS.length(); i++){
    if(PROCS[i]->state()!=QProcess::NotRunning){ PROCS[i]->kill(); }
  }
  QCoreApplication::processEvents();
  for(int i=0; i<PROCS.length(); i++){
    if(PROCS[i]->state()!=QProcess::NotRunning){ PROCS[i]->terminate(); }
  }
  QCoreApplication::exit(0);
}

void LSession::procFinished(){
  //Go through and check the status on all the procs to determine which one finished
  int stopped = 0;
  //qDebug() << "Got Process Stopped Signal:";
  for(int i=0; i<PROCS.length(); i++){
    if(PROCS[i]->state()==QProcess::NotRunning){
      //qDebug() << " - Stopped:" << PROCS[i]->objectName();
      stopped++;
      if(!stopping){
        //See if this process is the main desktop binary
        if(PROCS[i]->objectName()=="runtime"){
          qDebug() << "Got Desktop Process Finished:" << PROCS[i]->exitCode();
          //if(PROCS[i]->exitCode()==787){ PROCS[i]->start(QIODevice::ReadOnly); } //special internal restart code
          //else{
          stopall(); //}
        }else if(PROCS[i]->objectName()=="wm" && wmfails<2){ wmfails++; PROCS[i]->start(QIODevice::ReadOnly); wmTimer->start(); } //restart the WM
        //if(PROCS[i]->program().section("/",-1) == "lumina-desktop"){ stopall();  } //start closing down everything
        //else{ PROCS[i]->start(QIODevice::ReadOnly); } //restart the process
        //break;
      }
    }
  }
  //qDebug() << " - Final Count:" << stopped << stopping;
  if(stopping || stopped==PROCS.length()){
    //Note about compton: It does not like running multiple sessions under the *same user*
    // (even on different displays). Run a blanket killall on it when closing down so that
    // any other Lumina sessions will automatically restart compton on that specific display
    //QProcess::execute("killall compton");
    QCoreApplication::exit(0);
  }else{
    //Make sure we restart the process as needed
    for(int i=0; i<PROCS.length(); i++){
      if(PROCS[i]->state()==QProcess::NotRunning){
        //runtime/wm processes have special restart rules above
        if(PROCS[i]->objectName()!="runtime" && PROCS[i]->objectName()!="wm"){
          PROCS[i]->start(QIODevice::ReadOnly);
        }
      }
    }
  }
}

void LSession::startProcess(QString ID, QString command, QStringList watchfiles){
  QString dir = QString(getenv("XDG_CONFIG_HOME"))+QString("/%1/logs").arg(DESKTOP_APP);
  QString display = QString(getenv("DISPLAY")).section(":",1,1);
  if(!QFile::exists(dir)){ QDir tmp(dir); tmp.mkpath(dir); }
  QString logfile = dir+"/"+ID+"_"+display+".log";
  if(QFile::exists(logfile+".old")){ QFile::remove(logfile+".old"); }
  if(QFile::exists(logfile)){ QFile::rename(logfile,logfile+".old"); }

  LProcess *proc = new LProcess(ID, watchfiles);
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->setProcessEnvironment( QProcessEnvironment::systemEnvironment() );
  proc->setStandardOutputFile(logfile);
  proc->setObjectName(ID);
  if(ID=="runtime"){
    //Bypass for a hidden dbus file requirement for Qt itself (Qt 5.5.1+?)
    if(!QFile::exists("/etc/machine-id") && !QFile::exists("/var/db/dbus/machine-id")){
      if(LUtils::isValidBinary("dbus-uuidgen") && LUtils::runCmd("dbus-uuidgen --ensure") ){ } //good - the UUID was created successfully
      else if(LUtils::isValidBinary("dbus-launch")){ command.prepend("dbus-launch --exit-with-session "); }
      else{
        //create a simple DBUS UUID and put it in the universal-fallback location (OS-independent)
        // TO-DO - root vs user level permissions issue?
        qDebug() << "Could not find '/etc/machine-id' or '/var/db/dbus/machine-id': Qt will most likely crash. \nPlease run 'dbus-uuidgen --ensure' with root permissions to generate this file if Lumina does not start properly.";
      }
   }
  }
  proc->start(command, QIODevice::ReadOnly);
  connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(procFinished()) );
  PROCS << proc;
}



void LSession::start()
{
    // set environment
    LXDG::setEnvironmentVars();
    setenv("DESKTOP_SESSION", DESKTOP_APP_NAME, 1);
    setenv("XDG_CURRENT_DESKTOP", DESKTOP_APP_NAME, 1);
    setenv("QT_QPA_PLATFORMTHEME","qt5ct", true);
    setenv("QT_NO_GLIB", "1", 1); // Disable the glib event loop within Qt at runtime (performance hit + bugs)
    unsetenv("QT_AUTO_SCREEN_SCALE_FACTOR"); // need exact-pixel measurements (no fake scaling)

    // Make sure the XDG user directories are created as needed first
    if(LUtils::isValidBinary("xdg-user-dirs-update")){
        QProcess::execute("xdg-user-dirs-update");
    }

    // Window Manager First
    startProcess("wm", Draco::windowManagerCmdStart());

    // Desktop Next
    startProcess("runtime", LUtils::isValidBinaryWithPath(Draco::desktopApp()));

    // Power last
    //startProcess("power", LUtils::isValidBinaryWithPath(Draco::powerApp()));
}
