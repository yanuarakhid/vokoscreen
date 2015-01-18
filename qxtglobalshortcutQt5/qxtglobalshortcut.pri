INCLUDEPATH	+= $$PWD
DEPENDPATH      += $$PWD

SOURCES += $$PWD/qxtglobal.cpp \
           $$PWD/qxtglobalshortcut.cpp

unix: SOURCES  += $$PWD/qxtglobalshortcut_x11.cpp
win32: SOURCES += $$PWD/qxtglobalshortcut_win.cpp
macx: SOURCES  += $$PWD/qxtglobalshortcut_mac.cpp

HEADERS += $$PWD/qxtglobalshortcut.h \
           $$PWD/qxtglobalshortcut_p.h \
           $$PWD/qxtglobal.h

unix: LIBS += -lX11
