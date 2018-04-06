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
    void vokoscreenFinishLoaded();
    int isVokoscreenLoaded();
    int startrecord();
    int stoprecord();
    int FullScreen();
    int Window();
    int Area();
    int AreaReset();
    int AudioOff();
    int AudioOn();
    QString WebcamOn();
    QString WebcamOff();
    QString CountDown(QString value );
    QString Tab(QString value );
    void quit();

   
signals:
    void close();
  
protected:  
  
  
private:
  Ui_screencast myUi;
  int vokoscreenLoaded = 1;
    
};

#endif
