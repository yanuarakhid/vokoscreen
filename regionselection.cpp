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

using namespace std;

regionselection::regionselection( int x, int y, int width, int height, int framewidth )
{

  setGeometry( x, y, width, height );
  border = framewidth;
  setWindowFlags( Qt::FramelessWindowHint );
  setAttribute ( Qt::WA_AlwaysShowToolTips, true );
  
  QPalette Pal( this->palette() );
  QColor Col( Qt::red );
  Pal.setColor( QPalette::Background, Col );
  this->setPalette( Pal ); 
  
  //Top
  borderTop = new QLabel( this );
  borderTop->setGeometry( 10, 0, this->width()-20, border );
  borderTop->show();
  borderTop->setAutoFillBackground( true );
  borderTop->setCursor( Qt::SizeVerCursor );
  // hintergrundfarbe für borderTop
  QPalette labPal( borderTop->palette() );
  QColor bgCol( Qt::blue );
  labPal.setColor( QPalette::Background, bgCol );
  borderTop->setPalette( labPal );
  
  //Bottom
  borderBottom = new QLabel( this );
  borderBottom->setGeometry( 10, this->height() - border, this->width() - 20, border );
  borderBottom->show();
  borderBottom->setAutoFillBackground( true );
  borderBottom->setCursor( Qt::SizeVerCursor );
  // hintergrundfarbe für borderBottom
  QPalette borderBottomPal( borderBottom->palette() );
  QColor borderBottomBgCol( Qt::blue );
  borderBottomPal.setColor( QPalette::Background, borderBottomBgCol );
  borderBottom->setPalette( borderBottomPal);

  //Left
  borderLeft = new QLabel( this );
  borderLeft->setGeometry( 0, 10, border, this->height()-20 );
  borderLeft->show();
  borderLeft->setAutoFillBackground( true );
  borderLeft->setCursor( Qt::SizeHorCursor );
  // hintergrundfarbe für borderLedt
  QPalette borderLeftPal( borderLeft->palette() );
  QColor borderLeftBgCol( Qt::blue );
  borderLeftPal.setColor( QPalette::Background, borderLeftBgCol );
  borderLeft->setPalette( borderLeftPal);

  //Right
  borderRight = new QLabel( this );
  borderRight->setGeometry( this->width() - border, 10, border, this->height()-20 );
  borderRight->show();
  borderRight->setAutoFillBackground( true );
  borderRight->setCursor( Qt::SizeHorCursor );
  // hintergrundfarbe für borderLedt
  QPalette borderRightPal( borderRight->palette() );
  QColor borderRightBgCol( Qt::blue );
  borderRightPal.setColor( QPalette::Background, borderRightBgCol );
  borderRight->setPalette( borderRightPal);
  

  QRegion maskedRegion1( 0,
             0,
             this->width(),
             this->height(),
             QRegion::Rectangle );
    
  // Dieser Teil wird abgezogen
  QRegion maskedRegion( border,
            border,
            this->width() - 2 * border,
            this->height() - 2 *border,
            QRegion::Rectangle );
    
  this->setMask( maskedRegion1.subtract( maskedRegion ) );

    //Framelock
    lockFrame(false);
}


regionselection::~regionselection()
{
}

