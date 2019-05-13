/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com>
# All rights reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser Public License
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
//  Copyright (c) 2013-2017, Ken Moore
//  Available under the 3-clause BSD license
//===========================================
// These structures/classes are for conforming to the FreeDesktop standards
// REFERENCE:
//    (DATABASE) https://www.freedesktop.org/wiki/Specifications/shared-mime-info-spec/
//    (APPLICATIONS) https://www.freedesktop.org/wiki/Specifications/mime-apps-spec/
// Mime Application Version Compliance: 1.0.1 (11/14/14) (Skips random *.desktop parsing: ~80% compliant)
//===========================================
#ifndef LUMINA_LIBRARY_XDG_MIME_CLASS_H
#define LUMINA_LIBRARY_XDG_MIME_CLASS_H

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QStringList>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QTextStream>
#include <QRegExp>
#include <QDebug>

class XDGMime{
public:
	// PRIMARY FUNCTIONS
	static QString fromFileName(QString filename); //Convert a filename into a mimetype
	static QStringList listFromFileName(QString filename); //Convert a filename into a list of mimetypes (arranged in descending priority)
	static QString toIconName(QString mime); //Mime type to icon name
	//Find the file extension for a particular mime-type
	static QStringList toFileExtensions(QString mime);

	// LESSER-USED FUNCTIONS
	//List all the mime-type directories
	static QStringList systemMimeDirs();
	//Find the mime-type of a particular file extension
	static QString findAppMimeForFile(QString filename, bool multiple = false);
	// Simplification function for finding all info regarding current mime defaults
	static QStringList listFileMimeDefaults();
	//Find the localized comment string for a particular mime-type
	static QString findMimeComment(QString mime);
	//Find the default application for a mime-type
	static QString findDefaultAppForMime(QString mime);
	//Fine the available applications for a mime-type
	static QStringList findAvailableAppsForMime(QString mime);
	//Set the default application for a mime-type
	static void setDefaultAppForMime(QString mime, QString app);
	//List all the registered audio/video file extensions
	static QStringList findAVFileExtensions();
	//Load all the "globs2" mime database files
	static QStringList loadMimeFileGlobs2();

};
#endif
