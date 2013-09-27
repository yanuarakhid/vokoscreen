Summary: Screencasting
Name: vokoscreen
Version: 1.7.14
Release: 0
License: GPL-2.0
Group: Productivity/Multimedia/CD/Record
URL: http://www.kohaupt-online.de/hp

# screencast::ffmpeg
Requires: ffmpeg 

# screencast::mkvmerge
Requires: mkvtoolnix

BuildRequires: libqt4-devel alsa-devel opencv-devel

BuildRoot: %{_tmppath}/%{name}-%{version}-build
Source: %{name}-%{version}.tar.gz

%description
vokoscreen is an easy to use screencast creator to record educational
videos, live recordings of browser, installation, videoconferences, etc.

%prep
# Quellen entpacken und vorbereiten 
%setup

%build
qmake
make

%install
install -D -m 755 vokoscreen %{buildroot}%{_bindir}/vokoscreen
install -D -m 644 applications/%{name}.png %{buildroot}%{_datadir}/pixmaps/%{name}.png
install -D -m 644 applications/%{name}.desktop %{buildroot}%{_datadir}/applications/%{name}.desktop
install -D -m 644 man/man1/%{name}.1.gz %{buildroot}%{_mandir}/man1/%{name}.1.gz

%files
%defattr(-, root, root)
%{_bindir}/vokoscreen
%{_datadir}/pixmaps/%{name}.png
%{_datadir}/applications/%{name}.desktop
%{_mandir}/man1/%{name}.1.gz

%changelog
* Sun Mar 21 2012 Volker Kohaupt <vkohaupt@freenet.de> 1.6.3
- new Version
