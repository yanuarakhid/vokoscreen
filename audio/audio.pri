INCLUDEPATH	+= $$PWD
DEPENDPATH      += $$PWD
HEADERS		+= $$PWD/QvkAlsaWatcher.h \
                   $$PWD/QvkAlsaDevice.h \
                   $$PWD/alsa_device.h
                   
SOURCES		+= $$PWD/QvkAlsaWatcher.cpp \
                   $$PWD/QvkAlsaDevice.cpp \
                   $$PWD/alsa_device.c

