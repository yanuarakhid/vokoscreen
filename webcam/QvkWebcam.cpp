#include "QvkWebcam.h" 

using namespace std;

QvkWebcam::QvkWebcam( int deviceNumber )
{
  //qDebug() << "Begin QvkWebcam::QvkWebcam( int deviceNumber ) *****************************";
  
  setDeviceNumber( deviceNumber );
  setWindowTitle( "vokoscreen webcam");

  label = new QLabel( this );
  label->setAlignment( Qt::AlignCenter );
  label->setScaledContents( true );
  label->show();
  
  QSettings settings( "vokoscreen", "vokoscreen" );   
  settings.beginGroup( "Webcam" );
    setValueBorder( settings.value( "Border", true ).toBool() );
    setValueX( settings.value( "X", 100).toUInt() );
    setValueY( settings.value( "Y", 100).toUInt() );
    setValueWidth( settings.value( "Width", 320).toUInt() );
    setValueHeight( settings.value( "Height", 240).toUInt() );
  settings.endGroup();

  setGeometry( getValueX(),
               getValueY(),
               getValueWidth(),
               getValueHeight() );
   
}

void QvkWebcam::setValueBorder( bool value )
{
  border = value; 
}

void QvkWebcam::setValueX( int value )
{
  X =  value;
}

void QvkWebcam::setValueY( int value )
{
  Y = value;  
}

void QvkWebcam::setValueWidth( int value )
{
  Width = value;
}

void QvkWebcam::setValueHeight( int value )
{
  Height = value; 
}

bool QvkWebcam::getValueBorder()
{
  return border; 
}

int QvkWebcam::getValueX()
{
  return X; 
}

int QvkWebcam::getValueY()
{
  return Y; 
}

int QvkWebcam::getValueWidth()
{
  return Width;
}

int QvkWebcam::getValueHeight()
{
  return Height; 
}

/**
 * Wird von vokoscreen.cpp aufgerufen
 */
void QvkWebcam::saveSettings()
{
  QSettings settings( "vokoscreen", "vokoscreen" );   
  settings.beginGroup( "Webcam" );
    settings.setValue( "Border",getValueBorder() );
    settings.setValue( "X", x() );
    settings.setValue( "Y", y() );
    settings.setValue( "Width", width() );
    settings.setValue( "Height", height() );
  settings.endGroup();
}


QvkWebcam::~QvkWebcam()
{
   //qDebug() << "Begin QvkWebcam::~QvkWebcam() *****************************";
   //qDebug() << "End   QvkWebcam::~QvkWebcam() *****************************";
}


/**
 *  closeEvent wird ausgelößt wenn das webcamfenster geschloßen wird
 */
void QvkWebcam::closeEvent( QCloseEvent * event )
{
  //qDebug() << "Begin void QvkWebcam::closeEvent( QCloseEvent * event ) *****************************";
  (void)event;
  emit closeWebcam();
  //qDebug() << "End   void QvkWebcam::closeEvent( QCloseEvent * event ) *****************************";
}


void QvkWebcam::moveEvent( QMoveEvent * event )
{
  //qDebug() << "End  void QvkWebcam::moveEvent( QMoveEvent * event )  *****************************";
  (void) event;
  setValueX( x() );
  setValueY( y() );
}


/**
 * 
 */
void QvkWebcam::setClose()
{
  //qDebug() << "Begin void QvkWebcam::setClose() *****************************";
  killTimer( idTimer );
  cvReleaseCapture(&source);
  //qDebug() << "End   void QvkWebcam::setClose() *****************************";
}


/**
 * Wird vom Popupmenue aufgerufen
 */
void QvkWebcam::closeMenue()
{
  killTimer( idTimer );
  cvReleaseCapture(&source);
  close();
}


bool QvkWebcam::isBusy()
{
  source = cvCaptureFromCAM( getDeviceNumber() );
  if ( source == NULL )
  {
    cvReleaseCapture(&source);
    return true;
  }
  else
  {
    cvReleaseCapture(&source);
    return false;
  }
}


void QvkWebcam::showWebcam()
{
  //qDebug() << "Begin void QvkWebcam::showWebcam() *****************************";
  
  source = cvCaptureFromCAM( getDeviceNumber() );
  idTimer = startTimer( 25 );
   
  if ( getValueBorder() )
    setBorder();  
  else
    setNoBorder();
    
    setGeometry( getValueX(),
                 getValueY(),
                 getValueWidth(),
                 getValueHeight() );
   
  //qDebug() << "End   void QvkWebcam::showWebcam() *****************************";
}


void QvkWebcam::contextMenuEvent( QContextMenuEvent *event )
{
     //qDebug() << "Begin void QvkWebcam::contextMenuEvent( QContextMenuEvent *event ) *****************************";
  
     QAction * action160x120 = new QAction( tr ( "160 x 120" ), this );
     connect( action160x120, SIGNAL( triggered() ), this, SLOT( set160x120() ) );
     
     QAction * action320x240 = new QAction( tr ( "320 x 240" ), this );
     connect( action320x240, SIGNAL( triggered() ), this, SLOT( set320x240() ) );

     QAction * action640x480 = new QAction( tr ( "640 x 480" ), this );
     connect( action640x480, SIGNAL( triggered() ), this, SLOT( set640x480() ) );

     QAction * actionNoBorder = new QAction( tr ( "Noborder" ), this );
     connect( actionNoBorder, SIGNAL( triggered() ), this, SLOT( setNoBorder() ) );

     QAction * actionBorder = new QAction( tr ( "Border" ), this );
     connect( actionBorder, SIGNAL( triggered() ), this, SLOT( setBorder() ) );

     QAction * actionClose = new QAction( tr ( "Close" ), this );
     connect( actionClose, SIGNAL( triggered() ), this, SLOT( closeMenue() ) );
     
     
     QMenu menu( this );
     menu.addAction( action160x120 );
     menu.addAction( action320x240 );
     menu.addAction( action640x480 );
     menu.addSeparator();
     menu.addAction( actionNoBorder );
     menu.addAction( actionBorder );
     menu.addSeparator();
     menu.addAction( actionClose );
     menu.exec( event->globalPos() );
 }

 
void QvkWebcam::set160x120()
{
  setGeometry( x(), y(), 160, 120 );
  setValueWidth( 160 );
  setValueHeight( 120 );
}


void QvkWebcam::set320x240()
{
  setGeometry( x(), y(), 320, 240 );
  setValueWidth( 320 );
  setValueHeight( 240 );
}


void QvkWebcam::set640x480()
{
  setGeometry( x(), y(), 640, 480 );
  setValueWidth( 640 );
  setValueHeight( 480 );
}


void QvkWebcam::setNoBorder()
{
  setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
  show();
  setValueBorder( false );
}


void QvkWebcam::setBorder()
{
  setWindowFlags( Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );
  show();
  setValueBorder( true );
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

