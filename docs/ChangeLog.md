# Draco 0.9.0 RC 1 (20190515)

First public *(pre-release)* version of the Draco Desktop Environment. Forked from Lumina in January 2019, Draco aims to create a small and stable desktop environment that confirms to the XDG and freedesktop.org specifications. Draco is developed on and for Slackware Linux, but should work on any modern distribution.

## Changes compared to Lumina 1.5.0

 - Stripped-down, includes only essential desktop features
 - Improved XDG support
 - Includes many bug fixes
 - Uses Openbox as window manager *(better standards and desktop support)*
 - Power management *(Draco has merged with ``powerkit``)*
 - Storage management *(Draco includes several features taken from ``qtfm``)*
 - freedesktop.org integration
   - UPower
   - UDisks2
   - ConsoleKit/logind
   - ScreenSaver
   - PowerManagement
 - Improved Linux support
 - Improved display detection
 - The theme engine has been removed in favor of using *(upstream)* ``qt5ct``
 - CMake support
 - Internal hotkey support *(does not depend on WM for shortcuts)*
 - Desktop has proper file copy support on drag-and-drop
 - GTK icon theme and font integration

There is still a lot to do ... some features may still be under development.
