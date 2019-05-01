#
# Draco Desktop Environment <https://desktop.dracolinux.org>
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

set(SERVICE_NAME org.dracolinux.Desktop)
set(
    SERVICE_USER
    "root"
    CACHE
    STRING
    "Run ${PROJECT_NAME}d as this user (needs access to /sys)."
)
set(
    SERVICE_GROUP
    "power"
    CACHE
    STRING
    "group that have access to ${PROJECT_NAME}d, this should be all desktop users."
)

add_definitions(-DDESKTOP_APP="${PROJECT_NAME}")
add_definitions(-DDESKTOP_APP_NAME="Draco")
add_definitions(-DDESKTOP_APP_DOMAIN="dracolinux.org")
add_definitions(-DDESKTOP_APP_VERSION="${PROJECT_VERSION}")
add_definitions(-DDESKTOP_SERVICE_NAME="${SERVICE_NAME}")

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
    Draco
    SHARED
    src/lib/DesktopSettings.cpp
    src/lib/LDesktopUtils.cpp
    src/lib/LIconCache.cpp
    src/lib/LInputDevice.cpp
    src/lib/LUtils.cpp
    src/lib/LuminaRandR-X11.cpp
    src/lib/LuminaX11.cpp
    src/lib/LuminaXDG.cpp
    src/lib/LFileInfo.cpp
    src/lib/ResizeMenu.cpp
    src/lib/XDGMime.cpp
    src/lib/draco.cpp
    src/lib/hotkey/QHotkey/qhotkey.cpp
    src/lib/hotkey/QHotkey/qhotkey_x11.cpp
    src/lib/hotkey/QHotkey/qhotkey_p.h
    src/lib/fm/libfm/qtcopydialog/qtcopydialog.cpp
    src/lib/fm/libfm/qtcopydialog/qtfilecopier.cpp
    src/lib/fm/libfm/qtcopydialog/qtcopydialog.ui
    src/lib/fm/libfm/qtcopydialog/qtoverwritedialog.ui
    src/lib/fm/libfm/qtcopydialog/qtotherdialog.ui
    share/share.qrc
)
set_target_properties(
    Draco
    PROPERTIES
    VERSION
    ${PROJECT_VERSION}
)
set_target_properties(
    Draco
    PROPERTIES
    SOVERSION
    ${PROJECT_VERSION_MAJOR}
)
target_include_directories(
    Draco
    PRIVATE
    src/lib
    src/lib/fm/libfm/qtcopydialog
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
    Draco
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
    Draco
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
install(
    TARGETS
    Draco
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}
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
)
target_link_libraries(
    start-${PROJECT_NAME}-desktop
    Qt5::Core
    Qt5::DBus
    Draco
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

# draco-desktop
add_executable(
    ${PROJECT_NAME}-desktop
    src/desktop/main.cpp
    src/desktop/LXcbEventFilter.cpp
    src/desktop/LSession.cpp
    src/desktop/LDesktop.cpp
    src/desktop/LDesktopBackground.cpp
    src/desktop/LDesktopPluginSpace.cpp
    src/desktop/LPanel.cpp
    src/desktop/LWinInfo.cpp
    src/desktop/AppMenu.cpp
    src/desktop/SystemWindow.cpp
    src/desktop/LWinInfo.cpp
    src/desktop/desktop-plugins/LDPlugin.cpp
    src/desktop/panel-plugins/LPPlugin.h
    src/desktop/panel-plugins/NewPP.h
    src/desktop/panel-plugins/LTBWidget.h
    src/desktop/desktop-plugins/NewDP.h
    src/desktop/SystemWindow.ui
    src/desktop/desktop-plugins/applauncher/AppLauncherPlugin.cpp
    src/desktop/desktop-plugins/applauncher/AppLauncherPlugin.h
    src/desktop/desktop-plugins/applauncher/OutlineToolButton.h
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
    src/desktop/desktop.qrc
)
target_include_directories(
    ${PROJECT_NAME}-desktop
    PRIVATE
    src/desktop
    src/lib
    src/lib/fm/libfm/qtcopydialog
    src/lib/hotkey/QHotkey
    src/desktop
    src/desktop/desktop-plugins
    src/desktop/panel-plugins
)
target_link_libraries(
    ${PROJECT_NAME}-desktop
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    Qt5::Widgets
    Qt5::Concurrent
    Qt5::X11Extras
    Draco
)
install(
    TARGETS
    ${PROJECT_NAME}-desktop
    DESTINATION
    ${CMAKE_INSTALL_BINDIR}
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
    ${CMAKE_INSTALL_DOCDIR}/${PROJECT_NAME}-desktop-${PROJECT_VERSION}
)

# etc
set(DEFAULT_SYSCONFDIR "${CMAKE_INSTALL_PREFIX}/etc")
if(CMAKE_INSTALL_PREFIX STREQUAL "/usr")
    set(DEFAULT_SYSCONFDIR "/etc")
endif()

# install
configure_file(
    src/lib/power/share/conf.in
    ${CMAKE_BINARY_DIR}/${SERVICE_NAME}.conf
    @ONLY
)
configure_file(
    src/lib/power/share/service.in
    ${CMAKE_BINARY_DIR}/${SERVICE_NAME}.service
    @ONLY
)
install(
    FILES
    ${CMAKE_BINARY_DIR}/${SERVICE_NAME}.conf
    DESTINATION
    ${DEFAULT_SYSCONFDIR}/dbus-1/system.d
)
install(
    FILES
    ${CMAKE_BINARY_DIR}/${SERVICE_NAME}.service
    DESTINATION
    ${CMAKE_INSTALL_DATAROOTDIR}/dbus-1/system-services
)
