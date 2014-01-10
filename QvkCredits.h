
#ifndef QVKCREDITS_H
#define QVKCREDITS_H

#include <QApplication>
#include <QMainWindow> 
#include <QLabel>

class QvkCredits: public QMainWindow
{
    Q_OBJECT
    
public:
    QvkCredits();
    virtual ~QvkCredits();
    
    //QLabel * labelPaper;
    QPainter * painter;
  
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
