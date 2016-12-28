/* vokoscreen - A desktop recorder
 * Copyright (C) 2011-2014 Volker Kohaupt
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301 USA 
 */

#include "QvkRegionChoise.h"
#include <QTest>
#include <QBitmap>

QvkRegionChoise::QvkRegionChoise()
{
  vkSettings.readAll();
  
  setAttribute ( Qt::WA_AlwaysShowToolTips );
  setToolTip( tr( "double-click to maximize" ) );
  
  handlePressed = NoHandle;
  handleUnderMouse = NoHandle;
  painter =  new QPainter();
  
  setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip );
  
  setAttribute( Qt::WA_TranslucentBackground, true );

  setMouseTracking( true );
  
  borderLeft = 20;
  borderTop = 20;
  borderRight = 20;
  borderBottom = 20;
 
  // Von außen bis Mitte blauer Rahmen
  // Breite blauer Rahmen
  frameWidth = 4;
  
  radius = 20;

  penWidth = 2;
  penHalf = penWidth / 2;
 
  // Framelock
  lockFrame( false );
  
  setGeometry( vkSettings.getAreaX(),
               vkSettings.getAreaY(),
               vkSettings.getAreaWidth() + borderLeft + borderRight + frameWidth,
               vkSettings.getAreaHeight() + borderTop + borderBottom + frameWidth
             );
  show();
}


QvkRegionChoise::~QvkRegionChoise()
{
}


void QvkRegionChoise::HandleTopLeft()
{
  QColor color = Qt::green;
  QColor arrow = Qt::black;
  
  painter->setBrush( QBrush( color, Qt::SolidPattern ) );
  painter->setPen( QPen( Qt::black, penWidth ) );
  
  QRectF rectangle = QRectF( borderLeft - radius + penHalf, borderTop - radius + penHalf, 2 * radius, 2 * radius );
  painter->drawEllipse( rectangle );
  
  // Begin Pfeil zeichnen
  double h = 2 * radius / 3;
  painter->setPen( QPen( arrow, 2 ) );

  QPainterPath painterPath;
  painterPath.moveTo( borderLeft - frameWidth / 2 , borderTop - frameWidth / 2 );
  painterPath.lineTo( borderLeft - h + penHalf, borderTop - h + penHalf );
  painterPath.lineTo( borderLeft - h + penHalf + 7, borderTop - h + penHalf + 3 );
  painterPath.lineTo( borderLeft - h + penHalf + 3, borderTop - h + penHalf + 7 );
  painterPath.lineTo( borderLeft - h + penHalf, borderTop - h + penHalf );
  painter->drawPath( painterPath );
}


void QvkRegionChoise::HandleTopMiddle()
{
  QColor color = Qt::green;
  QColor arrow = Qt::black;
  
  painter->setBrush( QBrush( color, Qt::SolidPattern ) );
  painter->setPen( QPen( Qt::black, penWidth ) );
  
  QRectF rectangle = QRectF( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius, borderTop - radius + penHalf, 2 * radius, 2 * radius );
  painter->drawEllipse( rectangle );
  painter->setPen( QPen( arrow, 2 ) );
  QPainterPath painterPath;
  painterPath.moveTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     borderTop );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     borderTop - radius + penWidth + penHalf );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - 3, borderTop - radius + penWidth+7 );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + 3, borderTop - radius + penWidth+7 );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     borderTop - radius + penWidth + penHalf );
  painter->drawPath( painterPath );
}


void QvkRegionChoise::HandleTopRight()
{
  QColor color = Qt::green;
  QColor arrow = Qt::black;
  
  painter->setBrush( QBrush( color, Qt::SolidPattern ) );
  painter->setPen( QPen( Qt::black, penWidth ) );

  QRectF rectangle = QRectF( width() - borderRight - radius - penHalf, borderTop - radius + penHalf, 2 * radius, 2 * radius );
  painter->drawEllipse( rectangle );

  // Begin Pfeil zeichnen
  double h = 2 * radius / 3;
  painter->setPen( QPen( arrow, 2 ) );

  QPainterPath painterPath;
  painterPath.moveTo( width() - borderRight + frameWidth / 2  , borderTop - frameWidth / 2 );
  painterPath.lineTo( width() - borderRight + h - penWidth / 2, borderTop - h + penWidth / 2 );
  painterPath.lineTo( width() - borderRight + h - 7           , borderTop - h + 3 );
  painterPath.lineTo( width() - borderRight + h - 3           , borderTop - h + 7 );
  painterPath.lineTo( width() - borderRight + h - penWidth    , borderTop - h + penWidth );  
  
  painterPath.setFillRule( Qt::OddEvenFill );
  
  painter->drawPath( painterPath );
  
  // End Pfeil zeichnen
}


