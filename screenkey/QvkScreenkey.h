#ifndef QVKSCREENKEY_H 
#define QVKSCREENKEY_H

#include <QCoreApplication>
#include <QObject>
#include <QTest>
#include <QThread>

class QvkScreenkey: public QThread
{ 
Q_OBJECT
public:
  QvkScreenkey();
  virtual ~QvkScreenkey();
  void setScreenkeyOff();
  void setScreenkeyOn();
  
  
public slots:

  
private slots:


signals:
  void pressedKey( QString value );

  
protected:  
  void run();
  
private:
    bool onOff;

};

#endif
