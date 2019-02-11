//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012-2016, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef LUMINA_LIBRARY_DESKTOP_UTILS_H
#define LUMINA_LIBRARY_DESKTOP_UTILS_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QSettings>

//Other classes needed
#include <LUtils.h>
#include <LuminaXDG.h>
#include <LuminaOS.h>

class LDesktopUtils{
public:




	//Various functions for the favorites sub-system
	// Formatting Note: "<name>::::[dir/app/<mimetype>]::::<path>"
	//   the <name> field might not be used for "app" flagged entries
	static QStringList listFavorites();
	static bool saveFavorites(QStringList);
	static bool isFavorite(QString path);
	static bool addFavorite(QString path, QString name = "");
	static void removeFavorite(QString path);
	static void upgradeFavorites(int fromoldversionnumber);



	//Migrating desktop settings from one ID to another
	static void MigrateDesktopSettings(QSettings *settings, QString fromID, QString toID);

};

#endif
