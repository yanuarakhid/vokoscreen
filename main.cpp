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

#include "screencast.h"
//#include <QSplashScreen>
#include <QtSingleApplication/qtsingleapplication.h>
#include <QDebug>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

int main(int argc, char** argv)
{
    QtSingleApplication app(argc, argv);

    if (app.isRunning())
    {
      qDebug() << "[vokoscreen] vokoscreen can only be started once, vokoscreen runs";
      return !app.sendMessage("aa");
    }
    
/**
 * qt_ muß um z.b mit "de" erweitert werden und nicht um "de_DE" dann sollte es funktionieren
 */    
/*    
    // Dies sollte für die fertigen qt-Dialoge sein, funktioniert aber nicht.
    QTranslator * qtTranslator = new QTranslator();
    qtTranslator->load( "qt_" + QLocale::system().name(), QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
    app.installTranslator( qtTranslator );
    qDebug() << QLibraryInfo::location( QLibraryInfo::TranslationsPath ) << QLocale::system().name();
*/    
    QTranslator translator;
    translator.load( "vokoscreen_" + QLocale::system().name(), ":/language" );
    app.installTranslator( &translator );
    qDebug();
    qDebug() << "[vokoscreen]" << "Locale:" << QLocale::system().name();
    
    QDesktopWidget *desk = QApplication::desktop();
    int vokoscreenWidth = 580;
    int vokoscreenHeight = 210 + 30 + 20 ;
    int x = ( desk->screenGeometry().width() / 2 ) - ( vokoscreenWidth / 2 );
    int y = ( desk->screenGeometry().height() / 2 ) -( vokoscreenHeight / 2 );
    
    screencast foo;
      foo.setGeometry( x, y, vokoscreenWidth, vokoscreenWidth );
      foo.setFixedSize( vokoscreenWidth, vokoscreenHeight );
      app.setActivationWindow( &foo );
    foo.show();
     
    return app.exec();
}
