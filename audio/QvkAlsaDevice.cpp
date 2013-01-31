#include "QvkAlsaDevice.h"

#include <QDebug>
#include <QDir>

#include <QFileInfo>

#include <stdio.h>
#include <string.h>
#include <alsa/asoundlib.h>



using namespace std;


QvkAlsaDevice::QvkAlsaDevice( QString value )
{
  setCard( value );
  setAlsaHw();
  setChannel();
  setAlsaName();
  setAlsaSample();
}


QvkAlsaDevice::QvkAlsaDevice()
{
  // Leerer Kontructor z.b für getAlsaVersion() abfragen
}


QvkAlsaDevice::~QvkAlsaDevice()
{
}


QString QvkAlsaDevice::getAlsaVersion()
{
  const char * version = snd_asoundlib_version();
  return version;
}


void QvkAlsaDevice::setAlsaSample()
{
  
  AlsaSample = "48000";
  QString cardNumber = getCard().remove( "card" );

  QFile file( "/proc/asound/card" + cardNumber + "/stream0" );
  if ( file.exists() )
  {
    file.open ( QIODevice::ReadOnly );

    QTextStream textStream( &file );
    QString line = textStream.readLine();
    
    QStringList stringlist;
    stringlist.append( line + "\n");
  
    while ( !line.isNull() )
    {
      line = textStream.readLine();
      stringlist.append( line + "\n" );
    }

    file.close();

    QStringList sampleList = stringlist.filter( "Rates:", Qt::CaseInsensitive );
    sampleList.replaceInStrings( "Rates:", "", Qt::CaseInsensitive );
    sampleList.replaceInStrings( " ", "", Qt::CaseInsensitive );
    sampleList.replaceInStrings( "\n", "", Qt::CaseInsensitive );

    QList<int> integerList;
    for ( int i = 0; i < sampleList.count(); i++ )
    {
      int value = sampleList[ i ].toInt();
      integerList.append( value );
    }
    
    qSort( integerList.begin(), integerList.end() );
    
    AlsaSample = QString::number( integerList[ sampleList.count() - 1 ] );
  }
}


QString QvkAlsaDevice::getAlsaSample()
{
  return AlsaSample;
}


bool QvkAlsaDevice::isbusy()
{
    QString cardNumber = getCard().remove( "card" );
  
    QFile file( "/proc/asound/card" + cardNumber + "/pcm0c/sub0/status" );
    
    file.open ( QIODevice::ReadOnly );

    QTextStream textStream( &file );
    QString line = textStream.readLine();
    
    QStringList stringlist;
    stringlist.append( line + "\n");
  
    while ( !line.isNull() )
    {
      line = textStream.readLine();
      stringlist.append( line + "\n" );
    }

    file.close();
  
    bool busy;
    QStringList closedList = stringlist.filter( "closed", Qt::CaseInsensitive );
    if ( closedList.count() == 1 )
      busy = false;
    else
      busy = true;
    
    return busy;
}


void QvkAlsaDevice::setAlsaName()
{
   register int  err;
   int           cardNum;
   QString alsaName;

   QString cardNumber = getCard().remove( "card" );
  
   // Start with first card
   cardNum = -1;
   for (;;)
   {
      snd_ctl_t *cardHandle;
      if ( ( err = snd_card_next( &cardNum ) ) < 0 )
      {
         printf( "Can't get the next card number: %s\n", snd_strerror( err ) );
         break;
      }
      
      if ( cardNum < 0 ) break;

      if ( cardNumber == QString::number( cardNum ) )
      {
        char   str[ 64 ];
        sprintf( str, "hw:%i", cardNum );
        if ( ( err = snd_ctl_open( &cardHandle, str, 0 ) ) < 0 )
        {
          printf( "Can't open card %i: %s\n", cardNum, snd_strerror( err ) );
          continue;
        }
      
        snd_ctl_card_info_t *cardInfo;
        snd_ctl_card_info_alloca( &cardInfo );
        if ( ( err = snd_ctl_card_info( cardHandle, cardInfo ) ) < 0 )
          printf( "Can't get info for card %i: %s\n", cardNum, snd_strerror( err ) );
        else
        {
          alsaName = snd_ctl_card_info_get_name( cardInfo );
        }
     }
   }
   
   AlsaName = "[" + getAlsaHw() + "] " + alsaName;
      
}  

