/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
# Available under the 3-clause BSD license
# See the LICENSE file for full details
#
*/

#include <iostream>
#include "LuminaXDG.h"
#include "XDGMime.h"
#include "draco.h"

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QDebug>
#include <QUrl>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // verify our xdg-open shadow file
    Draco::xdgOpenCheck();

    QString fileName;
    if (argc<=1) { // do nothing
        std::cout << DESKTOP_APP_NAME << " xdg-open version " << DESKTOP_APP_VERSION << std::endl;
        return 0;
    } else if (argc>1) { // get "filename"
        fileName = argv[1];
        if (fileName.isEmpty()) {
            std::cout << "invalid input" << std::endl;
            return  1;
        }
    }

    bool isLocalFile = false;
    bool openFile = false;
    bool openInBrowser = false;
    bool isDesktop = false;
    QString cmd, desktopFile;

    if (QFile::exists(fileName)) { // local
        isLocalFile = true;
        if (fileName.endsWith(QString(".desktop")) &&
            fileName.startsWith("/")) { isDesktop = true; }
        else { openFile = true; }
    } else { // remote
        QUrl url(fileName);
        if (url.isValid() &&
            (url.scheme()=="http" ||
             url.scheme()=="https" ||
             url.scheme()=="ftp")) { openInBrowser = true; }
    }

    if (isLocalFile && !isDesktop) { // handle local file
        QFileInfo localInfo(fileName);
        if (localInfo.isDir()) { // is directory, get default file manager
            desktopFile = XDGMime::findDefaultAppForMime("application/x-directory");
        } else { // is file, try to get default application for mime type
            desktopFile = XDGMime::findDefaultAppForMime(XDGMime::findAppMimeForFile(fileName));
        }
    } else if (openInBrowser) { // handle remote url
        QUrl url(fileName);
        QString scheme;
        if (url.scheme()=="http") {
            scheme = "x-scheme-handler/http";
        } else if (url.scheme()=="https") {
            scheme = "x-scheme-handler/https";
        } else if (url.scheme()=="ftp") {
            scheme = "x-scheme-handler/ftp";
        }
        desktopFile = XDGMime::findDefaultAppForMime(scheme);
    }

    if (isDesktop) { desktopFile = fileName; } // is application

    if (!desktopFile.isEmpty()) { // get cmd from .desktop
        XDGDesktop desktop(desktopFile);
        if (!desktop.getDesktopExec().isEmpty()) { cmd = desktop.getDesktopExec(); }
    }

    if (!cmd.isEmpty()) { // now run something
        cmd.replace("%u","");
        cmd.replace("%U","");
        cmd.replace("%f","");
        cmd.replace("%F","");
        if (openFile || openInBrowser) { cmd = QString("%1 \"%2\"").arg(cmd).arg(fileName); } // add original filename
        qDebug() << "what to do?" << cmd;
        if (cmd.contains("google-chrome") || cmd.contains("chromium")) {
            // trick chrome to think it's running on Xfce
            // this is needed to enable power/screensaver/keyring compatibility in Draco
            // this should of course be fixed upstream, but for now this works ...
            setenv("DESKTOP_SESSION", "xfce", 1);
            setenv("XDG_CURRENT_DESKTOP", "xfce", 1);
            QProcess proc;
            proc.startDetached(cmd);
            setenv("DESKTOP_SESSION", DESKTOP_APP_NAME, 1);
            setenv("XDG_CURRENT_DESKTOP", DESKTOP_APP_NAME, 1);
        } else { QProcess::startDetached(cmd); }
    }
    return 0;
}
