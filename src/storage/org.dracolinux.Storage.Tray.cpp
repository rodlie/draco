#include "org.dracolinux.Storage.Tray.h"
#include "draco.h"
#include "LuminaXDG.h"
#include "XDGMime.h"
#include "LUtils.h"

#include <QIcon>
#include <QProcess>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QApplication>
#include <QDir>

SysTray::SysTray(QObject *parent)
    : QObject(parent)
    , disktray(nullptr)
    , menu(nullptr)
    , man(nullptr)
    , showNotifications(true)
    //, mimeUtilsPtr(Q_NULLPTR)
    , autoMount(false)
{
    // set icon theme
    //Common::setupIconTheme(qApp->applicationFilePath());

    menu = new QMenu();

    disktray = new QSystemTrayIcon(QIcon::fromTheme("drive-removable-media"),
                                   this);
    disktray->setToolTip(tr("Removable Devices"));

    connect(disktray,
            SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,
            SLOT(disktrayActivated(QSystemTrayIcon::ActivationReason)));
    connect(disktray,
            SIGNAL(messageClicked()),
            this,
            SLOT(handleDisktrayMessageClicked()));

    man = new Disks(this);
    connect(man,
            SIGNAL(updatedDevices()),
            this,
            SLOT(generateContextMenu()));
    connect(man,
            SIGNAL(deviceErrorMessage(QString, QString)),
            this,
            SLOT(handleDeviceError(QString, QString)));
    connect(man,
            SIGNAL(mediaChanged(QString, bool)),
            this,
            SLOT(handleDeviceMediaChanged(QString, bool)));
    connect(man,
            SIGNAL(mountpointChanged(QString, QString)),
            this,
            SLOT(handleDeviceMountpointChanged(QString, QString)));
    connect(man,
            SIGNAL(foundNewDevice(QString)),
            this,
            SLOT(handleFoundNewDevice(QString)));

    // load settings
    loadSettings();

    generateContextMenu();
    QTimer::singleShot(10000,
                       this,
                       SLOT(generateContextMenu())); // slow start to make sure udisks etc are running
}

void SysTray::loadSettings()
{
    showNotifications = Draco::readSetting(Draco::storageSettingsFile(), "trayNotify", true).toBool();
    autoMount = Draco::readSetting(Draco::storageSettingsFile(), "trayAutoMount", false).toBool();
}

void SysTray::generateContextMenu()
{
    for(int i=0;i<menu->actions().size();i++) {
        menu->actions().at(i)->disconnect();
        delete menu->actions().at(i);
    }
    menu->clear();

    QMapIterator<QString, StorageDevice*> device(man->devices);
    while (device.hasNext()) {
        device.next();

        if ((device.value()->isOptical &&
             !device.value()->hasMedia) ||
             (!device.value()->isOptical && !device.value()->isRemovable) ||
             (!device.value()->isOptical && !device.value()->hasPartition))
        {
            continue;
        }

        QAction *deviceAction = new QAction(this);
        deviceAction->setData(device.key());
        deviceAction->setText(QString("%1 (%2)")
                              .arg(device.value()->name)
                              .arg(device.value()->dev));

        connect(deviceAction,
                SIGNAL(triggered(bool)),
                this,
                SLOT(handleContextMenuAction()));
        menu->addAction(deviceAction);

        if (device.value()->mountpoint.isEmpty()) {
            deviceAction->setIcon(QIcon::fromTheme(device.value()->isOptical?"drive-optical":"drive-removable-media"));
            bool hasAudio = device.value()->opticalAudioTracks>0?true:false;
            bool hasData = device.value()->opticalDataTracks>0?true:false;
            if (device.value()->isBlankDisc ||
                (hasAudio && !hasData)) { deviceAction->setIcon(QIcon::fromTheme("media-eject")); }
        } else { deviceAction->setIcon(QIcon::fromTheme("media-eject")); }
    }

    qDebug() << menu->actions();
    handleShowHideDisktray();
}

