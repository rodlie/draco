//===========================================
//  Lumina-DE source code
//  Copyright (c) 2016, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================

#include "session.h"

#include <QObject>
#include <QProcess>
#include <QProcessEnvironment>
#include <QDebug>
#include <QSettings>
#include <QDir>

#include <LUtils.h>
//#include <LuminaOS.h>
#include <LDesktopUtils.h>
#include <LuminaSingleApplication.h>
#include "common.h"

void LSession::stopall(){
  stopping = true;
  for(int i=0; i<PROCS.length(); i++){
    if(PROCS[i]->state()!=QProcess::NotRunning){ PROCS[i]->kill(); }
  }
  QCoreApplication::processEvents();
  for(int i=0; i<PROCS.length(); i++){
    if(PROCS[i]->state()!=QProcess::NotRunning){ PROCS[i]->terminate(); }
  }
  QCoreApplication::exit(0);
}

void LSession::procFinished(){
  //Go through and check the status on all the procs to determine which one finished
  int stopped = 0;
  //qDebug() << "Got Process Stopped Signal:";
  for(int i=0; i<PROCS.length(); i++){
    if(PROCS[i]->state()==QProcess::NotRunning){
      //qDebug() << " - Stopped:" << PROCS[i]->objectName();
      stopped++;
      if(!stopping){
        //See if this process is the main desktop binary
        if(PROCS[i]->objectName()=="runtime"){
          qDebug() << "Got Desktop Process Finished:" << PROCS[i]->exitCode();
          //if(PROCS[i]->exitCode()==787){ PROCS[i]->start(QIODevice::ReadOnly); } //special internal restart code
          //else{
          stopall(); //}
        }else if(PROCS[i]->objectName()=="wm" && wmfails<2){ wmfails++; PROCS[i]->start(QIODevice::ReadOnly); wmTimer->start(); } //restart the WM
        //if(PROCS[i]->program().section("/",-1) == "lumina-desktop"){ stopall();  } //start closing down everything
        //else{ PROCS[i]->start(QIODevice::ReadOnly); } //restart the process
        //break;
      }
    }
  }
  //qDebug() << " - Final Count:" << stopped << stopping;
  if(stopping || stopped==PROCS.length()){
    //Note about compton: It does not like running multiple sessions under the *same user*
    // (even on different displays). Run a blanket killall on it when closing down so that
    // any other Lumina sessions will automatically restart compton on that specific display
    //QProcess::execute("killall compton");
    QCoreApplication::exit(0);
  }else{
    //Make sure we restart the process as needed
    for(int i=0; i<PROCS.length(); i++){
      if(PROCS[i]->state()==QProcess::NotRunning){
        //runtime/wm processes have special restart rules above
        if(PROCS[i]->objectName()!="runtime" && PROCS[i]->objectName()!="wm"){
          PROCS[i]->start(QIODevice::ReadOnly);
        }
      }
    }
  }
}

void LSession::startProcess(QString ID, QString command, QStringList watchfiles){
  QString dir = QString(getenv("XDG_CONFIG_HOME"))+"/draco-desktop/logs";
  QString display = QString(getenv("DISPLAY")).section(":",1,1);
  if(!QFile::exists(dir)){ QDir tmp(dir); tmp.mkpath(dir); }
  QString logfile = dir+"/"+ID+"_"+display+".log";
  if(QFile::exists(logfile+".old")){ QFile::remove(logfile+".old"); }
  if(QFile::exists(logfile)){ QFile::rename(logfile,logfile+".old"); }

  LProcess *proc = new LProcess(ID, watchfiles);
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->setProcessEnvironment( QProcessEnvironment::systemEnvironment() );
  proc->setStandardOutputFile(logfile);
  proc->setObjectName(ID);
  if(ID=="runtime"){
    //Bypass for a hidden dbus file requirement for Qt itself (Qt 5.5.1+?)
    if(!QFile::exists("/etc/machine-id") && !QFile::exists("/var/db/dbus/machine-id")){
      if(LUtils::isValidBinary("dbus-uuidgen") && LUtils::runCmd("dbus-uuidgen --ensure") ){ } //good - the UUID was created successfully
      else if(LUtils::isValidBinary("dbus-launch")){ command.prepend("dbus-launch --exit-with-session "); }
      else{
        //create a simple DBUS UUID and put it in the universal-fallback location (OS-independent)
        // TO-DO - root vs user level permissions issue?
        qDebug() << "Could not find '/etc/machine-id' or '/var/db/dbus/machine-id': Qt will most likely crash. \nPlease run 'dbus-uuidgen --ensure' with root permissions to generate this file if Lumina does not start properly.";
      }
   }
  }
  proc->start(command, QIODevice::ReadOnly);
  connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(procFinished()) );
  PROCS << proc;
}



void LSession::start()
{
    // First check for a valid installation
    if (!LUtils::isValidBinary(QString("%1-desktop").arg(DESKTOP_APP).toUtf8())) {
        qWarning() << "Desktop manager not found!";
        exit(1);
    }
    if (!LUtils::isValidBinary("powerkit")) {
        qWarning() << "Power manager not found!";
        exit(1);
    }
    if (!LUtils::isValidBinary("qtfm-tray")) {
        qWarning() << "Disk manager not found!";
        exit(1);
    }
    if (!LUtils::isValidBinary(Draco::launcherApp().toUtf8())) {
        qWarning() << "Application Launcher not found";
        exit(1);
    }
    if(!LUtils::isValidBinary(Draco::windowManager().toUtf8())){
        qWarning() << "Window manager not found!";
        exit(1);
    }

    LXDG::setEnvironmentVars();
    setenv("DESKTOP_SESSION", DESKTOP_APP_NAME, 1);
    setenv("XDG_CURRENT_DESKTOP", DESKTOP_APP_NAME, 1);
    setenv("QT_QPA_PLATFORMTHEME","qt5ct", true);
    setenv("QT_NO_GLIB", "1", 1); // Disable the glib event loop within Qt at runtime (performance hit + bugs)
    unsetenv("QT_AUTO_SCREEN_SCALE_FACTOR"); // need exact-pixel measurements (no fake scaling)

    if(LUtils::isValidBinary("xdg-user-dirs-update")){
        // Make sure the XDG user directories are created as needed first
        QProcess::execute("xdg-user-dirs-update");
    }

    // Window Manager First
    startProcess("wm", Draco::windowManagerCmdStart());

    // Desktop Next
    LSingleApplication::removeLocks("draco-desktop");
    startProcess("runtime","draco-desktop");
}
