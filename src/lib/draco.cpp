/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
# Available under the 3-clause BSD license
# See the LICENSE file for full details
#
*/

#include "draco.h"
//#include "XDGMime.h"
#include "LuminaXDG.h"
#include "LUtils.h"

Draco::Draco() {}

Draco::~Draco() {}

const QString Draco::desktopSessionName()
{
    return DESKTOP_SERVICE_NAME;
}

const QString Draco::desktopSessionPath()
{
    return QString("/Desktop");
}

const QString Draco::desktopSessionFullPath()
{
    return QString("/%1").arg(DESKTOP_SERVICE_NAME).replace(".", "/");
}

const QString Draco::powerdSessionName()
{
    return POWERD_SERVICE_NAME;
}

const QString Draco::powerdSessionPath()
{
    return QString("/Powerd");
}

const QString Draco::powerdSessionFullPath()
{
    return QString("/%1").arg(POWERD_SERVICE_NAME).replace(".", "/");
}

const QString Draco::powerSessionName()
{
    return POWER_SERVICE_NAME;
}

const QString Draco::powerSessionPath()
{
    return QString("/Power");
}

const QString Draco::powerSessionFullPath()
{
    return QString("/%1").arg(POWER_SERVICE_NAME).replace(".", "/");
}

const QString Draco::xconfig()
{
    return QString("%1-display-manager").arg(DESKTOP_APP);
}

const QString Draco::storageApp()
{
    return QString("%1-storage-manager").arg(DESKTOP_APP);
}

const QString Draco::powerApp()
{
    return POWER_SERVICE_NAME;
}

const QString Draco::desktopApp()
{
    return DESKTOP_SERVICE_NAME;
}

const QString Draco::launcherApp()
{
    return LUtils::isValidBinaryWithPath(XDG_OPEN);
}

const QString Draco::terminalApp()
{
    QString term = LXDG::findDefaultAppForMime("application/terminal");
    if (term.endsWith(".desktop")) {
        XDGDesktop DF(term);
        if (DF.isValid()) { return DF.getDesktopExec(); }
    }
    if (LUtils::isValidBinary("qterminal")) { term = "qterminal"; }
    else if (LUtils::isValidBinary("lxterminal")) { term = "lxterminal"; }
    else if (LUtils::isValidBinary("konsole")) { term = "konsole"; }
    else if (LUtils::isValidBinary("gnome-terminal")) { term = "gnome-terminal"; }
    else if (LUtils::isValidBinary("xfce4-terminal")) { term = "xfce4-terminal"; }
    else if (LUtils::isValidBinary("rxvt")) { term = "rxvt"; }
    else if (LUtils::isValidBinary("xterm")) { term = "xterm"; }
    return term;
}

const QString Draco::configDir()
{
    QString path = QString("%1/.config/%2-desktop").arg(QDir::homePath()).arg(DESKTOP_APP);
    QDir dir(path);
    if (!dir.exists(path)) { dir.mkpath(path); }
    return path;
}

const QString Draco::sessionSettingsFile()
{
    QString file = QString("%1/%2.conf")
            .arg(configDir())
            .arg(DE_SESSION_SETTINGS);
    if (!QFile::exists(file)) {
        qDebug() << "no user session settings!";
        QFile mkfile(file);
        if (mkfile.open(QIODevice::WriteOnly)) { mkfile.close(); }
    }
    return file;
}

const QString Draco::desktopSettingsFile()
{
    QString file = QString("%1/%2.conf")
            .arg(configDir())
            .arg(DE_DESKTOP_SETTINGS);
    if (!QFile::exists(file)) {
        qDebug() << "no user desktop settings!";
        QFile mkfile(file);
        if (mkfile.open(QIODevice::WriteOnly)) { mkfile.close(); }
    }
    return file;
}

const QString Draco::envSettingsFile()
{
    QString file = QString("%1/%2.conf")
            .arg(configDir())
            .arg(DE_ENV_SETTINGS);
    if (!QFile::exists(file)) {
        qDebug() << "no user env settings!";
        QFile mkfile(file);
        if (mkfile.open(QIODevice::WriteOnly)) { mkfile.close(); }
    }
    return file;
}

QStringList Draco::iconLocations(const QString &appPath)
{
    QStringList result;
    result << QString("%1/.local/share/icons").arg(QDir::homePath());
    result << QString("%1/../share/icons").arg(appPath);
    result << "/usr/share/icons" << "/usr/local/share/icons";
    result << "/usr/pkg/share/icons";
    return result;
}

