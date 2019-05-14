#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com>
# All rights reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
#

cmake_minimum_required(VERSION 3.0)
include(GNUInstallDirs)

project(draco VERSION 0.9.0)
set(PROJECT_VERSION_EXTRA _rc1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/share/cmake")

set(LIB_NAME Draco)
set(PUBLISHER dracolinux)
set(DOMAIN org.${PUBLISHER})
set(XDG_OPEN ${DOMAIN}.XDG)
set(POWER_SERVICE_NAME ${DOMAIN}.Power)
set(POWERD_SERVICE_NAME ${DOMAIN}.Powerd)
set(STORAGE_SERVICE_NAME ${DOMAIN}.Storage)
set(DESKTOP_SERVICE_NAME ${DOMAIN}.Desktop)
set(
    POWERD_SERVICE_USER
    "root"
    CACHE
    STRING
    "Run powerd service as this user (needs access to /sys)."
)
set(
    POWERD_SERVICE_GROUP
    "power"
    CACHE
    STRING
    "group that have access to powerd service, this should be all desktop users."
)

add_definitions(-DDESKTOP_APP="${PROJECT_NAME}")
add_definitions(-DDESKTOP_APP_NAME="${LIB_NAME}")
add_definitions(-DDESKTOP_APP_DOMAIN="${DOMAIN}")
add_definitions(-DDESKTOP_APP_VERSION="${PROJECT_VERSION}")
add_definitions(-DDESKTOP_APP_VERSION_EXTRA="${PROJECT_VERSION_EXTRA}")
add_definitions(-DDESKTOP_SERVICE_NAME="${DESKTOP_SERVICE_NAME}")
add_definitions(-DPOWER_SERVICE_NAME="${POWER_SERVICE_NAME}")
add_definitions(-DPOWERD_SERVICE_NAME="${POWERD_SERVICE_NAME}")
add_definitions(-DSTORAGE_SERVICE_NAME="${STORAGE_SERVICE_NAME}")
add_definitions(-DDAEMON_APP_NAME="${PUBLISHER}.Powerd")
add_definitions(-DPUBLISHER="${PUBLISHER}")
add_definitions(-DXDG_OPEN="${XDG_OPEN}")
add_definitions(-DOPENBOX_THEME_DIR="${CMAKE_INSTALL_FULL_DATAROOTDIR}/openbox/themes")

if(CMAKE_BUILD_TYPE MATCHES "^(release|Release|RELEASE)$")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
else()
    message("-- DRACO DEBUG BUILD")
endif()

# openbox/glib (needed by obconf)
find_package(PkgConfig)
pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_check_modules(
    OPENBOX
    REQUIRED
    obrender-3.5
    obt-3.5
)

# qt
find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5X11Extras REQUIRED)

# x11
find_package(X11 REQUIRED)
find_package(XCB REQUIRED)
if(NOT XCB_FOUND)
    message(FATAL_ERROR "xcb(-*) not found")
endif()
if(NOT X11_Xrandr_FOUND)
    message(FATAL_ERROR "libXrandr not found")
endif()
if(NOT X11_Xdamage_FOUND)
    message(FATAL_ERROR "libXdamage not found")
endif()
if(NOT X11_Xfixes_FOUND)
    message(FATAL_ERROR "libXfixes not found")
endif()
if(${CMAKE_VERSION} VERSION_LESS "3.14.3")
    if(NOT X11_Xscreensaver_FOUND)
        message(FATAL_ERROR "libXScrnSaver not found")
    endif()
else()
    if(NOT X11_Xss_FOUND)
        message(FATAL_ERROR "libXScrnSaver not found")
    endif()
endif()

