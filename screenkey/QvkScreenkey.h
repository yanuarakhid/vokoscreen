#ifndef QVKSCREENKEY_H 
#define QVKSCREENKEY_H

#include <QCoreApplication>
#include <QObject>
#include <QTest>

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
