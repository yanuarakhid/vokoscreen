#ifndef QvkDbus_H 
#define QvkDbus_H

#include "ui_vokoscreen.h"

#include <QObject>

class QvkDbus: public QObject
{
    
Q_OBJECT

public:
  QvkDbus();
  QvkDbus( Ui_screencast value );
  virtual ~QvkDbus();
  
public slots:
  QString showAllMethods();

  
private slots: 
    QString isVokoscreenLoaded();
    QString startrecord();
    QString stoprecord();
    QString FullScreen();
    QString Window();
    QString Area();
    QString AreaReset();
    QString MagnifierOn();
    QString MagnifierOff();
    QString CountDown(QString value );

    QString AudioOff();
    QString AudioOn();

    QString CameraOn();
    QString CameraOff();
    QString Tab(QString value );

    void quit();

   
signals:
    void close();
  
protected:  
  
  
private:
  Ui_screencast myUi;
    
};

#endif
