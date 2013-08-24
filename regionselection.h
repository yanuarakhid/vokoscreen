/* vokoscreen - A desktop recorder
 * Copyright (C) 2011-2012 Volker Kohaupt
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

#ifndef regionselection_H 
#define regionselection_H

#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QRegion>
#include <QCursor>
#include <QX11Info>
#include <QPainterPath>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>

#include <math.h>

class regionselection : public QWidget
{ 
Q_OBJECT
public:
  regionselection( int x, int y, int width, int height, int border );
  virtual ~regionselection();
  void lockFrame( bool status = true );
  bool isFrameLocked();

public slots:
  int getX();
  int getY();
  int getHeight();
  int getWidth();
  //void printSize();

private slots:
  void moveTopLeft    ( QMouseEvent *event );
  void moveTopMiddle  ( QMouseEvent *event );
  void moveTopRight   ( QMouseEvent *event );
  void moveRightMiddle( QMouseEvent *event );
  void moveBottomRight( QMouseEvent *event );
  void moveBottomMiddle( QMouseEvent *event);
  void moveBottomLeft( QMouseEvent *event );

  void HandleTopLeft();
  void HandleTopMiddle();
  void HandleTopRight();
  void HandleRightMiddle();
  void HandleBottomRight();
  void HandleBottomMiddle();
  void HandleBottomLeft();
  void HandleLeftMiddle();

protected:
  void mouseMoveEvent( QMouseEvent *event );
  void mousePressEvent( QMouseEvent *event );
  void mouseReleaseEvent ( QMouseEvent * event );
  
  void paintEvent( QPaintEvent *event );
  QPainter * painter;
  
  void resizeEvent( QResizeEvent * event ); //****************************************************
  
  
private:
  enum Handle { NoHandle, TopLeft, TopMiddle, TopRight, RightMiddle, BottomRight, BottomMiddle, BottomLeft, LeftMiddle, Middle };

  int frameWidth;
  int radius;
  int penWidth;
  int penHalf;

  int borderLeft;
  int borderTop;
  int borderRight;
  int borderBottom;
  
  bool frameLocked;
  void handlingFrameLock();
  
  //QLabel *labelSize;

  //! Fuer die Bewegung des Fensters
  QPoint m_dragPosition;
  
  int currentMouseLocalX;
  int currentMouseLocalY;
  
  int currentWidgetWidth;
  int currentWidgetHeight;
  int currentWidgetY;
  int currentbottomY;
  
  Handle handlePressed;
  Handle handleUnderMouse;
};
#endif
