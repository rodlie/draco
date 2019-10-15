%define debug_package %{nil}

Name: draco
Version: 0.9
Release: 1%{?dist}
Summary: Draco Desktop Environment

License: GPL-2.0
URL: https://dracolinux.org
Source0: %{name}-%{version}.tar.gz

BuildRequires: qt5-qtbase-devel qt5-qtx11extras-devel cmake3 gcc-c++ pkg-config libXScrnSaver-devel libXdamage-devel libXrandr-devel libxcb-devel xcb-util-devel xcb-util-wm-devel xcb-util-image-devel
Requires: xdg-utils shared-mime-info hicolor-icon-theme adwaita-icon-theme openbox xscreensaver qt5ct xorg-x11-xinit upower udisks udisks2 xorg-x11-xkb-utils xorg-x11-server-utils

%description
Simple Qt desktop environment.

%prep
%setup -q

%build
mkdir build && cd build
cmake3 -DCMAKE_INSTALL_PREFIX=%{_prefix} -DCMAKE_INSTALL_LIBDIR=%{_lib} -DCMAKE_BUILD_TYPE=Release ..

%install
cd build
make DESTDIR=%{buildroot} install

%files
%defattr(-,root,root,-)
%{_sysconfdir}/X11/xinit/xinitrc.draco
%{_sysconfdir}/dbus-1/system.d/org.dracolinux.Powerd.conf
%{_sysconfdir}/xdg/autostart/org.dracolinux.Power.desktop
%{_sysconfdir}/xdg/autostart/org.dracolinux.Storage.desktop
%{_bindir}/draco-settings
%{_bindir}/draco-settings-x11
%{_bindir}/start-draco
%{_docdir}/%{name}-*
%{_libdir}/libDraco.so*
%{_libexecdir}/org.dracolinux.Desktop
%{_libexecdir}/org.dracolinux.Power
%{_libexecdir}/org.dracolinux.Powerd
%{_libexecdir}/org.dracolinux.Storage
%{_libexecdir}/org.dracolinux.XDG
%{_datadir}/applications/draco-settings-x11.desktop
%{_datadir}/applications/draco-settings.desktop
%{_datadir}/dbus-1/system-services/org.dracolinux.Powerd.service
%{_datadir}/xsessions/draco.desktop

%changelog
