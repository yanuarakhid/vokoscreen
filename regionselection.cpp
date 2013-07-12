/* vokoscreen - A desktop recorder
 * Copyright (C) 2011-2013 Volker Kohaupt
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "regionselection.h" 

#include <QRegion>
#include <QCursor>

using namespace std;


/**
 * int x : 
 * int y :
 * int width :
 * int height :
 * int framewidth :
 */
regionselection::regionselection( int x, int y, int width, int height, int framewidth )
{
  
  qDebug() << tr( "Das ist ein Test Gruss Volker" );
  
  setAttribute( Qt::WA_TranslucentBackground );
  
  setMouseTracking ( true );
  
  qDebug() << "width:" << width << "height:" << height;
  
  // Von außen bis Mitte blauer Rahmen
  Rand = 20;
  
  // Breite blauer Rahmen
  frameWidth = 4;
  
  radius = 20;

  penWidth = 2;
  penHalf = penWidth / 2;
  
  setGeometry( x, y, width, height );
  setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );

  //**********************************************************************
  // handle top left
  handleTopLeft = new QLabel( this );
  handleTopLeft->setGeometry( 0, 0, 2 * Rand, 2 * Rand );
  handleTopLeft->show();
  handleTopLeft->setCursor( Qt::SizeFDiagCursor );
  
  QPixmap * pixmap = new QPixmap( handleTopLeft->width(), handleTopLeft->height() );
  pixmap->fill( Qt::transparent );
  
  QPainter * handleTopLeftPainter = new QPainter( pixmap );
  handleTopLeftPainter->setRenderHints( QPainter::Antialiasing, true );
  handleTopLeftPainter->setPen( QPen( Qt::black, penWidth ) );
  
  QBrush brushTopLeft( Qt::red, Qt::SolidPattern );
  handleTopLeftPainter->setBrush( brushTopLeft );

  QRectF rectangleTopLeft = QRectF( penHalf, penHalf, 2 * ( Rand - penHalf ), 2 * ( Rand -penHalf ) );
  int startAngle = 0 * 16;
  int spanAngle = 270 * 16;
  handleTopLeftPainter->drawPie( rectangleTopLeft, startAngle, spanAngle );
  
  // Die schwarze Linie am unteren Rand von drawPie ändern in blau
  handleTopLeftPainter->setPen( QPen( Qt::blue, frameWidth ) );
  handleTopLeftPainter->drawLine( handleTopLeft->width() / 2, handleTopLeft->height() / 2 , handleTopLeft->width(), handleTopLeft->height() / 2 );
  handleTopLeftPainter->drawLine( handleTopLeft->width() / 2, handleTopLeft->height() / 2 , handleTopLeft->width() / 2, handleTopLeft->height() );
  
  handleTopLeft->setPixmap( *pixmap );
  //**********************************************************************
  //**********************************************************************
  // handle Top middle
  handleTopMiddle = new QLabel( this );
  handleTopMiddle->setGeometry( this->width() / 2 - Rand, 0, 2 * radius, radius );
  handleTopMiddle->show();
  handleTopMiddle->setCursor( Qt::SizeVerCursor );
  
  pixmap = new QPixmap( handleTopMiddle->width(), handleTopMiddle->height() );
  pixmap->fill( Qt::transparent );
  
  QPainter * handleTopMiddlePainter = new QPainter( pixmap );
  handleTopMiddlePainter->setRenderHints( QPainter::Antialiasing, true );
  handleTopMiddlePainter->setPen( QPen( Qt::black, penWidth ) );
  
  QBrush brushTopMiddle( Qt::red, Qt::SolidPattern );
  handleTopMiddlePainter->setBrush( brushTopMiddle );

  QRectF rectangle = QRectF( penHalf, penHalf, 2 * ( Rand - penHalf ), 2 * ( Rand -penHalf ) );
  startAngle = 0 * 16;
  spanAngle = 180 * 16;
  handleTopMiddlePainter->drawPie( rectangle, startAngle, spanAngle );
  
  // Die schwarze Linie am unteren Rand von drawPie ändern in blau
  handleTopMiddlePainter->setPen( QPen( Qt::blue, frameWidth ) );
  handleTopMiddlePainter->drawLine( 0, handleTopMiddle->height(), handleTopMiddle->width(), handleTopMiddle->height() );

  handleTopMiddle->setPixmap( *pixmap );
  //**********************************************************************
  
  // Framelock
  lockFrame( false );
}


