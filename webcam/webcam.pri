INCLUDEPATH	+= $$PWD
DEPENDPATH      += $$PWD
                   
HEADERS += $$PWD/QvkCapturethread.h \
           $$PWD/QvkWebcamWatcher.h \
           $$PWD/QvkWebcamController.h \
           $$PWD/QvkWebcamWindow.h \
           $$PWD/QvkWebcamBusyDialog.h

SOURCES += $$PWD/QvkCapturethread.cpp \
           $$PWD/QvkWebcamWatcher.cpp \
           $$PWD/QvkWebcamController.cpp \
           $$PWD/QvkWebcamWindow.cpp \
           $$PWD/QvkWebcamBusyDialog.cpp
           
FORMS += $$PWD/QvkWebcamBusyDialog.ui
