#ifndef QvkDbus_H 
#define QvkDbus_H

#include "ui_vokoscreen.h"

#include <QObject>

class QvkDbus: public QObject
{
    
Q_OBJECT

public:
  QvkDbus( Ui_screencast value );
  virtual ~QvkDbus();
  
public slots:

  
private slots: 
    void vokoscreenFinishLoaded();
    QString isVokoscreenLoaded();
    int startrecord();
    int stoprecord();
    int setFullScreen();
    int setWindow();
    int setArea();
    int setAreaReset();
    int setAudioOff();
    int setAudioOn();
    int setWebcamOn();
    int setWebcamOff();
    int setCountDown( int value );
    int setTab( int value );
    void quit();

   
signals:
    void close();
  
protected:  
  
  
private:
  Ui_screencast myUi;
  QString vokoscreenLoaded;
    
};

#endif
