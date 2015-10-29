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

#ifndef QvkShowkeyWindow_H 
#define QvkShowkeyWindow_H

#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QLabel>

class QvkShowkeyWindow : public QWidget
{ 
  Q_OBJECT
public:
  QvkShowkeyWindow();
  virtual ~QvkShowkeyWindow();
  QLabel *keyLabel;
  
  
public slots:

  
private slots:

  
protected:
  
  
private:
  
};
#endif
