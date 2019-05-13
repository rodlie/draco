/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com>
# All rights reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
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
//  Lumina Desktop Source Code
//  Copyright (c) 2016, Ken Moore
//  Available under the 3-clause BSD license
//===========================================
#include "page_session_options.h"
#include "ui_page_session_options.h"
#include "draco.h"

#include <QSettings>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QDebug>
#include "LuminaXDG.h"
#include "LUtils.h"
#include <QFileDialog>
#include <QScreen>
#include <QTimer>

//==========
//    PUBLIC
//==========
page_session_options::page_session_options(QWidget *parent) : PageWidget(parent), ui(new Ui::page_session_options()){
  ui->setupUi(this);
  loading = false;
  //Display formats for panel clock
  ui->combo_session_datetimeorder->clear();
  ui->combo_session_datetimeorder->addItem( tr("Time (Date as tooltip)"), "timeonly");
  ui->combo_session_datetimeorder->addItem( tr("Date (Time as tooltip)"), "dateonly");
  ui->combo_session_datetimeorder->addItem( tr("Time first then Date"), "timedate");
  ui->combo_session_datetimeorder->addItem( tr("Date first then Time"), "datetime");

  connect(ui->tool_help_time, SIGNAL(clicked()), this, SLOT(sessionShowTimeCodes()) );
  connect(ui->tool_help_date, SIGNAL(clicked()), this, SLOT(sessionShowDateCodes()) );
  connect(ui->line_session_time, SIGNAL(textChanged(QString)), this, SLOT(sessionLoadTimeSample()) );
  connect(ui->line_session_date, SIGNAL(textChanged(QString)), this, SLOT(sessionLoadDateSample()) );
  connect(ui->combo_session_datetimeorder, SIGNAL(currentIndexChanged(int)), this, SLOT(settingChanged()) );
  connect(ui->check_autoapplinks, SIGNAL(toggled(bool)), this, SLOT(settingChanged()) );
  connect(ui->check_desktop_autolaunchers, SIGNAL(clicked()), this, SLOT(settingChanged()) );
  connect(ui->check_media_icons, SIGNAL(clicked()), this, SLOT(settingChanged()) );

  connect(ui->check_automount_storage, SIGNAL(clicked()), this, SLOT(settingChanged()) );
  connect(ui->check_autoplay_storage_cd, SIGNAL(clicked()), this, SLOT(settingChanged()) );
  connect(ui->check_show_storage_notify, SIGNAL(clicked()), this, SLOT(settingChanged()) );
  connect(ui->check_autoplay_storage_dvd, SIGNAL(clicked()), this, SLOT(settingChanged()) );

  updateIcons();
}

page_session_options::~page_session_options(){

}

//================
//    PUBLIC SLOTS
//================
void page_session_options::SaveSettings()
{
    QSettings sessionsettings(DESKTOP_APP, DE_SESSION_SETTINGS);
    sessionsettings.setValue("AutomaticDesktopAppLinks",  ui->check_autoapplinks->isChecked());
    sessionsettings.setValue("TimeFormat", ui->line_session_time->text());
    sessionsettings.setValue("DateFormat", ui->line_session_date->text());
    sessionsettings.setValue("DateTimeOrder", ui->combo_session_datetimeorder->currentData().toString());

    sessionsettings.setValue("trayNotify", ui->check_show_storage_notify->isChecked());
    sessionsettings.setValue("trayAutoMount", ui->check_automount_storage->isChecked());
    sessionsettings.setValue("autoPlayAudioCD", ui->check_autoplay_storage_cd->isChecked());
    sessionsettings.setValue("autoPlayDVD", ui->check_autoplay_storage_dvd->isChecked());


    QSettings desktopsettings(DESKTOP_APP, DE_DESKTOP_SETTINGS);
    QString screenID = QApplication::screens().at(cscreen)->name();
    QString DPrefix = "desktop-"+screenID+"/";
    desktopsettings.setValue(DPrefix+"generateDesktopIcons", ui->check_desktop_autolaunchers->isChecked() );
    desktopsettings.setValue(DPrefix+"generateMediaIcons", ui->check_media_icons->isChecked() );

    emit HasPendingChanges(false);
}

