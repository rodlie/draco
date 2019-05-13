/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
#
*/

#ifndef DRACO_H
#define DRACO_H

#include <QObject>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QDirIterator>
#include <QApplication>
#include <QVariant>
#include <QFont>

#define DE_SESSION_SETTINGS "sessionsettings"
#define DE_DESKTOP_SETTINGS "desktopsettings"
#define DE_PLUGIN_SETTINGS "pluginsettings"
#define DE_ENV_SETTINGS "envsettings"

class Draco : public QObject
{
    Q_OBJECT

public:
    Draco();
    virtual ~Draco();

    static const QString desktopSessionName();
    static const QString desktopSessionPath();
    static const QString desktopSessionFullPath();
    static const QString powerdSessionName();
    static const QString powerdSessionPath();
    static const QString powerdSessionFullPath();
    static const QString powerSessionName();
    static const QString powerSessionPath();
    static const QString powerSessionFullPath();
    static const QString xconfig();
    static const QString storageApp();
    static const QString powerApp();
    static const QString desktopApp();
    static const QString launcherApp();
    static const QString terminalApp();
    static const QString configDir();
    static const QString sessionSettingsFile();
    static const QString desktopSettingsFile();
    static const QString envSettingsFile();
    static const QString powerSettingsFile();
    static const QString storageSettingsFile();
    static const QString xconfigSettingsFile();
    static const QString keyboardSettingsFile();
    static const QString themeSettingsFile();
    static QStringList iconLocations(const QString &appPath);
    static QStringList pixmapLocations(const QString &appPath);
    static QStringList applicationLocations(const QString &appPath);
    static const QString xdgConfigHome();
    static const QString xdgDataHome();
    static bool xdgOpenCheck();
    static void xdgMimeCheck();
    static const QString windowManager();
    static const QString windowManagerConf();
    static const QString windowManagerCmdStart();
    static const QString windowManagerCmdReConf();
    static const QString dracoStyleConf();
    static const QString panelStyleSheet();
    static const QString desktopStyleSheet();
    static void themeEngineCheckConf(const QString &theme = QString());
    static void checkGtk2Conf(const QString &theme = QString(), QFont font = QFont());
    static void checkGtk3Conf(const QString &theme = QString(), QFont font = QFont());
    static void checkConfigs();
    static const QString getProperMime(const QString &mime);
    static const QString filterIconName(const QString &name);
    static QVariant readSetting(QString conf,
                                QString key,
                                QVariant fallback = QVariant());
    static void writeSetting(QString conf,
                             QString key,
                             QVariant value);
    static bool isBlacklistedApplication(const QString &exec);
    static const QString getOSReleaseInfo(const QString &type);
    static bool kernelCanResume();
};

#endif // DRACO_H
