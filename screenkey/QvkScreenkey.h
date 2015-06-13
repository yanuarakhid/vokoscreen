#ifndef QVKSCREENKEY_H 
#define QVKSCREENKEY_H

#include <QtGui>
#include <QObject>

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
  bool onOff;
  
public slots:
  void setScreenkeyOff();
  void setScreenkeyOn();

  
private slots:


signals:

  
protected:  

  
private:
  
};

#endif
