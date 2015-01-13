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

#include "regionselection.h" 
#include <QSettings>

regionselection::regionselection()
{
  setAttribute ( Qt::WA_AlwaysShowToolTips );
  setToolTip( tr( "doubleclick extended to the edge" ) );
  
  handlePressed = NoHandle;
  handleUnderMouse = NoHandle;
  painter =  new QPainter();
  
  // Muß in die vokoscreen.conf von Hand eingetragen werden. Keine GUI Unterstützung.
  // NoShowInTaskBar=1
  QSettings settings( "vokoscreen", "vokoscreen" );
  settings.beginGroup( "Area" );
    if ( settings.value( "NoShowInTaskBar", 1 ).toUInt() == 1 )
    {
      setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip );
      Setting_Area_NoShowInTaskBar = 1;
    }
    else
    {
      setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
      Setting_Area_NoShowInTaskBar = 0;
    }
  settings.endGroup();
  
  //if( QX11Info::isCompositingManagerRunning() ) // ******************************* vorerst ersatzlos gestrichen ********************************************************
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
}


regionselection::~regionselection()
{
}


void regionselection::saveSettings()
{
  QSettings settings( "vokoscreen", "vokoscreen" );   
  settings.beginGroup( "Area" );
    settings.setValue( "NoShowInTaskBar", Setting_Area_NoShowInTaskBar );
  settings.endGroup();
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


void regionselection::HandleBottomMiddle()
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


void regionselection::HandleBottomLeft()
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


void regionselection::HandleLeftMiddle()
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


void regionselection::HandleMiddle()
{
  QColor color, arrow;
  
  color = Qt::green;
  arrow = Qt::black;

  QBrush brush( color, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );
  
  QRect rect( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius, 
              ( height() - borderTop - borderBottom ) / 2 + borderTop - radius,
                2 * radius,
                2 * radius );
  
  painter->drawEllipse ( rect );
  
  //Begin Pfeil zeichnen
  painter->setPen( QPen( arrow, 2 ) );

  QPainterPath * painterPath = new QPainterPath();
  //arrow left
  painterPath->moveTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,                        ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius + penWidth,    ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius + penWidth + 7,( height() - borderTop - borderBottom ) / 2 + borderTop + 3 );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius + penWidth + 7,( height() - borderTop - borderBottom ) / 2 + borderTop - 3 );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius + penWidth,    ( height() - borderTop - borderBottom ) / 2 + borderTop );
  
  //arrow top
  painterPath->moveTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     ( height() - borderTop - borderBottom ) / 2 + borderTop - radius + penWidth );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + 3, ( height() - borderTop - borderBottom ) / 2 + borderTop - radius + penWidth + 7 );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - 3, ( height() - borderTop - borderBottom ) / 2 + borderTop - radius + penWidth + 7 );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     ( height() - borderTop - borderBottom ) / 2 + borderTop - radius + penWidth );
  
  //arrow right
  painterPath->moveTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,                         ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + radius - penWidth,     ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + radius - penWidth - 7, ( height() - borderTop - borderBottom ) / 2 + borderTop + 3 );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + radius - penWidth - 7, ( height() - borderTop - borderBottom ) / 2 + borderTop - 3 );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + radius - penWidth,     ( height() - borderTop - borderBottom ) / 2 + borderTop );

  //arrow bottom
  painterPath->moveTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     ( height() - borderTop - borderBottom ) / 2 + borderTop );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     ( height() - borderTop - borderBottom ) / 2 + borderTop + radius - penWidth );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft + 3, ( height() - borderTop - borderBottom ) / 2 + borderTop + radius - penWidth - 7 );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft - 3, ( height() - borderTop - borderBottom ) / 2 + borderTop + radius - penWidth - 7 );
  painterPath->lineTo( ( width() - borderLeft - borderRight ) / 2 + borderLeft,     ( height() - borderTop - borderBottom ) / 2 + borderTop + radius - penWidth );
  
  painterPath->setFillRule( Qt::OddEvenFill );
  
  painter->drawPath( *painterPath );
  // End Pfeil zeichnen
  
  rect.setLeft( rect.left() - 2 );
  rect.setTop( rect.top() - 2 );
  rect.setWidth( rect.width() + 2 );
  rect.setHeight( rect.height() + 2 );
  setHandleMiddleForMask( rect );
}


void regionselection::setHandleMiddleForMask( QRect rec )
{
  HandleMiddleForMask = rec;
}


QRect regionselection::getHandleMiddleForMask()
{
  return HandleMiddleForMask;
}