void QvkRegionChoise::HandleRightMiddle()
{
  QColor color = Qt::green;
  QColor arrow = Qt::black;
  
  painter->setBrush( QBrush ( color, Qt::SolidPattern ) );
  painter->setPen( QPen( Qt::black, penWidth ) );
  
  QRectF rectangle = QRectF( width() - borderRight - radius - penHalf, ( height() - borderTop - borderBottom ) / 2 + borderTop - radius,  2 * radius, 2 * radius );
  painter->drawEllipse( rectangle );
  
   // Begin Pfeil zeichnen
  painter->setPen( QPen( arrow, 2 ) );

  QPainterPath painterPath;
  painterPath.moveTo( width() - borderRight + frameWidth / 2       , ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath.lineTo( width() - borderRight + radius - penWidth - 1, ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath.lineTo( width() - borderRight + radius - penWidth - 7, ( height() - borderTop - borderBottom ) / 2 + borderTop + 3 );
  painterPath.lineTo( width() - borderRight + radius - penWidth - 7, ( height() - borderTop - borderBottom ) / 2 + borderTop - 3 );
  painterPath.lineTo( width() - borderRight + radius - penWidth - 1, ( height() - borderTop - borderBottom ) / 2 + borderTop );

  painterPath.setFillRule( Qt::OddEvenFill );
  
  painter->drawPath( painterPath );
  // End Pfeil zeichnen
}


void QvkRegionChoise::HandleBottomRight()
{
  QColor color = Qt::green;
  QColor arrow = Qt::black;
  
  painter->setBrush( QBrush ( color, Qt::SolidPattern ) );
  painter->setPen( QPen( Qt::black, penWidth ) );
  
  QRectF rectangle = QRectF( width() - borderRight - radius - penHalf, height() - borderBottom - radius - penHalf, 2 * radius, 2 * radius );
  painter->drawEllipse( rectangle );
  
  // Begin Pfeil zeichnen
  double h = 2 * radius / 3;
  
  painter->setPen( QPen( arrow, 2 ) );

  QPainterPath painterPath;
  painterPath.moveTo( width() - borderRight + frameWidth / 2  , height() - borderBottom + frameWidth / 2 );
  painterPath.lineTo( width() - borderRight + h - penWidth / 2, height() - borderBottom + h - penWidth / 2 );
  painterPath.lineTo( width() - borderRight + h - 7           , height() - borderBottom + h - 3 );
  painterPath.lineTo( width() - borderRight + h - 3           , height() - borderBottom + h - 7 );
  painterPath.lineTo( width() - borderRight + h - penWidth / 2, height() - borderBottom + h - penWidth / 2);
  
  painterPath.setFillRule( Qt::OddEvenFill );
  
  painter->drawPath( painterPath );
  // End Pfeil zeichnen
}


void QvkRegionChoise::HandleBottomMiddle()
{
  QColor color = Qt::green;
  QColor arrow = Qt::black;
  
  painter->setBrush( QBrush( color, Qt::SolidPattern ) );
  painter->setPen( QPen( Qt::black, penWidth ) );

  QRectF rectangle = QRectF( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius, height() - borderBottom - radius - penHalf, 2 * radius, 2 * radius );
  painter->drawEllipse( rectangle );
  
  // Begin Pfeil zeichnen
  painter->setPen( QPen( arrow, 2 ) );

  QPainterPath painterPath;
  painterPath.moveTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     height() - borderBottom );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     height() - borderBottom + radius - penWidth - 1 );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - 3, height() - borderBottom + radius - penWidth - 7 );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + 3, height() - borderBottom + radius - penWidth - 7 );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     height() - borderBottom + radius - penWidth - 1 );
  
  painterPath.setFillRule( Qt::OddEvenFill );
  
  painter->drawPath( painterPath );
  // End Pfeil zeichnen
}


void QvkRegionChoise::HandleBottomLeft()
{
  QColor color = Qt::green;
  QColor arrow = Qt::black;
  
  painter->setBrush( QBrush( color, Qt::SolidPattern ) );
  painter->setPen( QPen( Qt::black, penWidth ) );

  QRectF rectangle = QRectF( borderLeft - radius + penHalf, height() - borderBottom - radius - penHalf, 2 * radius, 2 * radius );
  painter->drawEllipse( rectangle );

  // Begin Pfeil zeichnen
  painter->setPen( QPen( arrow, 2 ) );

  double h = 2 * radius / 3;

  QPainterPath painterPath;
  painterPath.moveTo( borderLeft - frameWidth / 2, height() - borderBottom + frameWidth / 2 );
  painterPath.lineTo( borderLeft - h + penHalf,    height() - borderBottom + h - penHalf );
  painterPath.lineTo( borderLeft - h + 3,          height() - borderBottom + h - 7 );
  painterPath.lineTo( borderLeft - h + 7,          height() - borderBottom + h - 3 );
  painterPath.lineTo( borderLeft - h + penHalf,    height() - borderBottom + h - penHalf );
  
  painterPath.setFillRule( Qt::OddEvenFill );
  
  painter->drawPath( painterPath );
  // End Pfeil zeichnen
}


