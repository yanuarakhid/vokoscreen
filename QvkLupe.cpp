#include "QvkLupe.h"
#include <QPixmap>
#include <QDesktopWidget>
#include <QDebug>
#include <QApplication>
#include <QTimer>

using namespace std;


// Lupe Rund, Qadratisch, Oval

//QvkLupe::QvkLupe( int width, int height, int framewidth )
QvkLupe::QvkLupe()
{
  QString lupeVersion = "1.0.5";
  int faktor = 2;
  distanzX = 50;
  distanzY = 50;
  resize( 2 * distanzX * faktor, 2 * distanzY * faktor );
  setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
  border = 3;

  label = new QLabel( this );
  label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
  label->setAlignment( Qt::AlignCenter );
  label->setScaledContents( true );
  label->show();
 
  QTimer *timer = new QTimer( this );
  connect( timer, SIGNAL( timeout() ), this, SLOT( mytimer() ) );
  timer->start( 40 );  
  
}


int QvkLupe::NewDistanzXLeft()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();
  
  int newDistanzX = ( ( desk->screenGeometry().width() / 2 ) - cursor.pos().x() ) * 
                 ( distanzX + ( width() / 2 ) ) / 
                 ( desk->screenGeometry().width() / 2 - distanzX ) - 
                 ( width() / 2 );
  return  newDistanzX;
}


int QvkLupe::NewDistanzXRight()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();
  
  int newDistanX = ( ( desk->screenGeometry().width() / 2 ) - cursor.pos().x() ) * 
                 ( -distanzX - ( width() / 2 ) ) / 
                 ( desk->screenGeometry().width() / 2 - distanzX ) - 
                 ( width() / 2 );
  return  newDistanX;
}


QvkLupe::~QvkLupe()
{
}