# libDraco
set(
    LIB_SOURCES
    src/lib/draco.cpp
    src/lib/power_def.h
    src/lib/org.dracolinux.Disks.cpp
    src/lib/org.dracolinux.Power.Client.cpp
    src/lib/org.dracolinux.Power.Device.cpp
    src/lib/org.dracolinux.Power.HotPlugX11.cpp
    src/lib/org.dracolinux.Power.Manager.cpp
    src/lib/org.dracolinux.Power.ScreenX11.cpp
    src/lib/org.dracolinux.Power.Settings.cpp
    src/lib/org.dracolinux.Powerd.Manager.Backlight.cpp
    src/lib/org.dracolinux.Powerd.Manager.CPU.cpp
    src/lib/org.dracolinux.Powerd.Manager.RTC.cpp
    src/lib/org.dracolinux.Powerd.Manager.cpp
    src/lib/org.freedesktop.PowerManagement.cpp
    src/lib/org.freedesktop.ScreenSaver.cpp
    src/lib/org.freedesktop.UDisks2.cpp
    src/lib/hotkey/qhotkey.cpp
    src/lib/hotkey/qhotkey_x11.cpp
    src/lib/hotkey/qhotkey_p.h
    src/lib/lumina/DesktopSettings.cpp
    src/lib/lumina/LDesktopUtils.cpp
    src/lib/lumina/LIconCache.cpp
    src/lib/lumina/LUtils.cpp
    src/lib/lumina/LuminaRandR-X11.cpp
    src/lib/lumina/LuminaX11.cpp
    src/lib/lumina/LuminaXDG.cpp
    src/lib/lumina/LFileInfo.cpp
    src/lib/lumina/ResizeMenu.cpp
    src/lib/lumina/XDGMime.cpp
    src/lib/keyboard_common.h
    src/lib/qtcopydialog/qtcopydialog.cpp
    src/lib/qtcopydialog/qtfilecopier.cpp
    src/lib/qtcopydialog/qtcopydialog.ui
    src/lib/qtcopydialog/qtoverwritedialog.ui
    src/lib/qtcopydialog/qtotherdialog.ui
    )
#if(USE_XINPUT)
#    set(LIB_SOURCES "${LIB_SOURCES};src/lib/lumina/LInputDevice.cpp")
#else()
#    add_definitions(-DNO_XINPUT)
#endif()
add_library(
    ${LIB_NAME}
    SHARED
    ${LIB_SOURCES}
    share/share.qrc
)
set_target_properties(
    ${LIB_NAME}
    PROPERTIES
    VERSION
    ${PROJECT_VERSION}
)
set_target_properties(
    ${LIB_NAME}
    PROPERTIES
    SOVERSION
    ${PROJECT_VERSION_MAJOR}
)
target_include_directories(
    ${LIB_NAME}
    PRIVATE
    src/lib
    src/lib/lumina
    src/lib/qtcopydialog
    src/lib/hotkey
)
if(${CMAKE_VERSION} VERSION_LESS "3.14.3")
    set(XSS_INCLUDE ${X11_Xscreensaver_INCLUDE_PATH})
    set(XSS_LIB ${X11_Xscreensaver_LIB})
else()
    set(XSS_INCLUDE ${X11_Xss_INCLUDE_PATH})
    set(XSS_LIB ${X11_Xss_LIB})
endif()
target_include_directories(
    ${LIB_NAME}
    PUBLIC
    src
    ${X11_X11_INCLUDE_PATH}
    ${XCB_INCLUDE_DIRS}
    ${X11_Xrandr_INCLUDE_PATH}
    ${X11_Xfixes_INCLUDE_PATH}
    ${XSS_INCLUDE}
    ${X11_Xdamage_INCLUDE}
)
target_link_libraries(
    ${LIB_NAME}
    ${X11_LIBRARIES}
    ${XCB_LIBRARIES}
    ${X11_Xrandr_LIB}
    ${X11_Xfixes_LIB}
    ${XSS_LIB}
    ${X11_Xdamage_LIB}
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    Qt5::Widgets
    Qt5::Concurrent
    Qt5::X11Extras
)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
else()
    set(LIB_INSTALL_DIR ${CMAKE_INSTALL_FULL_LIBDIR})
endif()

