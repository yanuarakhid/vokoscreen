#-------------------------------------------------
#
# Project created by QtCreator 2013-07-25T17:14:10
#
#-------------------------------------------------

QT       += gui x11extras

TARGET = qxtglobalshortcut
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    qxtglobal.cpp \
    qxtglobalshortcut.cpp

unix: SOURCES += qxtglobalshortcut_x11.cpp
win32: SOURCES += qxtglobalshortcut_win.cpp
macx: SOURCES += qxtglobalshortcut_mac.cpp

HEADERS += \
    qxtglobalshortcut.h \
    qxtglobalshortcut_p.h \
    qxtglobal.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

unix: LIBS += -lX11
