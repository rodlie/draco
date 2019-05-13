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
#ifndef _LUMINA_DESKTOP_TASK_MANAGER_PLUGIN_H
#define _LUMINA_DESKTOP_TASK_MANAGER_PLUGIN_H

// Qt includes
#include <QWidget>
#include <QList>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QEvent>
#include <QDateTime>

// libLumina includes
#include <LuminaX11.h>

// Local includes
#include "LTaskButton.h"
#include "LWinInfo.h"
#include "../LPPlugin.h"

class LTaskManagerPlugin : public LPPlugin{
	Q_OBJECT
public:
	LTaskManagerPlugin(QWidget *parent=0, QString id="taskmanager", bool horizontal=true);
	~LTaskManagerPlugin();
//	int vertsizeicon;
//	int *dpi;  //this comes from the PCDM dpi

private:
	QList<LTaskButton*> BUTTONS; //to keep track of the current buttons
	QTimer *timer;
	QDateTime updating; //quick flag for if it is currently working
	bool usegroups;
    bool showText;

private slots:
	void UpdateButtons();
	void UpdateButton(WId win);
	void checkWindows();

public slots:
	void LocaleChange(){
	  UpdateButtons();
	}
    void settingsChange(QSettings *settings, const QString &prefix);
	void ThemeChange(){
	  UpdateButtons();
	}
	void OrientationChange(){
	  if(this->layout()->direction()==QBoxLayout::LeftToRight){ //horizontal
	    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	    this->layout()->setAlignment(Qt::AlignLeft);
	    QSize sz(this->height(), this->height());
	    for(int i=0; i<BUTTONS.length(); i++){
	      BUTTONS[i]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	      BUTTONS[i]->setIconSize(sz);
	    }
	  }else{ //vertical
	    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	    this->layout()->setAlignment(Qt::AlignTop);
	    QSize sz(this->width(), this->width());
//            QSize sz(this->width(), this->height());  //we want to increase the width but not the height of the icons
	    for(int i=0; i<BUTTONS.length(); i++){
	      BUTTONS[i]->setToolButtonStyle(Qt::ToolButtonIconOnly);
//	      BUTTONS[i]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//		if( dpi = 196 ){ int vertsizeicon = ; BUTTONS[i]->setIconSize(vertsizeicon);}
//		elseif( dpi = 144 ) { int vertsizeicon = ; BUTTONS[i]->setIconSize(vertsizeicon);}
//		elseif( dpi = 96 ) { int vertsizeicon = ; BUTTONS[i]->setIconSize(vertsizeicon);}
//		elseif( dpi = 48 ) { int vertsizeicon = ; BUTTONS[i]->setIconSize)vertsizeicon);}
	      BUTTONS[i]->setIconSize(sz);
	    }
	  }
	}
};
#endif