void page_session_options::LoadSettings(int screennum)
{
    if (screennum>=0){ cscreen = screennum; }

    emit HasPendingChanges(false);
    emit ChangePageTitle( tr("Desktop Settings") );
    loading = true;

    QSettings sessionsettings(DESKTOP_APP, DE_SESSION_SETTINGS);
    ui->check_autoapplinks->setChecked( sessionsettings.value("AutomaticDesktopAppLinks",true).toBool() );
    ui->line_session_time->setText( sessionsettings.value("TimeFormat","").toString() );
    ui->line_session_date->setText( sessionsettings.value("DateFormat","").toString() );
    int index = ui->combo_session_datetimeorder->findData( sessionsettings.value("DateTimeOrder","timeonly").toString() );
    ui->combo_session_datetimeorder->setCurrentIndex(index);

    ui->check_automount_storage->setChecked(sessionsettings.value("trayAutoMount", false).toBool());
    ui->check_autoplay_storage_cd->setChecked(sessionsettings.value("autoPlayAudioCD", false).toBool());
    ui->check_show_storage_notify->setChecked(sessionsettings.value("trayNotify", true).toBool());
    ui->check_autoplay_storage_dvd->setChecked(sessionsettings.value("autoPlayDVD", false).toBool());

    QSettings desktopsettings(DESKTOP_APP, DE_DESKTOP_SETTINGS);
    QString screenID = QApplication::screens().at(cscreen)->name();
    QString DPrefix = "desktop-"+screenID+"/";
    ui->check_desktop_autolaunchers->setChecked(desktopsettings.value(DPrefix+"generateDesktopIcons", true).toBool() );
    ui->check_media_icons->setChecked(desktopsettings.value(DPrefix+"generateMediaIcons", true).toBool() );

    sessionLoadTimeSample();
    sessionLoadDateSample();
    QApplication::processEvents(); // throw away any interaction events from loading
    loading = false;
}

void page_session_options::updateIcons()
{
    ui->tool_help_time->setIcon( LXDG::findIcon("help-about","") );
    ui->tool_help_date->setIcon( LXDG::findIcon("help-about","") );
}

//=================
//         PRIVATE
//=================

//void page_session_options::FindWindowManagerOptions(){
  /*  // Try to find all available window managers and add them to drop-down box.
    ui->mywindowmanager->clear();
    ui->mywindowmanager->addItem("Default (fluxbox)"); // make sure there is a default at position 0
    QStringList wms; wms << "kwin" << "openbox" << "i3";
    //NOTE: the "fluxbox" window manager is already assumed by the selection of the "Lumina" default
    wms.sort();
    for(int i=0; i<wms.length(); i++){
      QString path = wms[i];
      if(LUtils::isValidBinary(path)){ //This will change the "path" variable to the full path if it exists
        ui->mywindowmanager->addItem(wms[i], path);
      }
    }*/
    /*if (QFileInfo::exists("/usr/bin/fluxbox"))
        ui->mywindowmanager->addItem("/usr/bin/fluxbox");
    else if (QFileInfo::exists("/usr/local/bin/fluxbox"))
        ui->mywindowmanager->addItem("/usr/local/bin/fluxbox");
    if (QFileInfo::exists("/usr/bin/kwin"))
        ui->mywindowmanager->addItem("/usr/bin/kwin");
    else if (QFileInfo::exists("/usr/local/bin/kwin"))
        ui->mywindowmanager->addItem("/usr/local/bin/kwin");
    if (QFileInfo::exists("/usr/bin/openbox"))
        ui->mywindowmanager->addItem("/usr/bin/openbox");
    else if (QFileInfo::exists("/usr/local/bin/openbox"))
        ui->mywindowmanager->addItem("/usr/local/bin/openbox");*/
//}

bool page_session_options::verifySettingsReset(){
  bool ok =(QMessageBox::Yes ==  QMessageBox::warning(this, tr("Verify Settings Reset"), tr("Are you sure you want to reset your desktop settings? This change cannot be reversed!"), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel) );
  return ok;
}

