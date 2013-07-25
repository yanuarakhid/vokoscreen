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
#include <QX11Info>

#include <QCoreApplication>

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
  setGeometry( x, y, width, height );
  setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
  
  if( QX11Info::isCompositingManagerRunning() )
    setAttribute( Qt::WA_TranslucentBackground, true );
  
  setMouseTracking ( true );
  
  qDebug() << "width:" << width << "height:" << height;
  
  borderLeft = 20;
  borderTop = 20;
  borderRight =20;
  borderBottom = 20;
  
  Rand = 20; // Wenn fertig wird dies nicht mehr bnötigt
  
  // Von außen bis Mitte blauer Rahmen
  // Breite blauer Rahmen
  frameWidth = 4;
  
  radius = 15;

  penWidth = 2;
  penHalf = penWidth / 2;
  
  //**********************************************************************
  // handle top left
  handleTopLeft = new QLabel( this );
  handleTopLeft->setGeometry( borderLeft - radius, borderTop - radius, 2 * radius, 2 * radius );
  handleTopLeft->show();
  handleTopLeft->setCursor( Qt::SizeFDiagCursor );
  
  QPixmap * pixmap = new QPixmap( handleTopLeft->width(), handleTopLeft->height() );
  pixmap->fill( Qt::transparent );
  
  QPainter * handleTopLeftPainter = new QPainter( pixmap );
  handleTopLeftPainter->setRenderHints( QPainter::Antialiasing, true );
  handleTopLeftPainter->setPen( QPen( Qt::black, penWidth ) );
  
  QBrush brushTopLeft( Qt::red, Qt::SolidPattern );
  handleTopLeftPainter->setBrush( brushTopLeft );

  QRectF rectangleTopLeft = QRectF( penHalf, penHalf, 2 * ( radius - penHalf ), 2 * ( radius -penHalf ) );
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
  handleTopMiddle->setGeometry( this->width() / 2 - radius, borderTop - radius , 2 * radius, radius );
  handleTopMiddle->show();
  handleTopMiddle->setCursor( Qt::SizeVerCursor );
  
  pixmap = new QPixmap( handleTopMiddle->width(), handleTopMiddle->height() );
  pixmap->fill( Qt::transparent );
  
  QPainter * handleTopMiddlePainter = new QPainter( pixmap );
  handleTopMiddlePainter->setRenderHints( QPainter::Antialiasing, true );
  handleTopMiddlePainter->setPen( QPen( Qt::black, penWidth ) );
  
  QBrush brushTopMiddle( Qt::red, Qt::SolidPattern );
  handleTopMiddlePainter->setBrush( brushTopMiddle );

  QRectF rectangleTopMiddle = QRectF( penHalf, penHalf, 2 * ( radius - penHalf ), 2 * ( radius -penHalf ) );
  startAngle = 0 * 16;
  spanAngle = 180 * 16;
  handleTopMiddlePainter->drawPie( rectangleTopMiddle, startAngle, spanAngle );
  
  // Die schwarze Linie am unteren Rand von drawPie ändern in blau
  handleTopMiddlePainter->setPen( QPen( Qt::blue, frameWidth ) );
  handleTopMiddlePainter->drawLine( 0, handleTopMiddle->height(), handleTopMiddle->width(), handleTopMiddle->height() );

  handleTopMiddle->setPixmap( *pixmap );
  //**********************************************************************
  //**********************************************************************
  // handle top right
  handleTopRight = new QLabel( this );
  handleTopRight->setGeometry( this->width() - borderRight - radius, borderTop - radius, 2 * radius, 2 * radius );
  handleTopRight->show();
  handleTopRight->setCursor( Qt::SizeBDiagCursor );
  
  pixmap = new QPixmap( handleTopRight->width(), handleTopRight->height() );
  pixmap->fill( Qt::transparent );
  
  QPainter * handleTopRightPainter = new QPainter( pixmap );
  handleTopRightPainter->setRenderHints( QPainter::Antialiasing, true );
  handleTopRightPainter->setPen( QPen( Qt::black, penWidth ) );
  
  QBrush brushTopRight( Qt::red, Qt::SolidPattern );
  handleTopRightPainter->setBrush( brushTopRight );

  QRectF rectangleTopRight = QRectF( penHalf, penHalf, 2 * ( radius - penHalf ), 2 * ( radius -penHalf ) );
  startAngle = 180 * 16;
  spanAngle =  -270  * 16;
  handleTopRightPainter->drawPie( rectangleTopRight, startAngle, spanAngle );
  
  // Die schwarze Linie am unteren Rand von drawPie ändern in blau
  handleTopRightPainter->setPen( QPen( Qt::blue, frameWidth ) );
  handleTopRightPainter->drawLine( 0, handleTopRight->height() / 2, handleTopRight->width() / 2, handleTopRight->height() / 2 );
  handleTopRightPainter->drawLine( handleTopRight->height() / 2, handleTopRight->height() / 2, handleTopRight->width() / 2, handleTopRight->height() );
  handleTopRight->setPixmap( *pixmap );
  
  //**********************************************************************
  //**********************************************************************
  // handle right middle
  handleRightMiddle = new QLabel( this );
  handleRightMiddle->setGeometry( this->width() - borderRight - radius, this->height() / 2 - radius, 2 * radius, 2 * radius );
  handleRightMiddle->show();
  handleRightMiddle->setCursor( Qt::SizeHorCursor );
  
  pixmap = new QPixmap( handleRightMiddle->width(), handleRightMiddle->height() );
  pixmap->fill( Qt::transparent );
  
  QPainter * handleRightMiddlePainter = new QPainter( pixmap );
  handleRightMiddlePainter->setRenderHints( QPainter::Antialiasing, true );
  handleRightMiddlePainter->setPen( QPen( Qt::black, penWidth ) );
  
  QBrush brushRightMiddle( Qt::red, Qt::SolidPattern );
  handleRightMiddlePainter->setBrush( brushRightMiddle );

  QRectF rectangleRightMiddle = QRectF( penHalf, penHalf, 2 * ( radius - penHalf ), 2 * ( radius -penHalf ) );
  startAngle = 90 * 16;
  spanAngle =  -180  * 16;
  handleRightMiddlePainter->drawPie( rectangleRightMiddle, startAngle, spanAngle );
  
  // Die schwarze Linie am unteren Rand von drawPie ändern in blau
  handleRightMiddlePainter->setPen( QPen( Qt::blue, frameWidth ) );
  handleRightMiddlePainter->drawLine( handleRightMiddle->width() - radius,
				      handleRightMiddle->height() / 2 - radius,
				      handleRightMiddle->width() - radius,
				      handleTopRight->height() / 2 + radius);
  handleRightMiddle->setPixmap( *pixmap );
  
  //**********************************************************************
  //**********************************************************************
  // handle bottom right
  handleBottomRight = new QLabel( this );
  handleBottomRight->setGeometry( this->width() - borderRight - radius, this->height() - borderRight - radius, 2 * radius, 2 * radius );
  handleBottomRight->show();
  handleBottomRight->setCursor( Qt::SizeFDiagCursor );
  
  pixmap = new QPixmap( handleRightMiddle->width(), handleRightMiddle->height() );
  pixmap->fill( Qt::transparent );
  
  QPainter * handleBottomRightPainter = new QPainter( pixmap );
  handleBottomRightPainter->setRenderHints( QPainter::Antialiasing, true );
  handleBottomRightPainter->setPen( QPen( Qt::black, penWidth ) );
 
  QBrush brushBottmRight( Qt::red, Qt::SolidPattern );
  handleBottomRightPainter->setBrush( brushBottmRight );
  
  QRectF rectangleBottmRight = QRectF( penHalf, penHalf, 2 * ( radius - penHalf ), 2 * ( radius -penHalf ) );
  startAngle = 90 * 16;
  spanAngle =  -270  * 16;
  handleBottomRightPainter->drawPie( rectangleBottmRight, startAngle, spanAngle );
  
  // Die schwarze Linie am unteren Rand von drawPie ändern in blau
  handleBottomRightPainter->setPen( QPen( Qt::blue, frameWidth ) );
  handleBottomRightPainter->drawLine( handleBottomRight->width() - radius,
				      handleBottomRight->height() / 2 - radius,
				      handleBottomRight->width() - radius,
				      handleBottomRight->height() / 2 );
  handleBottomRightPainter->drawLine( 0, handleTopRight->height() / 2, handleTopRight->width() / 2, handleTopRight->height() / 2 );
  
  handleBottomRight->setPixmap( *pixmap );
  
  
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
*/  
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
  painter.drawLine( borderLeft, borderTop, borderLeft, height() - borderBottom );
  
  // Top Line
  painter.drawLine( borderLeft, borderTop, width() - borderRight, borderTop );
  
  // Right Line
  painter.drawLine( width() - borderRight, borderTop, width() - borderRight, height() - borderBottom );
  
  // Bottome Line
  painter.drawLine( borderLeft, height() - borderBottom, width() - borderRight, height() - borderBottom );
  
  
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
  return this->x() + borderLeft + frameWidth / 2; 
}


