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

#ifndef POWER_SETTINGS_WIDGET_H
#define POWER_SETTINGS_WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPixmap>
#include <QTabWidget>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>
#include <QSlider>
#include <QGroupBox>
#include <QDateTime>
#include <QScrollArea>

class PowerSettingsWidget : public QWidget
{
    Q_OBJECT

public:
   explicit PowerSettingsWidget(QWidget *parent = nullptr);
   ~PowerSettingsWidget();

private:
    QDBusInterface *dbus;
    QComboBox *lidActionBattery;
    QComboBox *lidActionAC;
    QComboBox *criticalActionBattery;
    QSpinBox *criticalBattery;
    QSpinBox *autoSleepBattery;
    QSpinBox *autoSleepAC;
    QCheckBox *desktopSS;
    QCheckBox *desktopPM;
    QCheckBox *showNotifications;
    QCheckBox *showSystemTray;
    QCheckBox *disableLidAction;
    QComboBox *autoSleepBatteryAction;
    QComboBox *autoSleepACAction;
    QString backlightDevice;
    QSlider *backlightSliderBattery;
    QSlider *backlightSliderAC;
    QCheckBox *backlightBatteryCheck;
    QCheckBox *backlightACCheck;
    QCheckBox *backlightBatteryLowerCheck;
    QCheckBox *backlightACHigherCheck;
    QCheckBox *warnOnLowBattery;
    QCheckBox *warnOnVeryLowBattery;

    QCheckBox *notifyOnBattery;
    QCheckBox *notifyOnAC;
    QLabel *lidActionACLabel;
    QLabel *lidActionBatteryLabel;
    QLabel *batteryBacklightLabel;
    QLabel *acBacklightLabel;
    QCheckBox *backlightMouseWheel;
    QCheckBox *suspendLockScreen;

    QSpinBox *suspendBatteryWakeTimer;
    QLabel *suspendBatteryWakeTimerLabel;
    QSpinBox *suspendACWakeTimer;
    QLabel *suspendACWakeTimerLabel;

private slots:
    void setupWidgets();
    void populate();
    void loadSettings();
    void saveSettings();
    void setDefaultAction(QComboBox *box, int action);
    void setDefaultAction(QSpinBox *box, int action);
    void setDefaultAction(QComboBox *box, QString value);
    void handleLidActionBattery(int index);
    void handleLidActionAC(int index);
    void handleCriticalAction(int index);
    void handleCriticalBattery(int value);
    void handleAutoSleepBattery(int value);
    void handleAutoSleepAC(int value);
    void handleDesktopSS(bool triggered);
    void handleDesktopPM(bool triggered);
    void handleShowNotifications(bool triggered);
    void handleShowSystemTray(bool triggered);
    void handleDisableLidAction(bool triggered);
    void handleAutoSleepBatteryAction(int index);
    void handleAutoSleepACAction(int index);
    void checkPerms();
    void handleBacklightBatteryCheck(bool triggered);
    void handleBacklightACCheck(bool triggered);
    void handleBacklightBatterySlider(int value);
    void handleBacklightACSlider(int value);
    void hibernateWarn();
    void sleepWarn();
    void handleBacklightBatteryCheckLower(bool triggered);
    void handleBacklightACCheckHigher(bool triggered);
    void enableBacklight(bool enabled);
    void handleWarnOnLowBattery(bool triggered);
    void handleWarnOnVeryLowBattery(bool triggered);
    void handleNotifyBattery(bool triggered);
    void handleNotifyAC(bool triggered);
    void enableLid(bool enabled);
    void handleBacklightMouseWheel(bool triggered);
    void handleSuspendLockScreen(bool triggered);
    void enableBattery(bool enabled);
    void handleSuspendWakeBatteryTimer(int value);
    void handleSuspendWakeACTimer(int value);
};

#endif // POWER_SETTINGS_WIDGET_H
