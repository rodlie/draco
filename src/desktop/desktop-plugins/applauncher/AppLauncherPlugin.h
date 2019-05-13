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
//  Copyright (c) 2014-2017, Ken Moore
//  Available under the 3-clause BSD license
//===========================================
//  This class is a quick sample desktop plugin
//===========================================
#ifndef _LUMINA_DESKTOP_DESKTOP_PLUGIN_APPLICATION_LAUNCHER_H
#define _LUMINA_DESKTOP_DESKTOP_PLUGIN_APPLICATION_LAUNCHER_H

#include <QToolButton>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QProcess>
#include <QFile>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QMenu>
#include <QCursor>
#include <QDrag>
#include <QMimeData>
#include <QtConcurrent>

#include "../LDPlugin.h"

#include <LuminaXDG.h>

class AppLauncherPlugin : public LDPlugin{
	Q_OBJECT
public:
	AppLauncherPlugin(QWidget* parent, QString ID);
	~AppLauncherPlugin(){}

	void Cleanup(); //special function for final cleanup

private:
	QToolButton *button;
	QFileSystemWatcher *watcher;
	//QMenu *menu;
	QInputDialog *inputDLG;
	QString iconID;
	int icosize;
	QPoint dragstartpos;

private slots:
	void loadButton();
	void buttonClicked(bool openwith = false);
	void iconLoaded(QString);

	//void openContextMenu();

	//void increaseIconSize();
	//void decreaseIconSize();
	//void deleteFile();
	void startDragNDrop();

	void actionTriggered(QAction *act);
	void openWith();
	void fileProperties();
	void fileDelete();
	void fileCut();
	void fileCopy();
	void fileRename();
	void renameFinished(int result);

	void fileDrop(bool copy, QList<QUrl> urls);

public slots:
	void LocaleChange(){
	  loadButton(); //force reload
	}

protected:
	void resizeEvent(QResizeEvent *ev){
	  LDPlugin::resizeEvent(ev);
	  QTimer::singleShot(100, this, SLOT(loadButton()) );
	}
    void changeEvent(QEvent *ev)
    {
        LDPlugin::changeEvent(ev);
        QEvent::Type tmp = ev->type();
        if (tmp == QEvent::StyleChange ||
        tmp==QEvent::ThemeChange ||
        tmp==QEvent::LanguageChange ||
        tmp==QEvent::LocaleChange)
        {
            QTimer::singleShot(100,
                               this,
                               SLOT(loadButton()));
        }
    }

	void mousePressEvent(QMouseEvent *ev){
	  if(ev->button()==Qt::LeftButton){
	    dragstartpos = ev->pos();
	  }
	}

	void mouseMoveEvent(QMouseEvent *ev){
	  if( (ev->buttons() & Qt::LeftButton) ){
	    if((ev->pos() - dragstartpos).manhattanLength() > (this->width()/4) ){
	      emit StartMoving(this->ID());
	    }else if(false){ //
	      startDragNDrop();
	    }
	  }else{
	    LDPlugin::mouseMoveEvent(ev);
	  }
	}

    /*void dragEnterEvent(QDragEnterEvent *ev){
	  if(ev->mimeData()->hasUrls() && this->acceptDrops()){ ev->acceptProposedAction(); }
    }*/

    /*void dropEvent(QDropEvent *ev){
	  QList<QUrl> urls = ev->mimeData()->urls();
	  bool ok = !urls.isEmpty() && this->acceptDrops();
	  if(ok){
	    if(ev->proposedAction() == Qt::MoveAction){
	      ev->setDropAction(Qt::MoveAction);
	      QtConcurrent::run(this, &AppLauncherPlugin::fileDrop, false, urls);
	    }else if(ev->proposedAction() == Qt::CopyAction){
	      ev->setDropAction(Qt::CopyAction);
	      QtConcurrent::run(this, &AppLauncherPlugin::fileDrop, true, urls);
	    }else{ ok = false; }
	  }
	  if(ok){ ev->acceptProposedAction(); }
    }*/
};
#endif
