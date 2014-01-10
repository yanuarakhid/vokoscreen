
#include "QvkCredits.h"

#include <QPixmap>
#include <QX11Info>
#include <QDebug>
#include <QPainter>
#include <QDesktopWidget>

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

    painter->drawPixmap( 0, 0, pixmap );

    QFont font;
    font.setPointSize( 10 );
    painter->setFont( font );
    
    int x = 155;
    int y = 155;
    int deltaX = 6;
    int deltaY = 15;
    
    painter->drawText( x - 1 * deltaX, y + 1 * deltaY, "Volker Kohaupt (developer)" );
    painter->drawText( x - 2 * deltaX, y + 2 * deltaY, "Michael Kohaupt (public relations, support)" ); 
    painter->drawText( x - 3 * deltaX, y + 3 * deltaY, "Dominique (maintainer for debian/ubuntu packages)" );
    painter->drawText( x - 4 * deltaX, y + 4 * deltaY, "Marc Kohaupt (technical ideas)" ); 
    painter->drawText( x - 5 * deltaX, y + 5 * deltaY, "Bersil (translations into Italian and Spanish)" );
    painter->drawText( x - 6 * deltaX, y + 6 * deltaY, "atilacamurca (brazilian portuguese translation)" ); 
    painter->drawText( x - 7 * deltaX, y + 7 * deltaY, "agaida (code reworking)" ); 
    painter->drawText( x - 8 * deltaX, y + 8 * deltaY, "Karel (dutch translation)" ); 
    painter->drawText( x - 9 * deltaX, y + 9 * deltaY, "Phan Anh (vietnamese translation)" ); 
    painter->drawText( x - 10 * deltaX, y + 10 * deltaY, "Fisiu (maintainer for packman pakages)" ); 
    painter->drawText( x - 11 * deltaX, y + 11 * deltaY, "Rob Frohne (code for timestamp)" ); 
    painter->drawText( x - 12 * deltaX, y + 12 * deltaY, "Fayssal Chamekh (arabic translation)" ); 
    painter->drawText( x - 13 * deltaX, y + 13 * deltaY, "Lowrider (russian translation)" ); 
    painter->drawText( x - 14 * deltaX, y + 14 * deltaY, "Fitoschido (spanish translation)" ); 
    painter->drawText( x - 15 * deltaX, y + 15 * deltaY, "Varga Gabor (hungrian translation)" ); 
    painter->drawText( x - 16 * deltaX, y + 16 * deltaY, "Arthur Titeica (romanian translation)" ); 
    painter->drawText( x - 17 * deltaX, y + 17 * deltaY, "Llumex003 (asturian translation)" ); 
    painter->drawText( x - 18 * deltaX, y + 18 * deltaY, "zeugma (turkish translation)" ); 
    painter->drawText( x - 18 * deltaX, y + 19 * deltaY, "Christoph Schenk (swedish translation)" ); 
    painter->drawText( x - 18 * deltaX, y + 20 * deltaY, "Mariuz (polish translation)" ); 
    painter->drawText( x - 18 * deltaX, y + 21 * deltaY, "Adam Kosik (czech translation)" ); 
    painter->drawText( x - 18 * deltaX, y + 22 * deltaY, "Brommer Matthieu ( french translation)" ); 
    
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
