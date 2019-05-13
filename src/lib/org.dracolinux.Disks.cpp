/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
#
*/

#include "org.dracolinux.Disks.h"
#include "org.freedesktop.UDisks2.h"
#include <QDebug>
#include <QTimer>
#include <QDBusConnection>

StorageDevice::StorageDevice(const QString block, QObject *parent)
    : QObject(parent)
    , path(block)
    , isOptical(false)
    , isRemovable(false)
    , hasMedia(false)
    , opticalDataTracks(0)
    , opticalAudioTracks(0)
    , isBlankDisc(false)
    , hasPartition(false)
    , dbus(0)
{
    QDBusConnection system = QDBusConnection::systemBus();
    dbus = new QDBusInterface(DBUS_SERVICE, path, QString("%1.Block").arg(DBUS_SERVICE), system, parent);
    system.connect(dbus->service(), dbus->path(), DBUS_PROPERTIES, "PropertiesChanged", this, SLOT(handlePropertiesChanged(QString,QMap<QString,QVariant>)));
    updateDeviceProperties();
}

void StorageDevice::mount()
{
    qDebug() << "mount?" << path << mountpoint;
    if (!dbus->isValid() || !mountpoint.isEmpty()) { return; }
    QString reply;
    if (isOptical) {
        reply = uDisks2::mountOptical(path);
        qDebug() << "optical=" << uDisks2::getMountPointOptical(path);
    }
    else { reply = uDisks2::mountDevice(path); }
    if (!reply.isEmpty()) {
        emit errorMessage(path, reply);
        return;
    }
    updateDeviceProperties();
}

void StorageDevice::unmount()
{
    if (!dbus->isValid() || (mountpoint.isEmpty() && !isOptical)) { return; }
    QString reply;
    if (isOptical) { reply = uDisks2::unmountOptical(path); }
    else { reply = uDisks2::unmountDevice(path); }
    updateDeviceProperties();
    if (!reply.isEmpty() || (!mountpoint.isEmpty())) {
        if (reply.isEmpty()) { reply = QObject::tr("Failed to umount %1").arg(name); }
        emit errorMessage(path, reply);
        return;
    }
    if (isOptical) { eject(); }
}

void StorageDevice::eject()
{
    if (!dbus->isValid()) { return; }
    QString reply = uDisks2::ejectDevice(drive);
    updateDeviceProperties();
    if (!reply.isEmpty()/* || hasMedia*/) {
        if (reply.isEmpty()) { reply = QObject::tr("Failed to eject %1").arg(name); }
        emit errorMessage(path, reply);
    }
}

void StorageDevice::updateDeviceProperties()
{
    if (!dbus->isValid()) { return; }

    bool hadMedia =  hasMedia;
    QString lastMountpoint = mountpoint;
    QString lastName = name;

    drive = uDisks2::getDrivePath(path);
    name = uDisks2::getDeviceLabel(path);

    if (name.isEmpty()) { name = uDisks2::getDeviceName(drive); }
    if (name.isEmpty()) { name = QObject::tr("Storage"); }

    dev = path.split("/").takeLast();
    isRemovable = uDisks2::isRemovable(drive);
    filesystem = uDisks2::getFileSystem(path);
    isOptical = uDisks2::isOptical(drive);
    if (isOptical) {
        mountpoint = uDisks2::getMountPointOptical(path);
    } else {
        mountpoint = uDisks2::getMountPoint(path);
    }
    hasMedia = uDisks2::hasMedia(drive);
    opticalDataTracks = uDisks2::opticalDataTracks(drive);
    opticalAudioTracks = uDisks2::opticalAudioTracks(drive);
    isBlankDisc = uDisks2::opticalMediaIsBlank(drive);
    hasPartition = uDisks2::hasPartition(path);

    if (hadMedia != hasMedia) {
        emit mediaChanged(path, hasMedia);
    }
    if (lastMountpoint != mountpoint) {
        emit mountpointChanged(path, mountpoint);
    }
    if (lastName != name) {
        emit nameChanged(path, name);
    }
}