void QvkRegionChoise::HandleLeftMiddle()
{
  QColor color = Qt::green;
  QColor arrow = Qt::black;
  
  painter->setBrush( QBrush( color, Qt::SolidPattern ) );
  painter->setPen( QPen( Qt::black, penWidth ) );

  QRectF rectangle = QRectF( borderLeft - radius + penHalf, ( height() - borderTop - borderBottom ) / 2 + borderTop - radius, 2 * radius, 2 * radius );
  painter->drawEllipse( rectangle );
  
  // Begin Pfeil zeichnen
  painter->setPen( QPen( arrow, 2 ) );

  QPainterPath painterPath;
  painterPath.moveTo( borderLeft - frameWidth / 2,        ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath.lineTo( borderLeft - radius + penWidth,     ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath.lineTo( borderLeft - radius + penWidth + 7, ( height() - borderTop - borderBottom ) / 2 + borderTop + 3 );
  painterPath.lineTo( borderLeft - radius + penWidth + 7, ( height() - borderTop - borderBottom ) / 2 + borderTop - 3 );
  painterPath.lineTo( borderLeft - radius + penWidth,     ( height() - borderTop - borderBottom ) / 2 + borderTop );
  
  painterPath.setFillRule( Qt::OddEvenFill );
  
  painter->drawPath( painterPath );
  // End Pfeil zeichnen
}


void QvkRegionChoise::HandleMiddle()
{
  QColor color = Qt::green;
  QColor arrow = Qt::black;

  painter->setBrush( QBrush( color, Qt::SolidPattern ) );
  painter->setPen( QPen( Qt::black, penWidth ) );
  
  QRect rect( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius, 
              ( height() - borderTop - borderBottom ) / 2 + borderTop - radius,
                2 * radius,
                2 * radius );
  
  painter->drawEllipse ( rect );
  
  //Begin Pfeil zeichnen
  painter->setPen( QPen( arrow, 2 ) );

  QPainterPath painterPath;
  //arrow left
  painterPath.moveTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,                        ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius + penWidth,    ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius + penWidth + 7,( height() - borderTop - borderBottom ) / 2 + borderTop + 3 );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius + penWidth + 7,( height() - borderTop - borderBottom ) / 2 + borderTop - 3 );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius + penWidth,    ( height() - borderTop - borderBottom ) / 2 + borderTop );
  
  //arrow top
  painterPath.moveTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     ( height() - borderTop - borderBottom ) / 2 + borderTop - radius + penWidth );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + 3, ( height() - borderTop - borderBottom ) / 2 + borderTop - radius + penWidth + 7 );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - 3, ( height() - borderTop - borderBottom ) / 2 + borderTop - radius + penWidth + 7 );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     ( height() - borderTop - borderBottom ) / 2 + borderTop - radius + penWidth );
  
  //arrow right
  painterPath.moveTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,                         ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + radius - penWidth,     ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + radius - penWidth - 7, ( height() - borderTop - borderBottom ) / 2 + borderTop + 3 );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + radius - penWidth - 7, ( height() - borderTop - borderBottom ) / 2 + borderTop - 3 );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + radius - penWidth,     ( height() - borderTop - borderBottom ) / 2 + borderTop );

  //arrow bottom
  painterPath.moveTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     ( height() - borderTop - borderBottom ) / 2 + borderTop + radius - penWidth );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + 3, ( height() - borderTop - borderBottom ) / 2 + borderTop + radius - penWidth - 7 );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - 3, ( height() - borderTop - borderBottom ) / 2 + borderTop + radius - penWidth - 7 );
  painterPath.lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     ( height() - borderTop - borderBottom ) / 2 + borderTop + radius - penWidth );
  
  painterPath.setFillRule( Qt::OddEvenFill );
  
  painter->drawPath( painterPath );
  // End Pfeil zeichnen
  
  rect.setLeft( rect.left() - 2 );
  rect.setTop( rect.top() - 2 );
  rect.setWidth( rect.width() + 2 );
  rect.setHeight( rect.height() + 2 );
}


