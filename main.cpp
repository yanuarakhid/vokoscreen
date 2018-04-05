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
//#include "vokoscreenmain_adaptor.h"

#include <QDebug>
#include <QTranslator>
#include <QDBusConnection>
#include <QLibraryInfo>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    bool isRunning = false;

    QTranslator * qtTranslator = new QTranslator();
    qtTranslator->load( "qt_" + QLocale::system().name(), QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
    app.installTranslator( qtTranslator );

    QTranslator translator;
    translator.load( "vokoscreen_" + QLocale::system().name(), ":/language" );
    app.installTranslator( &translator );

    if( QDBusConnection::sessionBus().registerService( "org.vokoscreen.screencast" ) )
    {
        isRunning = false;
    }
    else
    {
        isRunning = true;
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
}