QStringList Draco::pixmapLocations(const QString &appPath)
{
    QStringList result;
    result << QString("%1/.local/share/pixmaps").arg(QDir::homePath());
    result << QString("%1/../share/pixmaps").arg(appPath);
    result << "/usr/share/pixmaps" << "/usr/local/share/pixmaps";
    result << "/usr/pkg/share/pixmaps";
    return result;
}

QStringList Draco::applicationLocations(const QString &appPath)
{
    QStringList result;
    result << QString("%1/.local/share/applications").arg(QDir::homePath());
    result << QString("%1/../share/applications").arg(appPath);
    result << "/usr/share/applications" << "/usr/local/share/applications";
    result << "/usr/pkg/share/applications";
    return result;
}

bool Draco::xdgOpenCheck()
{
    // "replace" xdg-open with our own (since xdg-open does not support us yet)
    QString shadowPath =  QString("%1/bin").arg(configDir());
    QString shadowXDG = QString("%1/xdg-open").arg(shadowPath);
    if (!QFile::exists(shadowPath)) {
        QDir shadowDir(shadowPath);
        if (!shadowDir.mkpath(shadowPath)) {
            qWarning() << "Failed to create directory" << shadowPath;
            return false;
        }
    }
    QFileInfo info(shadowXDG);
    if (!info.isReadable()) {
        if (QFile::exists(shadowXDG) || info.isSymLink()) {
            if (!QFile::remove(shadowXDG)) {
                qDebug() << "Failed to remove existing symlink" << shadowXDG;
                return false;
            }
        }
        if (!QFile::link(launcherApp(), shadowXDG)) {
            qDebug() << "Failed to create symlink" << launcherApp() << shadowXDG;
            return false;
        }
    }
    return true;
}

const QString Draco::windowManager()
{
    QString wm = "openbox";
    return wm;
}

const QString Draco::windowManagerConf()
{
    QString conf = QString("%1/rc.xml").arg(configDir());
    if (!QFile::exists(conf)) {
        qDebug() << "RC XML MISSING!";
        QFile rc(conf);
        QFile def(":/theme/rc.xml");
        if (def.open(QIODevice::ReadOnly|QIODevice::Text)) {
            qDebug() << "OPEN FALLBACK RC XML";
            if (rc.open(QIODevice::WriteOnly|QIODevice::Text)) {
                rc.write(def.readAll());
                qDebug() << "WRITE NEW RC XML";
                rc.close();
            }
            def.close();
        }
    }
    return conf;
}

const QString Draco::windowManagerCmdStart()
{
    QString cmd = QString("%1 --config-file %2").arg(windowManager()).arg(windowManagerConf());
    return cmd;
}

const QString Draco::windowManagerCmdReConf()
{
    QString reconf = QString("%1 --reconfigure").arg(windowManagerCmdStart());
    return reconf;
}

const QString Draco::dracoStyleConf()
{
    QString style = QString("%1/draco.qss").arg(configDir());
    if (!QFile::exists(style)) { return ":/theme/draco.qss"; }
    return style;
}

const QString Draco::panelStyleSheet()
{
    QString result;
    QFile qss(dracoStyleConf());
    if (qss.open(QIODevice::ReadOnly|QIODevice::Text)) {
        result = qss.readAll();
        qss.close();
    }
    return result;
}

const QString Draco::desktopStyleSheet()
{
    QString result;
    QFile qss(dracoStyleConf());
    if (qss.open(QIODevice::ReadOnly|QIODevice::Text)) {
        result = qss.readAll();
        qss.close();
    }
    return result;
}

void Draco::themeEngineCheckConf()
{
    QString conf = QString("%1/.config/qt5ct/qt5ct.conf").arg(QDir::homePath());
    if (!QFile::exists(conf)) {
        qDebug() << "QT5CT CONF MISSING!";
        QFile file(conf);
        QFile def(":/theme/qt5ct.conf");
        if (def.open(QIODevice::ReadOnly|QIODevice::Text)) {
            qDebug() << "OPEN FALLBACK QT5CT CONF";
            if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
                file.write(def.readAll());
                qDebug() << "WRITE NEW QT5CT CONF";
                file.close();
            }
            def.close();
        }
    }
}

