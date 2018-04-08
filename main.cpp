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
#include "QvkDbus.h"

#include <QDebug>
#include <QTranslator>
#include <QDBusConnection>
#include <QLibraryInfo>
#include <QDBusInterface>
#include <QDBusReply>
#include <iostream>

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
             ( arguments[ 1 ] == "-help"  ) or
             ( arguments[ 1 ] == "-h"     ) or
             ( arguments[ 1 ] == "-?"     ) )
        {
            qDebug( " " );
            qDebug() << "Usage: vokoscreen [method] [arg]";
            qDebug( " " );
            qDebug() << "Hint:";
            qDebug() << "  vokoscreen must running, bevor call method.";
            qDebug( " " );
            qDebug( "Methods:" );
            QvkDbus *vkDbus = new QvkDbus();
            qDebug().noquote() << "  " << vkDbus->showAllMethods();
            qDebug( " " );
            return close( 0 );
        }
    }

    if ( ( isRunning == false ) and
         ( QApplication::instance()->arguments().count() > 1 ) )
    {
        qDebug() << "Please start vokoscreen first, without a method.";
        return 0;
    }


    // DBus option arguments
    QDBusReply<QString> reply;
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface dbus_iface("org.vokoscreen.screencast", "/gui",
                              "org.vokoscreen.gui", bus);
    if ( ( isRunning == true ) and ( QApplication::instance()->arguments().count() > 1 ) )
    {
        if ( QApplication::instance()->arguments().count() == 2 )
        {
            reply = dbus_iface.call( QApplication::instance()->arguments().at( 1 ) );
        }

        if ( QApplication::instance()->arguments().count() == 3 )
        {
            reply = dbus_iface.call( QApplication::instance()->arguments().at( 1 ),
                                     QApplication::instance()->arguments().at( 2 ));
        }

        // vokoscreens Ausgabe wird mit qDebug auf stderr ausgegeben
        // std::cout gibt die Ausgabe auf stdout aus, und somit kann in einem script die Ausgabe
        // in eine Variable geleitet werden.
        std::cout << reply.value().toStdString() << std::endl;

        goto stop;
    }


    {
        QTranslator * qtTranslator = new QTranslator();
        qtTranslator->load( "qt_" + QLocale::system().name(), QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
        app.installTranslator( qtTranslator );

        QTranslator translator;
        translator.load( "vokoscreen_" + QLocale::system().name(), ":/language" );
        app.installTranslator( &translator );
    }


    if( isRunning == false )
    {
        screencast *foo = new screencast();
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


    stop:
    {
        return 0;
    }
}
