# Draco Desktop Environment

![screenshot](draco-screenshot.gif)

A simple and modern desktop environment for (``Slackware``) Linux. 

**Currently not ready for end-users, (first) version ``0.9.0`` will be released during May of 2019.**

## About
## Features
## Usage
## FAQ

### Hey, this looks very similar to ``Lumina``!?

And right you are, ``Draco`` is a fork of ``Lumina``. Why? Well, difference of opinion is probably the best answer. I wanted a simple desktop environment on Linux. Something that *"just works"* out-of-the-box, with minimal fuzz (and installed applications). This was hard since the focus for ``Lumina`` is ``FreeBSD``/``Trident``, and adding changes that would benefit Linux (using D-Bus etc) would not be accepted, also I didn't agree with the road map for ``Lumina`` v2. So here we are, a stripped-down fork of ``Lumina`` targeting (``Slackware``) Linux, including the bare essentials for a modern desktop experience. 

### Project goals/focus?

To provide a simple and modern desktop environment on (``Slackware``) Linux that *"just works"* on minimal resources and with minimal fuzz.

Feature requests will have a low priority while bug reports will have a high priority.

## Requirements

  * **``Qt``** *(5.2+, 5.9.7/5.12.x LTS recommended)*
    * ``QtCore``
    * ``QtGui``
    * ``QtWidgets``
    * ``QtNetwork``
    * ``QtConcurrent``
    * ``QtX11Extras``
    * ``QtDBus``
  * **``X11``**
    * ``libxdamage``
    * ``libxrandr``
    * ``xrandr``
    * ``xinit``
    * ``x11``
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

  * **``powerkit``**
    * *Takes care of everything power related and implements ``org.freedesktop`` services. Will not work without.*
  * **``xscreensaver``**
    * Takes care of screen blanking and locking. Will not work without.
  * **``qtfm``**
    * *Currently needed for ``qtfm-tray``, this will be optional in the future. Will not work without (at the moment).*
  * **``qt5ct``**
    * *Needed to change the (Qt) theme (and icons). Not strictly needed, but highly recommended.*
  * **``xdg-utils``**
  * **``hicolor-icon-theme``**
  * **``adwaita-icon-theme``** (=< 3.30.0)
    * *The standard icon set for ``Draco``. **Do not use version higher than 3.30.x.***
  * **``openbox``**
    * *Window manager. Will not work without.*
  * **``D-Bus``**
    * *``D-Bus`` is needed to have a modern desktop, will not work without.*
  * **``ConsoleKit``**/**``logind``**
    * *``ConsoleKit`` or ``logind`` is needed by some power related features, will work without but it's not recommended to run ``Draco`` without.*

## Build
## License
## Credits
## Related