void Draco::checkGtk2Conf()
{
    QString conf = QString("%1/.gtkrc-2.0").arg(QDir::homePath());
    if (!QFile::exists(conf)) {
        qDebug() << "GTK2 CONF MISSING!";
        QFile file(conf);
        QFile def(":/theme/gtkrc-2.0");
        if (def.open(QIODevice::ReadOnly|QIODevice::Text)) {
            qDebug() << "OPEN FALLBACK GTK2 CONF";
            if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
                file.write(def.readAll());
                qDebug() << "WRITE NEW GTK2 CONF";
                file.close();
            }
            def.close();
        }
    }
}

void Draco::checkGtk3Conf()
{
    QString gtkdir = QString("%1/.config/gtk-3.0").arg(QDir::homePath());
    QString conf = QString("%1/%2/settings.ini").arg(QDir::homePath()).arg(gtkdir);
    if (!QFile::exists(conf)) {
        QDir dir(gtkdir);
        if (!dir.exists()) { dir.mkpath(gtkdir); }
        qDebug() << "GTK3 CONF MISSING!";
        QFile file(conf);
        QFile def(":/theme/settings.ini");
        if (def.open(QIODevice::ReadOnly|QIODevice::Text)) {
            qDebug() << "OPEN FALLBACK GTK3 CONF";
            if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
                file.write(def.readAll());
                qDebug() << "WRITE NEW GTK3 CONF";
                file.close();
            }
            def.close();
        }
    }
}

void Draco::checkConfigs()
{
    xdgOpenCheck();
    themeEngineCheckConf();
    checkGtk2Conf();
    checkGtk3Conf();
}

const QString Draco::getProperMime(const QString &mime)
{
    QString result;
    if (mime.startsWith("image")) {
        result = "image-x-generic";
    } else if(mime.startsWith("audio")) {
        result = "audio-x-generic";
    } else if(mime.startsWith("video")) {
        result = "video-x-generic";
    } else if (mime.contains("-tar") ||
               mime.contains("compressed") ||
               mime.contains("xz") ||
               mime.contains("bz2") ||
               mime.contains("gz") ||
               mime.contains("rar") ||
               mime.contains("zip") ||
               mime.contains("rpm") ||
               mime.contains("deb")) {
        result = "package-x-generic";
    } else if (mime.endsWith("cd-image") ||
               mime.endsWith("apple-diskimage") ||
               mime.endsWith("disk-image") ||
               mime.endsWith("saturn-rom") ||
               mime.endsWith("wii-rom") ||
               mime.endsWith("gamecube-rom") ||
               mime.endsWith("appimage")) {
        result = "media-optical";
    } else if (mime.contains("office")) {
        if (mime.contains("document")) {
            result = "x-office-document";
        }
        else if (mime.contains("drawing")) {
            result = "x-office-drawing";
        }
        else if (mime.contains("presentation")) {
            result = "x-office-presentation";
        }
        else if (mime.contains("spreadsheet")) {
            result = "x-office-spreadsheet";
        }
    } else if (mime.startsWith("text")) {
        if (mime.contains("python") ||
                mime.contains("perl") ||
                mime.contains("php") ||
                mime.contains("ruby") ||
                mime.contains("script") ||
                mime.contains("shell"))
        {
            result = "text-x-script";
        } else if (mime.contains("html")) {
            result = "text-html";
        } else {
            result = "text-x-generic";
        }
    } else if (mime.endsWith("-executable")) {
        result = "application-x-executable";
    } else {
        result = "text-x-generic";
    }
    if (result.isEmpty()) { return mime; }
    return  result;
}

const QString Draco::filterIconName(const QString &name)
{
    QString result;
    if (name == "xscreensaver") { result = "preferences-desktop-screensaver"; }
    else if (name == "xine") { result = "applications-multimedia"; }
    else if (name == "cups") { result = "printer"; }
    else if (name == "/usr/share/icons/Humanity/devices/48/printer.svg") { result = "printer"; }
    else if (name == "QtIcon") { result = "qt-logo"; }
    else if (name.contains("distcc")) { result = "utilities-system-monitor"; }
    else if (name == "system-reboot") { result = "system-shutdown"; }
    else if (name == "system") { result = "computer"; }
    else if (name == "dialog-cancel") { result = "process-stop"; }
    else if (name == "dialog") { result = "user-available"; }
    else if (name == "preferences-desktop-icons") { result = "preferences-desktop-theme"; }
    else if (name == "transform") { result = "object-rotate-left"; }
    else if (name == "transform-scale") { result = "view-fullscreen"; }
    else if (name == "transform-move") { result = "view-restore"; }
    if (result.isEmpty()) { return name; }
    return result;
}
