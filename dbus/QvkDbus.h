#ifndef QvkDbus_H
#define QvkDbus_H

#include <QObject>
#include <QDebug>

#include "ui_vokoscreen.h"

class QvkDbus: public QObject
{
    Q_OBJECT
    
public:
    QvkDbus();
    QvkDbus(Ui_screencast GUI );
    virtual ~QvkDbus();
    Ui_screencast myUi;
   
signals:

    
public slots:
    void startrecord();
    //void stoprecord();
    //void setAudioOn();
    //void setAudioOff();

    
private slots:
  
  
private:

  
protected:  

};

#endif
