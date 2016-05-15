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

#include "QvkRegionRecord.h" 

QvkRegionRecord::QvkRegionRecord()
{
  
  setAttribute ( Qt::WA_AlwaysShowToolTips );
  
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

  hide();
}


QvkRegionRecord::~QvkRegionRecord()
{
}


void QvkRegionRecord::setRecordGeometry( int x, int y, int width, int height )
{
  setGeometry( x, y, width + borderLeft + borderRight + frameWidth, height + borderTop + borderBottom + frameWidth );
}


void QvkRegionRecord::HandleTopLeft()
{
  QColor color, arrow;
  
  color = Qt::red;
  arrow = Qt::red;
  
  QBrush brush( color, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );
  
  QRectF rectangle = QRectF( borderLeft - radius + penHalf, borderTop - radius + penHalf, 2 * radius, 2 * radius );
  int startAngle = 0 * 16;
  int spanAngle = 360 * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );

  // Begin Pfeil zeichnen
  double h = 2 * radius / 3;
  painter->setPen( QPen( arrow, 2 ) );

  QPainterPath * painterPath = new QPainterPath();
  painterPath->moveTo( borderLeft - frameWidth / 2 , borderTop - frameWidth / 2 );
  painterPath->lineTo( borderLeft - h + penHalf, borderTop - h + penHalf );
  painterPath->lineTo( borderLeft - h + penHalf + 7, borderTop - h + penHalf + 3 );
  painterPath->lineTo( borderLeft - h + penHalf + 3, borderTop - h + penHalf + 7 );
  painterPath->lineTo( borderLeft - h + penHalf, borderTop - h + penHalf );
  painter->drawPath( *painterPath );
}


void QvkRegionRecord::HandleTopMiddle()
{
  QColor color, arrow;
  
  color = Qt::red;
  arrow = Qt::red;
  
  QBrush brush( color, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );
  
  QRectF rectangle = QRectF( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius, borderTop - radius + penHalf, 2 * radius, 2 * radius );
  int startAngle = 0 * 16;
  int spanAngle = 360 * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
  painter->setPen( QPen( arrow, 2 ) );
  QPainterPath * painterPath = new QPainterPath();
  painterPath->moveTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     borderTop );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     borderTop - radius + penWidth + penHalf );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - 3, borderTop - radius + penWidth+7 );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + 3, borderTop - radius + penWidth+7 );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     borderTop - radius + penWidth + penHalf );
  painter->drawPath( *painterPath );
}


void QvkRegionRecord::HandleTopRight()
{
  QColor color, arrow;
  
  color = Qt::red;
  arrow = Qt::red;
  
  QBrush brush( color, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );

  QRectF rectangle = QRectF( width() - borderRight - radius - penHalf, borderTop - radius + penHalf, 2 * radius, 2 * radius );
  int startAngle = 180 * 16;
  int spanAngle =  -360  * 16;
  
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


void QvkRegionRecord::HandleRightMiddle()
{
  QColor color, arrow;
  
  color = Qt::red;
  arrow = Qt::red;
  
  QBrush brush( color, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );
  
  QRectF rectangle = QRectF( width() - borderRight - radius - penHalf, ( height() - borderTop - borderBottom ) / 2 + borderTop - radius,  2 * radius, 2 * radius );
  int startAngle = 90 * 16;
  int spanAngle =  -360  * 16;
  painter->drawPie( rectangle, startAngle, spanAngle ); 
  
   // Begin Pfeil zeichnen
  painter->setPen( QPen( arrow, 2 ) );

  QPainterPath * painterPath = new QPainterPath();
  painterPath->moveTo( width() - borderRight + frameWidth / 2       , ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath->lineTo( width() - borderRight + radius - penWidth - 1, ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath->lineTo( width() - borderRight + radius - penWidth - 7, ( height() - borderTop - borderBottom ) / 2 + borderTop + 3 );
  painterPath->lineTo( width() - borderRight + radius - penWidth - 7, ( height() - borderTop - borderBottom ) / 2 + borderTop - 3 );
  painterPath->lineTo( width() - borderRight + radius - penWidth - 1, ( height() - borderTop - borderBottom ) / 2 + borderTop );

  painterPath->setFillRule( Qt::OddEvenFill );
  
  painter->drawPath( *painterPath );
  // End Pfeil zeichnen
}


void QvkRegionRecord::HandleBottomRight()
{
  QColor color, arrow;
  
  color = Qt::red;
  arrow = Qt::red;
  
  QBrush brush( color, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );
  
  QRectF rectangle = QRectF( width() - borderRight - radius - penHalf, height() - borderBottom - radius - penHalf, 2 * radius, 2 * radius );
  int startAngle = 90 * 16;
  int spanAngle =  -360  * 16;
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


void QvkRegionRecord::HandleBottomMiddle()
{
  QColor color, arrow;
  
  color = Qt::red;
  arrow = Qt::red;
  
  QBrush brush( color, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );

  QRectF rectangle = QRectF( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius, height() - borderBottom - radius - penHalf, 2 * radius, 2 * radius );
  int startAngle = 0 * 16;
  int spanAngle =  -360  * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
  
  // Begin Pfeil zeichnen
  painter->setPen( QPen( arrow, 2 ) );

  QPainterPath * painterPath = new QPainterPath();
  painterPath->moveTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     height() - borderBottom );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     height() - borderBottom + radius - penWidth - 1 );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - 3, height() - borderBottom + radius - penWidth - 7 );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + 3, height() - borderBottom + radius - penWidth - 7 );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     height() - borderBottom + radius - penWidth - 1 );
  
  painterPath->setFillRule( Qt::OddEvenFill );
  
  painter->drawPath( *painterPath );
  // End Pfeil zeichnen
}


