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
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================

#ifndef DESKTOP_APP_MENU_H
#define DESKTOP_APP_MENU_H

#include <QMenu>
#include <QFileSystemWatcher>
#include <QString>
#include <QList>
#include <QTimer>
#include <QDateTime>
#include <QHash>
#include <QAction>
#include <QSettings>
#include "LuminaXDG.h"

class AppMenu : public QMenu
{
    Q_OBJECT

public:
    AppMenu(QWidget *parent = Q_NULLPTR);
    ~AppMenu();

    QHash<QString, QList<XDGDesktop*> > *currentAppHash();
    QDateTime lastHashUpdate;

private:
    QString appstorelink, controlpanellink;
    QList<QMenu> MLIST;
    XDGDesktopList *sysApps;
    QHash<QString, QList<XDGDesktop*> > APPS;
    void updateAppList(); //completely update the menu lists

private slots:
    void start(); //This is called in a new thread after initialization
    void watcherUpdate();
    void launchApp(QAction *act);

signals:
    void AppMenuUpdated();
};

#endif
