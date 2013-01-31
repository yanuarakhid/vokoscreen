
#include "QvkAlsaWatcher.h"

#include <QTimer>
#include <QDir>
#include <QDebug>

#include <stdio.h>
#include <string.h>
#include <alsa/asoundlib.h>

using namespace std;


QvkAlsaWatcher::QvkAlsaWatcher()
{
    AlsaCardCounter = -1;
    QTimer *timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( AlsaWatcherTimer() ) );
    timer->start( 500);
}


QvkAlsaWatcher::~QvkAlsaWatcher()
{
}


void QvkAlsaWatcher::AlsaWatcherTimer()
{
   register int  err;
   int           cardNum;
   cardNum = -1;
   QStringList deviceList;
   for (;;)
   {
      if ( ( err = snd_card_next( &cardNum ) ) < 0 )
      {
         printf( "Can't get the next card number: %s\n", snd_strerror( err ) );
         break;
      }
      else
      {
	if ( cardNum > -1 ) 
	  deviceList.append( "card" + QString::number( cardNum ) );
      }
      
      if ( cardNum < 0 ) break;
   }
   
   if ( AlsaCardCounter != deviceList.count() )
   {
      AlsaCardCounter = deviceList.count() ;
      emit changed( deviceList );
   }
}

/*
void QvkAlsaWatcher::AlsaWatcherTimer()
{
    QDir dir( "/proc/asound/" );
    QStringList filters;
    filters << "card*";

    QStringList deviceList = dir.entryList( filters, QDir::Dirs );
    
    if ( AlsaCardCounter != deviceList.count() )
    {
        AlsaCardCounter = deviceList.count() ;
	emit changed( deviceList );
    }
}
*/
