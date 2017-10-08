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
#include <QDBusConnection>
#include <QCommandLineOption>

void runningWithArguments( QString argument, int &returnValue )
{
   QDBusConnection bus = QDBusConnection::sessionBus();
   QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                             "org.vokoscreen.screencast.vokoscreenInterface", bus);
   QDBusReply<int> reply = dbus_iface.call( argument );
   returnValue = reply.value();
}

void runningWithArguments_2( QString argument, int arg1, int &returnValue )
{
   QDBusConnection bus = QDBusConnection::sessionBus();
   QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                             "org.vokoscreen.screencast.vokoscreenInterface", bus);
   QDBusReply<int> reply = dbus_iface.call( argument, arg1 );
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
      QDBusReply<int> reply;
      do {
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);
        reply = dbus_iface.call( "isVokoscreenLoaded" );
        int waitTime = 10;
        QTest::qSleep( waitTime );
        if ( reply == 1 )
          qDebug() << "[vokoscreen] return value is" << reply.value() << "wait" << waitTime << "ms";
        else
          qDebug() << "[vokoscreen] return value is" << reply.value() << "vokoscreen completely loaded";
      } while( reply.value() > 0 );
    }

    QvkSettings vkSettings;
    vkSettings.readAll();
    QCoreApplication::setApplicationVersion( vkSettings.getVersion() );
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription( "Only one option is accepted per call. Return value: 0=ok, 1=faild" );

    QCommandLineOption startrecord( "startrecord", "Starts a recording. If vokoscreen not running and starts with this option, audio will disable." );
    parser.addOption( startrecord );

    QCommandLineOption stoprecord( "stoprecord", "Stop the record" );
    parser.addOption( stoprecord );

    QCommandLineOption setFullScreen( "setFullScreen", "Enable record for fullscreen" );
    parser.addOption( setFullScreen );

    QCommandLineOption setWindow( "setWindow", "Enable record for window" );
    parser.addOption( setWindow );

    QCommandLineOption setArea( "setArea", "Enable record for area" );
    parser.addOption( setArea );

    QCommandLineOption setAreaReset( "setAreaReset", "Reset size and position" );
    parser.addOption( setAreaReset );

    QCommandLineOption setCountDown( "setCountDown", "Sets value for countdown", "0-99", "0" );
    parser.addOption( setCountDown );

    QCommandLineOption setAudioOn( "setAudioOn", "Enable audio" );
    parser.addOption( setAudioOn );

    QCommandLineOption setAudioOff( "setAudioOff", "Disable audio" );
    parser.addOption( setAudioOff );

    QCommandLineOption setWebcamOn( "setWebcamOn", "Enable webcam" );
    parser.addOption( setWebcamOn );

    QCommandLineOption setWebcamOff( "setWebcamOff", "Disable webcam" );
    parser.addOption( setWebcamOff );

    QCommandLineOption quit( "quit", "Close vokoscreen" );
    parser.addOption( quit );

    parser.process(app);
    
    QTranslator * qtTranslator = new QTranslator();
    qtTranslator->load( "qt_" + QLocale::system().name(), QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
    app.installTranslator( qtTranslator );

    QTranslator translator;
    translator.load( "vokoscreen_" + QLocale::system().name(), ":/language" );
    app.installTranslator( &translator );

    QStringList arguments = QApplication::instance()->arguments();

    // if not running and argument is --startrecord
    if ( ( isRunning == false ) and ( arguments.count() > 1 ) )
    {
      if ( (isRunning == false) and ( arguments[ 1 ] == "--startrecord" ) )
      {
        screencast *foo = new screencast();
        foo->show();

        new VokoscreenInterfaceAdaptor(foo);
        QDBusConnection dbusConnection = QDBusConnection::sessionBus();
        dbusConnection.registerObject("/record", foo);
        dbusConnection.registerService("org.vokoscreen.screencast");
  
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface dbus_iface("org.vokoscreen.screencast", "/record",
                                  "org.vokoscreen.screencast.vokoscreenInterface", bus);

        // Wait until the audio devices are loaded
        QDBusReply<int> reply;
        do {
          reply = dbus_iface.call( "isVokoscreenLoaded" );
          int waitTime = 10;
          QTest::qSleep( waitTime );
          QCoreApplication::processEvents( QEventLoop::AllEvents );
          if ( reply == 1 )
            qDebug() << "[vokoscreen] return value is" << reply.value() << "wait" << waitTime << "ms";
          else
            qDebug() << "[vokoscreen] return value is" << reply.value() << "vokoscreen completely loaded";
        } while( reply.value() > 0 );
        qDebug( " " );

        dbus_iface.call("startrecord");
        return app.exec();
      }
    }
     
    if ( ( isRunning == true ) and ( arguments.count() > 1 ) )
    {
      int returnValue = 0;
      if ( parser.isSet( startrecord ) ) {
          runningWithArguments( startrecord.names().at(0), returnValue );
          return returnValue; }
      if ( parser.isSet( stoprecord ) ) {
          runningWithArguments( stoprecord.names().at(0), returnValue );
          return returnValue; }
      if ( parser.isSet( setFullScreen ) ) {
          runningWithArguments( setFullScreen.names().at(0), returnValue );
          return returnValue; }
      if ( parser.isSet( setWindow ) ) {
          runningWithArguments( setWindow.names().at(0), returnValue );
          return returnValue; }
      if ( parser.isSet( setArea ) ) {
          runningWithArguments( setArea.names().at(0), returnValue );
          return returnValue; }
      if ( parser.isSet( setAreaReset ) ) {
          runningWithArguments( setAreaReset.names().at(0), returnValue );
          return returnValue; }
      if ( parser.isSet( setAudioOn ) ) {
          runningWithArguments( setAudioOn.names().at(0), returnValue );
          return returnValue; }
      if ( parser.isSet( setAudioOff ) ) {
          runningWithArguments( setAudioOff.names().at(0), returnValue );
          return returnValue; }
      if ( parser.isSet( setWebcamOn ) ) {
          runningWithArguments( setWebcamOn.names().at(0), returnValue );
          return returnValue; }
      if ( parser.isSet( setWebcamOff ) ) {
          runningWithArguments( setWebcamOff.names().at(0), returnValue );
          return returnValue; }
      if ( parser.isSet( setCountDown ) ) {
          runningWithArguments_2( setCountDown.names().at(0), parser.value( setCountDown ).toInt(), returnValue );
          return returnValue; }
      if ( parser.isSet( quit ) ) {
          runningWithArguments( quit.names().at(0), returnValue );
          return returnValue; }
    }

    // normal start without argument
    if ( isRunning == false )
    {
      screencast *foo = new screencast();

      // The following calls register the slots
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

    qDebug() << "********** This line should appear only after the message box **********";
}
