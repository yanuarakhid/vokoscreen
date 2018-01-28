INCLUDEPATH	+= $$PWD
DEPENDPATH      += $$PWD
                   
HEADERS += $$PWD/QvkWebcamController.h \
           $$PWD/QvkWebcamWindow.h \
           $$PWD/QvkWebcamWatcher.h \
           $$PWD/QvkVideoSurface.h
           
           
SOURCES += $$PWD/QvkWebcamController.cpp \
           $$PWD/QvkWebcamWindow.cpp \
           $$PWD/QvkWebcamWatcher.cpp
          
FORMS += $$PWD/QvkWebcamBusyDialog.ui          