# org.dracolinux.Desktop
add_executable(
    ${DESKTOP_SERVICE_NAME}
    src/desktop/main.cpp
    src/desktop/LXcbEventFilter.cpp
    src/desktop/LWinInfo.cpp
    src/desktop/LSession.cpp
    src/desktop/AppMenu.cpp
    src/desktop/LDesktop.cpp
    src/desktop/LDesktopBackground.cpp
    src/desktop/LDesktopPluginSpace.cpp
    src/desktop/LPanel.cpp
    src/desktop/desktop-plugins/LDPlugin.cpp
    src/desktop/desktop-plugins/NewDP.h
    src/desktop/desktop-plugins/applauncher/AppLauncherPlugin.cpp
    src/desktop/desktop-plugins/applauncher/AppLauncherPlugin.h
    src/desktop/desktop-plugins/applauncher/OutlineToolButton.h
    src/desktop/panel-plugins/LPPlugin.h
    src/desktop/panel-plugins/NewPP.h
    src/desktop/panel-plugins/LTBWidget.h
    src/desktop/panel-plugins/taskmanager/LTaskManagerPlugin.cpp
    src/desktop/panel-plugins/taskmanager/LTaskButton.cpp
    src/desktop/panel-plugins/systemtray/LSysTray.cpp
    src/desktop/panel-plugins/systemtray/TrayIcon.cpp
    src/desktop/panel-plugins/clock/LClock.cpp
    src/desktop/panel-plugins/desktopswitcher/LDesktopSwitcher.cpp
    src/desktop/panel-plugins/showdesktop/LHomeButton.cpp
    src/desktop/panel-plugins/appmenu/LAppMenuPlugin.cpp
    src/desktop/panel-plugins/applauncher/AppLaunchButton.cpp
    src/desktop/panel-plugins/spacer/LSpacer.h
    src/desktop/panel-plugins/line/LLine.h
    src/desktop/SystemWindow.cpp
    src/desktop/SystemWindow.ui
    src/desktop/desktop.qrc
)
target_include_directories(
    ${DESKTOP_SERVICE_NAME}
    PRIVATE
    src/desktop
    src/lib
    src/lib/lumina
    src/lib/hotkey
    src/lib/qtcopydialog
)
target_link_libraries(
    ${DESKTOP_SERVICE_NAME}
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    Qt5::Widgets
    Qt5::Concurrent
    Qt5::X11Extras
    ${LIB_NAME}
)

# org.dracolinux.Powerd
add_executable(
    ${POWERD_SERVICE_NAME}
    src/power/${POWERD_SERVICE_NAME}.cpp
)
target_include_directories(
    ${POWERD_SERVICE_NAME}
    PRIVATE
    src/lib
)
target_link_libraries(
    ${POWERD_SERVICE_NAME}
    Qt5::Core
    Qt5::DBus
    ${LIB_NAME}
)

# org.dracolinux.Power
add_executable(
    ${POWER_SERVICE_NAME}
    src/power/${POWER_SERVICE_NAME}.cpp
    src/power/${POWER_SERVICE_NAME}.Tray.cpp
)
target_include_directories(
    ${POWER_SERVICE_NAME}
    PRIVATE
    src/lib
    src/power
    ${XSS_INCLUDE}
)
target_link_libraries(
    ${POWER_SERVICE_NAME}
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    Qt5::Widgets
    Qt5::X11Extras
    ${XSS_LIB}
    ${LIB_NAME}
)

# org.dracolinux.Storage
add_executable(
    ${STORAGE_SERVICE_NAME}
    src/storage/${STORAGE_SERVICE_NAME}.cpp
    src/storage/${STORAGE_SERVICE_NAME}.Tray.cpp
)
target_include_directories(
    ${STORAGE_SERVICE_NAME}
    PRIVATE
    src/lib
    src/lib/lumina
)
target_link_libraries(
    ${STORAGE_SERVICE_NAME}
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    Qt5::Widgets
    ${LIB_NAME}
)

# org.dracolinux.XDG
add_executable(
    ${XDG_OPEN}
    src/session/xdg.cpp
    src/lib/lumina/AppDialog.h
    src/lib/lumina/AppDialog.ui
    src/desktop/logo.qrc
)
target_include_directories(
    ${XDG_OPEN}
    PRIVATE
    src/lib
    src/lib/lumina
)
target_link_libraries(
    ${XDG_OPEN}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    ${LIB_NAME}
)

