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

void runningWithArguments( QStringList arguments, int &returnValue )
{
   QDBusConnection bus = QDBusConnection::sessionBus();
   QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                             "org.vokoscreen.screencast.vokoscreenInterface", bus);
   QDBusReply<int> reply = dbus_iface.call( arguments[1].replace(0,2,""));
   returnValue = reply.value();
}

void runningWithArguments_2( QStringList arguments, int arg1, int &returnValue )
{
   QDBusConnection bus = QDBusConnection::sessionBus();
   QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                             "org.vokoscreen.screencast.vokoscreenInterface", bus);
   QDBusReply<int> reply = dbus_iface.call( arguments[1].replace(0,2,""), arg1 );
   qDebug() << "+++++++++++++++++++++++++++" << arguments << arg1;
   returnValue = reply.value();
}

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
      int x = 0;
      QDBusReply<int> reply;
      do {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        reply = dbus_iface.call( "isVokoscreenLoaded" );
        QTest::qSleep( 50 );
        qDebug() << x++ << reply.value();
      } while( reply.value() > 0 );

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
         qDebug() << "  return value: 0=ok, 1=faild";
         qDebug( " " );
         qDebug() << "Options:";
         qDebug() << "  --help                Show this help message";
         qDebug() << "  --startrecord         Starts a recording";
         qDebug() << "                             If vokoscreen not running and starts";
         qDebug() << "                             with this option, audio will disable.";
         qDebug() << "  For the following options, vokoscreen must be running";
         qDebug() << "  --stoprecord          Stops record";
         qDebug() << "  --setFullScreen       Enable record for Fullscreen";
         qDebug() << "  --setWindow           Enable record for Window";
         qDebug() << "  --setArea             Enable record for Area";
         qDebug() << "  --setAreaReset        Reset size and position";
         qDebug() << "  --setCountDown 0-99   Set Value Countdown";
         qDebug() << "  --setAudioOn          Enable Audio";
         qDebug() << "  --setAudioOff         Disable Audio";
         qDebug() << "  --setWebcamOn         Enable Webcam";
         qDebug() << "  --setWebcamOff        Disable Webcam";
         qDebug() << "  --quit                Close vokoscreen";
         qDebug( " " );
         return 0;
      }
    }
/*
    // if running and argument is --isVokoscreenLoaded
    for( int i = 1; i < arguments.count(); ++i )
    {
      if ( (isRunning == true) and ( arguments[ 1 ] == "--isVokoscreenLoaded" ) )
      {
        qDebug() << "1111111111111111111111   --isVokoscreenLoaded";
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        QDBusReply<int> reply = dbus_iface.call( "isVokoscreenLoaded" );
        qDebug() << "2222222222222222222222" << reply.value();
        return reply.value();
      }
    }
*/
    
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

    int returnValue = 0;
    if ( ( isRunning == true ) and ( arguments.count() > 1 ) )
    {
      if( arguments[1] == "--startrecord") {
          runningWithArguments( arguments, returnValue );
          return returnValue;
      } else if( arguments[1] == "--stoprecord") {
          runningWithArguments( arguments, returnValue );
          return returnValue;
      }  else if( arguments[1] == "--setFullScreen") {
          runningWithArguments( arguments, returnValue );
          return returnValue;
      }  else if( arguments[1] == "--setWindow") {
          runningWithArguments( arguments, returnValue );
          return returnValue;
      }  else if( arguments[1] == "--setArea") {
          runningWithArguments( arguments, returnValue );
          return returnValue;
      }  else if( arguments[1] == "--setAreaReset") {
          runningWithArguments( arguments, returnValue );
          return returnValue;
      }  else if( arguments[1] == "--setAudioOn") {
          runningWithArguments( arguments, returnValue );
          return returnValue;
      }  else if( arguments[1] == "--setAudioOff") {
          runningWithArguments( arguments, returnValue );
          return returnValue;
      }  else if( arguments[1] == "--setWebcamOn") {
          runningWithArguments( arguments, returnValue );
          return returnValue;
      }  else if( arguments[1] == "--setWebcamOff") {
          runningWithArguments( arguments, returnValue );
          return returnValue;
      }  else  if( arguments[1] == "--setCountDown") {
          runningWithArguments_2( arguments, 3, returnValue );
          return returnValue;
      }  else if( arguments[1] == "--quit") {
          runningWithArguments( arguments, returnValue );
          return returnValue;
      }  else {
          qDebug() << "[vokoscreen]" << "option" << arguments[1] << "not available";
          return 0;
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

    qDebug() << "********** Diese Zeile sollte nie erscheinen **********";
}
