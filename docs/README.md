# Draco Desktop Environment
![GitHub last commit](https://img.shields.io/github/last-commit/rodlie/draco.svg)
[![Build Status](https://travis-ci.com/rodlie/draco.svg?branch=master)](https://travis-ci.com/rodlie/draco)
[![GitHub release](https://img.shields.io/github/release/rodlie/draco.svg)](https://github.com/rodlie/draco/releases)
![GitHub Release Date](https://img.shields.io/github/release-date/rodlie/draco.svg)
[![Github commits (since latest release)](https://img.shields.io/github/commits-since/rodlie/draco/latest.svg)](https://github.com/rodlie/draco)
![GitHub All Releases](https://img.shields.io/github/downloads/rodlie/draco/total.svg)
![GitHub issues](https://img.shields.io/github/issues/rodlie/draco.svg)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/rodlie/draco.svg)
![GitHub repo size](https://img.shields.io/github/repo-size/rodlie/draco.svg)
![GitHub top language](https://img.shields.io/github/languages/top/rodlie/draco.svg)

![GitHub stars](https://img.shields.io/github/stars/rodlie/draco.svg?style=social)
![GitHub watchers](https://img.shields.io/github/watchers/rodlie/draco.svg?style=social)

**NOT ready for usage, (first) version (``0.9.0``) will be released by the end of May 2019.**

Draco is a simple and lightweight desktop environment. While small still features XDG integration, freedesktop services and integration, power and storage management, desktop, panels, multi-monitor support and much more. Draco does not include any user applications.

Draco is developed for and on [Slackware Linux](http://www.slackware.com).

## Features

### Desktop
### Panel
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
  * RTC support **(WIP)**
  * CPU support **(WIP)**

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
  * **``hicolor-icon-theme``**
  * **``adwaita-icon-theme``** *(=< 3.28.0)*
  * **``openbox``**
  * **``UDisks``** *(or compatible service)*
  * **``UPower``** *(>= 0.9.23 or compatible service)*
  * **``ConsoleKit``**/**``logind``** *(or compatible service)*

## Recommended

Optional recommended applications.
  * **``qterminal``** *(terminal emulator)*
  * **``qtfm``** *(file manager)*
  * **``pnmixer``** *(audio system tray)*

## Build

### Slackware Linux (14.2+)

Install the following packages from [SlackBuilds.org](http://slackbuilds.org) (packages may be available [here](http://www.slackware.com/~alien/slackbuilds/)):
 * qt5
 * qt5ct
 * openbox
 * pnmixer (optional)
 * qtfm (optional)
 * qterminal (optional)

 The rest should be available in a standard installation.

A [draco.SlackBuild](share/slackware/draco.SlackBuild) is available (will build latest from git):
```
git clone https://github.com/rodlie/draco
cd draco/share/slackware
sudo sh draco.SlackBuild
```
or build it manually:

```
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=lib64 -DCMAKE_INSTALL_MANDIR=/usr/man -DCMAKE_INSTALL_DOCDIR=/usr/doc -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```

### Ubuntu (Xenial+)

Minimal testing is done on Ubuntu, some features may not work.

Dependencies:

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
