# Draco Desktop Environment

![screenshot](draco-screenshot.gif)

A simple and modern desktop environment for Linux. 

**Currently not ready for usage, (first) version ``0.9.0`` will be released during May of 2019.**

## Features
## Usage
## FAQ

### Hey, this looks very similar to Lumina!?

And right you are, this is a fork of the [Lumina Desktop Enviroment](https://github.com/lumina-desktop/lumina). Why? Well, difference of opinion is probably the best answer. 

This fork is aimed at working as good as possible on (``Slackware``) Linux with the ``freedesktop(1)`` services and specs while including the ``bare essentials(2)`` for a modern desktop experience.

``(1)``: We use and implement most of the common desktop services and specs from [freedesktop.org](https://www.freedesktop.org/wiki/).

``(2)``: While we do reinvent the wheel in regards to the desktop/panel, we don't want to reinvent the complete wheel (just the important part ;) ). Draco does not include any applications. Why make another text editor? Why make another image viewer? etc ... Meh, most users already have their own list of prefered applications, why force them to install stuff they don't need or use?


### Project goals/focus?

To provide a simple and modern desktop environment on Linux with minimal fuzz.

Feature requests will have a low priority while bug reports will have a high priority.

## Requirements

  * **``Qt``** *(5.2+, 5.9.7/5.12.x LTS recommended)*
    * ``QtCore``
    * ``QtDBus``
    * ``QtGui``
    * ``QtWidgets``
    * ``QtNetwork``
    * ``QtConcurrent``
    * ``QtX11Extras``
  * **``X11``**
    * ``libX11``
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

  * **``powerkit``** (2.0+)
    * *Takes care of everything power related and implements important ``org.freedesktop`` services.*
  * **``xscreensaver``**
    * *Takes care of screen blanking and locking. Also required by powerkit.*
  * **``qt5ct``**
    * *Needed to set/change the desktop and applications theme (look, color, font, icons). Not strictly needed, but highly recommended.*
  * **``xdg-utils``**
  * **``hicolor-icon-theme``**
  * **``adwaita-icon-theme``** (=< 3.28.0)
    * *The standard icon set used. **Do not use v3.3x, we recommend v3.18.***
  * **``openbox``**
    * *The standard window manager used. ***We do not support any other window manager for the desktop enviroment***, you can however combine ``draco-desktop`` with any window manager and setup, more information regarding this will be added later. Note that we (currently) do not provide any GUI application(s) to adjust the window manager settings.*
  * **``ConsoleKit``**/**``logind``**
    * *``ConsoleKit`` or ``logind`` is needed by some power(kit) related features, will work without but it's not recommended to run without them.*

## Build
## License
## Credits
## Related