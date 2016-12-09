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
  QRectF rectangle = QRectF( borderLeft - radius + penHalf, borderTop - radius + penHalf, 2 * radius, 2 * radius );
  int startAngle = 0 * 16;
  int spanAngle = 360 * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
}


void QvkRegionRecord::HandleTopMiddle()
{
  QRectF rectangle = QRectF( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius, borderTop - radius + penHalf, 2 * radius, 2 * radius );
  int startAngle = 0 * 16;
  int spanAngle = 360 * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
}


void QvkRegionRecord::HandleTopRight()
{
  QRectF rectangle = QRectF( width() - borderRight - radius - penHalf, borderTop - radius + penHalf, 2 * radius, 2 * radius );
  int startAngle = 180 * 16;
  int spanAngle =  -360  * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
}


void QvkRegionRecord::HandleRightMiddle()
{
  QRectF rectangle = QRectF( width() - borderRight - radius - penHalf, ( height() - borderTop - borderBottom ) / 2 + borderTop - radius,  2 * radius, 2 * radius );
  int startAngle = 90 * 16;
  int spanAngle =  -360  * 16;
  painter->drawPie( rectangle, startAngle, spanAngle ); 
}


void QvkRegionRecord::HandleBottomRight()
{
  QRectF rectangle = QRectF( width() - borderRight - radius - penHalf, height() - borderBottom - radius - penHalf, 2 * radius, 2 * radius );
  int startAngle = 90 * 16;
  int spanAngle =  -360  * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
}


void QvkRegionRecord::HandleBottomMiddle()
{
  QRectF rectangle = QRectF( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius, height() - borderBottom - radius - penHalf, 2 * radius, 2 * radius );
  int startAngle = 0 * 16;
  int spanAngle =  -360  * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
}


void QvkRegionRecord::HandleBottomLeft()
{
  QRectF rectangle = QRectF( borderLeft - radius + penHalf, height() - borderBottom - radius - penHalf, 2 * radius, 2 * radius );
  int startAngle = 90 * 16;
  int spanAngle =  360  * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
}


void QvkRegionRecord::HandleLeftMiddle()
{
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

  clearMask();

  // Aufruf von QRegion = x,y,w,h
  QRegion RegionWidget( 0, 0, width(), height() );
  RegionWidget = RegionWidget.subtracted( RegionWidget );
  
  QRegion RegionHandleTopLeft( borderLeft - radius, borderTop - radius, 2*radius + penWidth, 2*radius + penWidth);
  RegionWidget = RegionWidget.united( RegionHandleTopLeft );
  
  QRegion RegionHandleTopMiddle( ( width() - borderLeft - borderRight ) / 2 - penHalf, borderTop - radius, 2*radius + penWidth, 2*radius + penWidth );
  RegionWidget = RegionWidget.united( RegionHandleTopMiddle );
  
  QRegion RegionHandleTopRight( width() - borderRight - radius - penWidth, borderTop - radius, 2*radius + penWidth, 2*radius + penWidth );
  RegionWidget = RegionWidget.united( RegionHandleTopRight );
  
  QRegion RegionHandleRightMiddle( width() - borderRight - radius - penHalf, ( height() - borderTop - borderBottom ) / 2 - penHalf,  2*radius + penWidth, 2*radius + penWidth );
  RegionWidget = RegionWidget.united( RegionHandleRightMiddle );
  
  QRegion RegionHandleRightBottom( width() - borderRight - radius - penWidth, height() - borderBottom - radius - penWidth, 2*radius + penWidth, 2*radius + penWidth );
  RegionWidget = RegionWidget.united( RegionHandleRightBottom );
  
  QRegion RegionHandleBottomMiddle( ( width() - borderLeft - borderRight ) / 2 - penHalf, height() - borderBottom - radius - penHalf, 2*radius + penWidth, 2*radius + penWidth );
  RegionWidget = RegionWidget.united( RegionHandleBottomMiddle );
  
  QRegion RegionHandleBottomLeft( borderLeft - radius, height() - borderBottom - radius - penWidth, 2*radius + penWidth, 2*radius + penWidth );
  RegionWidget = RegionWidget.united( RegionHandleBottomLeft );
  
  QRegion RegionHandleLeftMiddle( borderLeft - radius, ( height() - borderTop - borderBottom ) / 2 - penHalf, 2*radius + penWidth, 2*radius + penWidth );
  RegionWidget = RegionWidget.united( RegionHandleLeftMiddle );
  
  // Aufruf von QRegion = x,y,w,h
  QRegion RegionTopLine( borderLeft - frameWidth, borderTop - frameWidth/2, width() - borderLeft - borderRight - penWidth, frameWidth );
  RegionWidget = RegionWidget.united( RegionTopLine );
  
  QRegion RegionRightLine(  width() - borderRight - frameWidth/2, borderTop - frameWidth/2, frameWidth, height() - borderBottom );
  RegionWidget = RegionWidget.united( RegionRightLine );
  
  QRegion RegionBottomLine( borderLeft, height() - borderBottom - frameWidth/2, width() - borderRight, frameWidth );
  RegionWidget = RegionWidget.united( RegionBottomLine );

  QRegion RegionLeftLine( borderLeft - frameWidth/2, borderTop, frameWidth, height() - borderTop - borderBottom );
  RegionWidget = RegionWidget.united( RegionLeftLine );
  
  // subtract the record Area
  QRegion RegionArea  ( borderLeft + frameWidth / 2,
                        borderTop + frameWidth / 2,
                        width() - ( borderLeft + frameWidth / 2 ) - ( borderRight + frameWidth / 2 ),
                        height() - ( borderTop + frameWidth / 2 ) - ( borderBottom + frameWidth / 2 ) );
  RegionWidget = RegionWidget.subtracted( RegionArea );

  setMask( RegionWidget );

  QBrush brush( Qt::red, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );
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

  painter->drawRect( borderLeft,
		     borderTop, 
		     width() - borderRight- borderRight,
		     height() - borderTop - borderBottom);
 
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