# start-draco
add_executable(
    start-${PROJECT_NAME}
    src/session/main.cpp
    src/session/session.cpp
)
target_include_directories(
    start-${PROJECT_NAME}
    PRIVATE
    src/lib
    src/lib/lumina
)
target_link_libraries(
    start-${PROJECT_NAME}
    Qt5::Core
    Qt5::DBus
    ${LIB_NAME}
)

# draco-settings-x11
add_executable(
    ${PROJECT_NAME}-settings-x11
    src/settings/xconf/main.cpp
    src/settings/xconf/MainUI.cpp
    src/settings/xconf/ScreenSettings.cpp
    src/settings/xconf/MainUI.ui
)
target_include_directories(
    ${PROJECT_NAME}-settings-x11
    PRIVATE
    src/lib
    src/lib/lumina
)
target_link_libraries(
    ${PROJECT_NAME}-settings-x11
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    ${LIB_NAME}
)

# draco-settings
add_executable(
    ${PROJECT_NAME}-settings
    src/settings/config/GetPluginDialog.cpp
    src/settings/config/LPlugins.cpp
    src/settings/config/PanelWidget.cpp
    src/settings/config/main.cpp
    src/settings/config/mainWindow.cpp
    src/settings/config/pages/PageWidget.h
    src/settings/config/pages/getPage.cpp
    src/settings/config/pages/getPage.h
    src/settings/config/pages/page_keyboard.cpp
    src/settings/config/pages/page_power.cpp
    src/settings/config/pages/page_autostart.cpp
    src/settings/config/pages/page_defaultapps.cpp
    src/settings/config/pages/page_interface_panels.cpp
    src/settings/config/pages/page_main.cpp
    src/settings/config/pages/page_main.ui
    #src/settings/config/pages/page_session_locale.cpp
    src/settings/config/pages/page_session_options.cpp
    src/settings/config/pages/page_wallpaper.cpp
    src/lib/org.dracolinux.Power.SettingsWidget.cpp
    src/lib/lumina/AppDialog.h
    src/lib/lumina/AppDialog.ui
    src/settings/config/pages/page_keyboard.ui
    src/settings/config/pages/page_power.ui
    src/settings/config/pages/page_autostart.ui
    src/settings/config/pages/page_defaultapps.ui
    src/settings/config/pages/page_interface_panels.ui
    #src/settings/config/pages/pages/page_session_locale.ui
    src/settings/config/pages/page_session_options.ui
    src/settings/config/pages/page_wallpaper.ui
    src/settings/config/GetPluginDialog.ui
    src/settings/config/PanelWidget.ui
    src/settings/config/mainWindow.ui
)
target_include_directories(
    ${PROJECT_NAME}-settings
    PRIVATE
    src/lib
    src/lib/lumina
    src/settings/config
    src/settings/config/pages
)
target_link_libraries(
    ${PROJECT_NAME}-settings
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    ${LIB_NAME}
)

# draco-settings-wm
add_executable(
    ${PROJECT_NAME}-settings-wm
    src/settings/obconf/src/obconf-qt.cpp
    src/settings/obconf/src/maindialog.cpp
    src/settings/obconf/src/appearance.cpp
    src/settings/obconf/src/windows.cpp
    src/settings/obconf/src/mouse.cpp
    src/settings/obconf/src/moveresize.cpp
    src/settings/obconf/src/margins.cpp
    src/settings/obconf/src/desktops.cpp
    src/settings/obconf/src/tree.cpp
    src/settings/obconf/src/archive.cpp
    src/settings/obconf/src/theme.cpp
    src/settings/obconf/src/fontbutton.cpp
    src/settings/obconf/src/obconf.ui
)
target_include_directories(
    ${PROJECT_NAME}-settings-wm
    PRIVATE
    src/settings/obconf/src
    ${GLIB_INCLUDE_DIRS}
    ${OPENBOX_INCLUDE_DIRS}
)
target_link_libraries(
    ${PROJECT_NAME}-settings-wm
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::X11Extras
    ${GLIB_LIBRARIES}
    ${OPENBOX_LDFLAGS}
)

