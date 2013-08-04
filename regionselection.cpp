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

#include <math.h>
#include <QPainterPath>

#include <QMouseEvent>

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
  handlePressed = NoHandle;
  handleUnderMouse = NoHandle;
  painter =  new QPainter( this );

  (void)framewidth;
  setGeometry( x, y, width, height );
  setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
  
  if( QX11Info::isCompositingManagerRunning() )
    setAttribute( Qt::WA_TranslucentBackground, true );
  
  setMouseTracking( true );
  
  qDebug() << "width:" << width << "height:" << height;
  
  borderLeft = 20;
  borderTop = 20;
  borderRight = 20;
  borderBottom = 20;
  
  Rand = 20; // Wenn fertig wird dies nicht mehr bnötigt
  
  // Von außen bis Mitte blauer Rahmen
  // Breite blauer Rahmen
  frameWidth = 4;
  
  radius = 20;

  penWidth = 2;
  penHalf = penWidth / 2;

/*
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
  
  QBrush brushTopLeft( Qt::green, Qt::SolidPattern );
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
*/  
 
  // Framelock
  lockFrame( false );
  
 
}


regionselection::~regionselection()
{
}


void regionselection::HandleTopLeft()
{
  QColor color, arrow;
  
  if ( isFrameLocked() )
  {
    color = Qt::red;
    arrow = Qt::red;
  }
  else
  {
    color = Qt::green;
    arrow = Qt::black;
  }
  
  QBrush brush( color, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );
  
  QRectF rectangle = QRectF( borderLeft - radius + penHalf, borderTop - radius + penHalf, 2 * radius, 2 * radius );
  int startAngle = 0 * 16;
  int spanAngle = 270 * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
  
}



void regionselection::HandleTopMiddle()
{
  QColor color, arrow;
  
  if ( isFrameLocked() )
  {
    color = Qt::red;
    arrow = Qt::red;
  }
  else
  {
    color = Qt::green;
    arrow = Qt::black;
  }
  
  QBrush brush( color, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );
  
  QRectF rectangle = QRectF( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius, borderTop - radius + penHalf, 2 * radius, 2 * radius );
  int startAngle = 0 * 16;
  int spanAngle = 180 * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );

  painter->setPen( QPen( arrow, 2 ) );
  QPainterPath * painterPath = new QPainterPath();
  painterPath->moveTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft, borderTop );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft, borderTop - radius + penWidth + 1);
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - 3, borderTop - radius + 7 );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + 3, borderTop - radius + 7 );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft, borderTop -radius + penWidth + 1);
  painter->drawPath( *painterPath );
}


void regionselection::HandleTopRight()
{
  QColor color, arrow;
  
  if ( isFrameLocked() )
  {
    color = Qt::red;
    arrow = Qt::red;
  }
  else
  {
    color = Qt::green;
    arrow = Qt::black;
  }
  
  QBrush brush( color, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );

  QRectF rectangle = QRectF( width() - borderRight - radius - penHalf, borderTop - radius + penHalf, 2 * radius, 2 * radius );
  int startAngle = 180 * 16;
  int spanAngle =  -270  * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );

  // Begin Pfeil zeichnen
  double h = 2 * radius / 3;
  painter->setPen( QPen( arrow, 2 ) );

  QPainterPath * painterPath = new QPainterPath();
  painterPath->moveTo( width() - borderRight + frameWidth / 2  , borderTop - frameWidth / 2 );
  painterPath->lineTo( width() - borderRight + h - penWidth / 2, borderTop - h + penWidth / 2 );
  painterPath->lineTo( width() - borderRight + h - 7           , borderTop - h + 3 );
  painterPath->lineTo( width() - borderRight + h - 3           , borderTop - h + 7 );
  painterPath->lineTo( width() - borderRight + h - penWidth    , borderTop - h + penWidth );  
  
  painterPath->setFillRule( Qt::OddEvenFill );
  
  painter->drawPath( *painterPath );
  // End Pfeil zeichnen
}


