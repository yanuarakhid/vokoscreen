#include <QProcess>
#include <QDebug>

#include "QvkFormatsAndCodecs.h"

/**
 * value is ffmpeg
 */
QvkFormatsAndCodecs::QvkFormatsAndCodecs( QString value )
{
  recordApplikation = value;
  
  QProcess SystemCall;
    SystemCall.start( recordApplikation + " " + "-encoders" );
    SystemCall.waitForFinished();
    codecs = SystemCall.readAllStandardOutput();
  SystemCall.close();
  
}


QvkFormatsAndCodecs::~QvkFormatsAndCodecs()
{
}


/**
 * value is videocodec libx264, libx265, mpeg4 etc.
 * or Audiocodec libmp3lame, libvorbis etc.
 * see output from "ffmpeg -encoders" second column
 */
bool QvkFormatsAndCodecs::getCodec( QString value )
{
  QStringList listAll = codecs.split( "\n" );
  if ( listAll.isEmpty() )
    return false;
    
  for ( int i = 0; i < listAll.count(); i++ )
  {
    listAll[ i ] = listAll[ i ].simplified();
    listAll[ i ] = listAll[ i ].remove( 0, 7 );
  }

  QStringList listCodec = listAll.filter( QRegExp( "^" + value ) );
  if ( listCodec.isEmpty() )
    return false;
  
  QString stringCodec = listCodec[ 0 ];
  
  QStringList codec = stringCodec.split( " " );
  if ( codec.isEmpty() )
  {
    return false;
  }
  else
  {
    return true;
  }
}



