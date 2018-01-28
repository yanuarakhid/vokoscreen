TEMPLATE = app
TARGET = vokoscreen

# Input
HEADERS += screencast.h

SOURCES += main.cpp \
           screencast.cpp

FORMS += QvkNoPlayerDialog.ui
FORMS += vokoscreen.ui

# In der screencast.qrc stehen *.qm Einträge, diese Dateien sind aber noch nicht
# im Unterverzeichnis "language" vorhanden da diese "eigentlich" erst mit make geniert werden,
# daher gibt es eine Fehlermeldung wenn der Befehl qmake aufgerufen und "RESOURCES += screencast.qrc" abgearbeitet wird.
# Um der Fehlermeldung entgegenzuwirken das keine *.qm Dateien vorhanden sind wird lrelease als Systemaufruf vorher aufgerufen.
# Das Script/Macro siehe weiter unten "# language packages" muß weiter bestehen bleiben damit "make clean" die *.qm Dateien löscht.

system(lrelease-qt5 language/vokoscreen_*.ts)

RESOURCES += screencast.qrc
                        
TRANSLATIONS = $$files(language/vokoscreen_*.ts)

# language packages

!isEmpty(TRANSLATIONS) {
  isEmpty(QMAKE_LRELEASE) {
    win32: QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
      else: QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease-qt5
  }
  isEmpty(TS_DIR):TS_DIR = language
  TSQM.name = lrelease-qt5 ${QMAKE_FILE_IN}
  TSQM.input = TRANSLATIONS
  TSQM.output = $$TS_DIR/${QMAKE_FILE_BASE}.qm
  TSQM.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN}
  TSQM.CONFIG = no_link 
  QMAKE_EXTRA_COMPILERS += TSQM
  PRE_TARGETDEPS += compiler_TSQM_make_all
}
  else: message(No translation files in project)

# Install paths
isEmpty(PREFIX) {
    PREFIX = /usr
}

image.path = $$PREFIX/share/pixmaps
image.files += applications/vokoscreen.png
desktop.path = $$PREFIX/share/applications
desktop.files += applications/vokoscreen.desktop
man.path = $$PREFIX/share
man.files += man
target.path = $$PREFIX/bin

INSTALLS += target image desktop man

# Clean target
QMAKE_CLEAN += $$TARGET */*~

CONFIG += link_pkgconfig

# libqxt
include(libqxtQt5/libqxt.pri)

# audio
include(audio/audio.pri)
PKGCONFIG += alsa

# send
include(send/send.pri)

# webcam
include(webcam/webcam.pri)

# settings
include(settings/settings.pri)

# showkey
include(showkey/showkey.pri)

# showclick
include(showclick/showclick.pri)

# magnifier
include(magnifier/magnifier.pri)

# credits
include(credits/credits.pri)

# version
include(version/version.pri)

# winInfo
include(winInfo/winInfo.pri)

# region
include(region/regionselection.pri)

# formatsAndCodecs
include(formatsAndCodecs/formatsAndCodecs.pri)

# countdown
include(countdown/countdown.pri)

# pulse
include(pulse/pulse.pri)

# log
include(log/log.pri)

QT += core gui widgets x11extras network testlib dbus multimedia multimediawidgets

DBUS_ADAPTORS += vokoscreen.xml