regionselection::~regionselection()
{
}


void regionselection::paintEvent( QPaintEvent *event ) 
{
  (void)event;
  
  QRectF rectangle = QRectF();
  int startAngle;
  int spanAngle;
  
  QPainter painter( this );
  painter.setRenderHints( QPainter::Antialiasing, true );
  painter.setPen( QPen( Qt::black, penWidth ) );

  QBrush brush( Qt::SolidPattern );
  brush.setColor( Qt::red );
  painter.setBrush ( brush );

/*
  // http://de.academic.ru/dic.nsf/dewiki/797970
  // Knob left top
  rectangle.setRect( penHalf, penHalf, 2 * Rand, 2 * Rand );
  startAngle = 0 * 16;
  spanAngle = 270 * 16;
  painter.drawPie( rectangle, startAngle, spanAngle );
  
  
  // Knob top middle
  QPixmap * pixmap = new QPixmap( 50, 50 );
  pixmap->fill(Qt::green);
  QPainter handleTopMiddlePainter( pixmap );
  handleTopMiddle->setPixmap( *pixmap );
  
  handleTopMiddlePainter.setBrush ( brush );
  rectangle.setRect( penHalf, penHalf, 2 * Rand, 2 * Rand);
  startAngle = 0 * 16;
  spanAngle = 180 * 16;
  handleTopMiddlePainter.drawPie( rectangle, startAngle, spanAngle ); 

  handleTopMiddle->setPixmap( *pixmap );
*/
  // Knob top right
  rectangle.setRect( width() - 2 * Rand - penHalf, penHalf, 2 * Rand, 2 * Rand );
  startAngle = 180 * 16;
  spanAngle =  -270  * 16;
  painter.drawPie( rectangle, startAngle, spanAngle ); 
  
  // Knob right middle
  rectangle.setRect( width() - 2 * Rand - penHalf, height() / 2 - Rand, 2 * Rand, 2 * Rand );
  startAngle = 90 * 16;
  spanAngle =  -180  * 16;
  painter.drawPie( rectangle, startAngle, spanAngle ); 
  
  // Knob right bottom
  rectangle.setRect( width() - 2 * Rand - penHalf, height() - 2 * Rand - penHalf, 2 * Rand, 2 * Rand );
  startAngle = 90 * 16;
  spanAngle =  -270  * 16;
  painter.drawPie( rectangle, startAngle, spanAngle );
  
  // Knob bottom middle
  rectangle.setRect( width() / 2 - Rand - penHalf, height() - 2 * Rand - penHalf, 2 * Rand, 2 * Rand );
  startAngle = 0 * 16;
  spanAngle =  -180  * 16;
  painter.drawPie( rectangle, startAngle, spanAngle );
  
  // Knob Bottom left
  rectangle.setRect( penHalf, height() - 2 * Rand - penHalf, 2 * Rand, 2 * Rand );
  startAngle = 90 * 16;
  spanAngle =  270  * 16;
  painter.drawPie( rectangle, startAngle, spanAngle );
  
  // Knob left middle
  rectangle.setRect( penHalf, height()/2 - Rand, 2 * Rand, 2 * Rand );
  startAngle = 90 * 16;
  spanAngle =  180  * 16;
  painter.drawPie( rectangle, startAngle, spanAngle );

  // Kreis in der Mitte
  painter.drawEllipse ( width()/2 - radius, height()/2 - radius, 2 * radius, 2 * radius );

  
  // Blue Frame
  painter.setPen( QPen( Qt::blue, frameWidth ) );
  
  // Left Line
  painter.drawLine( Rand, Rand, Rand, height() - Rand );
  
  // Top Line
  painter.drawLine( Rand, Rand, width() - Rand, Rand);
  
  // Right Line
  painter.drawLine( width() - Rand, Rand, width() - Rand, height() - Rand);
  
  // Bottome Line
  painter.drawLine( Rand, height() - Rand, width() - Rand, height() - Rand);
  
  
}