void SysTray::disktrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
    case QSystemTrayIcon::Context:
    case QSystemTrayIcon::Trigger:
        if (menu->actions().size()>0) { menu->popup(QCursor::pos()); }
        break;
    default:;
    }
}

void SysTray::handleDisktrayMessageClicked()
{
    handleShowHideDisktray();
}

void SysTray::showMessage(QString title,
                          QString message)
{
    if (!disktray->isSystemTrayAvailable() || !showNotifications) { return; }
    if (!disktray->isVisible()) { disktray->show(); }
    disktray->showMessage(title, message);
    QTimer::singleShot(10000,
                       this,
                       SLOT(handleShowHideDisktray()));
}

void SysTray::handleContextMenuAction()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action==nullptr) { return; }
    QString path = action->data().toString();
    if (path.isEmpty()) { return; }
    if (!man->devices.contains(path)) { return; }

    if (man->devices[path]->mountpoint.isEmpty()) { // mount
        if (man->devices[path]->isOptical &&
                (man->devices[path]->isBlankDisc ||
                 man->devices[path]->opticalDataTracks==0)) { man->devices[path]->eject(); }
        else { man->devices[path]->mount(); }
    } else { // unmount
        man->devices[path]->unmount();
    }
    generateContextMenu();
}

void SysTray::handleDeviceError(QString path,
                                QString error)
{
    if (!man->devices.contains(path)) { return; }
    showMessage(QObject::tr("Error for device %1").arg(man->devices[path]->name),
                error);
}

void SysTray::handleDeviceMediaChanged(QString path,
                                       bool media)
{
    if (!man->devices.contains(path)) { return; }
    qDebug() << "handle device media changed" << path << media;
    generateContextMenu();
    if (man->devices[path]->isOptical && media) {
        bool isData = man->devices[path]->opticalDataTracks>0?true:false;
        bool isAudio = man->devices[path]->opticalAudioTracks>0?true:false;
        QString opticalType;
        if (isData&&isAudio) { opticalType = QObject::tr("data+audio"); }
        else if (isData) { opticalType = QObject::tr("data"); }
        else if (isAudio) { opticalType = QObject::tr("audio"); }
        showMessage(QObject::tr("%1 has media").arg(man->devices[path]->name),
                    QObject::tr("Detected %1 media in %2")
                    .arg(opticalType)
                    .arg(man->devices[path]->name));

        bool openMedia = false;
        // auto mount if enabled
        if (Draco::readSetting(Draco::storageSettingsFile(), "trayAutoMount", false).toBool() && isData) {
            qDebug() << "auto mount optical";
            man->devices[path]->mount();
            openMedia = true;
        }
        // auto play CD if enabled
        if (Draco::readSetting(Draco::storageSettingsFile(), "autoPlayAudioCD", false).toBool() && isAudio) {
            openMedia = false;
            /*QStringList apps = mimeUtilsPtr->getDefault("x-content/audio-cdda");
            QString desktop = Common::findApplication(qApp->applicationFilePath(), apps.at(0));
            if (desktop.isEmpty()) { return; }
            DesktopFile df(desktop);*/
            XDGDesktop desktop(XDGMime::findDefaultAppForMime("x-content/audio-cdda"));
            QString app = desktop.getDesktopExec().split(" ").takeFirst();
            if (app.isEmpty()) { return; }
            QProcess::startDetached(QString("%1 cdda://%2")
                                    .arg(app)
                                    .arg(man->devices[path]->mountpoint));
        }
        // auto play DVD if enabled
        if (Draco::readSetting(Draco::storageSettingsFile(), "autoPlayDVD", false).toBool() && isData) {
            if (man->devices[path]->mountpoint.isEmpty()) {
                man->devices[path]->mount();
            }
            openMedia = false;
            QDir tsVideo(QString("%1/video_ts")
                         .arg(man->devices[path]->mountpoint));
            QDir tsAudio(QString("%1/audio_ts")
                         .arg(man->devices[path]->mountpoint));
            if (!tsVideo.exists()) {
                tsVideo.setPath(QString("%1/VIDEO_TS")
                                .arg(man->devices[path]->mountpoint));
            }
            if (!tsAudio.exists()) {
                tsAudio.setPath(QString("%1/AUDIO_TS")
                                .arg(man->devices[path]->mountpoint));
            }
            QString mime;
            if (tsVideo.exists()) {
                mime  = XDGMime::findDefaultAppForMime("x-content/video-dvd");
                //QStringList apps = mimeUtilsPtr->getDefault("x-content/video-dvd");
                //qDebug() << "video dvd apps" << apps;
                //desktop = Common::findApplication(qApp->applicationFilePath(),
                  //                                apps.at(0));

            } else if (!tsVideo.exists() && tsAudio.exists()) {
                mime  = XDGMime::findDefaultAppForMime("x-content/audio-dvd");
                //QStringList apps = mimeUtilsPtr->getDefault("x-content/audio-dvd");
                //qDebug() << "audio dvd apps" << apps;
                //desktop = Common::findApplication(qApp->applicationFilePath(),
                  //                                apps.at(0));
            }
            if (mime.isEmpty()) { return; }
            //DesktopFile df(desktop);
            XDGDesktop desktop(mime);
            QString app = desktop.getDesktopExec().split(" ").takeFirst();
            if (app.isEmpty()) { return; }
            if (app.endsWith("mplayer")) {
                // workaround for mplayer
                QProcess::startDetached(QString("%1 dvd:// -dvd-device /dev/%2")
                                        .arg(app)
                                        .arg(path.split("/").takeLast()));
            } else {
                QProcess::startDetached(QString("%1 dvd://%2")
                                        .arg(app).arg(man->devices[path]->mountpoint));
            }
        }
        if (openMedia) {
            openMountpoint(man->devices[path]->mountpoint);
        }
        generateContextMenu();
    }
}