void QvkRegionChoise::printSize()
{
  QString widthHeigtSize = QString::number( getWidth() ) + " x " + QString::number( getHeight() );
  
  QFont font;
  font.setPointSize( 14 );
  painter->setFont( font );
  
  QFontMetrics fontMetrics( font );
  int pixelWidth = fontMetrics.width( widthHeigtSize );
  QRect rect( ( width() - borderLeft - borderRight ) / 2 + borderLeft - pixelWidth / 2 - 5,
	      ( height() - borderTop - borderBottom ) / 2 + borderTop - 2 * radius - 20,
	        pixelWidth + 10,
	        16 + 10 );
  
  painter->setBrush( QBrush( Qt::yellow, Qt::SolidPattern ) );
  painter->setPen( QPen( Qt::black, penWidth ) );  

  painter->drawRoundedRect( rect, 7, 7 );

  painter->drawText( rect, Qt::AlignCenter, widthHeigtSize );
  
  rect.setLeft( rect.left() - 2 );
  rect.setTop( rect.top() - 2 );
  rect.setWidth( rect.width() + 2 );
  rect.setHeight( rect.height() + 2 );
}


void QvkRegionChoise::paintEvent( QPaintEvent *event ) 
{
  QPixmap pixmap( width(), height() );
  pixmap.fill( Qt::transparent );
   
  painter->begin( &pixmap );
    painter->setRenderHints( QPainter::Antialiasing, true );
    HandleTopLeft();
    HandleTopMiddle();
    HandleTopRight();
    HandleRightMiddle();
    HandleBottomRight();
    HandleBottomMiddle();
    HandleBottomLeft();
    HandleLeftMiddle();
    printSize();
    HandleMiddle();

    // Blue Frame
    QPen pen( Qt::blue, frameWidth );
    pen.setJoinStyle( Qt::MiterJoin );
    painter->setPen( pen );
    painter->setBrush( QBrush( Qt::transparent, Qt::SolidPattern ) );
    painter->drawRect( borderLeft,
                       borderTop, 
                       width() - borderRight- borderRight,
                       height() - borderTop - borderBottom );
  painter->end();

  painter->begin( this);
    painter->drawPixmap( QPoint( 0, 0 ), pixmap );
  painter->end();
  
  setMask( pixmap.mask() ); //Extracts a bitmap mask from the pixmaps alpha channel.

  event->accept();
}


void QvkRegionChoise::lockFrame( bool status )
{
  frameLocked = status;
  handlingFrameLock();
}


bool QvkRegionChoise::isFrameLocked()
{
    return frameLocked;
}


void QvkRegionChoise::handlingFrameLock()
{
  repaint();
  update();
  
  // Der Desktopanimation "Langsames ausblenden" entgegenwirken
  clearMask();
  QRegion RegionWidget( 0, 0, width(), height() );
  QRegion RegionArea  ( borderLeft + frameWidth / 2,
                        borderTop + frameWidth / 2,
                        width() - ( borderLeft + frameWidth / 2 ) - ( borderRight + frameWidth / 2),
                        height() - ( borderTop + frameWidth / 2 ) - ( borderBottom + frameWidth / 2) );

  // subtract the record Area
  QRegion RegionNew = RegionWidget.subtracted( RegionArea );
  setMask( RegionNew );
}


/**
 * Return x from window
 */
int QvkRegionChoise::getX()
{
  return geometry().x(); 
}


/**
 * Return y from window
 */
int QvkRegionChoise::getY()
{
  return  geometry().y();
}


/**
 * Return x from record area
 */
int QvkRegionChoise::getXRecordArea()
{
  return geometry().x() + borderLeft + frameWidth / 2; 
}


/**
 * Return y from record area
 */
int QvkRegionChoise::getYRecordArea()
{
  return  geometry().y() + borderTop + frameWidth / 2;
}


/**
 * Return height from record area
 */
int QvkRegionChoise::getHeight()
{
  return height() - borderTop - borderBottom - frameWidth; 
}


/**
 * Return width from record Area
 */
int QvkRegionChoise::getWidth()
{
  return width() - borderLeft - borderRight - frameWidth; 
}