void regionselection::lockFrame(bool status)
{
    frameLocked = status;
    qDebug() << "[vokoscreen][Regional selection] Frame locked:" << status;
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


void regionselection::setAllBorder()
{
  int widgetWidth = geometry().width();
  int widgetHeight = geometry().height();

  borderTop->setGeometry( 10, 
			  0,
			  widgetWidth - 20,
			  border );
  
  borderBottom->setGeometry( 10, 
			     widgetHeight - border,
			     widgetWidth - 20,
			     border );
  
  borderLeft->setGeometry( 0, 
			   10,
			   border,
			   widgetHeight - 20 );
  
  borderRight->setGeometry( widgetWidth - border,
			    10,
			    border,
			    widgetHeight - 20 );
  
}


void regionselection::moveTop( QMouseEvent *event )
{
   // Globale Mauskoordinaten 
  int mouseY = event->globalY();
    
  // Alte Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetWidth = geometry().width();
  int widgetHeight = geometry().height();
  
  // Minimale Größe des Widget begrenzen
  if ( mouseY >= widgetY + widgetHeight - 100)
    mouseY = widgetY + widgetHeight - 100;
  
  // Neue Geometry des Dialogfenster setzen
  this->setGeometry( widgetX,
		     mouseY, 
		     widgetWidth,
		     widgetHeight + ( widgetY - mouseY ) );
  
  QRegion maskedRegion1( 0, 
			 0, 
			 widgetWidth,
			 widgetHeight + ( widgetY - mouseY ), 
			 QRegion::Rectangle );
  
  // Dieser Teil wird abgezogen
  QRegion maskedRegion( border,
			border,
			widgetWidth - 2 * border,
			widgetHeight + ( widgetY - mouseY ) - 2 * border,
			QRegion::Rectangle );
  
  this->setMask( maskedRegion1.subtract( maskedRegion ) );
  
  setAllBorder();
  
  event->accept();
}


void regionselection::moveBottom( QMouseEvent *event )
{
  // Globale Mauskoordinaten 
  int mouseY = event->globalY();
    
  // Alte Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetWidth = geometry().width();

  // Minimale Größe des Widget begrenzen
  if ( mouseY <= widgetY +  100)
    mouseY = widgetY + 100;
  
  
  // Neue Geometry des Dialogfenster setzen
  this->setGeometry( widgetX, 
		     widgetY, 
		     widgetWidth, 
		     mouseY - widgetY );
   
  QRegion maskedRegion1( 0, 
			 0, 
			 widgetWidth, 
			 mouseY - widgetY,
			 QRegion::Rectangle );
  
  // Dieser Teil wird abgezogen
  QRegion maskedRegion( border,
			border,
			widgetWidth - 2 * border,
			mouseY - widgetY - 2 * border,
			QRegion::Rectangle );
  
  this->setMask( maskedRegion1.subtract( maskedRegion ) );

  setAllBorder();
  
  event->accept();
}


void regionselection::moveLeft( QMouseEvent *event )
{  
  // Globale Mauskoordinaten 
  int mouseX = event->globalX();
    
  // Alte Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetWidth = geometry().width();
  int widgetHeight = geometry().height();

  // Minimale Größe des Widget begrenzen
  if ( mouseX >= widgetX + widgetWidth -  100 )
    mouseX = widgetX + widgetWidth -  100;
  
  // Neue Geometry des Dialogfenster setzen
  this->setGeometry( mouseX, 
		     widgetY, 
		     widgetWidth + widgetX - mouseX,
		     widgetHeight );

  QRegion maskedRegion1( 0, 
			 0, 
			 widgetWidth + widgetX - mouseX,
			 widgetHeight,
			 QRegion::Rectangle );
  
  // Dieser Teil wird abgezogen
  QRegion maskedRegion( border,
			border,
			widgetWidth + widgetX - mouseX - 2 * border,
			widgetHeight - 2 * border,
			QRegion::Rectangle );
  
  this->setMask( maskedRegion1.subtract( maskedRegion ) );

  setAllBorder();

  event->accept();
  
}


void regionselection::moveRight( QMouseEvent *event )
{
  // Globale Mauskoordinaten 
  int mouseX = event->globalX();
    
  // Alte Widget Koordinaten
  int widgetX = geometry().x();
  int widgetY = geometry().y();
  int widgetHeight = geometry().height();

  // Minimale Größe des Widget begrenzen
  if ( mouseX <= widgetX + 100 )
    mouseX = widgetX + 100;
  
  // Neue Geometry des Dialogfenster setzen
  this->setGeometry( widgetX, 
		     widgetY, 
		     mouseX - widgetX,
		     widgetHeight );
  
  QRegion maskedRegion1( 0, 
			 0, 
			 mouseX - widgetX,
			 widgetHeight,
			 QRegion::Rectangle );
  
  // Dieser Teil wird abgezogen
  QRegion maskedRegion( border,
			border,
			mouseX - widgetX - 2 * border,
			widgetHeight - 2 * border,
			QRegion::Rectangle );
  
  this->setMask( maskedRegion1.subtract( maskedRegion ) );
  
  setAllBorder();

  event->accept();
}


void regionselection::mouseMoveEvent( QMouseEvent *event )
{
    if(frameLocked){
        return;
    }

  if ( borderTop->underMouse() )
  {
    moveTop( event );
    return;
  }
  
  if ( borderBottom->underMouse() )
  {
    moveBottom( event );
    return;
  }

  if ( borderLeft->underMouse() )
  {
    moveLeft( event );
    return;
  }

  if ( borderRight->underMouse() )
  {
    moveRight( event );
    return;
  }
  
}

void regionselection::handlingFrameLock()
{
    if(frameLocked){
        borderLeft->setGeometry( 0, 0, border, this->height());
        borderRight->setGeometry( this->width()-border,0, border, this->height() );
        borderBottom->setGeometry(0, this->height() - border, this->width(), border );
        borderTop->setGeometry( 0, 0, this->width(), border );
    }
    else {
        setAllBorder();
    }
}
