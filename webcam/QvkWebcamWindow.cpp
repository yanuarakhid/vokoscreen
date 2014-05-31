#include "QvkWebcamWindow.h" 

using namespace std;

QvkWebcamWindow::QvkWebcamWindow()
{
  setWindowTitle( "vokoscreen webcam");
  setGeometry( 0, 0, 320, 240 );
  
  webcamLabel = new QLabel( this );
  webcamLabel->setGeometry( 0, 0, width(), height() );
  webcamLabel->setAlignment( Qt::AlignCenter );
  webcamLabel->setScaledContents( true );
  webcamLabel->show();
}


QvkWebcamWindow::~QvkWebcamWindow()
{
}

void QvkWebcamWindow::setValueBorder( bool value )
{
  border = value; 
}

void QvkWebcamWindow::setValueX( int value )
{
  X =  value;
}

void QvkWebcamWindow::setValueY( int value )
{
  Y = value;  
}

void QvkWebcamWindow::setValueWidth( int value )
{
  Width = value;
}

void QvkWebcamWindow::setValueHeight( int value )
{
  Height = value; 
}

bool QvkWebcamWindow::getValueBorder()
{
  return border; 
}

int QvkWebcamWindow::getValueX()
{
  return X; 
}

int QvkWebcamWindow::getValueY()
{
  return Y; 
}

int QvkWebcamWindow::getValueWidth()
{
  return Width;
}

int QvkWebcamWindow::getValueHeight()
{
  return Height; 
}

/**
 *  closeEvent wird ausgelößt wenn das webcamfenster geschloßen wird
 */
void QvkWebcamWindow::closeEvent( QCloseEvent * event )
{
  (void)event;
  emit closeWebcamWindow();
}


void QvkWebcamWindow::contextMenuEvent( QContextMenuEvent *event )
{
     //qDebug() << "Begin void QvkWebcam::contextMenuEvent( QContextMenuEvent *event ) *****************************";
  
     QAction * action160x120 = new QAction( "160 x 120", this );
     connect( action160x120, SIGNAL( triggered() ), this, SLOT( set160x120() ) );
     
     QAction * action320x240 = new QAction( "320 x 240", this );
     connect( action320x240, SIGNAL( triggered() ), this, SLOT( set320x240() ) );

     QAction * action640x480 = new QAction( "640 x 480", this );
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

/**
 * Wird vom Popupmenue aufgerufen
 */
void QvkWebcamWindow::closeMenue()
{
  close();
}
 
 
void QvkWebcamWindow::set160x120()
{
  setGeometry( x(), y(), 160, 120 );
  setValueWidth( 160 );
  setValueHeight( 120 );
}


void QvkWebcamWindow::set320x240()
{
  setGeometry( x(), y(), 320, 240 );
  setValueWidth( 320 );
  setValueHeight( 240 );
}


void QvkWebcamWindow::set640x480()
{
  setGeometry( x(), y(), 640, 480 );
  setValueWidth( 640 );
  setValueHeight( 480 );
}


void QvkWebcamWindow::setNoBorder()
{
  setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
  show();
  setValueBorder( false );
}


void QvkWebcamWindow::setBorder()
{
  setWindowFlags( Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );
  show();
  setValueBorder( true );
}

void QvkWebcamWindow::resizeEvent ( QResizeEvent * )
{
  webcamLabel->setGeometry( 0, 0, this->width(), this->height() );
}
