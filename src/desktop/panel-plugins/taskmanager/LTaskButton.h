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
#ifndef _LUMINA_DESKTOP_TASK_BUTTON_H
#define _LUMINA_DESKTOP_TASK_BUTTON_H

// Qt includes
#include <QWidget>
#include <QList>
#include <QIcon>
#include <QCursor>
#include <QMenu>
#include <QEvent>
#include <QAction>
#include <QTimer>

// libLumina includes
#include <LuminaXDG.h>
#include <LuminaX11.h>

// Local includes
#include "../../LWinInfo.h"
#include "../LTBWidget.h"

class LTaskButton : public LTBWidget{
	Q_OBJECT
public:
	LTaskButton(QWidget *parent=0, bool smallDisplay = true);
	~LTaskButton();

	//Window Information
	QList<WId> windows();
	QString classname();
	bool isActive();

	//Window Management
	void addWindow(WId win); //Add a window to this button
	void rmWindow(WId win); //Remove a window from this button

private:
	QList<LWinInfo> WINLIST;
	QList<LWinInfo> LWINLIST;
	QMenu *actMenu; // action menu (custom context menu)
	QMenu *winMenu; // window menu (if more than 1)
	LWinInfo cWin;
	QString cname; //class name for the entire button
	bool noicon, showText;

	LWinInfo currentWindow(); //For getting the currently-active window
	LXCB::WINDOWVISIBILITY cstate; //current state of the button

public slots:
	void UpdateButton(); //re-sync the current window infomation
	void UpdateMenus(); //re-create the menus (text + icons)
    void setShowText(bool show);

private slots:

	void buttonClicked();
	void closeWindow(); //send the signal to close a window
	void maximizeWindow(); //send the signal to maximize/restore a window
	void minimizeWindow(); //send the signal to minimize a window (iconify)
	void showAllWindows();
	void hideAllWindows();
	void closeAllWindows();
	void triggerWindow(); //change b/w visible and invisible
	void winClicked(QAction*);
	void openActionMenu();
protected:
	void changeEvent(QEvent *ev){
	  LTBWidget::changeEvent(ev);
	  QEvent::Type tmp = ev->type();
	  if(tmp==QEvent::ThemeChange || tmp==QEvent::LanguageChange || tmp==QEvent::LocaleChange){
	    QTimer::singleShot(qrand()%100+500, this, SLOT(UpdateButton()) );
	  }
	}
signals:
	void MenuClosed();
};
#endif
