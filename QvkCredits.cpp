
#include "QvkCredits.h"

QvkCredits::QvkCredits()
{
  setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
  if( QX11Info::isCompositingManagerRunning() )
    setAttribute( Qt::WA_TranslucentBackground, true );

  resize( 600, 600 );
  
  QDesktopWidget *desktop = QApplication::desktop();  
  int x = ( desktop->width() - width() ) / 2;
  int y = ( desktop->height() - height() ) / 2;
  move( x, y );
  
  painter =  new QPainter();
  
  QFile file( ":/CREDITS");
  file.open( QIODevice::ReadOnly | QIODevice::Text );
  while ( !file.atEnd() )
  {
    QString string = file.readLine();
    string = string.simplified();
    if ( string > "" )
      creditsStringlist.append( string );
  } 
  file.close();
  creditsStringlist.removeFirst();
  creditsStringlist.removeLast();
  creditsStringlist.insert( 0, "Michael Kohaupt (public relations, support)" );
  creditsStringlist.insert( 0, "Volker Kohaupt (developer by vokoscreen)" );
}

QvkCredits::~QvkCredits()
{
  
  delete painter;
}

void QvkCredits::paintEvent( QPaintEvent *event )
{
  (void)event;
  painter->begin( this );

    QPixmap pixmap( 0, 0 );
    pixmap.load( ":/pictures/Scroll.png" );
    pixmap = pixmap.scaledToWidth( 550, Qt::SmoothTransformation );

    QFont font;
    font.setPointSize( 10 );
    font.setItalic( true );
    
    int x = 155;
    int y = 155;
    int deltaX = 6;
    int deltaY = 15;
    
    painter->setPen( Qt::darkYellow );
    painter->setFont( font );
    painter->drawPixmap( 0, 0, pixmap );

    painter->drawText( x - 1 * deltaX, y + 1 * deltaY, creditsStringlist.value( 0 ) );
    painter->drawText( x - 2 * deltaX, y + 2 * deltaY, creditsStringlist.value( 1 ) ); 
    painter->drawText( x - 3 * deltaX, y + 3 * deltaY, creditsStringlist.value( 2 ) );
    painter->drawText( x - 4 * deltaX, y + 4 * deltaY, creditsStringlist.value( 3 ) ); 
    painter->drawText( x - 5 * deltaX, y + 5 * deltaY, creditsStringlist.value( 4 ) );
    painter->drawText( x - 6 * deltaX, y + 6 * deltaY, creditsStringlist.value( 5 ) ); 
    painter->drawText( x - 7 * deltaX, y + 7 * deltaY, creditsStringlist.value( 6 ) ); 
    painter->drawText( x - 8 * deltaX, y + 8 * deltaY, creditsStringlist.value( 7 ) ); 
    painter->drawText( x - 9 * deltaX, y + 9 * deltaY, creditsStringlist.value( 8 ) ); 
    painter->drawText( x - 10 * deltaX, y + 10 * deltaY, creditsStringlist.value( 9 ) ); 
    painter->drawText( x - 11 * deltaX, y + 11 * deltaY, creditsStringlist.value( 10 ) ); 
    painter->drawText( x - 12 * deltaX, y + 12 * deltaY, creditsStringlist.value( 11 ) ); 
    painter->drawText( x - 13 * deltaX, y + 13 * deltaY, creditsStringlist.value( 12 ) ); 
    painter->drawText( x - 14 * deltaX, y + 14 * deltaY, creditsStringlist.value( 13 ) ); 
    painter->drawText( x - 15 * deltaX, y + 15 * deltaY, creditsStringlist.value( 14 ) ); 
    painter->drawText( x - 16 * deltaX, y + 16 * deltaY, creditsStringlist.value( 15 ) ); 
    painter->drawText( x - 17 * deltaX, y + 17 * deltaY, creditsStringlist.value( 16 ) ); 
    painter->drawText( x - 18 * deltaX, y + 18 * deltaY, creditsStringlist.value( 17 ) ); 
    painter->drawText( x - 18 * deltaX, y + 19 * deltaY, creditsStringlist.value( 18 ) ); 
    painter->drawText( x - 18 * deltaX, y + 20 * deltaY, creditsStringlist.value( 19 ) ); 
    painter->drawText( x - 18 * deltaX, y + 21 * deltaY, creditsStringlist.value( 20 ) ); 
    painter->drawText( x - 18 * deltaX, y + 22 * deltaY, creditsStringlist.value( 21 ) ); 
    painter->drawText( x - 18 * deltaX, y + 23 * deltaY, creditsStringlist.value( 22 ) ); 
    painter->drawText( x - 18 * deltaX, y + 24 * deltaY, creditsStringlist.value( 23 ) ); 
    
  painter->end();
}


void QvkCredits::mousePressEvent ( QMouseEvent * event )
{
  (void)event;
  close();
}


/**
 *  closeEvent wird ausgelößt wenn das fenster geschloßen wird
 */
void QvkCredits::closeEvent( QCloseEvent * event )
{
  //qDebug() << "Begin void QvkCredits::closeEvent( QCloseEvent * event ) *****************************";
  (void)event;
  emit closeCredits();
  //qDebug() << "End   void QvkCredits::closeEvent( QCloseEvent * event ) *****************************";
}