void SysTray::handleDeviceMountpointChanged(QString path,
                                            QString mountpoint)
{
    if (!man->devices.contains(path)) { return; }
    generateContextMenu();
    if (!man->devices[path]->isRemovable || man->devices[path]->isOptical) { return; }
    if (mountpoint.isEmpty()) {
        if (!man->devices[path]->isOptical) {
            // TODO
            showMessage(QObject::tr("%1 removed").arg(man->devices[path]->name),
                        QObject::tr("%1 was safely removed.").arg(man->devices[path]->name));
        }
    } else { openMountpoint(mountpoint); }
}

void SysTray::openMountpoint(QString mountpoint)
{
    if (mountpoint.isEmpty() || !LUtils::isValidBinary(XDG_OPEN)) { return; }
    qDebug() << "open mountpoint" << mountpoint;
    QProcess::startDetached(QString("%1 \"%2\"")
                            .arg(LUtils::isValidBinaryWithPath(XDG_OPEN))
                            .arg(mountpoint));
}

void SysTray::handleFoundNewDevice(QString path)
{
    if (!man->devices.contains(path)) { return; }
    if (man->devices[path]->isOptical) { return; }
    loadSettings();

    showMessage(QObject::tr("Found %1").arg(man->devices[path]->name),
                QObject::tr("Found a new device (%1)").arg(man->devices[path]->dev));

    // auto mount if enabled
    if (!autoMount) { return; }
    if (!man->devices[path]->mountpoint.isEmpty()) { return; }
    man->devices[path]->mount();
}

void SysTray::handleShowHideDisktray()
{
    if (menu->actions().size()==0) {
        if (disktray->isVisible()) { disktray->hide(); }
    } else {
        if (!disktray->isVisible() && disktray->isSystemTrayAvailable()) { disktray->show(); }
    }
}