void regionselection::printSize()
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
  
  QBrush brush( Qt::yellow, Qt::SolidPattern );
  painter->setBrush( brush );
  painter->setPen( QPen( Qt::black, penWidth ) );  

  painter->drawRoundedRect( rect, 7, 7 );

  painter->drawText( rect, Qt::AlignCenter, widthHeigtSize );
  
  rect.setLeft( rect.left() - 2 );
  rect.setTop( rect.top() - 2 );
  rect.setWidth( rect.width() + 2 );
  rect.setHeight( rect.height() + 2 );
  setPrintSizeRectForMask( rect );
}


void regionselection::setPrintSizeRectForMask( QRect rec )
{
  printSizeRectForMask = rec;
}


QRect regionselection::getPrintSizeRectForMask()
{
  return printSizeRectForMask;
}


void regionselection::paintEvent( QPaintEvent *event ) 
{
  (void)event;
  painter->begin( this );
  painter->setRenderHints( QPainter::Antialiasing, true );

  // Maskiert den Bereich für PrintSize und HandleMiddle
  if ( !isFrameLocked() )
  {
    printSize();
    HandleMiddle();
    clearMask();
    
    QRegion RegionWidget( 0, 0, width(), height() );
    
    QRegion RegionArea  ( borderLeft + frameWidth / 2 + radius,
                          borderTop + frameWidth / 2 + radius,
                          width() - ( borderLeft + frameWidth / 2 ) - ( borderRight + frameWidth / 2 + 2 * radius ),
                          height() - ( borderTop + frameWidth / 2 ) - ( borderBottom + frameWidth / 2 + 2 * radius) );

    // subtract the record Area
    #ifdef QT4
    QRegion RegionNew = RegionWidget.subtract( RegionArea );
    #endif
    #ifdef QT5
    QRegion RegionNew = RegionWidget.subtracted( RegionArea );
    #endif
    
    // Retrieves and merge display-area-size in record Area
    QRegion r1 = RegionNew.united( getPrintSizeRectForMask() );
    
    // HandleMiddle
    // Retrieves and merge HandleMiddle in record Area
    r1 = r1.united( getHandleMiddleForMask() );
    setMask( r1 );
  }
  else
  {
    // Widget
    clearMask();
    QRegion RegionWidget( 0, 0, width(), height() );
    
    // RecordArea
    QRegion RegionArea  ( borderLeft + frameWidth / 2,
                          borderTop + frameWidth / 2,
                          width() - ( borderLeft + frameWidth / 2 ) - ( borderRight + frameWidth / 2 ),
                          height() - ( borderTop + frameWidth / 2 ) - ( borderBottom + frameWidth / 2 ) );

    // subtract the record Area
    #ifdef QT4
    QRegion RegionNew = RegionWidget.subtract( RegionArea );
    #endif
    #ifdef QT5
    QRegion RegionNew = RegionWidget.subtracted( RegionArea );
    #endif
    setMask( RegionNew );
  }

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


void regionselection::lockFrame( bool status )
{
    frameLocked = status;
    qDebug() << "[vokoscreen] [Regional selection] Frame locked:" << status;
    qDebug();
    handlingFrameLock();
}


bool regionselection::isFrameLocked()
{
    return frameLocked;
}


void regionselection::handlingFrameLock()
{
  repaint();
  update();
}


/**
 * Return x from window
 */
int regionselection::getX()
{
  return this->x(); 
}


/**
 * Return y from window
 */
int regionselection::getY()
{
  return  this->y();
}


/**
 * Return x from record area
 */
int regionselection::getXRecordArea()
{
  return this->x() + borderLeft + frameWidth / 2; 
}


/**
 * Return y from record area
 */
int regionselection::getYRecordArea()
{
  return  this->y() + borderTop + frameWidth / 2;
}


/**
 * Return height from record area
 */
int regionselection::getHeight()
{
  return this->height() - borderTop - borderBottom - frameWidth; 
}


/**
 * Return width from record Area
 */
int regionselection::getWidth()
{
  return this->width() - borderLeft - borderRight - frameWidth; 
}


/*! MouseMove fuer das bewegen des Fensters und Ränder
\param event QMouseEvent Mouse Event
*/
void regionselection::mouseMoveEvent( QMouseEvent *event )
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
  if ( mouseGlobalY >= widgetY + widgetHeight + currentMouseLocalY - 200 )
    mouseGlobalY = widgetY + widgetHeight + currentMouseLocalY - 200;

  if ( mouseGlobalX >= widgetX + widgetWidth + currentMouseLocalX - 200 )
    mouseGlobalX = widgetX + widgetWidth + currentMouseLocalX - 200;
  
  // Maximale Größe begrenzen
//  if ( mouseGlobalY - currentMouseLocalY < 0 )
  if ( mouseGlobalY - currentMouseLocalY < radius * -1 - frameWidth/2 )
    mouseGlobalY = widgetY + currentMouseLocalY;
  
  //if ( mouseGlobalX - currentMouseLocalX < 0 )
  if ( mouseGlobalX - currentMouseLocalX < radius * -1 - frameWidth/2 )
    mouseGlobalX = widgetX + currentMouseLocalX;
  
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
  if ( mouseGlobalY >= widgetY + widgetHeight + currentMouseLocalY - 200 )
    mouseGlobalY = widgetY + widgetHeight + currentMouseLocalY - 200;
  
  // Maximale Größe begrenzen
//  if ( mouseGlobalY - currentMouseLocalY < 0 )
  if ( mouseGlobalY - currentMouseLocalY < radius * -1 - frameWidth/2 )
    mouseGlobalY = widgetY + currentMouseLocalY;
  
  // Neue Geometry des HauptWidget setzen
  this->setGeometry( widgetX,
		     mouseGlobalY - currentMouseLocalY,
		     widgetWidth,
		     widgetHeight + ( widgetY - mouseGlobalY + currentMouseLocalY ) );
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
  int widgetWidth = geometry().width();

  // Minimale Größe des Widget begrenzen
  if ( mouseGlobalX <= widgetX + 200 - currentMouseRightLocalX )
    mouseGlobalX = widgetX + 200 - currentMouseRightLocalX;
  
  if ( mouseGlobalY >= widgetY + widgetHeight + currentMouseLocalY - 200 )
    mouseGlobalY = widgetY + widgetHeight + currentMouseLocalY - 200;
  
  // Maximale Größe begrenzen
  //if ( mouseGlobalY - currentMouseLocalY < 0 )
  if ( mouseGlobalY - currentMouseLocalY < radius * -1 - frameWidth/2 )
    mouseGlobalY = widgetY + currentMouseLocalY;
  
  QDesktopWidget *desk = QApplication::desktop();
  // if ( mouseGlobalX + currentMouseRightLocalX > desk->width() )
  if ( mouseGlobalX + currentMouseRightLocalX > desk->width() - radius * -1 + frameWidth/2 )
    mouseGlobalX = widgetX + widgetWidth - currentMouseRightLocalX;
  
  // Neue Geometry des Fenster setzen
  this->setGeometry( widgetX,
		     mouseGlobalY - currentMouseLocalY,
		     currentWidgetWidth + ( mouseGlobalX - ( widgetX + currentMouseLocalX ) ),
     		     widgetHeight  + ( widgetY - mouseGlobalY + currentMouseLocalY ) );
}


