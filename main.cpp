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
 
    bool isRunning = false;

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
      if ( ( arguments[ 1 ] == "--help" ) or
           ( arguments[ 1 ] == "-h"     ) or
           ( arguments[ 1 ] == "-?"     ) )
      {
         qDebug( " " );
         qDebug() << "Usage: vokoscreen [OPTION]";
         qDebug( " " );
         qDebug() << "Hint:";
         qDebug() << "  Only one option is accepted per call.";
         qDebug() << "  The options delivered to time no useful return-values.";
         qDebug( " " );
         qDebug() << "Options:";
         qDebug() << "  --help              Show this help message";
         qDebug() << "  --startrecord       Starts a recording";
         qDebug() << "                             If vokoscreen not running and starts";
         qDebug() << "                             with this option, audio will disable.";
         qDebug() << "  --stoprecord        Stops record";
         qDebug() << "  --setFullScreen     Enable record for Fullscreen";
         qDebug() << "  --setWindow         Enable record for Window";
         qDebug() << "  --setArea           Enable record for Area";
         qDebug() << "  --setAreaReset      Reset size and position";
         qDebug() << "  --setAudioOn        Enable Audio";
         qDebug() << "  --setAudioOff       Disable Audio";
         qDebug() << "  --setWebcamOn       Enable Webcam";
         qDebug() << "  --setWebcamOff      Disable Webcam";
         qDebug() << "  --quit              Close vokoscreen";
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
    
    // if not running and argument is --startrecord
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
        
        dbus_iface.call("setAudioOff");
        dbus_iface.call("startrecord");
   
        foo->show();
        return app.exec();
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
        QDBusReply<int> reply = dbus_iface.call("startrecord");
        return reply.value();
      }
    }

    // if running and argument is --stoprecord
    for( int i = 1; i < arguments.count(); ++i )
    {
      if ( (isRunning == true) and ( arguments[ 1 ] == "--stoprecord" ) )
      {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        QDBusReply<int> reply = dbus_iface.call( "stoprecord" );
        return reply.value();
      }
    }

    // if running and argument is --setFullScreen
    for( int i = 1; i < arguments.count(); ++i )
    {
      if ( (isRunning == true) and ( arguments[ 1 ] == "--setFullScreen" ) )
      {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        dbus_iface.call("setFullScreen");
        goto test;
      }
    }
    
    // if running and argument is --setWindow
    for( int i = 1; i < arguments.count(); ++i )
    {
      if ( (isRunning == true) and ( arguments[ 1 ] == "--setWindow" ) )
      {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        dbus_iface.call("setWindow");
        goto test;
      }
    }

    // if running and argument is --setArea
    for( int i = 1; i < arguments.count(); ++i )
    {
      if ( (isRunning == true) and ( arguments[ 1 ] == "--setArea" ) )
      {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        dbus_iface.call("setArea");
        goto test;
      }
    }

    // if running and argument is --setAreaReset
    for( int i = 1; i < arguments.count(); ++i )
    {
      if ( (isRunning == true) and ( arguments[ 1 ] == "--setAreaReset" ) )
      {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        dbus_iface.call("setAreaReset");
        goto test;
      }
    }

    // if running and argument is --setAudioOff
    for( int i = 1; i < arguments.count(); ++i )
    {
      if ( (isRunning == true) and ( arguments[ 1 ] == "--setAudioOff" ) )
      {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        dbus_iface.call("setAudioOff");
        goto test;
      }
    }
    
    // if running and argument is --setAudioOn
    for( int i = 1; i < arguments.count(); ++i )
    {
      if ( (isRunning == true) and ( arguments[ 1 ] == "--setAudioOn" ) )
      {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        dbus_iface.call("setAudioOn");
        goto test;
      }
    }

    // if running and argument is --setWebcamOn
    for( int i = 1; i < arguments.count(); ++i )
    {
      if ( (isRunning == true) and ( arguments[ 1 ] == "--setWebcamOn" ) )
      {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        dbus_iface.call("setWebcamOn");
        goto test;
      }
    }
    
    // if running and argument is --setWebcamOff
    for( int i = 1; i < arguments.count(); ++i )
    {
      if ( (isRunning == true) and ( arguments[ 1 ] == "--setWebcamOff" ) )
      {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        dbus_iface.call("setWebcamOff");
        goto test;
      }
    }
    
    // if running and argument is --quit
    for( int i = 1; i < arguments.count(); ++i )
    {
      if ( (isRunning == true) and ( arguments[ 1 ] == "--quit" ) )
      {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        dbus_iface.call("quit");
        goto test;
      }
    }
    
    // normal start without argument
    if ( isRunning == false )
    {
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
