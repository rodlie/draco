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
#ifndef _LUMINA_CONFIG_PAGE_DEFAULTAPPS_H
#define _LUMINA_CONFIG_PAGE_DEFAULTAPPS_H
//#include "../globals.h"
#include "PageWidget.h"

#include <QToolButton>

namespace Ui{
	class page_defaultapps;
};

class page_defaultapps : public PageWidget{
	Q_OBJECT
public:
	page_defaultapps(QWidget *parent);
	~page_defaultapps();

public slots:
	void SaveSettings();
	void LoadSettings(int screennum);
	void updateIcons();

private:
	Ui::page_defaultapps *ui;
  QString defaultBrowser;
  QString defaultEmail;
  QString defaultFileManager;
  QString defaultTerminal;
  QString defaultCDPlayer;
  QString defaultDVDPlayer;

	QString getSysApp(bool allowreset, QString defaultPath = "");

	void updateDefaultButton(QToolButton *button, QString app);

private slots:
	//Simple defaults tab
	void changeDefaultBrowser();
	void changeDefaultEmail();
	void changeDefaultFileManager();
	void changeDefaultTerminal();
    void changeDefaultCDPlayer();
    void changeDefaultDVDPlayer();
	//Advanced defaults tab
	void cleardefaultitem();
	void setdefaultitem();
	void setdefaultbinary();
	void checkdefaulticons();

};
#endif
