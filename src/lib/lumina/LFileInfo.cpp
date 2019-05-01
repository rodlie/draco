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
//  Copyright (c) 2013-2017, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LFileInfo.h"
#include <LUtils.h>
#include <QRegExp>
#include <unistd.h>

LFileInfo::LFileInfo() : QFileInfo(){
  desk = 0;
  c_uid = -1;
}

LFileInfo::LFileInfo(QString filepath) : QFileInfo(){ //overloaded contructor
  desk = 0;
  this->setFile(filepath);
  loadExtraInfo();
}

LFileInfo::LFileInfo(QFileInfo info) : QFileInfo(){ //overloaded contructor
  desk = 0;
  this->swap(info); //use the given QFileInfo without re-loading it
  loadExtraInfo();
}
LFileInfo::~LFileInfo(){
  if(desk!=0){ desk->deleteLater(); }
}

//Need some extra information not usually available by a QFileInfo
void LFileInfo::loadExtraInfo(){
  if(desk!=0){ desk->deleteLater(); }
  desk = 0;
  c_uid = geteuid();
  //Now load the extra information
  QString abspath = this->absoluteFilePath();
  if( this->isDir() ){
    mime = "inode/directory";
    //Special directory icons
    QString name = this->fileName().toLower();
    if(name=="desktop" || abspath == LUtils::standardDirectory(LUtils::Desktop, false) ){ iconList << "user-desktop"; }
    else if(name=="tmp"){ iconList << "folder-temp"; }
    else if(name=="video" || name=="videos" || abspath == LUtils::standardDirectory(LUtils::Videos, false)){ iconList << "folder-video" << "camera-photo-film" ; }
    else if(name=="music" || name=="audio" || abspath == LUtils::standardDirectory(LUtils::Music, false)){ iconList << "folder-sound" << "media-playlist-audio"; }
    else if(name=="projects" || name=="devel"){ iconList << "folder-development"; }
    else if(name=="notes"){ iconList << "folder-txt" << "note-multiple-outline" << "note-multiple"; }
    else if(name=="downloads" || abspath == LUtils::standardDirectory(LUtils::Downloads, false)){ iconList << "folder-downloads" << "folder-download"; }
    else if(name=="documents" || abspath == LUtils::standardDirectory(LUtils::Documents, false)){ iconList << "folder-documents"; }
    else if(name=="images" || name=="pictures" || abspath == LUtils::standardDirectory(LUtils::Pictures, false)){ iconList << "folder-image"; }
    else if(this->absoluteFilePath().startsWith("/net/")){ iconList << "folder-remote"; }
    else if( !this->isReadable() ){ iconList << "folder-locked"<< "folder-lock"; }
    iconList << "folder";
  }else if( this->suffix()=="desktop"){
    mime = "application/x-desktop";
    desk = new XDGDesktop(this->absoluteFilePath(), 0);
    if(desk->type!=XDGDesktop::BAD){
      //use the specific desktop file info (if possible)
      if(!desk->icon.isEmpty()){ iconList << desk->icon; }
    }
  }else{
    //Generic file, just determine the mimetype
    mime = LXDG::findAppMimeForFile(this->fileName());
  }
  //check the mimetype for an icon as needed
  QString tmp = mime;
    tmp.replace("/","-");
    iconList << tmp;
  if(this->isExecutable()){
    iconList << "application-x-executable";
  }
}

//Functions for accessing the extra information
// -- Return the mimetype for the file
QString LFileInfo::mimetype(){
  if(mime=="inode/directory"){ return ""; }
  else{ return mime; }
}

// -- Return the icon to use for this file
QString LFileInfo::iconfile(){
  //Go through the icon list and find the first one that exists in the current theme
  for(int i=0; i<iconList.length(); i++){
    if( QIcon::hasThemeIcon(iconList[i])){ return iconList[i]; }
    else if (desk!=Q_NULLPTR && iconList[i] == desk->icon) {
      // See if this icon is in the old "pixmaps" icon directory
      QDir dir("/usr/local/share/pixmaps");
      QStringList matches = dir.entryList(QStringList() << desk->icon+".png" << desk->icon+".jpg");
      if(!matches.isEmpty()){
        return dir.absoluteFilePath(matches.first());
      }
    }
  }
  return ""; //Fall back to nothing
}

QIcon LFileInfo::icon(){
  QString ifile = iconfile();
  if(ifile.startsWith("/")){ return QIcon(ifile); }
  else if(ifile.isEmpty()){ return QIcon::fromTheme("unknown"); }
  else{ return QIcon::fromTheme(ifile); }
}

// -- Check if this is an XDG desktop file
bool LFileInfo::isDesktopFile(){
  if(desk==0){ return false; }
  return (!desk->filePath.isEmpty());
}

// -- Allow access to the XDG desktop data structure
XDGDesktop* LFileInfo::XDG(){
  return desk;
}

// -- Check if this is a readable video file (for thumbnail support)
bool LFileInfo::isVideo(){
  if(!mime.startsWith("video/")){ return false; }
  //Check the hardcoded list of known supported video formats to see if the thumbnail can be generated
  return ( !LUtils::videoExtensions().filter(this->suffix().toLower()).isEmpty() );
}

// -- Check if this is a readable image file
bool LFileInfo::isImage(){
  if(!mime.startsWith("image/")){ return false; } //quick return for non-image files
  //Check the Qt subsystems to see if this image file can be read
  return ( !LUtils::imageExtensions().filter(this->suffix().toLower()).isEmpty() );
}

bool LFileInfo::isAVFile(){
  return (mime.startsWith("audio/") || mime.startsWith("video/") );
}