void regionselection::moveRightMiddle( QMouseEvent *event )
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
  //if ( mouseGlobalX + currentMouseRightLocalX > desk->width() )
  if ( mouseGlobalX + currentMouseRightLocalX > desk->width() - radius * -1 + frameWidth/2)
    mouseGlobalX = widgetX + widgetWidth - currentMouseRightLocalX;
  
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
  int widgetWidth = geometry().width();
  int widgetHeight = geometry().height();

  // Minimale Größe des Widget begrenzen
  if ( mouseGlobalX <= widgetX + 200 - currentMouseRightLocalX )
    mouseGlobalX = widgetX + 200 - currentMouseRightLocalX;
  
  if ( mouseGlobalY <= widgetY + 200 - currentMouseRightLocalY )
    mouseGlobalY = widgetY + 200 - currentMouseRightLocalY;
  
  // Maximale größe begrenzen
  QDesktopWidget *desk = QApplication::desktop();
  //if ( mouseGlobalX + currentMouseRightLocalX > desk->width() )
  if ( mouseGlobalX + currentMouseRightLocalX > desk->width() - radius * -1 + frameWidth/2 )
    mouseGlobalX = widgetX + widgetWidth - currentMouseRightLocalX;
  
  //if ( mouseGlobalY + currentMouseRightLocalY > desk->height() )
  if ( mouseGlobalY + currentMouseRightLocalY > desk->height() - radius * -1 + frameWidth/2 )
    mouseGlobalY = widgetY + widgetHeight - currentMouseRightLocalY;
  
  this->setGeometry( widgetX,
		     widgetY,
		     currentWidgetWidth + ( mouseGlobalX - ( widgetX + currentMouseLocalX ) ),
		     currentWidgetHeight + ( mouseGlobalY - ( widgetY + currentMouseLocalY ) ) );
}


