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
//  Lumina Desktop Source Code
//  Copyright (c) 2016, Ken Moore
//  Available under the 3-clause BSD license
//===========================================
#ifndef _LUMINA_CONFIG_MAIN_WINDOW_H
#define _LUMINA_CONFIG_MAIN_WINDOW_H
//#include "globals.h"
//#include "pages/getPage.h"

#include "LuminaXDG.h"

#include <QMainWindow>
#include <QShortcut>
#include <QTimer>
#include <QResizeEvent>
#include <QMoveEvent>

namespace Ui{
	class mainWindow;
};

class mainWindow : public QMainWindow{
	Q_OBJECT
public:
	mainWindow();
	~mainWindow();

public slots:
	void slotSingleInstance(QStringList args);
	void setupIcons();
	void loadMonitors();

private:
	Ui::mainWindow *ui;
	QShortcut *backShortcut, *quitShortcut;
	QString cpage; //current page
	QTimer *geomTimer;

	void changePage(QString id);

private slots:
	void saveWinGeometry();

	//Page signals
	void pageCanSave(bool);
	void pageSetTitle(QString);
	bool page_change(QString);
	//Toolbar actions
	void on_actionSave_triggered();
	void on_actionBack_triggered();
        void changeMonitor(QAction*);
	void quitShortcut_Triggered();

protected:
	void resizeEvent(QResizeEvent *ev){
	  if(geomTimer->isActive()){ geomTimer->stop(); }
	  geomTimer->start();
	  QMainWindow::resizeEvent(ev);
	}
	void moveEvent(QMoveEvent *ev){
	  if(geomTimer->isActive()){ geomTimer->stop(); }
	  geomTimer->start();
	  QMainWindow::moveEvent(ev);
	}
};
#endif