/*! MouseMove fuer das bewegen des Fensters und Ränder
\param event QMouseEvent Mouse Event
*/
void QvkRegionChoise::mouseMoveEvent( QMouseEvent *event )
{
  if ( isFrameLocked() )
    return;
   
  switch ( handlePressed )
  {
    case NoHandle    :                            break;
    case TopLeft     : moveTopLeft( event );      break;
    case TopMiddle   : moveTopMiddle( event );    break;
    case TopRight    : moveTopRight( event );     break;
    case RightMiddle : moveRightMiddle( event );  break;
    case BottomRight : moveBottomRight( event );  break;
    case BottomMiddle: moveBottomMiddle( event ); break;
    case BottomLeft  : moveBottomLeft( event );   break;
    case LeftMiddle  : moveLeftMiddle( event );   break;
    case Middle      : moveMiddle( event );       break;
    return;
  }
 
  if ( ( event->x() > ( borderLeft - radius ) ) and
       ( event->x() < ( borderLeft + radius ) ) and 
       ( event->y() > ( borderTop - radius  ) ) and 
       ( event->y() < ( borderTop + radius) ) )
  {
    setCursor( Qt::SizeFDiagCursor );  
    handleUnderMouse = TopLeft;
    return;
  }

  if ( ( event->x() > ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius ) and
       ( event->x() < ( width() - borderLeft - borderRight ) / 2 + borderLeft + radius ) and
       ( event->y() > borderTop - radius ) and
       ( event->y() < borderTop + frameWidth + radius) )
  {
    setCursor( Qt::SizeVerCursor );  
    handleUnderMouse = TopMiddle;
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
  
  if ( ( event->x() > width() - borderRight - frameWidth - radius ) and
       ( event->x() < width() - borderRight + radius ) and
       ( event->y() > ( ( height() - borderTop - borderBottom ) / 2 + borderTop - radius ) ) and
       ( event->y() < ( ( height() - borderTop - borderBottom ) / 2 + borderTop + radius ) ) )
  {
    setCursor( Qt::SizeHorCursor );
    handleUnderMouse = RightMiddle;
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

  if ( ( event->x() > ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius ) and 
       ( event->x() < ( width() - borderLeft - borderRight ) / 2 + borderLeft + radius ) and 
       ( event->y() < height() - borderBottom + radius ) and 
       ( event->y() > height() - borderBottom - frameWidth  - radius) )
  {
    setCursor( Qt::SizeVerCursor );
    handleUnderMouse = BottomMiddle;
    return;
  }
  
  if ( ( event->x() > ( borderLeft - radius ) ) and
       ( event->x() < ( borderLeft + radius ) ) and
       ( event->y() > ( height() - borderBottom - radius ) ) and
       ( event->y() < height() ) )
  {
    setCursor( Qt::SizeBDiagCursor );
    handleUnderMouse = BottomLeft;
    return;
  }
  
  if ( ( event->x() > ( borderLeft - radius ) ) and
       ( event->x() < ( borderLeft + radius ) ) and 
       ( event->y() > ( ( height() - borderTop - borderBottom ) / 2 + borderTop - radius ) ) and
       ( event->y() < ( ( height() - borderTop - borderBottom ) / 2 + borderTop + radius ) ) )
  {
    setCursor( Qt::SizeHorCursor );
    handleUnderMouse = LeftMiddle;
    return;
  }
  
  if ( ( event->x() > ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius ) and 
       ( event->x() < ( width() - borderLeft - borderRight ) / 2 + borderLeft + radius ) and
       ( event->y() > ( ( height() - borderTop - borderBottom ) / 2 + borderTop - radius ) ) and
       ( event->y() < ( ( height() - borderTop - borderBottom ) / 2 + borderTop + radius ) ) )
  {
    setCursor( Qt::SizeAllCursor );
    handleUnderMouse = Middle;
    return;
  }
  
  handleUnderMouse = NoHandle;
  
  unsetCursor();

  event->accept();
}


void QvkRegionChoise::moveTopLeft( QMouseEvent *event )
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
  if ( mouseGlobalY >= widgetY + widgetHeight + currentMouseLocalY - 200 )
    mouseGlobalY = widgetY + widgetHeight + currentMouseLocalY - 200;

  if ( mouseGlobalX >= widgetX + widgetWidth + currentMouseLocalX - 200 )
    mouseGlobalX = widgetX + widgetWidth + currentMouseLocalX - 200;
  
  // Maximale Größe begrenzen
  if ( mouseGlobalY - currentMouseLocalY < radius * -1 - frameWidth/2 )
    mouseGlobalY = widgetY + currentMouseLocalY;
  
  if ( mouseGlobalX - currentMouseLocalX < radius * -1 - frameWidth/2 )
    mouseGlobalX = widgetX + currentMouseLocalX;
  
  // Neue Geometry des Dialogfenster setzen
  this->setGeometry( mouseGlobalX - currentMouseLocalX,
		     mouseGlobalY - currentMouseLocalY, 
		     widgetWidth + ( widgetX - mouseGlobalX + currentMouseLocalX ),
		     widgetHeight + ( widgetY - mouseGlobalY + currentMouseLocalY ) );
  
  event->accept();
}


void QvkRegionChoise::moveTopMiddle( QMouseEvent *event )
{
  // Globale Mauskoordinaten 
  int mouseGlobalY = event->globalY();
  
  // Alte Globale Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetWidth = geometry().width();
  int widgetHeight = geometry().height();

  // Minimale Größe des Widget begrenzen
  if ( mouseGlobalY >= widgetY + widgetHeight + currentMouseLocalY - 200 )
    mouseGlobalY = widgetY + widgetHeight + currentMouseLocalY - 200;
  
  // Maximale Größe begrenzen
  if ( mouseGlobalY - currentMouseLocalY < radius * -1 - frameWidth/2 )
    mouseGlobalY = widgetY + currentMouseLocalY;
  
  // Neue Geometry des HauptWidget setzen
  this->setGeometry( widgetX,
		     mouseGlobalY - currentMouseLocalY,
		     widgetWidth,
		     widgetHeight + ( widgetY - mouseGlobalY + currentMouseLocalY ) );
}


void QvkRegionChoise::moveTopRight( QMouseEvent *event )
{
  // Globale Mauskoordinaten
  int mouseGlobalX = event->globalX();
  int mouseGlobalY = event->globalY();
  
  // Alte Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetHeight = geometry().height();
  int widgetWidth = geometry().width();

  // Minimale Größe des Widget begrenzen
  if ( mouseGlobalX <= widgetX + 200 - currentMouseRightLocalX )
    mouseGlobalX = widgetX + 200 - currentMouseRightLocalX;
  
  if ( mouseGlobalY >= widgetY + widgetHeight + currentMouseLocalY - 200 )
    mouseGlobalY = widgetY + widgetHeight + currentMouseLocalY - 200;
  
  // Maximale Größe begrenzen
  if ( mouseGlobalY - currentMouseLocalY < radius * -1 - frameWidth/2 )
    mouseGlobalY = widgetY + currentMouseLocalY;
  
  QDesktopWidget *desk = QApplication::desktop();
  if ( mouseGlobalX + currentMouseRightLocalX > desk->width() - radius * -1 + frameWidth/2 )
    mouseGlobalX = widgetX + widgetWidth - currentMouseRightLocalX;
  
  // Neue Geometry des Fenster setzen
  this->setGeometry( widgetX,
		     mouseGlobalY - currentMouseLocalY,
		     currentWidgetWidth + ( mouseGlobalX - ( widgetX + currentMouseLocalX ) ),
     		     widgetHeight  + ( widgetY - mouseGlobalY + currentMouseLocalY ) );
}


void QvkRegionChoise::moveRightMiddle( QMouseEvent *event )
{
  // Globale Mauskoordinaten
  int mouseGlobalX = event->globalX();
  
  // Alte Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetHeight = geometry().height();
  int widgetWidth = geometry().width();

  // Minimale Größe des Widget begrenzen
  if ( mouseGlobalX <= widgetX + 200 - currentMouseRightLocalX )
    mouseGlobalX = widgetX + 200 - currentMouseRightLocalX;

  // Maximale größe begrenzen
  QDesktopWidget *desk = QApplication::desktop();
  if ( mouseGlobalX + currentMouseRightLocalX > desk->width() - radius * -1 + frameWidth/2)
    mouseGlobalX = widgetX + widgetWidth - currentMouseRightLocalX;
  
  this->setGeometry( widgetX,
		     widgetY,
		     currentWidgetWidth + ( mouseGlobalX - ( widgetX + currentMouseLocalX ) ),
		     widgetHeight );
}


void QvkRegionChoise::moveBottomRight( QMouseEvent *event )
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
  if ( mouseGlobalX <= widgetX + 200 - currentMouseRightLocalX )
    mouseGlobalX = widgetX + 200 - currentMouseRightLocalX;
  
  if ( mouseGlobalY <= widgetY + 200 - currentMouseRightLocalY )
    mouseGlobalY = widgetY + 200 - currentMouseRightLocalY;
  
  // Maximale größe begrenzen
  QDesktopWidget *desk = QApplication::desktop();
  if ( mouseGlobalX + currentMouseRightLocalX > desk->width() - radius * -1 + frameWidth/2 )
    mouseGlobalX = widgetX + widgetWidth - currentMouseRightLocalX;
  
  if ( mouseGlobalY + currentMouseRightLocalY > desk->height() - radius * -1 + frameWidth/2 )
    mouseGlobalY = widgetY + widgetHeight - currentMouseRightLocalY;
  
  this->setGeometry( widgetX,
		     widgetY,
		     currentWidgetWidth + ( mouseGlobalX - ( widgetX + currentMouseLocalX ) ),
		     currentWidgetHeight + ( mouseGlobalY - ( widgetY + currentMouseLocalY ) ) );
}


