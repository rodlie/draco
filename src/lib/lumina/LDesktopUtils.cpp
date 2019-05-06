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
//  Copyright (c) 2012-2016, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LDesktopUtils.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QScreen>
#include <QSettings>

//#include "LuminaThemes.h"




QStringList LDesktopUtils::listFavorites(){
  //static QDateTime lastRead;
  QStringList fav;
  //QDateTime cur = QDateTime::currentDateTime();
  //if(lastRead.isNull() || fav.isEmpty() || lastRead<QFileInfo( QString(getenv("XDG_CONFIG_HOME"))+"/lumina-desktop/favorites.list").lastModified()){
    fav = LUtils::readFile(QString(getenv("XDG_CONFIG_HOME"))+QString("/%1/favorites.list").arg(DESKTOP_APP));
    fav.removeAll(""); //remove any empty lines
    fav.removeDuplicates();
    //lastRead = cur;
  //}
  return fav;
}

bool LDesktopUtils::saveFavorites(QStringList list){
  list.removeDuplicates();
  //qDebug() << "Save Favorites:" << list;
  bool ok = LUtils::writeFile(QString(getenv("XDG_CONFIG_HOME"))+QString("/%1/favorites.list").arg(DESKTOP_APP), list, true);
  //if(ok){ fav = list; } //also save internally in case of rapid write/read of the file
  return ok;
}

bool LDesktopUtils::isFavorite(QString path){
  QStringList fav = LDesktopUtils::listFavorites();
  for(int i=0; i<fav.length(); i++){
    if(fav[i].endsWith("::::"+path)){ return true; }
  }
  return false;
}

bool LDesktopUtils::addFavorite(QString path, QString name){
  //Generate the type of favorite this is
  QFileInfo info(path);
  QString type;
  if(info.isDir()){ type="dir"; }
  else if(info.suffix()=="desktop"){ type="app"; }
  else{ type = LXDG::findAppMimeForFile(path); }
  //Assign a name if none given
  if(name.isEmpty()){ name = info.fileName(); }
  //qDebug() << "Add Favorite:" << path << type << name;
  //Now add it to the list
  QStringList favs = LDesktopUtils::listFavorites();
  //qDebug() << "Current Favorites:" << favs;
  bool found = false;
  for(int i=0; i<favs.length(); i++){
    if(favs[i].endsWith("::::"+path)){ favs[i] = name+"::::"+type+"::::"+path; found = true; }
  }
  if(!found){ favs << name+"::::"+type+"::::"+path; }
  return LDesktopUtils::saveFavorites(favs);
}

void LDesktopUtils::removeFavorite(QString path){
  QStringList fav = LDesktopUtils::listFavorites();
  bool changed = false;
  for(int i=0; i<fav.length(); i++){
    if(fav[i].endsWith("::::"+path)){ fav.removeAt(i); i--; changed=true;}
  }
  if(changed){ LDesktopUtils::saveFavorites(fav); }
}

void LDesktopUtils::upgradeFavorites(int){ //fromoldversionnumber
  //NOTE: Version number syntax: <major>*1000000 + <minor>*1000 + <revision>
  // Example: 1.2.3 -> 1002003
}






void LDesktopUtils::MigrateDesktopSettings(QSettings *settings, QString fromID, QString toID){
  //desktop-ID
  QStringList keys = settings->allKeys();
  QStringList filter = keys.filter("desktop-"+fromID+"/");
  for(int i=0; i<filter.length(); i++){
    settings->setValue("desktop-"+toID+"/"+filter[i].section("/",1,-1), settings->value(filter[i]));
    settings->remove(filter[i]);
  }
  //panel_ID.[number]
  filter = keys.filter("panel_"+fromID+".");
  for(int i=0; i<filter.length(); i++){
    settings->setValue("panel_"+toID+"."+ filter[i].section("/",0,0).section(".",-1)+"/"+filter[i].section("/",1,-1), settings->value(filter[i]));
    settings->remove(filter[i]);
  }
}