void regionselection::moveBottomMiddle( QMouseEvent *event )
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


void regionselection::moveBottomLeft( QMouseEvent *event )
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
  //if ( mouseGlobalX - currentMouseLocalX < 0 )  
  if ( mouseGlobalX - currentMouseLocalX < radius * -1 - frameWidth/2 )
    mouseGlobalX = widgetX + currentMouseLocalX;

  QDesktopWidget *desk = QApplication::desktop();
  //if ( mouseGlobalY + currentMouseRightLocalY > desk->height() )
  if ( mouseGlobalY + currentMouseRightLocalY > desk->height() - radius * -1 + frameWidth/2 )
    mouseGlobalY = widgetY + widgetHeight - currentMouseRightLocalY;
  
  this->setGeometry( mouseGlobalX - currentMouseLocalX,
		     widgetY,
     		     widgetWidth + ( widgetX - mouseGlobalX + currentMouseLocalX ),
		     currentWidgetHeight + ( mouseGlobalY - ( widgetY + currentMouseLocalY ) ) );
}


void regionselection::moveLeftMiddle( QMouseEvent *event )
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
  //if ( mouseGlobalX - currentMouseLocalX < 0 )  
  if ( mouseGlobalX - currentMouseLocalX < radius * -1 - frameWidth/2)
    mouseGlobalX = widgetX + currentMouseLocalX;
  
  // Neue Geometry des Dialogfenster setzen
  this->setGeometry( mouseGlobalX - currentMouseLocalX,
		     widgetY,
		     widgetWidth + ( widgetX - mouseGlobalX + currentMouseLocalX ),
		     widgetHeight );
}


void regionselection::moveMiddle( QMouseEvent *event )
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


void regionselection::mousePressEvent( QMouseEvent *event )
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


void regionselection::mouseReleaseEvent( QMouseEvent * event )
{
  (void)event;
  handlePressed = NoHandle;
  event->accept();  
}


void regionselection::mouseDoubleClickEvent( QMouseEvent * event )
{
  (void)event;
  QDesktopWidget *desk = QApplication::desktop();
  
  if ( handleUnderMouse == TopLeft )
  {
    setGeometry( 0 - radius - frameWidth / 2,
		 0 - radius - frameWidth / 2,
		 geometry().x() + geometry().width() + radius + frameWidth / 2,
		 geometry().y() + geometry().height() + radius + frameWidth / 2 );
  }
  
  if ( handleUnderMouse == TopMiddle )
  {
    setGeometry( geometry().x(),
		 0 - radius - frameWidth / 2,
		 geometry().width(),
		 geometry().y() + geometry().height() + radius + frameWidth / 2 );
  }

  if ( handleUnderMouse == TopRight )
  {
    setGeometry( geometry().x(),
		 0 - radius - frameWidth / 2,
		 desk->width() - geometry().x() + radius + frameWidth / 2,
		 geometry().y() + geometry().height() + radius + frameWidth / 2 );
  }
    
  if ( handleUnderMouse == RightMiddle )
  {
    setGeometry( geometry().x(),
                 geometry().y(),
		 desk->width() - geometry().x() + radius + frameWidth / 2,
		 geometry().height() );
  }
    
  if ( handleUnderMouse == BottomRight )
  {
    setGeometry( geometry().x(),
                 geometry().y(),
		 desk->width() - geometry().x() + radius + frameWidth / 2,
		 desk->height() - geometry().y() + radius + frameWidth / 2 );
  }
  
  if ( handleUnderMouse == BottomMiddle )
  {
    setGeometry( geometry().x(),
                 geometry().y(),
 		 geometry().width(),
		 desk->height() - geometry().y() + radius + frameWidth / 2 );
  }
  
  if ( handleUnderMouse == BottomLeft )
  {
    setGeometry( 0 - radius - frameWidth / 2,
                 geometry().y(),
		 geometry().x() + geometry().width() + radius + frameWidth / 2,
		 desk->height() - geometry().y() + radius + frameWidth / 2 );
  }

  if ( handleUnderMouse == LeftMiddle )
  {
    setGeometry( 0 - radius - frameWidth / 2,
                 geometry().y(),
		 geometry().x() + geometry().width() + radius + frameWidth / 2,
		 geometry().height() );
  }
  
  if ( handleUnderMouse == Middle )
  {
    setGeometry( 0 - radius - frameWidth / 2,
                 0 - radius - frameWidth / 2,
		 desk->width() + 2 * radius + frameWidth,
		 desk->height() + 2 * radius + frameWidth );
  }
}