void regionselection::HandleRightMiddle()
{
  QColor color, arrow;
  
  if ( isFrameLocked() )
  {
    color = Qt::red;
    arrow = Qt::red;
  }
  else
  {
    color = Qt::green;
    arrow = Qt::black;
  }
  
  QBrush brush( color, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );
  
  QRectF rectangle = QRectF( width() - borderRight - radius - penHalf, ( height() - borderTop - borderBottom ) / 2 + borderTop - radius,  2 * radius, 2 * radius );
  int startAngle = 90 * 16;
  int spanAngle =  -180  * 16;
  painter->drawPie( rectangle, startAngle, spanAngle ); 
  
   // Begin Pfeil zeichnen
  painter->setPen( QPen( arrow, 2 ) );

  QPainterPath * painterPath = new QPainterPath();
  painterPath->moveTo( width() - borderRight + frameWidth / 2       , ( ( height() - borderTop - borderBottom ) / 2 + borderTop ) );
  painterPath->lineTo( width() - borderRight + radius - penWidth - 1, ( ( height() - borderTop - borderBottom ) / 2 + borderTop ) );
  painterPath->lineTo( width() - borderRight + radius - penWidth - 7, ( ( height() - borderTop - borderBottom ) / 2 + borderTop + 3 ) );
  painterPath->lineTo( width() - borderRight + radius - penWidth - 7, ( ( height() - borderTop - borderBottom ) / 2 + borderTop - 3 ) );
  painterPath->lineTo( width() - borderRight + radius - penWidth - 1, ( ( height() - borderTop - borderBottom ) / 2 + borderTop ) );

  painterPath->setFillRule( Qt::OddEvenFill );
  
  painter->drawPath( *painterPath );
  // End Pfeil zeichnen
}


void regionselection::HandleBottomRight()
{
  QColor color, arrow;
  
  if ( isFrameLocked() )
  {
    color = Qt::red;
    arrow = Qt::red;
  }
  else
  {
    color = Qt::green;
    arrow = Qt::black;
  }
  
  QBrush brush( color, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );
  
  QRectF rectangle = QRectF( width() - borderRight - radius - penHalf, height() - borderBottom - radius - penHalf, 2 * radius, 2 * radius );
  int startAngle = 90 * 16;
  int spanAngle =  -270  * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
  
  // Begin Pfeil zeichnen
  double h = 2 * radius / 3;
  
  painter->setPen( QPen( arrow, 2 ) );

  QPainterPath * painterPath = new QPainterPath();
  painterPath->moveTo( width() - borderRight + frameWidth / 2  , height() - borderBottom + frameWidth / 2 );
  painterPath->lineTo( width() - borderRight + h - penWidth / 2, height() - borderBottom + h - penWidth / 2 );
  painterPath->lineTo( width() - borderRight + h - 7           , height() - borderBottom + h - 3 );
  painterPath->lineTo( width() - borderRight + h - 3           , height() - borderBottom + h - 7 );
  painterPath->lineTo( width() - borderRight + h - penWidth / 2, height() - borderBottom + h - penWidth / 2);
  
  painterPath->setFillRule( Qt::OddEvenFill );
  
  painter->drawPath( *painterPath );
  // End Pfeil zeichnen
}


void regionselection::paintEvent( QPaintEvent *event ) 
{
  (void)event;
  
  QRegion maskedRegion1( 0, 
                         0, 
                         width(),
                         height(), 
                         QRegion::Rectangle );
  
  // Dieser Teil wird abgezogen
  QRegion maskedRegion( borderLeft + frameWidth / 2, 
                        borderTop + frameWidth / 2, 
                        width() - borderLeft - borderRight - frameWidth,
                        height() - borderTop - borderBottom - frameWidth, 
                        QRegion::Rectangle );
  
  QRegion maskedRegionTopLeft( borderLeft + radius + 2,
			       0,
			       width() / 2 - borderLeft - 2 * radius - 3,
			       borderTop - frameWidth / 2 );

  
  this->setMask( maskedRegion1.subtract( maskedRegion ) );
  this->setMask( maskedRegion1.subtract( maskedRegionTopLeft ) );
  
  
  
  QRectF rectangle = QRectF();
  int startAngle;
  int spanAngle;
  
  painter->begin( this );
  painter->setRenderHints( QPainter::Antialiasing, true );

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
*/
  HandleTopLeft();
  HandleTopMiddle();
  HandleTopRight();
  HandleRightMiddle();
  HandleBottomRight();
  
  
  
  // Knob bottom middle
  rectangle.setRect( width() / 2 - Rand - penHalf, height() - 2 * Rand - penHalf, 2 * Rand, 2 * Rand );
  startAngle = 0 * 16;
  spanAngle =  -180  * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
/*  
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

*/  
  // Blue Frame
  painter->setPen( QPen( Qt::blue, frameWidth ) );
  
  // Left Line
  painter->drawLine( borderLeft, borderTop, borderLeft, height() - borderBottom );
  
  // Top Line
  painter->drawLine( borderLeft, borderTop, width() - borderRight, borderTop );
  
  // Right Line
  painter->drawLine( width() - borderRight, borderTop, width() - borderRight, height() - borderBottom );
  
  // Bottome Line
  painter->drawLine( borderLeft, height() - borderBottom, width() - borderRight, height() - borderBottom );

  painter->end();
  
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
  repaint();
}