void QvkRegionChoise::moveBottomMiddle( QMouseEvent *event )
{
  // Globale Mauskoordinaten
  int mouseGlobalY = event->globalY();
  
  // Alte Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetWidth = geometry().width();
  int widgetHeight = geometry().height();

  // Minimale Größe des Widget begrenzen
  if ( mouseGlobalY <= widgetY + 200 - currentMouseRightLocalY )
    mouseGlobalY = widgetY + 200 - currentMouseRightLocalY;
  
  // Maximale größe begrenzen
  QDesktopWidget *desk = QApplication::desktop();

  //if ( mouseGlobalY + currentMouseRightLocalY > desk->height() )
  if ( mouseGlobalY + currentMouseRightLocalY > desk->height() - radius * -1 + frameWidth/2)
    mouseGlobalY = widgetY + widgetHeight - currentMouseRightLocalY;

  this->setGeometry( widgetX,
		     widgetY,
     		     widgetWidth,
		     currentWidgetHeight + ( mouseGlobalY - ( widgetY + currentMouseLocalY ) ) );
}


void QvkRegionChoise::moveBottomLeft( QMouseEvent *event )
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
  if ( mouseGlobalY <= widgetY + 200 - currentMouseRightLocalY )
    mouseGlobalY = widgetY + 200 - currentMouseRightLocalY;

  if ( mouseGlobalX >= widgetX + widgetWidth + currentMouseLocalX - 200 )
    mouseGlobalX = widgetX + widgetWidth + currentMouseLocalX - 200;

  // Maximale Größe des Widget begrenzen
  if ( mouseGlobalX - currentMouseLocalX < radius * -1 - frameWidth/2 )
    mouseGlobalX = widgetX + currentMouseLocalX;

  QDesktopWidget *desk = QApplication::desktop();
  if ( mouseGlobalY + currentMouseRightLocalY > desk->height() - radius * -1 + frameWidth/2 )
    mouseGlobalY = widgetY + widgetHeight - currentMouseRightLocalY;
  
  this->setGeometry( mouseGlobalX - currentMouseLocalX,
		     widgetY,
     		     widgetWidth + ( widgetX - mouseGlobalX + currentMouseLocalX ),
		     currentWidgetHeight + ( mouseGlobalY - ( widgetY + currentMouseLocalY ) ) );
}


