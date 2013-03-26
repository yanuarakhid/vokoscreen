#include "QvkWebcam.h"

using namespace std;

QvkWebcam::QvkWebcam( int deviceNumber )
{
  setDeviceNumber( deviceNumber );
  setWindowTitle( "vokoscreen webcam");
}


QvkWebcam::~QvkWebcam()
{
   cvReleaseCapture(&source);
}


void QvkWebcam::closeEvent( QCloseEvent * event )
{
  (void)event;
  emit closeWebcam();
}


void QvkWebcam::showWebcam()
{
    setGeometry( 100, 100, 320, 240 );

    source = cvCaptureFromCAM( getDeviceNumber() );
    startTimer( 25 );

    label = new QLabel( this );
    label->setAlignment( Qt::AlignCenter );
    label->setScaledContents( true );
    label->show();
}


void QvkWebcam::contextMenuEvent( QContextMenuEvent *event )
{
     QAction * action160x120 = new QAction( tr ( "160 x 120" ), this );
     connect( action160x120, SIGNAL( triggered() ), this, SLOT( set160x120() ) );
     
     QAction * action320x240 = new QAction( tr ( "320 x 240" ), this );
     connect( action320x240, SIGNAL( triggered() ), this, SLOT( set320x240() ) );

     QAction * action640x480 = new QAction( tr ( "640 x 480" ), this );
     connect( action640x480, SIGNAL( triggered() ), this, SLOT( set640x480() ) );

     QMenu menu( this );
     menu.addAction( action160x120 );
     menu.addAction( action320x240 );
     menu.addAction( action640x480 );
     menu.exec( event->globalPos() );
 }

 
void QvkWebcam::set160x120()
{
  setGeometry( x(), y(), 160, 120 );
}


void QvkWebcam::set320x240()
{
  setGeometry( x(), y(), 320, 240 );
}


void QvkWebcam::set640x480()
{
  setGeometry( x(), y(), 640, 480 );
}
 

void QvkWebcam::setDeviceNumber( int value )
{
  deviceNumber = value;
}


int QvkWebcam::getDeviceNumber()
{
  return deviceNumber;
}


void QvkWebcam::resizeEvent ( QResizeEvent * )
{
  label->setGeometry( 0, 0, this->width(), this->height() );
}


void QvkWebcam::timerEvent( QTimerEvent* )
{
    image = cvQueryFrame( source );
    label->setPixmap( QPixmap::fromImage( ConvertImage( image ) ) );
    //cvShowImage("Livebild", image);
}


/*
QImage ConvertImage( IplImage *source )
{
    QImage dummy;

    if ( source->depth != IPL_DEPTH_8U || source->nChannels != 3 )
        return dummy;

    // Bilddaten übernehmen
    unsigned char * data = ( unsigned char * ) source->imageData;

    // QImage mit Originaldaten erstellen
    QImage ret( data, source->width, source->height, QImage::Format_RGB888 );

    // Kanäle (BGR -> RGB) und Format (RGB888 -> RGB32) ändern
    return ret.rgbSwapped().convertToFormat( QImage::Format_RGB32 );
}
*/


QImage QvkWebcam::ConvertImage(IplImage *source)
{
    int cvIndex, cvLineStart;
    QImage ret;

    if(source->depth != IPL_DEPTH_8U || source->nChannels != 3)
        return ret;

    QImage temp(source->width, source->height, QImage::Format_RGB32);
    ret = temp;
    cvIndex = 0;
    cvLineStart = 0;
    for (int y = 0; y < source->height; y++)
    {
        unsigned char red,green,blue;
        cvIndex = cvLineStart;
        for (int x = 0; x < source->width; x++)
        {
            red = source->imageData[cvIndex+2];
            green = source->imageData[cvIndex+1];
            blue = source->imageData[cvIndex+0];

            ret.setPixel(x,y,qRgb(red, green, blue));
            cvIndex += 3;
        }
        cvLineStart += source->widthStep;
    }

    return ret;
}

