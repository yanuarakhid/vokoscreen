#ifndef QvkCountdown_H 
#define QvkCountdown_H

#include <QObject>

class QvkCountdown: public QObject
{
    Q_OBJECT
public:
    QvkCountdown();
    virtual ~QvkCountdown();

    
public slots:
    void Countdown( int value );

  
private:

  
private slots:

  
signals:
  
  
};

#endif