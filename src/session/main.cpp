//===========================================
//  Lumina-DE source code
//  Copyright (c) 2016, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================

#include <QDebug>
#include <QCoreApplication>
#include <QProcess>
#include <QString>
#include <QLockFile>
#include <QX11Info>

#include "session.h"
#include <LUtils.h>
#include <LDesktopUtils.h>
#include <LuminaThemes.h>
#include <LuminaXDG.h>


#include <unistd.h>

#include "common.h" // common stuff

int findAvailableSession(){
  int num = 0;
  while(QFile::exists("/tmp/.X11-unix/X"+QString::number(num))){ num++; }
  return num;
}

int main(int argc, char ** argv)
{
    //bool unified = false;
    /*if (argc > 1) {
      if (QString(argv[1]) == QString("--version")){
        qDebug() << LDesktopUtils::LuminaDesktopVersion();
        return 0;
      }else if(QString(argv[1]) == QString("--unified")){
        unified = true;
      }
    }*/
    /*if(!QFile::exists(LOS::LuminaShare())){
      qDebug() << "Lumina does not appear to be installed correctly. Cannot find: " << LOS::LuminaShare();
      return 1;
    }*/


    // Start X11 if needed
    QString disp = QString(getenv("DISPLAY")).simplified();
    if (disp.isEmpty()) {
        qDebug() << "No X11 session detected: will try to start one...";
        // No X session found. Go ahead and re-init this binary within an xinit call
        QString prog = QString(argv[0]).section("/", -1);
        LUtils::isValidBinary(prog); //will adjust the path to be absolute
        //if(unified){ prog = prog+" --unified"; }
        QStringList args; args << prog << "--" << ":" + QString::number(findAvailableSession());
        //if(LUtils::isValidBinary("x11vnc")){ args << "--" << "-listen" << "tcp"; } //need to be able to VNC into this session
        return QProcess::execute("xinit", args);
    }
    qDebug() << "Starting the desktop on current X11 session:" << disp;

    // Check for any stale desktop lock files and clean them up
    QString cfile = QDir::tempPath()+"/.LSingleApp-%1-%2-%3";
    QString desk = QString("%1-desktop").arg(DESKTOP_APP);
    //if(unified){ desk.append("-unified"); }
    cfile = cfile.arg( QString(getlogin()), desk, QString::number(QX11Info::appScreen()) );
    if (QFile::exists(cfile)) {
        qDebug() << "Found Desktop Lock for X session:" << disp;
        qDebug() << " - Disabling Lock and starting new desktop session";
        QLockFile lock(cfile+"-lock");
        if (lock.isLocked()) { lock.unlock(); }
        QFile::remove(cfile);
    }
    if (QFile::exists(QDir::tempPath()+QString("/.%1stopping").arg(DESKTOP_APP))) {
        QFile::remove(QDir::tempPath()+QString("/.%1stopping").arg(DESKTOP_APP));
    }

    // Configure X11 monitors if needed
    QString xconf = QString("%1-xconfig").arg(DESKTOP_APP);
    if (LUtils::isValidBinary(xconf)) {
        qDebug() << " - Resetting monitor configuration to last-used settings";
        QProcess::execute(QString("%1 --reset-monitors").arg(xconf));
    }

    qDebug() << " - Starting the session...";
    // Setup any initialization values
    LTHEME::LoadCustomEnvSettings();
    LXDG::setEnvironmentVars();
    setenv("DESKTOP_SESSION", DESKTOP_APP_NAME, 1);
    setenv("XDG_CURRENT_DESKTOP", DESKTOP_APP_NAME, 1);

    // Startup the session
    QApplication a(argc, argv);
    setenv("QT_QPA_PLATFORMTHEME","qt5ct",1); //make sure this is after the QApplication - not actually using the theme plugin for **this** process
    LSession sess;
    //sess.checkFiles(); //Make sure user files are created/installed first
    sess.start();
    return a.exec();
}
