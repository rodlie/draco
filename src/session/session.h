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

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QFileSystemWatcher>
#include <QTimer>

#include <sys/types.h>
#include <signal.h>

class LProcess : public QProcess{
	Q_OBJECT
private:
	QFileSystemWatcher *watcher;
	QString id;
private slots:
	void filechanged(QString path){
          //qDebug() << "File Changed:" << path;
	  //qDebug() << " - Program:" << this->program();
	  if(watcher==0){ return; } //just in case
	  if(this->state()==QProcess::Running){
        /*if(this->program().section(" ",0,0).section("/",-1) == "fluxbox" ){
             // qDebug() << "Sending Fluxbox signal to reload configs...";
              ::kill(this->pid(), SIGUSR2); } //Fluxbox needs SIGUSR2 to reload it's configs
	    else if(this->program().section(" ",0,0).section("/",-1) == "compton" ){
              //qDebug() << "Sending Compton signal to reload configs...";
              ::kill(this->pid(), SIGUSR1); } //Compton needs SIGUSR1 to reload it's configs*/
	  }
	  //Now make sure this file/dir was not removed from the watcher
	  QStringList watched; watched << watcher->files() << watcher->directories();
	  if(!watched.contains(path)){ watcher->addPath(path); } //re-add it
	}

public:
	LProcess(QString ID, QStringList watchfiles) : QProcess(){
	  id=ID;
	  watcher = 0;
          if(!watchfiles.isEmpty()){
            qDebug() << "Watch Files for changes:" << ID << watchfiles;
	    watcher = new QFileSystemWatcher(this);
	    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(filechanged(QString)) );
	    connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(filechanged(QString)) );
	    watcher->addPaths(watchfiles);
	  }
	}
	~LProcess(){

	}
	QString ID(){ return id; }

};

class LSession : public QObject{
	Q_OBJECT
private:
	QList<QProcess*> PROCS;
	bool stopping;
	int wmfails;
	QTimer *wmTimer;

    //void setupCompositor(bool force = false);

private slots:
	void stopall();

	void procFinished();

	void startProcess(QString ID, QString command, QStringList watchfiles = QStringList());

	void resetWMCounter(){ wmfails = 0; }
public:
	LSession(){
	stopping = false; wmfails = 0;
	  wmTimer = new QTimer(this);
	  wmTimer->setSingleShot(true);
	  wmTimer->setInterval(2000); //2 second timeout
           connect(wmTimer, SIGNAL(timeout()), this, SLOT(resetWMCounter()) );
	}
	~LSession(){ }

    void start();

	void checkFiles();

};