void QvkLupe::setLupe()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();


  // Lupe an oberen linke Ecke setzen
  if ( ( cursor.pos().x() < distanzX ) and ( cursor.pos().y() <  distanzY ) )
  {
    move( 2 * distanzX,  2 * distanzY );
    return;
  }

  // Lupe obere rechte Ecke setzen 
  if ( ( cursor.pos().x() > ( desk->screenGeometry().width() - distanzX ) ) and ( cursor.pos().y() < distanzY ) )
  {
    move( desk->screenGeometry().width() - 2 * distanzX - width(), 2 * distanzY);
    return;
  }
  
  // Lupe am oberen Rand setzen
  // Linke Hälfte am oberen Rand
  if ( ( cursor.pos().y() < distanzY ) and ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) )
  {
    move( cursor.pos().x() + NewDistanzXLeft(), 2 * distanzY );
    return;
  }
  // Rechte Hälfte am oberen Rand
  if ( ( cursor.pos().y() < distanzY ) and ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) )
  {
    move( cursor.pos().x() - NewDistanzXRight() - width(), 2 * distanzY );
    return;
  }

  // Lupe an untere rechte Ecke setzen
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanzX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanzY ) )
  {
      move( desk->screenGeometry().width() - ( 2 * distanzX + width() ), desk->screenGeometry().height() - ( 2 * distanzY + height() ) );
      return; 
  }
  
  // Lupe am rechten Rand setzen
  // Obere Hälfte am rechten Rand
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanzX ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) )// div 2
  {
    move( desk->screenGeometry().width() - ( 2 * distanzX + width() ), cursor.pos().y() + 1 * distanzY );
    return;
  }
  // untere Hälfte am rechten Rand
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanzX ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
  {
    move( desk->screenGeometry().width() - ( 2 * distanzX + width() ), cursor.pos().y() - distanzY - height() );
    return;
  }

  // Lupe an linken unteren Ecke setzen
  if ( ( cursor.pos().x() < distanzX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanzY ) )
  {
    move( 2 * distanzX, desk->screenGeometry().height() - 2 * distanzY - height() );
    return; 
  }
  
  // Lupe am unteren Rand setzen
  // Linke Hälfte unterer Rand
  if ( ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanzY ) )
  {
    move( cursor.pos().x() + NewDistanzXLeft(), desk->screenGeometry().height() - ( 2 * distanzY + height() ) );
    return; 
  }
  // Rechte Hälfte unterer Rand
  if ( ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanzY ) )
  {
    move( cursor.pos().x() - NewDistanzXRight() - width(), desk->screenGeometry().height() - 2 * distanzY - height() );
    return; 
  }
  
  // Lupe am linken Rand setzen
  // Obere Hälfte am linken Rand
  if ( ( cursor.pos().x() < distanzX ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) ) // div 2
    
  {
    move( 2 * distanzX, cursor.pos().y() + distanzY );
    return;
  }
  // Untere Hälfte am linken Rand
  if ( ( cursor.pos().x() < distanzX ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
  {
    move( 2 * distanzX, cursor.pos().y() - distanzY - height() );
    return; 
  }
  
  // Linke obere Hälfte 
  if ( ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) ) // div 2
    move( cursor.pos().x() + NewDistanzXLeft(), cursor.pos().y() + distanzY );
  
  // Rechte obere Hälfte
  if ( ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) )
    move( cursor.pos().x() - NewDistanzXRight() - width(), cursor.pos().y() + distanzY );
  
  // Linke untere Hälfte
  if ( ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
    move( cursor.pos().x() + NewDistanzXLeft(), cursor.pos().y() - distanzY - height() );
  
  // Rechte untere Hälfte
  if ( ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
    move( cursor.pos().x() - NewDistanzXRight() -width(), cursor.pos().y() - distanzY - width() );
}


void QvkLupe::mytimer()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();
//    Qt::Window | Qt::WindowStaysOnTopHint

  setLupe();

  // Obere linke Ecke
  if ( ( cursor.pos().x() < distanzX ) and ( cursor.pos().y() <  distanzY ) )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  0, 
					  0, 
					  2 * distanzX ,
					  2 * distanzY );
    label->setPixmap( originalPixmap );    
    return; 
  }
  
  // Obere rechte Ecke
  if ( ( cursor.pos().x() > ( desk->screenGeometry().width() - distanzX ) ) and ( cursor.pos().y() < distanzY ) )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  desk->screenGeometry().width() - 2 * distanzX, 
					  0, 
					  2 * distanzX ,
					  2 * distanzY );
    label->setPixmap( originalPixmap );    
    return; 
  }
  
  // Linke untere Ecke
  if ( ( cursor.pos().x() < distanzX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanzY ) )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  0, 
					  desk->screenGeometry().height() - 2 * distanzY, 
					  2 * distanzX ,
					  2 * distanzY );
    label->setPixmap( originalPixmap );    
    return; 
  }
  
  // Rechte untere Ecke  
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanzX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanzY ) )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  desk->screenGeometry().width() - 2 * distanzX, 
					  desk->screenGeometry().height() - 2 * distanzY, 
					  2 * distanzX ,
					  2 * distanzY );
    label->setPixmap( originalPixmap );    
    return; 
  }  
  
  // Unterer Rand
  if ( cursor.pos().y() > desk->screenGeometry().height() - distanzY )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  cursor.pos().x() - distanzX, 
					  desk->screenGeometry().height() - 2 * distanzY, 
					  2 * distanzX , 
					  2 * distanzY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Oberen Rand
  if ( cursor.pos().y() < distanzY )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  cursor.pos().x() - distanzX, 
					  0, 
					  2 * distanzX, 
					  2 * distanzY );
    label->setPixmap( originalPixmap );
    return;
  }
  
  // Rechter Rand
  if ( cursor.pos().x() > desk->screenGeometry().width() - distanzX )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  desk->screenGeometry().width() - 2 * distanzX, 
					  cursor.pos().y() - distanzY, 
					  2 * distanzX ,
					  2 * distanzY );
    label->setPixmap( originalPixmap );
    return;
  }
  
  // Linker Rand
  if ( cursor.pos().x() < distanzX )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  0, 
					  cursor.pos().y() - distanzY, 
					  2 * distanzX ,
					  2 * distanzY );
    label->setPixmap( originalPixmap );    
    return;
  }
  
  // Fläche
  QPixmap originalPixmap = QPixmap();
  originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					cursor.pos().x() - distanzX, 
					cursor.pos().y() - distanzY, 
					2 * distanzX , 
					2 * distanzY );
  label->setPixmap( originalPixmap );
}
