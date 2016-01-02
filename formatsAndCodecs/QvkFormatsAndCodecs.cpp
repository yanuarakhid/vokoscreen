#include <QProcess>
#include <QDebug>

#include "QvkFormatsAndCodecs.h"

/**
 * value: is ffmpeg
 */
QvkFormatsAndCodecs::QvkFormatsAndCodecs( QString value )
{
  recordApplikation = value;
  
  QProcess SystemCall;
    SystemCall.start( recordApplikation + " " + "-encoders" );
    SystemCall.waitForFinished();
    QString codecs = SystemCall.readAllStandardOutput();
  SystemCall.close();
  
  ListCodecs = codecs.split( "\n" );
  
  // delete Header inclusive " ------"
  int index = ListCodecs.indexOf( " ------" );
  for ( int i = 0; i <= index; i++ )
    ListCodecs.removeFirst();

  // remove spaces and gives then the first and the second columns back
  for ( int i = 0; i < ListCodecs.count(); i++ )
  {
    ListCodecs[ i ] = ListCodecs[ i ].simplified();
    ListCodecs[ i ] = ListCodecs[ i ].section( " ", 0, 1 );
  }
}


QvkFormatsAndCodecs::~QvkFormatsAndCodecs()
{
}


/**
 * typeOfCodec: is string "Audio" or "Video"
 * 
 * nameOfcodec: is videocodec libx264, libx265, mpeg4 etc.
 * or Audiocodec libmp3lame, libvorbis etc.
 * see output from "ffmpeg -encoders" second column
 */
bool QvkFormatsAndCodecs::getCodec( QString typeOfCodec, QString nameOfCodec, bool *experimental )
{
  // Give all Audio or Vidocodec
  QStringList TypeListCodec = ListCodecs.filter( QRegExp( "^" + typeOfCodec.left( 1 ) ) );

  bool available;
  for ( int i = 0; i < TypeListCodec.count(); i++ )
  {
    if ( TypeListCodec[ i ].section( " ", 1, 1 ) == nameOfCodec )
    {
      if ( TypeListCodec[ i ].section( " ", 0, 0 ).mid( 3, 1 ) == "X" )
	*experimental = true;
      else
	*experimental = false;
	
      available = true;
      break; 
    }
    else
      available = false;
  }
  
  return available;
}

