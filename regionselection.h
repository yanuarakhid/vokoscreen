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
#include <QDialog>
#include <QLabel>
#include <QMouseEvent>

class regionselection : public QDialog
{ 
Q_OBJECT
public:
  regionselection( int x, int y, int width, int height, int border );
  virtual ~regionselection();
  
  
public slots:
  void moveTop( QMouseEvent *event );
  void moveBottom( QMouseEvent *event );
  void moveLeft( QMouseEvent *event );
  void moveRight( QMouseEvent *event );
  void setAllBorder();
  int getX();
  int getY();
  int getHeight();
  int getWidth();

  
protected:  
  void mouseMoveEvent( QMouseEvent *event );
  
  
private:
  int border;
  QLabel *borderTop;
  QLabel *borderBottom;
  QLabel *borderLeft;
  QLabel *borderRight;
  
};


#endif