void regionselection::lockFrame( bool status )
{
    frameLocked = status;
    qDebug() << "[vokoscreen][Regional selection] Frame locked:" << status;
    qDebug();
    handlingFrameLock();
}


bool regionselection::isFrameLocked()
{
    return frameLocked;
}


int regionselection::getX()
{
  return this->x() + border; 
}


int regionselection::getY()
{
  return  this->y() + border;
}


int regionselection::getHeight()
{
  return this->height() - 2 * border; 
}


int regionselection::getWidth()
{
  return this->width() - 2 * border; 
}


void regionselection::handlingFrameLock()
{
/*  
    if(frameLocked){
        borderLeft->setGeometry( 0, 0, border, this->height());
        borderRight->setGeometry( this->width()-border,0, border, this->height() );
        borderBottom->setGeometry(0, this->height() - border, this->width(), border );
        borderTop->setGeometry( 0, 0, this->width(), border );
	labelSize->hide();
    }
    else {
        setAllBorder();
    }
*/    
}


/*! MouseMove fuer das Bewegen des Fensters
\param event QMouseEvent Mouse Event
*/
void regionselection::mouseMoveEvent( QMouseEvent *event )
{
  if ( handleTopLeft->underMouse() )
  {
    moveTopLeft( event );
    handleTopMiddle->setGeometry( this->width() / 2 - Rand, 0, 2 * radius, radius );
    return;
  }

  if ( handleTopMiddle->underMouse() )
  {
    moveTopMiddle( event );
    return;
  }
    
/*    
    if (  event->buttons() & Qt::LeftButton )
    {
        move( event->globalPos() - m_dragPosition );
        event->accept();
    }
*/  
    
}

void regionselection::moveTopLeft( QMouseEvent *event )
{
  // Globale Mauskoordinaten 
  int mouseY = event->globalY();
  int mouseX = event->globalX();
  
  // Alte Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetWidth = geometry().width();
  int widgetHeight = geometry().height();
  
  // Minimale Größe des Widget begrenzen
  if ( mouseY >= widgetY + widgetHeight - 200)
    mouseY = widgetY + widgetHeight - 200;

  if ( mouseX >= widgetX + widgetWidth - 200)
    mouseX = widgetX + widgetWidth - 200;
  
  
  // Neue Geometry des Dialogfenster setzen
  this->setGeometry( mouseX,
		     mouseY, 
		     widgetWidth + ( widgetX - mouseX ),
		     widgetHeight + ( widgetY - mouseY ) );
  
  
  event->accept();
}


void regionselection::moveTopMiddle( QMouseEvent *event )
{
  // Globale Mauskoordinaten 
  int mouseY = event->globalY();
    
  // Alte Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetWidth = geometry().width();
  int widgetHeight = geometry().height();
  
  // Minimale Größe des Widget begrenzen
  if ( mouseY >= widgetY + widgetHeight - 200)
    mouseY = widgetY + widgetHeight - 200;
  

  // Neue Geometry des Dialogfenster setzen
  this->setGeometry( widgetX,
		     mouseY, 
		     widgetWidth,
		     widgetHeight + ( widgetY - mouseY ) );
  
  event->accept();
}


/*! MousePressed fuer das Bewegen des Fensters
\param event QMouseEvent Mouse Event
*/

void regionselection::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton )
    {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

