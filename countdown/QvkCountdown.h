#ifndef QvkCountdown_H 
#define QvkCountdown_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QLabel>

class QvkCountdown: public QWidget
{
    Q_OBJECT
public:
    QvkCountdown( int value );
    virtual ~QvkCountdown();

    
public slots:

  
private:
    int countValue;
    int oldCountValue;
    int gradValue;
    QTimer *timer;
    QTimer *animationTimer;
    QPainter painter;

    
private slots:
   void updateTimer();
   void updateAnimationTimer();


protected:
    void paintEvent( QPaintEvent *event );
  
  
signals:
  
  
};

#endif