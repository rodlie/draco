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
//  Lumina-desktop source code
//  Copyright (c) 2017, Ken Moore
//  Available under the 3-clause BSD license
//===========================================
// This is a simple class for loading/serving icon files
// from the icon theme or local filesystem
//===========================================
#ifndef LUMINA_LIBRARY_ICON_CACHE_H
#define LUMINA_LIBRARY_ICON_CACHE_H

#include <QHash>
#include <QIcon>
#include <QPixmap>
#include <QFileSystemWatcher>
#include <QString>
#include <QFile>
#include <QDateTime>
#include <QAbstractButton>
#include <QLabel>
#include <QAction>
#include <QPointer>

//Data structure for saving the icon/information internally
struct icon_data{
  QString fullpath;
  QDateTime lastread;
  QList<QPointer<QLabel> > pendingLabels;
  QList<QPointer<QAbstractButton> > pendingButtons;
  QList<QPointer<QAction> > pendingActions;
  QList<QPointer<QMenu> > pendingMenus;
  QIcon icon;
  QIcon thumbnail;
};

class LIconCache : public QObject{
	Q_OBJECT
public:
	LIconCache(QObject *parent = 0);
	~LIconCache();

	//Static method for using this class (DO NOT MIX WITH GLOBAL OBJECT METHOD)
	// Either use this the entire time, or use a saved/global object - pick one and stick with it
	//  otherwise you may end up with multiple icon cache's running for your application
	static LIconCache* instance();

	//Icon Checks
	bool exists(QString icon);
	bool isLoaded(QString icon);
	QString findFile(QString icon); //find the full path of a given file/name (searching the current Icon theme)

	//Special loading routines for QLabel and QAbstractButton (pushbutton, toolbutton, etc)
	void loadIcon(QAbstractButton *button, QString icon, bool noThumb = false);
	void loadIcon(QLabel *label, QString icon, bool noThumb = false);
	void loadIcon(QAction *action, QString icon, bool noThumb = false);
	void loadIcon(QMenu *action, QString icon, bool noThumb = false);

	QIcon loadIcon(QString icon, bool noThumb = false); //generic loading routine - does not background the loading of icons when not in the cache

	void clearIconTheme(); //use when the icon theme changes to refresh all requested icons
	void clearAll(); //Clear all cached icons

private:
	QHash<QString, icon_data> HASH;
	QFileSystemWatcher *WATCHER;

	icon_data createData(QString icon);
	QStringList getChildIconDirs(QString path); //recursive function to find directories with icons in them
	QStringList getIconThemeDepChain(QString theme, QStringList paths);

	void startReadFile(QString id, QString path);
	void ReadFile(LIconCache *obj, QString id, QString path);

	bool isThemeIcon(QString id);
	QIcon iconFromTheme(QString id);

private slots:
	void IconLoaded(QString id, QDateTime sync, QByteArray *data);

signals:
	void InternalIconLoaded(QString, QDateTime, QByteArray*); //INTERNAL SIGNAL - DO NOT USE in other classes/objects
	void IconAvailable(QString); //way for classes to listen/reload icons as they change
};

#endif
