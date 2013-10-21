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
#include <QDebug>
#include <QRegion>
#include <QCursor>
#include <QX11Info>
#include <QPainterPath>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>

#include <QDialog>

class regionselection : public QDialog
{ 
Q_OBJECT
public:
  regionselection();
  regionselection( QWidget *parent );

  virtual ~regionselection();
  void lockFrame( bool status = true );
  bool isFrameLocked();

  int borderLeft;
  int borderTop;
  int borderRight;
  int borderBottom;
  int frameWidth;
  
public slots:
  int getX();
  int getY();
  int getHeight();
  int getWidth();
  void cleanRecordArea( bool state );
  int getXRecordArea();
  int getYRecordArea();
  void saveSettings();

private slots:
  void moveTopLeft     ( QMouseEvent *event );
  void moveTopMiddle   ( QMouseEvent *event );
  void moveTopRight    ( QMouseEvent *event );
  void moveRightMiddle ( QMouseEvent *event );
  void moveBottomRight ( QMouseEvent *event );
  void moveBottomMiddle( QMouseEvent *event );
  void moveBottomLeft  ( QMouseEvent *event );
  void moveLeftMiddle  ( QMouseEvent *event );
  void moveMiddle      ( QMouseEvent *event );

  void HandleTopLeft();
  void HandleTopMiddle();
  void HandleTopRight();
  void HandleRightMiddle();
  void HandleBottomRight();
  void HandleBottomMiddle();
  void HandleBottomLeft();
  void HandleLeftMiddle();
  void HandleMiddle();

  void printSize();
  void setPrintSizeRectForMask( QRect rec );
  QRect getPrintSizeRectForMask();
  void setHandleMiddleForMask( QRect rec );
  QRect getHandleMiddleForMask();

protected:
  void mouseMoveEvent( QMouseEvent *event );
  void mousePressEvent( QMouseEvent *event );
  void mouseReleaseEvent ( QMouseEvent * event );
  
  void paintEvent( QPaintEvent *event );
  QPainter * painter;
  
private:
  enum Handle { NoHandle, TopLeft, TopMiddle, TopRight, RightMiddle, BottomRight, BottomMiddle, BottomLeft, LeftMiddle, Middle };

  int radius;
  int penWidth;
  int penHalf;
  bool frameLocked;
  void handlingFrameLock();
  
  int currentMouseLocalX;
  int currentMouseLocalY;
  int currentMouseRightLocalX;
  int currentMouseRightLocalY;
  
  int currentWidgetWidth;
  int currentWidgetHeight;
  int currentWidgetY;
  int currentbottomY;
  
  Handle handlePressed;
  Handle handleUnderMouse;
  
  QRect printSizeRectForMask;
  QRect HandleMiddleForMask;
  
  int Setting_Area_NoShowInTaskBar;
};
#endif