void StorageDevice::handlePropertiesChanged(const QString &interfaceType, const QMap<QString, QVariant> &changedProperties)
{
    Q_UNUSED(interfaceType)
    Q_UNUSED(changedProperties)
    updateDeviceProperties();
}

Disks::Disks(QObject *parent)
    : QObject(parent)
    , dbus(Q_NULLPTR)
{
    setupDBus();
    timer.setInterval(60000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(checkUDisks()));
    timer.start();
}

void Disks::setupDBus()
{
    QDBusConnection system = QDBusConnection::systemBus();
    if (system.isConnected()) {
        system.connect(DBUS_SERVICE, DBUS_PATH, DBUS_OBJMANAGER, DBUS_DEVICE_ADDED, this, SLOT(deviceAdded(const QDBusObjectPath&)));
        system.connect(DBUS_SERVICE, DBUS_PATH, DBUS_OBJMANAGER, DBUS_DEVICE_REMOVED, this, SLOT(deviceRemoved(const QDBusObjectPath&)));
        if (dbus==Q_NULLPTR) { dbus = new QDBusInterface(DBUS_SERVICE, DBUS_PATH, DBUS_OBJMANAGER, system); } // only used to verify the UDisks is running
        scanDevices();
    }
}

void Disks::scanDevices()
{
    if (!dbus) { return; }
    if (!dbus->isValid()) { return; }
    qDebug() << "scanDevices";
    QStringList foundDevices = uDisks2::getDevices();
    for (int i=0;i<foundDevices.size();i++) {
        QString foundDevicePath = foundDevices.at(i);
        bool hasDevice = devices.contains(foundDevicePath);
        if (hasDevice) { continue; }
        StorageDevice *newDevice = new StorageDevice(foundDevicePath, this);
        connect(newDevice, SIGNAL(mediaChanged(QString,bool)), this, SLOT(handleDeviceMediaChanged(QString,bool)));
        connect(newDevice, SIGNAL(mountpointChanged(QString,QString)), this, SLOT(handleDeviceMountpointChanged(QString,QString)));
        connect(newDevice, SIGNAL(errorMessage(QString,QString)), this, SLOT(handleDeviceErrorMessage(QString,QString)));
        devices[foundDevicePath] = newDevice;
    }
    emit updatedDevices();
}

void Disks::deviceAdded(const QDBusObjectPath &obj)
{
    if (!dbus) { return; }
    if (!dbus->isValid()) { return; }
    QString path = obj.path();
    if (path.startsWith(QString("%1/jobs").arg(DBUS_PATH))) { return; }

    scanDevices();
    emit foundNewDevice(path);
}

void Disks::deviceRemoved(const QDBusObjectPath &obj)
{
    if (!dbus) { return; }
    if (!dbus->isValid()) { return; }
    QString path = obj.path();
    bool deviceExists = devices.contains(path);
    if (path.startsWith(QString("%1/jobs").arg(DBUS_PATH))) { return; }

    if (deviceExists) {
        if (uDisks2::getDevices().contains(path)) { return; }
        delete devices.take(path);
    }
    scanDevices();
    emit removedDevice(path);
}

void Disks::handleDeviceMediaChanged(QString devicePath, bool mediaPresent)
{
    emit mediaChanged(devicePath, mediaPresent);
}

void Disks::handleDeviceMountpointChanged(QString devicePath, QString deviceMountpoint)
{
    emit mountpointChanged(devicePath, deviceMountpoint);
}

void Disks::handleDeviceErrorMessage(QString devicePath, QString deviceError)
{
    emit deviceErrorMessage(devicePath, deviceError);
}

void Disks::checkUDisks()
{
    if (!QDBusConnection::systemBus().isConnected()) {
        setupDBus();
        //return;
    }
    if (!dbus) { return; }
    if (dbus->isValid()) { scanDevices(); }
}
