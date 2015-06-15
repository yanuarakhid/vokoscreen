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

#include "QvkScreenkeyWindow.h" 

QvkScreenkeyWindow::QvkScreenkeyWindow()
{
  int keyWindowHeight = 120;
  
  QDesktopWidget *desk = QApplication::desktop();
  setGeometry( 0, desk->height() / 10 * 7, desk->width(), keyWindowHeight );
  setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip );
  
  QPalette Pal(palette());
  Pal.setColor( QPalette::Background, Qt::lightGray );
  Pal.setColor( QPalette::Foreground, Qt::black );
  setAutoFillBackground( true );
  setPalette( Pal );  
  
  QFont qfont;;
  qfont.setPixelSize( keyWindowHeight - 20 );
  keyLabel = new QLabel( this );
  keyLabel->setGeometry( 0, 0, desk->width(), keyWindowHeight );
  keyLabel->setAlignment( Qt::AlignCenter );    
  keyLabel->setFont( qfont );

  setWindowOpacity( 0.7 );
}


QvkScreenkeyWindow::~QvkScreenkeyWindow()
{
}

