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

  //**************************************
  SystemCall.start( recordApplikation + " " + "-formats" );
  SystemCall.waitForFinished();
  QString formats = SystemCall.readAllStandardOutput();
  SystemCall.close();
  
  ListFormats = formats.split( "\n" );
  
  // delete Header inclusive " --"
  index = ListFormats.indexOf( " --" );
  for ( int i = 0; i <= index; i++ )
    ListFormats.removeFirst();

  // remove spaces and gives then the first and the second columns back
  for ( int i = 0; i < ListFormats.count(); i++ )
  {
    ListFormats[ i ] = ListFormats[ i ].remove( 0, 1 ); // First space remove
    if ( ListFormats[ i ].mid( 0, 1 ) == " " )
      ListFormats[ i ].replace( 0, 1, "." );
    if ( ListFormats[ i ].mid( 1, 1 ) == " " )
      ListFormats[ i ].replace( 1, 1, "." );
    
    ListFormats[ i ] = ListFormats[ i ].section( " ", 0, 1 );
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
bool QvkFormatsAndCodecs::isCodecAvailable( QString typeOfCodec, QString nameOfCodec, bool *experimental )
{
  // Give all Audio or Vidocodec
  QStringList TypeListCodec = ListCodecs.filter( QRegExp( "^" + typeOfCodec.left( 1 ) ) );

  bool available = false;
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
  }
  
  return available;
}

/*
 * typeOfFormat. is string "mkv" or "avi"
 * 
 */
bool QvkFormatsAndCodecs::isFormatAvailable( QString nameOfFormat )
{
  bool available;
  
  return false;
}