/*
void QvkAlsaDevice::setAlsaName()
{
  QString cardNumber = getCard().remove( "card" );
  
  
  // Begin Geräte Name auslesen
  
  QFile file( "/proc/asound/card" + cardNumber + "/pcm0c/info" );

  if ( file.exists() )
  {
    file.open ( QIODevice::ReadOnly );

    QTextStream textStream( &file );
    QString line = textStream.readLine();
    
    QStringList stringlist;
    stringlist.append( line + "\n");
  
    while ( !line.isNull() )
    {
      line = textStream.readLine();
      stringlist.append( line + "\n" );
    }

    file.close();
  
    QRegExp rx("^name:");
    QStringList alsaNameList = stringlist.filter( rx ).replaceInStrings( "\n", "");
    QString alsaName = alsaNameList[ 0 ].remove( "name:" );
    
    // End Geräte Name auslesen
    

    
    
    // Begin usbid auslesen  
    
    QString usbid = "";
    QString lineUsbid;
    QString usbFile = "/proc/asound/card" + cardNumber + "/usbid";
    QFile fileUsbid( usbFile );
    if ( fileUsbid.exists() )
    {
      fileUsbid.open ( QIODevice::ReadOnly );
      QTextStream textStreamUsbid( &fileUsbid );
      lineUsbid = textStreamUsbid.readLine();
    }
  
    fileUsbid.close();
    
    // End usbid auslesen  
    

    AlsaName = "[" + getAlsaHw() + "]" + alsaName + " " + lineUsbid;
  }
  else
    AlsaName = "unknown";
}
*/


QString QvkAlsaDevice::getAlsaName()
{
   return AlsaName;
}


void QvkAlsaDevice::setChannel()
{
  QString cardNumber = getCard().remove( "card" );
  
  QDir dir( "/proc/asound/card" + cardNumber + "/pcm0c/" );
  
  QStringList filters;
  filters << "sub*";
  QStringList cannelList = dir.entryList( filters, QDir::Dirs, QDir::Time );
  
  AlsaCannel = QString::number( cannelList.count() );
}


QString QvkAlsaDevice::getChannel()
{
  return AlsaCannel;
}


void QvkAlsaDevice::setAlsaHw()
{
   QString cardNumber = getCard().remove( "card" );
  
   register int  err;
   int           cardNum;
   cardNum = -1;
   for (;;)
   {
      snd_ctl_t *cardHandle;
      if ( ( err = snd_card_next( &cardNum ) ) < 0 )
      {
         printf( "Can't get the next card number: %s\n", snd_strerror( err ) );
         break;
      }
      
      if ( cardNum < 0 ) break;
    
    if ( cardNumber == QString::number( cardNum ) )
    {      
      char   str[ 64 ];
      sprintf( str, "hw:%i", cardNum );
      if ( ( err = snd_ctl_open( &cardHandle, str, 0 ) ) < 0 )
      {
        printf( "Can't open card %i: %s\n", cardNum, snd_strerror( err ) );
        continue;
      }
      
      snd_ctl_card_info_t *cardInfo;
      snd_ctl_card_info_alloca( &cardInfo );
      
      if ( ( err = snd_ctl_card_info( cardHandle, cardInfo ) ) < 0 )
         printf( "Can't get info for card %i: %s\n", cardNum, snd_strerror( err ) );
   
      int devNum;
      devNum = -1;
      if ( ( err = snd_ctl_pcm_next_device( cardHandle, &devNum ) ) < 0 )
      {
         printf( "Can't get next wave device number: %s\n", snd_strerror( err ) );
         break;
      }

      if ( devNum < 0 ) break;
 
      AlsaHw = "hw:" + QString::number( cardNum ) + "," + QString::number( devNum );
    }
   }
   snd_config_update_free_global();
}


/*
void QvkAlsaDevice::setAlsaHw()
{
  QString cardNumber = getCard().remove( "card" );

  QFile file( "/proc/asound/card" + cardNumber + "/pcm0c/info" );***************************** es kann vokommen das dieser Ordner nicht befüllt wird.
  if ( file.exists() )
  {
    file.open ( QIODevice::ReadOnly );
    QTextStream textStream( &file );

    QString line = textStream.readLine();
    QStringList stringlist;
    
    stringlist.append( line + "\n");
    
    while ( !line.isNull() )
    {
      line = textStream.readLine();
      stringlist.append( line + "\n" );
    }

    file.close();

    QRegExp rx("^device:");
    QStringList captureList = stringlist.filter( rx ).replaceInStrings( "\n", "").replaceInStrings( " ", "");
    QString deviceNumber = captureList[ 0 ].remove( "device:" );
    AlsaHw = "hw:" + cardNumber + "," + deviceNumber;
  }
  else
    AlsaHw = "unknown";
}
*/

QString QvkAlsaDevice::getAlsaHw()
{
  return AlsaHw;
}


void QvkAlsaDevice::setCard( QString value )
{
  cardx = value;
}


QString QvkAlsaDevice::getCard()
{
  return cardx;  
}
