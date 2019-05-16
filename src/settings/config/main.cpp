#include <QTranslator>

#include <QApplication>
#include <QDebug>
#include <QFile>


#include "mainWindow.h"
//#include <LuminaOS.h>
#include <LUtils.h>
//#include <LuminaThemes.h>
//#include <LuminaSingleApplication.h>
#include <LuminaXDG.h>
#include "draco.h"
#include <QDBusConnection>
#include <QDBusInterface>

XDGDesktopList *APPSLIST = nullptr;

int main(int argc, char ** argv)
{
    // Check for dbus
    if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning("Cannot connect to the D-Bus session bus.");
        return 1;
    }

    // Check for running settings
    QDBusInterface session(Draco::settingsSessionName(),
                           Draco::settingsSessionPath(),
                           Draco::settingsSessionName(),
                           QDBusConnection::sessionBus());
    if (session.isValid()) {
        qWarning("A settings session is already running");
        return 1;
    }

    // Register settings session
    if (!QDBusConnection::sessionBus().registerService(Draco::settingsSessionName())) {
        qWarning() << QDBusConnection::sessionBus().lastError().message();
        return 1;
    }

    //LTHEME::LoadCustomEnvSettings();
    QApplication a(argc, argv); //loads translations inside constructor
    //if(!a.isPrimaryProcess()){ return 0; }
    //LuminaThemeEngine theme(&a);
    QStringList args;
    for(int i=1; i<argc; i++){ args << QString(argv[i]); }
    mainWindow w;
    //QObject::connect(&a, SIGNAL(InputsAvailable(QStringList)), &w, SLOT(slotSingleInstance(QStringList)) );
    w.slotSingleInstance(args);
    w.show();

    int retCode = a.exec();
    return retCode;
}