# docs
install(
    FILES
    docs/LICENSE
    docs/COPYING
    docs/COPYING.LIB
    docs/ChangeLog.md
    docs/README.md
    DESTINATION
    ${CMAKE_INSTALL_DOCDIR}/${PROJECT_NAME}-${PROJECT_VERSION}${PROJECT_VERSION_EXTRA}
)

# etc
set(DEFAULT_SYSCONFDIR "${CMAKE_INSTALL_PREFIX}/etc")
if(CMAKE_INSTALL_PREFIX STREQUAL "/usr")
    set(DEFAULT_SYSCONFDIR "/etc")
endif()

# install
configure_file(
    share/daemon/conf.in
    ${CMAKE_BINARY_DIR}/${POWERD_SERVICE_NAME}.conf
    @ONLY
)
configure_file(
    share/daemon/service.in
    ${CMAKE_BINARY_DIR}/${POWERD_SERVICE_NAME}.service
    @ONLY
)
configure_file(
    share/autostart/${POWER_SERVICE_NAME}.desktop.in
    ${CMAKE_BINARY_DIR}/${POWER_SERVICE_NAME}.desktop
    @ONLY
)
configure_file(
    share/autostart/${STORAGE_SERVICE_NAME}.desktop.in
    ${CMAKE_BINARY_DIR}/${STORAGE_SERVICE_NAME}.desktop
    @ONLY
)
configure_file(
    share/applications/settings.desktop.in
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-settings.desktop
    @ONLY
)
install(
    TARGETS
    ${LIB_NAME}
    DESTINATION
    ${LIB_INSTALL_DIR}
)
install(
    TARGETS
    ${DESKTOP_SERVICE_NAME}
    DESTINATION
    ${CMAKE_INSTALL_LIBEXECDIR}
)
install(
    TARGETS
    ${POWERD_SERVICE_NAME}
    DESTINATION
    ${CMAKE_INSTALL_LIBEXECDIR}
)
install(
    TARGETS
    ${POWER_SERVICE_NAME}
    DESTINATION
    ${CMAKE_INSTALL_LIBEXECDIR}
)
install(
    TARGETS
    ${STORAGE_SERVICE_NAME}
    DESTINATION
    ${CMAKE_INSTALL_LIBEXECDIR}
)
install(
    TARGETS
    ${XDG_OPEN}
    DESTINATION
    ${CMAKE_INSTALL_LIBEXECDIR}
)
install(
    TARGETS
    start-${PROJECT_NAME}
    DESTINATION
    ${CMAKE_INSTALL_BINDIR}
)
install(
    TARGETS
    ${PROJECT_NAME}-settings
    DESTINATION
    ${CMAKE_INSTALL_BINDIR}
)
install(
    TARGETS
    ${PROJECT_NAME}-settings-x11
    DESTINATION
    ${CMAKE_INSTALL_BINDIR}
)
install(
    TARGETS
    ${PROJECT_NAME}-settings-wm
    DESTINATION
    ${CMAKE_INSTALL_BINDIR}
)
install(
    FILES
    share/xsessions/${PROJECT_NAME}.desktop
    DESTINATION
    ${CMAKE_INSTALL_DATAROOTDIR}/xsessions
)
install(
    FILES
    ${CMAKE_BINARY_DIR}/${POWER_SERVICE_NAME}.desktop
    ${CMAKE_BINARY_DIR}/${STORAGE_SERVICE_NAME}.desktop
    DESTINATION
    ${DEFAULT_SYSCONFDIR}/xdg/autostart
)
install(
    FILES
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-settings.desktop
    DESTINATION
    ${CMAKE_INSTALL_DATAROOTDIR}/applications
)
install(
    FILES
    ${CMAKE_BINARY_DIR}/${POWERD_SERVICE_NAME}.conf
    DESTINATION
    ${DEFAULT_SYSCONFDIR}/dbus-1/system.d
)
install(
    FILES
    ${CMAKE_BINARY_DIR}/${POWERD_SERVICE_NAME}.service
    DESTINATION
    ${CMAKE_INSTALL_DATAROOTDIR}/dbus-1/system-services
)
install(
    FILES
    share/xinit/xinitrc.draco
    DESTINATION
    ${DEFAULT_SYSCONFDIR}/X11/xinit
)