void QvkRegionChoise::moveLeftMiddle( QMouseEvent *event )
{
  // Globale Mauskoordinaten
  int mouseGlobalX = event->globalX();
  
  // Alte Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetWidth = geometry().width();
  int widgetHeight = geometry().height();
  
  // Minimale Größe des Widget begrenzen
  if ( mouseGlobalX >= widgetX + widgetWidth + currentMouseLocalX - 200 )
    mouseGlobalX = widgetX + widgetWidth + currentMouseLocalX - 200;
  
  // Maximale Größe des Widget begrenzen
  if ( mouseGlobalX - currentMouseLocalX < radius * -1 - frameWidth/2)
    mouseGlobalX = widgetX + currentMouseLocalX;
  
  // Neue Geometry des Dialogfenster setzen
  this->setGeometry( mouseGlobalX - currentMouseLocalX,
		     widgetY,
		     widgetWidth + ( widgetX - mouseGlobalX + currentMouseLocalX ),
		     widgetHeight );
}


void QvkRegionChoise::moveMiddle( QMouseEvent *event )
{
  // Globale Mauskoordinaten
  int mouseGlobalX = event->globalX();
  int mouseGlobalY = event->globalY();

  // Alte Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetWidth = geometry().width();
  int widgetHeight = geometry().height();
  
  QDesktopWidget *desk = QApplication::desktop();
  if ( desk->screenCount() < 2 ) 
  {
    // Das Verschieben begrenzen
    // Top
    if ( mouseGlobalY - currentMouseLocalY < radius * -1 - frameWidth/2 )
      mouseGlobalY = widgetY + currentMouseLocalY;

    // Right
    if ( mouseGlobalX + currentMouseRightLocalX > desk->width() - radius * -1 + frameWidth/2 )
      mouseGlobalX = widgetX + widgetWidth - currentMouseRightLocalX;
  
    // Bottom
    if ( mouseGlobalY + currentMouseRightLocalY > desk->height() - radius * -1 + frameWidth/2 )
      mouseGlobalY = widgetY + widgetHeight - currentMouseRightLocalY;
    
    // Left
    if ( mouseGlobalX - currentMouseLocalX < radius * -1 - frameWidth/2 )
      mouseGlobalX = widgetX + currentMouseLocalX;
  }
  
  move( mouseGlobalX - currentMouseLocalX, mouseGlobalY - currentMouseLocalY );
}