void QvkRegionRecord::HandleBottomLeft()
{
  QColor color, arrow;
  
  color = Qt::red;
  arrow = Qt::red;
  
  QBrush brush( color, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );

  QRectF rectangle = QRectF( borderLeft - radius + penHalf, height() - borderBottom - radius - penHalf, 2 * radius, 2 * radius );
  int startAngle = 90 * 16;
  int spanAngle =  360  * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );

  // Begin Pfeil zeichnen
  painter->setPen( QPen( arrow, 2 ) );

  double h = 2 * radius / 3;

  QPainterPath * painterPath = new QPainterPath();
  painterPath->moveTo( borderLeft - frameWidth / 2, height() - borderBottom + frameWidth / 2 );
  painterPath->lineTo( borderLeft - h + penHalf,    height() - borderBottom + h - penHalf );
  painterPath->lineTo( borderLeft - h + 3,          height() - borderBottom + h - 7 );
  painterPath->lineTo( borderLeft - h + 7,          height() - borderBottom + h - 3 );
  painterPath->lineTo( borderLeft - h + penHalf,    height() - borderBottom + h - penHalf );
  
  painterPath->setFillRule( Qt::OddEvenFill );
  
  painter->drawPath( *painterPath );
  // End Pfeil zeichnen
}


void QvkRegionRecord::HandleLeftMiddle()
{
  QColor color, arrow;
  
  color = Qt::red;
  arrow = Qt::red;
  
  QBrush brush( color, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );

  QRectF rectangle = QRectF( borderLeft - radius + penHalf, ( height() - borderTop - borderBottom ) / 2 + borderTop - radius, 2 * radius, 2 * radius );
  int startAngle = 90 * 16;
  int spanAngle =  360  * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
  
  // Begin Pfeil zeichnen
  painter->setPen( QPen( arrow, 2 ) );

  QPainterPath * painterPath = new QPainterPath();
  painterPath->moveTo( borderLeft - frameWidth / 2,        ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath->lineTo( borderLeft - radius + penWidth,     ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath->lineTo( borderLeft - radius + penWidth + 7, ( height() - borderTop - borderBottom ) / 2 + borderTop + 3 );
  painterPath->lineTo( borderLeft - radius + penWidth + 7, ( height() - borderTop - borderBottom ) / 2 + borderTop - 3 );
  painterPath->lineTo( borderLeft - radius + penWidth,     ( height() - borderTop - borderBottom ) / 2 + borderTop );
  
  painterPath->setFillRule( Qt::OddEvenFill );
  
  painter->drawPath( *painterPath );
  // End Pfeil zeichnen
}


void QvkRegionRecord::paintEvent( QPaintEvent *event ) 
{
  (void)event;
  painter->begin( this );
  painter->setRenderHints( QPainter::Antialiasing, true );

  // Maskiert den Bereich für PrintSize und HandleMiddle
  // Widget
  clearMask();
  QRegion RegionWidget( 0, 0, width(), height() );
    
  // RecordArea
  QRegion RegionArea  ( borderLeft + frameWidth / 2,
                        borderTop + frameWidth / 2,
                        width() - ( borderLeft + frameWidth / 2 ) - ( borderRight + frameWidth / 2 ),
                        height() - ( borderTop + frameWidth / 2 ) - ( borderBottom + frameWidth / 2 ) );

  // subtract the record Area
  QRegion RegionNew = RegionWidget.subtracted( RegionArea );
  setMask( RegionNew );

  HandleTopLeft();
  HandleTopMiddle();
  HandleTopRight();
  HandleRightMiddle();
  HandleBottomRight();
  HandleBottomMiddle();
  HandleBottomLeft();
  HandleLeftMiddle();

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

  event->accept();  

}


void QvkRegionRecord::lockFrame( bool status )
{
    frameLocked = status;
    handlingFrameLock();
}


bool QvkRegionRecord::isFrameLocked()
{
    return frameLocked;
}


void QvkRegionRecord::handlingFrameLock()
{
  repaint();
  update();
}


/**
 * Return x from window
 */
int QvkRegionRecord::getX()
{
  return geometry().x(); 
}


/**
 * Return y from window
 */
int QvkRegionRecord::getY()
{
  return  geometry().y();
}


/**
 * Return x from record area
 */
int QvkRegionRecord::getXRecordArea()
{
  return geometry().x() + borderLeft + frameWidth / 2; 
}


/**
 * Return y from record area
 */
int QvkRegionRecord::getYRecordArea()
{
  return  geometry().y() + borderTop + frameWidth / 2;
}


/**
 * Return height from record area
 */
int QvkRegionRecord::getHeight()
{
  return height() - borderTop - borderBottom - frameWidth; 
}


/**
 * Return width from record Area
 */
int QvkRegionRecord::getWidth()
{
  return width() - borderLeft - borderRight - frameWidth; 
}


