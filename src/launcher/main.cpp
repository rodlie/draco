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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);




    Draco::xdgOpenCheck();

    if (argc<=1) {
        std::cout << DESKTOP_APP_NAME << " XDG Open v" << DESKTOP_APP_VERSION << std::endl;
        return 0;
    }






    if (argc>1) {
        QString fileName = argv[1];
        if (QFile::exists(fileName)) {
            if (fileName.endsWith(QString(".desktop")) && fileName.startsWith("/")) { // open app
                qDebug() << "OPEN APP" << fileName;
                XDGDesktop desktop(fileName);
                if (!desktop.getDesktopExec().isEmpty()) {
                    QString exe = desktop.getDesktopExec();
                    qDebug() << "EXE?" << exe;
                    exe.replace("%u","");
                    exe.replace("%U","");
                    exe.replace("%f","");
                    exe.replace("%F","");
                    if (exe.contains("google-chrome") || exe.contains("chromium")) {
                        setenv("DESKTOP_SESSION", "xfce", 1);
                        setenv("XDG_CURRENT_DESKTOP", "xfce", 1);
                        QProcess proc;
                        proc.startDetached(exe);
                        setenv("DESKTOP_SESSION", DESKTOP_APP_NAME, 1);
                        setenv("XDG_CURRENT_DESKTOP", DESKTOP_APP_NAME, 1);
                    } else { QProcess::startDetached(exe); }
                }
            } else { // is file or folder
                QString mime = XDGMime::findAppMimeForFile(fileName);
                qDebug() << "IS FILE/FOLDER" << mime;
                QFileInfo info(fileName);
                if (info.isDir()) { // is folder
                    qDebug() << "IS FOLDER" << fileName;
                    QString dirApp = XDGMime::findDefaultAppForMime("application/x-directory");
                    if (!dirApp.isEmpty()) {
                        XDGDesktop desktop(dirApp);
                        if (!desktop.getDesktopExec().isEmpty()) {
                            QString exe = desktop.getDesktopExec();
                            qDebug() << "OPEN FOLDER EXE" << exe;
                            exe.replace("%u","");
                            exe.replace("%U","");
                            exe.replace("%f","");
                            exe.replace("%F","");
                            QProcess::startDetached(QString("%1 \"%2\"").arg(exe).arg(fileName));
                        }
                    }
                } else { // is file
                    if ((mime.contains("executable") || mime.contains("script") || mime.isEmpty()) &&
                        info.isExecutable())
                    { // run file
                        qDebug() << "RUN EXECUTABLE/SCRIPT" << fileName;
                        QProcess::startDetached(fileName);
                    } else { // open file
                        qDebug() << "OPEN FILE" << fileName << mime;
                        if (!mime.isEmpty()) {
                            QString mimeApp = XDGMime::findDefaultAppForMime(mime);
                            qDebug() << "OPEN FILE WITH" << mimeApp;
                            XDGDesktop desktop(mimeApp);
                            if (!desktop.getDesktopExec().isEmpty()) {
                                QString exe = desktop.getDesktopExec();
                                qDebug() << "OPEN FILE EXE" << exe;
                                exe.replace("%u","");
                                exe.replace("%U","");
                                exe.replace("%f","");
                                exe.replace("%F","");
                                if (exe.contains("google-chrome") || exe.contains("chromium")) {
                                    setenv("DESKTOP_SESSION", "xfce", 1);
                                    setenv("XDG_CURRENT_DESKTOP", "xfce", 1);
                                    QProcess proc;
                                    proc.startDetached(QString("%1 \"%2\"").arg(exe).arg(fileName));
                                    setenv("DESKTOP_SESSION", DESKTOP_APP_NAME, 1);
                                    setenv("XDG_CURRENT_DESKTOP", DESKTOP_APP_NAME, 1);
                                } else { QProcess::startDetached(QString("%1 \"%2\"").arg(exe).arg(fileName)); }
                            }
                        }
                    }
                }
            }
        } else if (fileName.startsWith("http") ||
                   fileName.startsWith("ftp"))
        { // open url in browser
            qDebug() << "OPEN IN BROWSER" << fileName;
            QString app = XDGMime::findDefaultAppForMime("application/x-extension-html");
            if (!app.isEmpty()) {
                XDGDesktop desktop(app);
                if (!desktop.getDesktopExec().isEmpty()) {
                    QString exe = desktop.getDesktopExec();
                    qDebug() << "BROWSER EXE" << exe;
                    exe.replace("%u","");
                    exe.replace("%U","");
                    exe.replace("%f","");
                    exe.replace("%F","");
                    if (exe.contains("google-chrome") || exe.contains("chromium")) {
                        setenv("DESKTOP_SESSION", "xfce", 1);
                        setenv("XDG_CURRENT_DESKTOP", "xfce", 1);
                        QProcess proc;
                        proc.startDetached(QString("%1 \"%2\"").arg(exe).arg(fileName));
                        setenv("DESKTOP_SESSION", DESKTOP_APP_NAME, 1);
                        setenv("XDG_CURRENT_DESKTOP", DESKTOP_APP_NAME, 1);
                    } else { QProcess::startDetached(QString("%1 \"%2\"").arg(exe).arg(fileName)); }
                }
            }
        }
    }
    return 0;
}
