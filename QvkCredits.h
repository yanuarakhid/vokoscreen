
#ifndef QVKCREDITS_H
#define QVKCREDITS_H

#include <QApplication>
#include <QMainWindow> 
#include <QLabel>
#include <QPixmap>
#include <QX11Info>
#include <QDebug>
#include <QPainter>
#include <QDesktopWidget>
#include <QStringList>
#include <QFile>

class QvkCredits: public QMainWindow
{
    Q_OBJECT
    
public:
    QvkCredits();
    virtual ~QvkCredits();
    QPainter * painter;
    QStringList creditsStringlist;
    
signals:
    void closeCredits();

    
private slots:
  
  
private:

  
protected:  
  void mousePressEvent ( QMouseEvent * event );
  void paintEvent( QPaintEvent *event );
  void closeEvent( QCloseEvent * event );

};

#endif