/*! MouseMove fuer das bewegen des Fensters und Ränder
\param event QMouseEvent Mouse Event
*/
void regionselection::mouseMoveEvent( QMouseEvent *event )
{
  
  if ( isFrameLocked() )
    return;
  
  // Handle tp left
  if ( handlePressed == TopLeft )
  {
    moveTopLeft( event );
    return;
  }
    
  
  
  // Handle top middle
  if ( handlePressed == TopMiddle )
  {
    moveTopMiddle( event );
    return;
  }
  
  if ( ( event->x() > ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius ) and
       ( event->x() < ( width() - borderLeft - borderRight ) / 2 + borderLeft + radius ) and
       ( event->y() > borderTop - radius ) and
       ( event->y() < borderTop ) )
  {
    setCursor( Qt::SizeVerCursor );  
    handleUnderMouse = TopMiddle;
    return;
  }

  // Handle top right
  if ( handlePressed == TopRight )
  {
    moveTopRight( event );
    return;
  }
  
  if ( ( event->x() > width() - borderRight - radius ) and 
       ( event->x() < width() - borderRight + radius ) and
       ( event->y() > borderTop - radius ) and
       ( event->y() < borderTop + radius ) )
  {
    setCursor( Qt::SizeBDiagCursor  );
    handleUnderMouse = TopRight;
    return;
  }
  
  
  // Handle right middle
  if ( handlePressed == RightMiddle )
  {
    moveRightMiddle( event );
    return;
  }
  
  if ( ( event->x() > width() - borderRight ) and
       ( event->x() < width() - borderRight + radius ) and
       ( event->y() > ( ( height() - borderTop - borderBottom ) / 2 + borderTop - radius ) ) and
       ( event->y() < ( ( height() - borderTop - borderBottom ) / 2 + borderTop + radius ) ) )
  {
    setCursor( Qt::SizeHorCursor );
    handleUnderMouse = RightMiddle;
    return;
  }

  // Handle bottom right
  if ( handlePressed == BottomRight )
  {
    moveBottomRight( event );
    return;
  }
  
  if ( ( event->x() > width() - borderRight - radius ) and
       ( event->x() < width() - borderRight + radius ) and
       ( event->y() > height() - borderBottom - radius ) and
       ( event->y() < height() - borderBottom + radius ) )
  {
     setCursor( Qt::SizeFDiagCursor );
     handleUnderMouse = BottomRight;
     return;
  }

  // Handle bottom middle
  if ( handlePressed == BottomMiddle )
  {
    moveBottomMiddle( event );
    return;
  }
  
  if ( ( event->x() > width() / 2 - radius ) and 
       ( event->x() < width() / 2 + radius ) and 
       ( event->y() < height() ) and 
       ( event->y() > height() - radius ) )
  {
    setCursor( Qt::SizeVerCursor );
    handleUnderMouse = BottomMiddle;
    return;
  }

  handleUnderMouse = NoHandle;
  
  unsetCursor();

  event->accept();

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
  //int widgetWidth = geometry().width();

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

}


// http://doc-snapshot.qt-project.org/4.8/qtransform.html#details

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


void regionselection::moveBottomMiddle( QMouseEvent *event)
{
  (void)event;
}


void regionselection::mousePressEvent( QMouseEvent *event )
{
  // Position bei klick im Fenster festhalten
  currentMouseLocalX = event->x();
  currentMouseLocalY = event->y();

  currentWidgetWidth = geometry().width();
  currentWidgetHeight = geometry().height();
  
  switch ( handleUnderMouse )
  {
    case NoHandle    : handlePressed = NoHandle;     break;
    case TopLeft     : handlePressed = TopLeft;      break;
    case TopMiddle   : handlePressed = TopMiddle;    break;
    case TopRight    : handlePressed = TopRight;     break;
    case RightMiddle : handlePressed = RightMiddle;  break;
    case BottomRight : handlePressed = BottomRight;  break;
    case BottomMiddle: handlePressed = BottomMiddle; break;
    case BottomLeft  : handlePressed = BottomLeft;   break;
    case LeftMiddle  : handlePressed = LeftMiddle;   break;
    case Middle      : handlePressed = Middle;       break;
  }
  
  
  // Wegen Zittern am unteren Rand. Wird nicht benützt, dient erstmal für weitere Überlegungen
  currentbottomY = y() + height();
  
  
  event->accept();  
}

void regionselection::mouseReleaseEvent( QMouseEvent * event )
{
  (void)event;
  handlePressed = NoHandle;
  event->accept();  
  
}
