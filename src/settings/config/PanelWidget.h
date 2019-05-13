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
#ifndef _LUMINA_CONFIG_PANEL_WIDGET_H
#define _LUMINA_CONFIG_PANEL_WIDGET_H

//#include "globals.h"

#include <QWidget>
#include <QSettings>

#include "LPlugins.h"

//namespace for using the *.ui file
namespace Ui{
	class PanelWidget;
};

class PanelWidget : public QWidget{
	Q_OBJECT
public:
	PanelWidget(QWidget *parent, QWidget *Main, LPlugins *Pinfo);
	~PanelWidget();

	void LoadSettings(QSettings *settings, int Dnum, int Pnum);
	void SaveSettings(QSettings *settings, QString screenID = ""); //save the current settings
	
	int PanelNumber();
	void ChangePanelNumber(int newnum);

public slots:
	void LoadIcons();

private:
	Ui::PanelWidget *ui;
	QWidget *mainui;
	LPlugins *PINFO;

	int dnum, pnum;

	void reloadColorSample();
	QString getSysApp(bool allowreset = false);
	QString getColorStyle(QString current, bool allowTransparency = true);
private slots:
	void on_tool_rm_clicked();
	void ItemChanged();
	void UseColorChanged();
	void on_tool_selectcolor_clicked();
	void on_tool_addplugin_clicked();
	void on_tool_remplugin_clicked();
	void on_tool_upplugin_clicked();
	void on_tool_downplugin_clicked();

signals:
	void PanelChanged();
	void PanelRemoved(int);

};

#endif
