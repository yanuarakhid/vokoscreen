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

#ifndef QvkShowkeyController_H 
#define QvkShowkeyController_H

#include "QvkShowkeyWindow.h"
#include "QvkShowkeyGetkey.h"

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QCheckBox>

class QvkShowkeyController : public QObject
{ 
  Q_OBJECT
public:
  QvkShowkeyController();
  QvkShowkeyController( QCheckBox *checkBox );
  virtual ~QvkShowkeyController();
  QThread *pThread;
  QvkShowkeyGetkey *xev;
  
  
public slots:

  
private slots:
  void showkeyReadKey( int value );
  void hideScreenkeyWindow();
  void showScreenkeyWindow( QString value);
  void finished();
  
protected:
  
  
private:
  QTimer *screenkeyTimer;
  QvkShowkeyWindow *showkeyWindow;
  QString WinID;
  QCheckBox *checkBox;
  
};
#endif
