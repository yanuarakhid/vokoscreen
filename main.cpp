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

#include "screencast.h"
#include "vokoscreen_adaptor.h"

#include <QDebug>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QDBusConnection>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
 
    bool isRunning;

    if( QDBusConnection::sessionBus().registerService( "org.vokoscreen.screencast" ) )
    {
      isRunning = false;
    }
    else
    {
      isRunning = true; 
    }
    
    QStringList arguments = QApplication::instance()->arguments();
    for( int i = 1; i < arguments.count(); ++i )
    {
      if ( arguments[ 1 ] == "--help" )
      {
         qDebug() << "Usage: vokoscreen [OPTIONS]";
         qDebug( " " );
         qDebug() << "Options:";
         qDebug() << "  --help              show this help message";
         qDebug() << "  --startrecord       starts a recording";
         qDebug() << "  --stoprecord        stops record";
         qDebug( " " );
         return close( 0 );
      }
    }

    qDebug() << "[vokoscreen] For comanndline option take: vokoscreen --help";
    
    QTranslator * qtTranslator = new QTranslator();
    qtTranslator->load( "qt_" + QLocale::system().name(), QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
    app.installTranslator( qtTranslator );

    QTranslator translator;
    translator.load( "vokoscreen_" + QLocale::system().name(), ":/language" );
    app.installTranslator( &translator );
    

    // startrecord
    for( int i = 1; i < arguments.count(); ++i )
    {
      if ( (isRunning == false) and ( arguments[ 1 ] == "--startrecord" ) )
      {
        screencast *foo = new screencast();

        new VokoscreenInterfaceAdaptor(foo);
        QDBusConnection dbusConnection = QDBusConnection::sessionBus();
        dbusConnection.registerObject("/record", foo);
        dbusConnection.registerService("org.vokoscreen.screencast");
  
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        
        //dbus_iface.call("setAudioOff");
        dbus_iface.call("startrecord");
   
        foo->show();
        return app.exec();
      }
    }
    
    // stoprecord
    for( int i = 1; i < arguments.count(); ++i )
    {
      if ( (isRunning == true) and ( arguments[ 1 ] == "--stoprecord" ) )
      {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        dbus_iface.call("stoprecord");
        goto test;
      }
    }
    
    // if running and argument is --startrecord
    for( int i = 1; i < arguments.count(); ++i )
    {
      if ( (isRunning == true) and ( arguments[ 1 ] == "--startrecord" ) )
      {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        //dbus_iface.call("setAudioOff");
        dbus_iface.call("startrecord");
        goto test;
      }
    }
    
    // normal start
    if ( isRunning == false )
    {
      //foo->commandLineStart( commandLine_Start );
      screencast *foo = new screencast();

      // Folgende aufrufe registrieren die Slots
      new VokoscreenInterfaceAdaptor(foo);
      QDBusConnection dbusConnection = QDBusConnection::sessionBus();
      dbusConnection.registerObject("/record", foo);
      dbusConnection.registerService("org.vokoscreen.screencast");

      foo->show();
      return app.exec();
    }
    else
    {
      QMessageBox::StandardButton ret = QMessageBox::information( NULL,
                                                                  QObject::tr( "Info" ),
                                                                  QObject::tr( "vokoscreen can be started only once" ),
                                                                  QMessageBox::Close );
      (void)ret;
    }
    
    test:{}
}
