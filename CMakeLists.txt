#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
# Available under the 3-clause BSD license
# See the LICENSE file for full details
#

cmake_minimum_required(VERSION 3.0)
include(GNUInstallDirs)

project(draco VERSION 0.9.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/share/cmake")

set(LIB_NAME Draco)
set(PUBLISHER dracolinux)
set(DOMAIN org.${PUBLISHER})
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
add_definitions(-DDESKTOP_SERVICE_NAME="${DESKTOP_SERVICE_NAME}")
add_definitions(-DPOWER_SERVICE_NAME="${POWER_SERVICE_NAME}")
add_definitions(-DPOWERD_SERVICE_NAME="${POWERD_SERVICE_NAME}")
add_definitions(-DSTORAGE_SERVICE_NAME="${STORAGE_SERVICE_NAME}")
add_definitions(-DDAEMON_APP_NAME="${PUBLISHER}.Powerd")

if(CMAKE_BUILD_TYPE MATCHES RELEASE)
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

# qt
find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
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
add_library(
    ${LIB_NAME}
    SHARED

    # Lumina
    src/lib/lumina/DesktopSettings.cpp
    src/lib/lumina/LDesktopUtils.cpp
    src/lib/lumina/LIconCache.cpp
    src/lib/lumina/LInputDevice.cpp
    src/lib/lumina/LUtils.cpp
    src/lib/lumina/LuminaRandR-X11.cpp
    src/lib/lumina/LuminaX11.cpp
    src/lib/lumina/LuminaXDG.cpp
    src/lib/lumina/LFileInfo.cpp
    src/lib/lumina/ResizeMenu.cpp
    src/lib/lumina/XDGMime.cpp

    # Draco
    src/lib/draco.cpp
    src/lib/power_def.h
    src/lib/org.dracolinux.Power.Client.cpp
    src/lib/org.dracolinux.Power.Device.cpp
    src/lib/org.dracolinux.Power.HotPlugX11.cpp
    src/lib/org.dracolinux.Power.ScreenX11.cpp
    src/lib/org.dracolinux.Powerd.Manager.Backlight.cpp
    src/lib/org.dracolinux.Powerd.Manager.CPU.cpp
    src/lib/org.dracolinux.Powerd.Manager.RTC.cpp
    src/lib/org.dracolinux.Powerd.Manager.cpp
    src/lib/org.freedesktop.PowerManagement.cpp
    src/lib/org.freedesktop.ScreenSaver.cpp

    # QHotkey
    src/lib/hotkey/QHotkey/qhotkey.cpp
    src/lib/hotkey/QHotkey/qhotkey_x11.cpp
    src/lib/hotkey/QHotkey/qhotkey_p.h

    # qtcopydialog
    src/lib/qtcopydialog/qtcopydialog.cpp
    src/lib/qtcopydialog/qtfilecopier.cpp
    src/lib/qtcopydialog/qtcopydialog.ui
    src/lib/qtcopydialog/qtoverwritedialog.ui
    src/lib/qtcopydialog/qtotherdialog.ui

    # res
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
    src/lib/hotkey/QHotkey
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
    Qt5::Network
    Qt5::X11Extras
)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
else()
    set(LIB_INSTALL_DIR ${CMAKE_INSTALL_FULL_LIBDIR})
endif()
install(
    TARGETS
    ${LIB_NAME}
    DESTINATION
    ${LIB_INSTALL_DIR}
)

# org.dracolinux.Desktop
add_executable(
    ${DESKTOP_SERVICE_NAME}

    # desktop and panel
    src/desktop/main.cpp
    src/desktop/LXcbEventFilter.cpp
    src/desktop/LWinInfo.cpp
    src/desktop/LSession.cpp
    src/desktop/AppMenu.cpp
    src/desktop/LDesktop.cpp
    src/desktop/LDesktopBackground.cpp
    src/desktop/LDesktopPluginSpace.cpp
    src/desktop/LPanel.cpp

    # desktop plugins
    src/desktop/desktop-plugins/LDPlugin.cpp
    src/desktop/desktop-plugins/NewDP.h
    src/desktop/desktop-plugins/applauncher/AppLauncherPlugin.cpp
    src/desktop/desktop-plugins/applauncher/AppLauncherPlugin.h
    src/desktop/desktop-plugins/applauncher/OutlineToolButton.h

    # panel plugins
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
    src/lib/hotkey/QHotkey
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
install(
    TARGETS
    ${DESKTOP_SERVICE_NAME}
    DESTINATION
    ${CMAKE_INSTALL_LIBEXECDIR}
)

# org.dracolinux.Powerd
add_executable(
    ${POWERD_SERVICE_NAME}
    src/${POWERD_SERVICE_NAME}.cpp
)
target_include_directories(
    ${POWERD_SERVICE_NAME}
    PRIVATE
    src/lib
    src/lib/lumina
)
target_link_libraries(
    ${POWERD_SERVICE_NAME}
    Qt5::Core
    Qt5::DBus
    ${LIB_NAME}
)
install(
    TARGETS
    ${POWERD_SERVICE_NAME}
    DESTINATION
    ${CMAKE_INSTALL_LIBEXECDIR}
)

# start-draco-desktop
add_executable(
    start-${PROJECT_NAME}-desktop
    src/session/main.cpp
    src/session/session.cpp
)
target_include_directories(
    start-${PROJECT_NAME}-desktop
    PRIVATE
    src/lib
    src/lib/lumina
)
target_link_libraries(
    start-${PROJECT_NAME}-desktop
    Qt5::Core
    Qt5::DBus
    ${LIB_NAME}
)
install(
    TARGETS
    start-${PROJECT_NAME}-desktop
    DESTINATION
    ${CMAKE_INSTALL_BINDIR}
)
install(
    FILES
    share/xsessions/${PROJECT_NAME}.desktop
    DESTINATION
    ${CMAKE_INSTALL_DATAROOTDIR}/xsessions
)

# draco-xdg
# dracod
# draco-storage-manager
# draco-power-manager
# draco-settings-x11
# draco-settings-xdg
# draco-settings-desktop
# draco-settings-panel
# draco-settings-general

# docs
install(
    FILES
    docs/LICENSE
    #docs/ChangeLog
    docs/README.md
    DESTINATION
    ${CMAKE_INSTALL_DOCDIR}/${PROJECT_NAME}-${PROJECT_VERSION}
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
