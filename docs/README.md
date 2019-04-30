# Simple Desktop Environment

![screenshot](draco-screenshot.gif)

### **Currently not ready for usage, (first) version ``0.9.0`` will be released during May of 2019.**

## Features

### Desktop
### Panel
### Storage
### Power management

* Implements [org.freedesktop.ScreenSaver](https://people.freedesktop.org/~hadess/idle-inhibition-spec/re01.html) service
 * Implements [org.freedesktop.PowerManagement](https://www.freedesktop.org/wiki/Specifications/power-management-spec/) service
 * Automatically suspend on idle
 * Automatically lock screen on idle
 * Automatically hibernate or shutdown on critical battery
 * Inhibit actions if external monitor(s) is connected
 * Enables applications to inhibit display and suspend
 * Advanced power settings
 * Screen locking support
 * Good laptop support
 * Display backlight support
 * Display hotplug support
 * RTC support
 * CPU support


## Usage
## FAQ

### How does an application inhibit the screen saver?

The preferred way to inhibit the screen saver from an application is to use the [org.freedesktop.ScreenSaver](https://people.freedesktop.org/~hadess/idle-inhibition-spec/re01.html) specification. Any application that uses [org.freedesktop.ScreenSaver](https://people.freedesktop.org/~hadess/idle-inhibition-spec/re01.html) will work with Draco.

*Note that Draco also includes ``SimulateUserActivity`` for backwards compatibility*.

Popular applications that uses this feature is Mozilla Firefox, Google Chrome, VideoLAN VLC and many more.

### How does an application inhibit suspend actions?

The preferred way to inhibit suspend actions from an application is to use the [org.freedesktop.PowerManagement](https://www.freedesktop.org/wiki/Specifications/power-management-spec/) specification. Any application that uses [org.freedesktop.PowerManagement](https://www.freedesktop.org/wiki/Specifications/power-management-spec/) will work with Draco.

Common use cases are audio playback, downloading and more.

### Hey, this looks very similar to Lumina!?

And right you are, this is a fork of the [Lumina Desktop Enviroment](https://github.com/lumina-desktop/lumina). Why? Well, difference of opinion is probably the best answer. 

This fork is aimed at working as good as possible on (``Slackware``) Linux with the ``freedesktop(1)`` services and specs while including the ``bare essentials(2)`` for a modern desktop experience.

``(1)``: We use and implement most of the common desktop services and specs from [freedesktop.org](https://www.freedesktop.org/wiki/).

``(2)``: While we do reinvent the wheel in regards to the desktop/panel, we don't want to reinvent the complete wheel (just the important part ;) ). Draco does not include any applications. Why make another text editor? Why make another image viewer? etc ... Most users already have their own suite of preferred applications.


## Requirements

  * **``Qt``** *(5.2+, latest LTS recommended)*
    * ``QtCore``
    * ``QtDBus``
    * ``QtGui``
    * ``QtWidgets``
    * ``QtNetwork``
    * ``QtConcurrent``
    * ``QtX11Extras``
  * **``X11``**
    * ``libX11``
    * ``libXss``
    * ``libXdamage``
    * ``libXrandr``
    * ``xrandr``
    * ``xinit``
    * ``xcb``
      * ``xcb-xinput``
      * ``xcb-randr``
      * ``xcb-ewmh``
      * ``xcb-icccm``
      * ``xcb-image``
      * ``xcb-composite``
      * ``xcb-damage``
      * ``xcb-util``
      * ``xcb-atom``

  * **``xscreensaver``**
  * **``qt5ct``**
  * **``xdg-utils``**
  * **``hicolor-icon-theme``**
  * **``adwaita-icon-theme``** *(=< 3.28.0)*
  * **``openbox``**
  * **``UPower``** *(>= 0.9.23)*
  * **``ConsoleKit``**/**``logind``** (or compatible ``freedesktop`` service)

## Build
## License
## Credits
## Related
