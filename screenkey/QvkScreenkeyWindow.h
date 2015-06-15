/* vokoscreen - A desktop recorder
 * Copyright (C) 2011-2015 Volker Kohaupt
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

#ifndef QvkScreenkeyWindow_H 
#define QvkScreenkeyWindow_H

#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QLabel>

class QvkScreenkeyWindow : public QWidget
{ 
  Q_OBJECT
public:
  QvkScreenkeyWindow();
  virtual ~QvkScreenkeyWindow();
  QLabel *keyLabel;
  
  
public slots:

  
private slots:

  
protected:
  
  
private:
  
};
#endif
