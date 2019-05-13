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
//  Copyright (c) 2012-2016, Ken Moore
//  Available under the 3-clause BSD license
//===========================================
#ifndef LUMINA_LIBRARY_UTILS_H
#define LUMINA_LIBRARY_UTILS_H

#include <QCoreApplication>
#include <QProcess>
#include <QTextStream>
#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QTranslator>
#include <QDebug>
#include <QDesktopWidget>
#include <QImageReader>
#include <QRegExp>
#include <QFuture>
#include <QScreen>
#include <QSettings>

class LUtils{
public:
	enum StandardDir {Desktop, Documents, Downloads, Music, Pictures, PublicShare, Templates, Videos};

	//Return the path to one of the XDG standard directories
	static QString standardDirectory(StandardDir dir, bool createAsNeeded = true);

	//Run an external command and return output & exit code
	static QString runCommand(bool &success, QString command, QStringList arguments = QStringList(), QString workdir = "", QStringList env = QStringList());

	//Run an external command and return the exit code
	static int runCmd(QString cmd, QStringList args = QStringList());
	//Run an external command and return any text output (one line per entry)
	static QStringList getCmdOutput(QString cmd, QStringList args = QStringList());

	//Read a text file
	static QStringList readFile(QString filepath);
	//Write a text file
	static bool writeFile(QString filepath, QStringList contents, bool overwrite=false);

	//Check whether a file/path is a valid binary
    static const QString isValidBinaryWithPath(QString bin);
	static bool isValidBinary(QString& bin); //full path or name only
	static bool isValidBinary(const char *bin){
	  QString bins(bin);
	  return isValidBinary(bins); //overload for a "junk" binary variable input
	}

	//Open the right settings file (user/permissions aware)
	static QSettings* openSettings(QString org, QString name, QObject *parent = 0);

	//Return all the dirs on the system which contain .desktop files
        static QStringList systemApplicationDirs();

	//Create the exec string to open a terminal in a particular directory
	static QString GenerateOpenTerminalExec(QString term, QString dirpath);

	//List all the sub-directories of a parent dir (recursive)
	static QStringList listSubDirectories(QString dir, bool recursive = true);

	//Convert an input file/dir path to an absolute file path
	static QString PathToAbsolute(QString path); //This is primarily for CLI usage (relative paths)
	static QString AppToAbsolute(QString path); //This is for looking up a binary/ *.desktop path

	//Get the list of all file extensions which Qt can read (lowercase)
	static QStringList imageExtensions(bool wildcards = false);
	static QStringList videoExtensions();

	//Load a translation file for a Lumina Project
	static QTranslator* LoadTranslation(QApplication *app, QString appname, QString locale = "", QTranslator *cTrans = 0);
	//Other localization shortcuts
	static QStringList knownLocales(); //Note: This only lists locales known to Lumina (so the i18n files need to be installed)
	static void setLocaleEnv(QString lang, QString msg="", QString time="", QString num="" ,QString money="",QString collate="", QString ctype="");
	static QString currentLocale();

	//Number format conversions
	static double DisplaySizeToBytes(QString num); //Turn a display size (like 50M or 50KB) into a double for calculations (bytes)
	static QString BytesToDisplaySize(qint64 bytes); //convert into a readable size (like 50M or 50KB)

	static QString SecondsToDisplay(int secs); //convert into a readable time
};
#endif
