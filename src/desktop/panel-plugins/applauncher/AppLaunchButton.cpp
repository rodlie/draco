/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
# Available under the 3-clause BSD license
# See the LICENSE file for full details
#
*/

//===========================================
//  Lumina-DE source code
//  Copyright (c) 2015, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "AppLaunchButton.h"
#include "LSession.h"

#include <LuminaXDG.h>
#include <LUtils.h>
#include <QInputDialog>
#include <LFileInfo.h>

#include "draco.h"

AppLaunchButtonPlugin::AppLaunchButtonPlugin(QWidget *parent, QString id, bool horizontal) : LPPlugin(parent, id, horizontal){
  button = new QToolButton(this);
    button->setAutoRaise(true);
    button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    appfile = id.section("---",0,0).section("::",1,1);
    if(!QFile::exists(appfile) && appfile.endsWith(".desktop")){
      //might be a relative path - try to find the file
      appfile = LUtils::AppToAbsolute(appfile.section("/",-1) );
    }
    if(!QFile::exists(appfile)){ appfile.clear(); }
    connect(button, SIGNAL(clicked()), this, SLOT(AppClicked()));
    this->layout()->setContentsMargins(0,0,0,0);
    this->layout()->addWidget(button);

  QTimer::singleShot(0,this, SLOT(OrientationChange())); //Update icons/sizes
}

AppLaunchButtonPlugin::~AppLaunchButtonPlugin(){

}

void AppLaunchButtonPlugin::updateButtonVisuals()
{
    QIcon icon;
    QString tooltip = tr("Click to assign an application");
    LFileInfo info(appfile);
    //qDebug() << "UPDATE BUTTON VISUALS" << appfile << info.iconfile() << info.isDesktopFile();
    if (info.iconfile().isEmpty()) {
        if (!info.XDG()->icon.isEmpty()) { icon = LXDG::findIcon(info.XDG()->icon, "application-x-executable"); }
    } else {
        icon = LXDG::findIcon(info.iconfile(), "application-x-executable");
    }
    if (info.isDesktopFile()) {
        if (icon.isNull()) { icon = LXDG::findIcon("application-x-executable"); }
        tooltip = QString(tr("Launch %1")).arg(info.XDG()->name);
    } else if (info.exists()) {
        if (icon.isNull()) { icon = LXDG::findIcon("application-x-executable"); }
        tooltip = QString(tr("Open %1")).arg(appfile.section("/",-1));
    } else {
        icon =  LXDG::findIcon("task-attention", "application-x-executable");
    }
    button->setIcon( icon );
    button->setToolTip(tooltip);
}

// ========================
//    PRIVATE FUNCTIONS
// ========================
void AppLaunchButtonPlugin::AppClicked(){
  if(appfile.isEmpty()){
    //No App File selected
    QList<XDGDesktop*> apps = LSession::handle()->applicationMenu()->currentAppHash()->value("All");
    QStringList names;
    for(int i=0; i<apps.length(); i++){ names << apps[i]->name; }
    bool ok = false;
    QString app = QInputDialog::getItem(this, tr("Select Application"), tr("Name:"), names, 0, false, &ok);
    if(!ok || names.indexOf(app)<0){ return; } //cancelled
    appfile = apps[ names.indexOf(app) ]->filePath;
    //Still need to find a way to set this value persistently
    // --- perhaps replace the plugin in the desktop settings file with the new path?
    // --- "applauncher::broken---<something>"  -> "applauncher::fixed---<something>" ?
    QTimer::singleShot(0,this, SLOT(updateButtonVisuals()));
  }else{
    LSession::LaunchApplication(QString("%1 \"%2\"").arg(Draco::launcherApp()).arg(appfile));
  }
}
