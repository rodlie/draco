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
//  This panel plugin is a simple button to hide all windows so the desktop is visible
//===========================================
#ifndef _LUMINA_DESKTOP_GO_HOME_PLUGIN_H
#define _LUMINA_DESKTOP_GO_HOME_PLUGIN_H

// Qt includes
#include <QToolButton>
#include <QString>
#include <QWidget>


// Lumina-desktop includes
#include "../LPPlugin.h" //main plugin widget

// libLumina includes
#include "LuminaXDG.h"

// PANEL PLUGIN BUTTON
class LHomeButtonPlugin : public LPPlugin{
	Q_OBJECT
	
public:
	LHomeButtonPlugin(QWidget *parent = 0, QString id = "homebutton", bool horizontal=true);
	~LHomeButtonPlugin();
	
private:
	QToolButton *button;

	void updateButtonVisuals();

private slots:
	void showDesktop();

public slots:
	void OrientationChange(){
	  if(this->layout()->direction()==QBoxLayout::LeftToRight){
	    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
	    button->setIconSize( QSize(this->height(), this->height()) );
	  }else{
	    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
	    button->setIconSize( QSize(this->width(), this->width()) );
	  }
	  this->layout()->update();
	  updateButtonVisuals();
	}
	
	void LocaleChange(){ 
	  updateButtonVisuals();
	}
	
	void ThemeChange(){
	  updateButtonVisuals();
	}
};

#endif
