# Draco Desktop Environment

**NOT ready for usage, (first) version (``0.9.0``) will be released by the end of May 2019.**

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
  * RTC support (WIP)
  * CPU support (WIP)

## Usage
## FAQ

### Hey, this looks very similar to Lumina!?

And right you are, this is a fork of the [Lumina Desktop Enviroment](https://github.com/lumina-desktop/lumina). Why? Well, difference of opinion is probably the best answer. We have different focus and goals.

## Requirements

Build and runtime requirements.
  * **``cmake``** *(>= 3.0)*
  * **``pkg-config``**
  * **``Qt``** *(5.2+, latest LTS recommended)*
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

### Build on Slackware Linux

Install the following packages from [SlackBuilds.org](http://slackbuilds.org) (packages may be available [here](http://www.slackware.com/~alien/slackbuilds/)):
 * qt5
 * qt5ct
 * openbox
 * pnmixer (optional)
 * qtfm (optional)
 * qterminal (optional)

 The rest should be available in a standard installation.

A [draco.SlackBuild](share/slackware/draco.SlackBuild) is available from our repository, or build it manually:

```
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=lib64 -DCMAKE_INSTALL_MANDIR=/usr/man -DCMAKE_INSTALL_DOCDIR=/usr/doc -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```

### Build on Arch Linux
### Build on Ubuntu

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
