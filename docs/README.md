# Draco Desktop Environment
![GitHub last commit](https://img.shields.io/github/last-commit/rodlie/draco.svg)
[![Build Status](https://travis-ci.com/rodlie/draco.svg?branch=master)](https://travis-ci.com/rodlie/draco)
![GitHub issues](https://img.shields.io/github/issues/rodlie/draco.svg)
[![GitHub release](https://img.shields.io/github/release/rodlie/draco.svg)](https://github.com/rodlie/draco/releases)

![screenshot](https://raw.githubusercontent.com/rodlie/draco/master/docs/draco-promo.gif)

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
  * Display hotplug support **(WIP)**
  * RTC support **(WIP)**
  * CPU support **(WIP)**

## Usage

...

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

Install the following packages from [SlackBuilds.org](http://slackbuilds.org) (packages may be available [here](http://www.slackware.com/~alien/slackbuilds/)):
 * qt5 *(5.9.7/5.12.x recommended)*
 * qt5ct
 * openbox
 * *pnmixer (optional)*
 * *qtfm (optional)*
 * *qterminal (optional)*

```
On Slackware64 Current do the following to setup dependencies:

wget http://www.slackware.com/~alien/slackbuilds/OpenAL/pkg64/current/OpenAL-1.19.1-x86_64-1alien.txz
sudo upgradepkg --install-new OpenAL-*.txz

wget http://www.slackware.com/~alien/slackbuilds/libxkbcommon/pkg64/current/libxkbcommon-0.8.3-x86_64-1alien.txz
sudo upgradepkg --install-new libxkbcommon-*.txz

wget http://www.slackware.com/~alien/slackbuilds/openbox/pkg64/current/openbox-3.6.1-x86_64-1alien.tgz
sudo upgradepkg --install-new openbox-*.tgz

wget http://www.slackware.com/~alien/slackbuilds/qt5/pkg64/current/qt5-5.12.1-x86_64-1alien.txz
sudo upgradepkg --install-new qt5-*.txz

You will need to build qt5ct from SlackBuilds.org:

wget https://slackbuilds.org/slackbuilds/14.2/desktop/qt5ct.tar.gz
tar xvf qt5ct.tar.gz
cd qt5ct.tar.gz
wget http://prdownloads.sourceforge.net/qt5ct/qt5ct-0.38.tar.bz2
sudo sh qt5ct.SlackBuild
sudo upgradepkg --install-new /tmp/qt5ct-*.tgz


```

 The rest should be available in a standard installation.

A [draco.SlackBuild](https://github.com/rodlie/draco/blob/master/share/slackware/draco.SlackBuild) is available (will build latest from git if run from repo):
```
git clone https://github.com/rodlie/draco
cd draco/share/slackware
sudo sh draco.SlackBuild
```

Install the package and off you go. Or you can of course build it manually:

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
sudo apt-get install build-essential cmake pkg-config openbox adwaita-icon-theme-full xscreensaver xdg-utils qt5ct qtbase5-dev libqt5x11extras5-dev libx11-dev libxss-dev libxdamage-dev libxrandr-dev libxfixes-dev libxcb1-dev libx11-xcb-dev libxcb-randr0-dev libxcb-ewmh-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-composite0-dev libxcb-damage0-dev libxcb-util0-dev libxcb-shm0-dev
```

Optional:

```
sudo apt-get install qterminal pnmixer
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
sudo dnf install qt5-qtbase-devel qt5-qtx11extras-devel cmake gcc-c++ pkg-config libXScrnSaver-devel libXdamage-devel libXrandr-devel libxcb-devel xcb-util-devel xcb-util-wm-devel xcb-util-image-devel xdg-utils shared-mime-info hicolor-icon-theme adwaita-icon-theme openbox xscreensaver qt5ct
```

Optional:

```
sudo dnf install qterminal pnmixer
```

Build and install:

```
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=lib64 -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```

**Note that the install prefix should be the same folder as D-Bus (usually in ``/usr``), else you will need to copy the Draco service files to the proper location manually.**