int regionselection::getY()
{
  return  this->y() + borderTop + frameWidth / 2;
}


int regionselection::getHeight()
{
  return this->height() - borderTop - borderBottom - frameWidth; 
}


int regionselection::getWidth()
{
  return this->width() - borderLeft - borderRight - frameWidth; 
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


/*! MouseMove fuer das bewegen des Fensters und Ränder
\param event QMouseEvent Mouse Event
*/
void regionselection::mouseMoveEvent( QMouseEvent *event )
{
  if ( handleTopLeft->underMouse() )
  {
    moveTopLeft( event );
    handleTopMiddle->setGeometry( this->width() / 2 - radius, borderTop - radius , 2 * radius, radius );
    handleTopRight->setGeometry( this->width() - borderRight - radius, borderTop - radius, 2 * radius, 2 * radius );
    handleRightMiddle->setGeometry( this->width() - borderRight - radius, this->height() / 2 - radius, 2 * radius, 2 * radius );
    handleBottomRight->setGeometry( this->width() - borderRight - radius, this->height() - borderRight - radius, 2 * radius, 2 * radius );
    return;
  }

  if ( handleTopMiddle->underMouse() )
  {
    moveTopMiddle( event );
    handleRightMiddle->setGeometry( this->width() - borderRight - radius, this->height() / 2 - radius, 2 * radius, 2 * radius );
    handleBottomRight->setGeometry( this->width() - borderRight - radius, this->height() - borderRight - radius, 2 * radius, 2 * radius );
    return;
  }

  if ( handleTopRight->underMouse() )
  {
    moveTopRight( event );
    handleTopMiddle->setGeometry( this->width() / 2 - radius, borderTop - radius , 2 * radius, radius );
    handleTopRight->setGeometry( this->width() - borderRight - radius, borderTop - radius, 2 * radius, 2 * radius );
    handleRightMiddle->setGeometry( this->width() - borderRight - radius, this->height() / 2 - radius, 2 * radius, 2 * radius );
    handleBottomRight->setGeometry( this->width() - borderRight - radius, this->height() - borderRight - radius, 2 * radius, 2 * radius );
    return;
  }

  if ( handleRightMiddle->underMouse() )
  {
    moveRightMiddle( event );
    handleTopMiddle->setGeometry( this->width() / 2 - radius, borderTop - radius , 2 * radius, radius );
    handleTopRight->setGeometry( this->width() - borderRight - radius, borderTop - radius, 2 * radius, 2 * radius );
    handleRightMiddle->setGeometry( this->width() - borderRight - radius, this->height() / 2 - radius, 2 * radius, 2 * radius );
    handleBottomRight->setGeometry( this->width() - borderRight - radius, this->height() - borderRight - radius, 2 * radius, 2 * radius );
    return;
  }
  
  if ( handleBottomRight->underMouse() )
  {
    moveBottomRight( event );
    handleTopMiddle->setGeometry( this->width() / 2 - radius, borderTop - radius , 2 * radius, radius );
    handleTopRight->setGeometry( this->width() - borderRight - radius, borderTop - radius, 2 * radius, 2 * radius );
    handleRightMiddle->setGeometry( this->width() - borderRight - radius, this->height() / 2 - radius, 2 * radius, 2 * radius );
    handleBottomRight->setGeometry( this->width() - borderRight - radius, this->height() - borderRight - radius, 2 * radius, 2 * radius );
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
  int mouseGlobalX = event->globalX();
  int mouseGlobalY = event->globalY();
  
  // Alte Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetWidth = geometry().width();
  int widgetHeight = geometry().height();
  
  // Minimale Größe des Widget begrenzen
  if ( mouseGlobalY >= widgetY + widgetHeight - 200 )
    mouseGlobalY = widgetY + widgetHeight - 200;

  if ( mouseGlobalX >= widgetX + widgetWidth - 200 )
    mouseGlobalX = widgetX + widgetWidth - 200;
  
  // Neue Geometry des Dialogfenster setzen
  this->setGeometry( mouseGlobalX - currentMouseLocalX,
		     mouseGlobalY - currentMouseLocalY, 
		     widgetWidth + ( widgetX - mouseGlobalX + currentMouseLocalX ),
		     widgetHeight + ( widgetY - mouseGlobalY + currentMouseLocalY ) );
  
  
  event->accept();
  qDebug() << "moveTopLeft";
}


void regionselection::moveTopMiddle( QMouseEvent *event )
{
  
  
  // Globale Mauskoordinaten 
  int mouseGlobalY = event->globalY();
  
  // Alte Globale Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetWidth = geometry().width();
  int widgetHeight = geometry().height();

  // Minimale Größe des Widget begrenzen
  if ( mouseGlobalY >= widgetY + widgetHeight - 200 )
    mouseGlobalY = widgetY + widgetHeight - 200;
  
  // Neue Geometry des HauptWidget setzen
  this->setGeometry( widgetX,
		     mouseGlobalY - currentMouseLocalY,
		     widgetWidth,
		     widgetHeight + ( widgetY - mouseGlobalY + currentMouseLocalY ) );

  event->accept();

  qDebug() << "moveTopMiddle";
}


void regionselection::moveTopRight( QMouseEvent *event )
{
  // Globale Mauskoordinaten
  int mouseGlobalX = event->globalX();
  int mouseGlobalY = event->globalY();
  
  // Alte Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetHeight = geometry().height();
  
  // Minimale Größe des Widget begrenzen
  if ( mouseGlobalX <= widgetX + 200 )
    mouseGlobalX = widgetX + 200;

  if ( mouseGlobalY >= widgetY + widgetHeight - 200 )
    mouseGlobalY = widgetY + widgetHeight - 200;
  
  // Neue Geometry des Fenster setzen
  this->setGeometry( widgetX,
		     mouseGlobalY - currentMouseLocalY,
		     currentWidgetWidth + ( mouseGlobalX - ( widgetX + currentMouseLocalX ) ),
     		     widgetHeight  + ( widgetY - mouseGlobalY + currentMouseLocalY ) );
  
  event->accept();

  qDebug() << "moveTopRight";
}


void regionselection::moveRightMiddle( QMouseEvent *event )
{
  // Globale Mauskoordinaten
  int mouseGlobalX = event->globalX();
  
  // Alte Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetHeight = geometry().height();
  
  // Minimale Größe des Widget begrenzen
  if ( mouseGlobalX <= widgetX + 200 )
    mouseGlobalX = widgetX + 200;
  
  this->setGeometry( widgetX,
		     widgetY,
		     currentWidgetWidth + ( mouseGlobalX - ( widgetX + currentMouseLocalX ) ),
		     widgetHeight );
}


void regionselection::moveBottomRight( QMouseEvent *event )
{
  // Globale Mauskoordinaten
  int mouseGlobalX = event->globalX();
  int mouseGlobalY = event->globalY();
  
  // Alte Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetHeight = geometry().height();

 
  // Minimale Größe des Widget begrenzen
  if ( mouseGlobalX <= widgetX + 200 )
    mouseGlobalX = widgetX + 200;
  
  if ( mouseGlobalY <= widgetY + 200 )
    mouseGlobalY = widgetY +  200;

  
  this->setGeometry( widgetX,
		     widgetY,
		     currentWidgetWidth + ( mouseGlobalX - ( widgetX + currentMouseLocalX ) ),
		     currentWidgetHeight + ( mouseGlobalY - ( widgetY + currentMouseLocalY ) ) );
}


void regionselection::mousePressEvent( QMouseEvent *event )
{
  // Position bei klick im Fenster festhalten
  currentMouseLocalX = event->x();
  currentMouseLocalY = event->y();

  currentWidgetWidth = geometry().width();
  currentWidgetHeight = geometry().height();
  
  // Wegen Zittern am unteren Rand. Wird nicht benützt, dient erstmal für weitere Überlegungen
  currentbottomY = geometry().y() + geometry().height();
  
  event->accept();  
}