void QvkRegionChoise::mousePressEvent( QMouseEvent *event )
{
  // Position bei klick im Fenster festhalten
  currentMouseLocalX = event->x();
  currentMouseLocalY = event->y();
  
  //Abstand von Rechte Seite Widget bis Cursor
  currentMouseRightLocalX = width() - currentMouseLocalX;
  currentMouseRightLocalY = height() - currentMouseLocalY;
  
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
  
  event->accept();  
}


void QvkRegionChoise::mouseReleaseEvent( QMouseEvent * event )
{
  (void)event;
  handlePressed = NoHandle;
  event->accept();  
}


void QvkRegionChoise::mouseDoubleClickEvent( QMouseEvent * event )
{
  // Hint:
  // 1. Without slow motion, hide() and show() must have under Ubuntu 16.04 otherwise glitch
  // 2. With slow motion, repaint must have otherwise glitch
  // 3. return wird gebraucht da ansonsten evtl. auch mal "Middle" aufgerufen wird
  
  // Am besten ist es wenn alle Knöpfe slow motion sind, dann werden keine ungereimtheiten unter opensuse und Ubuntu beobachtet.
  
  int sleep = 8;
  int slowMotionWay = 100;
  QDesktopWidget *desk = QApplication::desktop();
  
  if ( handleUnderMouse == TopLeft )
  {
    setGeometry( 0 - radius - frameWidth / 2,
		 0 - radius - frameWidth / 2,
		 geometry().x() + geometry().width() + radius + frameWidth / 2,
		 geometry().y() + geometry().height() + radius + frameWidth / 2 );
    show();
    event->accept();  
    return;
  }
  
  if ( handleUnderMouse == TopMiddle )
  {
    setGeometry( geometry().x(),
		 0 - radius - frameWidth / 2,
		 geometry().width(),
		 geometry().y() + geometry().height() + radius + frameWidth / 2 );
    show();
    event->accept();  
    return;
  }

  if ( handleUnderMouse == TopRight )
  {
    setGeometry( geometry().x(),
		 0 - radius - frameWidth / 2,
		 desk->width() - geometry().x() + radius + frameWidth / 2,
		 geometry().y() + geometry().height() + radius + frameWidth / 2 );
    show();
    event->accept();  
    return;
  }
    
  if ( handleUnderMouse == RightMiddle )
  {
    setGeometry( geometry().x(),
                 geometry().y(),
                 desk->width() - geometry().x() + radius + frameWidth / 2 - slowMotionWay,
                 geometry().height() );
    
    int width = this->width();
    for (int i = width; i <= desk->width() - geometry().x() + radius + frameWidth / 2; i+=1)
    {
      setGeometry( geometry().x(),
                   geometry().y(),
                   i,
                   geometry().height() );
      repaint();
      QTest::qSleep( sleep );
    }
    event->accept();  
    return;
  }
    
  if ( handleUnderMouse == BottomRight )
  {
    setGeometry( geometry().x(),
                 geometry().y(),
		 desk->width() - geometry().x() + radius + frameWidth / 2,
		 desk->height() - geometry().y() + radius + frameWidth / 2 );
    show();
    event->accept();  
    return;
  }
  
  if ( handleUnderMouse == BottomMiddle )
  {
    setGeometry( geometry().x(),
                 geometry().y(),
 		 geometry().width(),
		 desk->height() - geometry().y() + radius + frameWidth / 2 );
    show();
    event->accept();  
    return;
  }
  
  if ( handleUnderMouse == BottomLeft )
  {
    setGeometry( 0 - radius - frameWidth / 2,
                 geometry().y(),
		 geometry().x() + geometry().width() + radius + frameWidth / 2,
		 desk->height() - geometry().y() + radius + frameWidth / 2 );
    show();
    event->accept();  
    return;
  }

  if ( handleUnderMouse == LeftMiddle )
  {
    setGeometry( 0 - radius - frameWidth / 2 + slowMotionWay,
                 geometry().y(),
		 geometry().x() + geometry().width() + radius + frameWidth / 2 - slowMotionWay,
		 geometry().height() );
    
    int x = this->x();
    int width = this->width();
    for ( int i = x; i >= 0 - radius - frameWidth / 2; i-- )
    {
      setGeometry( i,
                   geometry().y(),
                   x-i + width,
                   geometry().height() );
      repaint();
      QTest::qSleep( sleep );
    }
    event->accept();  
    return;
  }
  
  if ( handleUnderMouse == Middle )
  {
    setGeometry( 0 - radius - frameWidth / 2,
                 0 - radius - frameWidth / 2,
		 desk->width() + 2 * radius + frameWidth,
		 desk->height() + 2 * radius + frameWidth );
    show();
  }

  event->accept();  
}
