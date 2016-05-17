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