//=================
//    PRIVATE SLOTS
//=================
/*void page_session_options::sessionChangeUserIcon(){
  //Prompt for a new image file
  QStringList imgformats;
  QList<QByteArray> fmts = QImageReader::supportedImageFormats();
  for(int i=0; i<fmts.length(); i++){
    imgformats << "*."+QString(fmts[i]);
  }
  QString filepath = QFileDialog::getOpenFileName(this, tr("Select an image"), QDir::homePath(), \
				tr("Images")+" ("+imgformats.join(" ")+")");
  if(filepath.isEmpty()){
    //User cancelled the operation
    if(QFile::exists(QDir::homePath()+"/.loginIcon.png")){
      if(QMessageBox::Yes == QMessageBox::question(this,tr("Reset User Image"), tr("Would you like to reset the user image to the system default?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) ){
	//QFile::remove(QDir::homePath()+"/.loginIcon.png");
    //ui->push_session_setUserIcon->setWhatsThis("reset");
      }else{
	return;
      }
    }
  }else{
    ui->push_session_setUserIcon->setWhatsThis(filepath);
  }
  //Now re-load the icon in the UI
  QString path = ui->push_session_setUserIcon->whatsThis();
  if(path.isEmpty()){ path = QDir::homePath()+"/.loginIcon.png"; }
  if(path=="reset"){ path.clear(); }
  ui->push_session_setUserIcon->setIcon( LXDG::findIcon(path, "user-identity") );
  settingChanged();
}

void page_session_options::sessionResetSys(){
  if( !verifySettingsReset() ){ return; } //cancelled
  //LDesktopUtils::LoadSystemDefaults();
  QTimer::singleShot(500,this, SLOT(LoadSettings()) );
}

void page_session_options::sessionResetLumina(){
  if( !verifySettingsReset() ){ return; } //cancelled
  //LDesktopUtils::LoadSystemDefaults(true); //skip OS customizations
  QTimer::singleShot(500,this, SLOT(LoadSettings()) );
}*/

void page_session_options::sessionLoadTimeSample(){
  if(ui->line_session_time->text().simplified().isEmpty()){
    ui->label_session_timesample->setText( QTime::currentTime().toString(Qt::DefaultLocaleShortDate) );
  }else{
    ui->label_session_timesample->setText( QTime::currentTime().toString( ui->line_session_time->text() ) );
  }
  settingChanged();
}

void page_session_options::sessionShowTimeCodes(){
  QStringList msg;
    msg << tr("Valid Time Codes:") << "\n";
    msg << QString(tr("%1: Hour without leading zero (1)")).arg("h");
    msg << QString(tr("%1: Hour with leading zero (01)")).arg("hh");
    msg << QString(tr("%1: Minutes without leading zero (2)")).arg("m");
    msg << QString(tr("%1: Minutes with leading zero (02)")).arg("mm");
    msg << QString(tr("%1: Seconds without leading zero (3)")).arg("s");
    msg << QString(tr("%1: Seconds with leading zero (03)")).arg("ss");
    msg << QString(tr("%1: AM/PM (12-hour) clock (upper or lower case)")).arg("A or a");
    msg << QString(tr("%1: Timezone")).arg("t");
  QMessageBox::information(this, tr("Time Codes"), msg.join("\n") );
}

void page_session_options::sessionLoadDateSample(){
  if(ui->line_session_date->text().simplified().isEmpty()){
    ui->label_session_datesample->setText( QDate::currentDate().toString(Qt::DefaultLocaleShortDate) );
  }else{
    ui->label_session_datesample->setText( QDate::currentDate().toString( ui->line_session_date->text() ) );
  }
  settingChanged();
}

void page_session_options::sessionShowDateCodes(){
  QStringList msg;
    msg << tr("Valid Date Codes:") << "\n";
    msg << QString(tr("%1: Numeric day without a leading zero (1)")).arg("d");
    msg << QString(tr("%1: Numeric day with leading zero (01)")).arg("dd");
    msg << QString(tr("%1: Day as abbreviation (localized)")).arg("ddd");
    msg << QString(tr("%1: Day as full name (localized)")).arg("dddd");
    msg << QString(tr("%1: Numeric month without leading zero (2)")).arg("M");
    msg << QString(tr("%1: Numeric month with leading zero (02)")).arg("MM");
    msg << QString(tr("%1: Month as abbreviation (localized)")).arg("MMM");
    msg << QString(tr("%1: Month as full name (localized)")).arg("MMMM");
    msg << QString(tr("%1: Year as 2-digit number (15)")).arg("yy");
    msg << QString(tr("%1: Year as 4-digit number (2015)")).arg("yyyy");
    msg << tr("Text may be contained within single-quotes to ignore replacements");
  QMessageBox::information(this, tr("Date Codes"), msg.join("\n") );
}
