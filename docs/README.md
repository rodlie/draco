[![GitHub last commit](https://img.shields.io/github/last-commit/rodlie/draco.svg)](https://github.com/rodlie/draco/commits/master)
[![Build Status](https://travis-ci.com/rodlie/draco.svg?branch=master)](https://travis-ci.com/rodlie/draco)
[![GitHub issues](https://img.shields.io/github/issues/rodlie/draco.svg)](https://github.com/rodlie/draco/issues)
[![GitHub release](https://img.shields.io/github/release/rodlie/draco.svg)](https://github.com/rodlie/draco/releases)
[![Telegram chat](https://img.shields.io/badge/public_chat-Telegram-blue.svg)](https://t.me/dracolinuxorg)
![screenshot](https://raw.githubusercontent.com/rodlie/draco/master/docs/images/screenshot-01.jpg)

# Draco Desktop Environment

Draco is a simple and lightweight desktop environment. While small still features XDG integration, freedesktop services and integration, power and storage management, desktop, panels, multi-monitor support and much more. Draco does not include any user applications.

Draco is developed for and on [Slackware Linux](http://www.slackware.com).

## Features

...

### Desktop

...

### Panel

...

### Storage management

  * Show available storage/optical devices in system tray
  * Auto mount (and open) storage/optical devices when added *(not default)*
  * Auto play CD/DVD *(not default)*

### Power management

  * Implements [org.freedesktop.ScreenSaver](https://people.freedesktop.org/~hadess/idle-inhibition-spec/re01.html) service
  * Implements [org.freedesktop.PowerManagement](https://www.freedesktop.org/wiki/Specifications/power-management-spec/) service
  * Automatically suspend on idle
  * Automatically lock screen on idle
  * Automatically hibernate or shutdown on critical battery
  * Inhibit actions if external monitor(s) is connected
  * Enables applications to inhibit display and suspend
  * Display backlight support
  * Display hotplug support
  * RTC wake support
  * cpufreq/pstate support **(WIP)**

## Usage

...

### Default shortcuts

| Shortcut | Action |
| -------- | ------ |
| ``Alt+F1`` | Default terminal |
| ``Alt+F2`` | Application launcher |
| ``Ctrl+Alt+Left`` | Switch desktop/workspace |
| ``Ctrl+Alt+Right`` | Switch desktop/workspace |
| ``Ctrl+Alt+Up`` | Switch desktop/workspace |
| ``Ctrl+Alt+Down`` | Switch desktop/workspace |
| ``Shift+Alt+Left`` | Move window to desktop/workspace |
| ``Shift+Alt+Right`` | Move window to desktop/workspace |
| ``Shift+Alt+Up`` | Move window to desktop/workspace |
| ``Shift+Alt+Down`` | Move window to desktop/workspace |
| ``Win+F1`` | Switch to desktop/workspace 1 |
| ``Win+F2`` | Switch to desktop/workspace 2 |
| ``Win+F3`` | Switch to desktop/workspace 3 |
| ``Win+F4`` | Switch to desktop/workspace 4 |
| ``Win+D`` | Toggle show desktop |
| ``Alt+Tab`` | Next window |
| ``Alt+Shift+Tab`` | Previous window |
| ``wheel/touchpad up/down on desktop`` | Switch desktop/workspace |

### Screen saver

Draco depends on [XScreenSaver](https://www.jwz.org/xscreensaver/) to handle the screen session, the default settings may need to be adjusted. You can launch the (XScreenSaver) configuration GUI with the ``xscreensaver-demo`` command or from settings.

Recommended settings are:

* Mode: ``Blank Screen Only``
* Blank After: ``5 minutes``
* Lock Screen After: ``enabled + 0 minutes``
* Display Power Management: ``enabled``
  * Standby After: ``0 minutes``
  * Suspend After: ``0 minutes``
  * Off After: ``0 minutes``
  * Quick Power-off in Blank Only Mode: ``enabled``

A custom configuration tool will be added in the future.

### Backlight

Backlight is supported through ``/sys/class/backlight``. The current brightness can be adjusted with the mouse wheel on the system tray icon.

### Hibernate

A swap partition (or file) is needed by the kernel to support hibernate/hybrid sleep. Edit the boot loader configuration and add the kernel option ``resume=<swap_partition/swap_file>``, then save and restart.

**Note that some distributions don't require ``resume=``, and some have hibernation disabled. Consult your distribution documentation regarding hibernation.**

## Components

Draco is divided into the following components:

 * **libDraco**
   * Common functions.
 * **start-draco**
   * Takes care of starting and running the desktop environment.
 * **draco-settings**
   * Manage settings.
 * **draco-settings-x11**
   * Manage monitors. *You can use this component outside of Draco.*
 * **org.dracolinux.Desktop**
   * The desktop session. Takes care of anything related to desktop, panels, clipboard and auto started applications/services. *You can use this component outside of Draco.*
 * **org.dracolinux.Power**
   * Takes care of anything related to power. This includes, but not limited to screensaver, freedesktop services, suspend, hibernate, monitors, backlight, keyboard, CPU. *You can use this component outside of Draco.*
 * **org.dracolinux.Powerd**
   * D-Bus system service for Draco. Used to modify backlight, cpufreq/pstate and RTC wake alarm.
 * **org.dracolinux.Storage**
   * Takes care of removable storage devices. *You can use this component outside of Draco.*
 * **org.dracolinux.XDG**
   * ``xdg-open`` replacement. Takes care of anything XDG related when running Draco. *You can use this component outside of Draco.*

## Screenshots

Various screenshots.


### Settings

Main settings:

![screenshot](https://raw.githubusercontent.com/rodlie/draco/master/docs/images/screenshot-02.jpg)


Draco XDG/mime settings:

![screenshot](https://raw.githubusercontent.com/rodlie/draco/master/docs/images/screenshot-03.jpg)

Applications and services that are launched during desktop startup:

![screenshot](https://raw.githubusercontent.com/rodlie/draco/master/docs/images/screenshot-04.jpg)

Basic keyboard configuration:

![screenshot](https://raw.githubusercontent.com/rodlie/draco/master/docs/images/screenshot-05.jpg)

Panels configuration:

![screenshot](https://raw.githubusercontent.com/rodlie/draco/master/docs/images/screenshot-06.jpg)

Power configuration:

![screenshot](https://raw.githubusercontent.com/rodlie/draco/master/docs/images/screenshot-07.jpg)

Wallpaper configuration:

![screenshot](https://raw.githubusercontent.com/rodlie/draco/master/docs/images/screenshot-08.jpg)

General desktop configuration:

![screenshot](https://raw.githubusercontent.com/rodlie/draco/master/docs/images/screenshot-09.jpg)

Monitor configuration:

![screenshot](https://raw.githubusercontent.com/rodlie/draco/master/docs/images/screenshot-10.jpg)


## History

Draco was an alternative GNU/Linux distribution developed between 2005 and 2015. Since then the OS has been dropped in favor of (upstream, and) creating a desktop environment. The distribution started out as a fork of Slackware Linux, while the desktop started out as a fork of [Lumina](https://github.com/lumina-desktop/lumina) aimed at Slackware Linux. *What goes around comes around ...*

## Requirements

Build and runtime requirements.
  * **``cmake``** *(>= 3.0)*
  * **``pkg-config``**
  * **``Qt``** *(5.5+, latest LTS recommended)*
    * ``QtCore``
    * ``QtDBus``
    * ``QtGui``
    * ``QtWidgets``
    * ``QtConcurrent``
    * ``QtX11Extras``
  * **``X11``**
    * ``libX11``
    * ``libXss``
    * ``libXdamage``
    * ``libXrandr``
    * ``libXfixes``
    * ``xrandr``
    * ``setxkbmap``
    * ``xinit``
    * ``xcb``
      * ``xcb-randr``
      * ``xcb-ewmh``
      * ``xcb-icccm``
      * ``xcb-image``
      * ``xcb-composite``
      * ``xcb-damage``
      * ``xcb-util``
      * ``xcb-shm``
  * **``xscreensaver``**
  * **``qt5ct``**
  * **``xdg-utils``**
  * **``shared-mime-info``**
  * **``hicolor-icon-theme``**
  * **``adwaita-icon-theme``** *(3.18 recommended)*
  * **``openbox``**
  * **``UDisks2``** *(or compatible service)*
  * **``UPower``** *(>= 0.9.23 or compatible service)*
  * **``ConsoleKit``**/**``logind``** *(or compatible service)*

## Recommended

Optional recommended applications.
  * **``qterminal``** *(terminal emulator)*
  * **``qtfm``** *(file manager)*
  * **``pnmixer``** *(audio system tray)*
  * **``obconf``** *(window manager config)*

## Build

### Daemon notice

Draco includes a power daemon (to be able to adjust screen and cpu) this daemon must run as ``root`` (or a user with write access to ``/sys``), this service run through D-Bus and can only be accessed by a predefined system group, this should be a common desktop user group like ``power`` or something similar. The default when building Draco is ``root`` as user and ``power`` as group. If you want something else you can change this during build.

Add the following during ``cmake`` configure:

```
-DPOWERD_SERVICE_USER=<run service as this user>
```
```
-DPOWERD_SERVICE_GROUP=<user group that have access to service>
```

You can also change the ``/etc/dbus-1/system.d/org.dracolinux.Powerd.conf`` file manually if you want.

### Slackware Linux (14.2+)

Install the following packages from [SlackBuilds.org](http://slackbuilds.org) or [slackware.com/~alien](http://www.slackware.com/~alien/slackbuilds/):
 * qt5 *(5.9.7+/5.12.x recommended)*
 * qt5ct
 * openbox

 The rest should be available in a standard installation.

A [draco.SlackBuild](https://github.com/rodlie/draco/blob/master/share/slackware/draco.SlackBuild) is available.

You can of course build it manually:

```
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=lib64 -DCMAKE_INSTALL_MANDIR=/usr/man -DCMAKE_INSTALL_DOCDIR=/usr/doc -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```

**Note that the install prefix should be the same folder as D-Bus (usually in ``/usr``), else you will need to copy the Draco service files to the proper location manually.**

### Ubuntu (Xenial+)

Minimal testing is done on Ubuntu, some features may not work.

Known issues:

 * Add your user to the ``power`` group before running Draco (add group if not exists).
 * Hibernate does not work, will add instructions on fixing this

Dependencies (based on a miminal install of Xenial):

```
sudo apt-get install build-essential cmake pkg-config openbox adwaita-icon-theme-full xscreensaver xdg-utils qt5ct qtbase5-dev libqt5x11extras5-dev libx11-dev libxss-dev libxdamage-dev libxrandr-dev libxfixes-dev libxcb1-dev libx11-xcb-dev libxcb-randr0-dev libxcb-ewmh-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-composite0-dev libxcb-damage0-dev libxcb-util0-dev libxcb-shm0-dev openbox-dev
```

Build and install:

```
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=lib64 -DCMAKE_INSTALL_LIBEXECDIR=bin -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```

**Note that the install prefix should be the same folder as D-Bus (usually in ``/usr``), else you will need to copy the Draco service files to the proper location manually.**

### Fedora

Minimal testing is done on Fedora, some features may not work.

Known issues:

 * Same as Ubuntu
 * Adwaita icons are broken, expect missing icons


Dependencies:

```
sudo dnf install qt5-qtbase-devel qt5-qtx11extras-devel cmake gcc-c++ pkg-config libXScrnSaver-devel libXdamage-devel libXrandr-devel libxcb-devel xcb-util-devel xcb-util-wm-devel xcb-util-image-devel xdg-utils shared-mime-info hicolor-icon-theme adwaita-icon-theme openbox xscreensaver qt5ct openbox-devel
```

Build and install:

```
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=lib64 -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```

**Note that the install prefix should be the same folder as D-Bus (usually in ``/usr``), else you will need to copy the Draco service files to the proper location manually.**


## License

This project has source files licensed under ``BSD-3``, ``LGPL-2.1`` and ``GPL-2``.
