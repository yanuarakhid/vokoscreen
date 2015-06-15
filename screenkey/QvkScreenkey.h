#ifndef QVKSCREENKEY_H 
#define QVKSCREENKEY_H

#include <QCoreApplication>
#include <QObject>
#include <QTest>

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <stdio.h>
#include <string.h>

class QvkScreenkey: public QObject
{ 
Q_OBJECT
public:
  QvkScreenkey();
  virtual ~QvkScreenkey();
  void readKey();
  void setScreenkeyOff();
  void setScreenkeyOn();
  
  
public slots:

  
private slots:


signals:
  void pressedKey( QString value );

  
protected:  

  
private:
    bool onOff;

};

#endif
