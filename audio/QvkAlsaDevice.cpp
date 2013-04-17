#include "QvkAlsaDevice.h"

#include "alsa_device.h" 

using namespace std;

QvkAlsaDevice::QvkAlsaDevice( QString value )
{
  setCard( value );
  setAlsaHw();
  //setChannel();
  setAlsaName();
  setAlsaSample();
  qDebug() << "[vokoscreen] Find CaptureCard:" << getAlsaName();//<< "with channel:" << getChannel() << "and" << getAlsaSample() << "Hz";
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

// Wird nicht Aufgerufen 
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
  std::string stdString( getAlsaHw().toStdString() );
  const char *device_name = stdString.c_str();
  
  bool rc;
  alsa_device_busy( device_name );
  if ( rcBusy == 0 )
    rc = false;
  else
    rc = true;
    
  return rc;
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



QString QvkAlsaDevice::getAlsaName()
{
   return AlsaName;
}


void QvkAlsaDevice::setChannel()
{
  // Beim Start von vokoscreen könnte ein Gerät belegt sein
  // Die Kanäle werden kurz vor der Aufnahme ermittelt.
  if ( isbusy() == false )  
  {  
    std::string stdString( getAlsaHw().toStdString() );
    const char *device_name = stdString.c_str();
    int i;
    for ( i = 1; i <= 255; i++ )
    {
      alsa_device_open( device_name, i );
      if ( rc == 1 )
      {
        AlsaCannel = QString::number( i );
        break;
      }
    }  
  }
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

      if ( devNum < 0 )
	break;
 
      AlsaHw = "hw:" + QString::number( cardNum ) + "," + QString::number( devNum );
    }
   }
   snd_config_update_free_global();
}


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
