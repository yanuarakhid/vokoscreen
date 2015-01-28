#ifndef QvkWidgetcircle_H 
#define QvkWidgetcircle_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QDebug>

class QvkWidgetcircle: public QWidget
{
Q_OBJECT
public:    
    virtual ~QvkWidgetcircle();
    QvkWidgetcircle( QWidget *parent );

    
public:

  
public slots:
  void setColor( const QColor color );
  void unsetPoint();
  void setPoint();

  
private:
  QColor widgetColor;
  bool set;

private slots:

  
protected:
  void paintEvent( QPaintEvent *event );
  void mouseReleaseEvent ( QMouseEvent * event );

  
signals:
  void clicked( bool value );

  
};

#endif // QvkWidgetcircle_H