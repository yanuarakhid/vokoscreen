#include "QvkWebcamWindow.h" 

using namespace std;

QvkWebcamWindow::QvkWebcamWindow()
{
  setWindowFlags( Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );
  setWindowTitle( "vokoscreen webcam");
  setGeometry( 0, 0, 320, 240 );

  if( QX11Info::isCompositingManagerRunning() )
    setAttribute( Qt::WA_TranslucentBackground, true );
  
  webcamLabel = new QLabel( this );
  webcamLabel->setGeometry( 0, 0, width(), height() );
  webcamLabel->setAlignment( Qt::AlignCenter );
  webcamLabel->setScaledContents( true );
  webcamLabel->show();
  
  action160x120 = new QAction( "160 x 120", this );
  connect( action160x120, SIGNAL( triggered() ), this, SLOT( set160x120() ) );
   
  action320x240 = new QAction( "320 x 240", this );
  connect( action320x240, SIGNAL( triggered() ), this, SLOT( set320x240() ) );

  action640x480 = new QAction( "640 x 480", this );
  connect( action640x480, SIGNAL( triggered() ), this, SLOT( set640x480() ) );

  actionBorder = new QAction( tr ( "Border" ), this );
  actionBorder->setCheckable( true );
  actionBorder->setChecked( true );
  connect( actionBorder, SIGNAL( triggered() ), this, SLOT( setBorder() ) );

  actionVisibleOverFullscreen = new QAction( tr ( "Show over fullscreen" ), this );
  connect( actionVisibleOverFullscreen, SIGNAL( triggered() ), this, SLOT( setVisibleOverFullscreen() ) );

  actionClose = new QAction( tr ( "Close" ), this );
  connect( actionClose, SIGNAL( triggered() ), this, SLOT( closeMenue() ) ); 
}


QvkWebcamWindow::~QvkWebcamWindow()
{
}

void QvkWebcamWindow::setValueBorder( bool value )
{
  border = value; 
}

bool QvkWebcamWindow::getValueBorder()
{
  return border; 
}

int QvkWebcamWindow::getValueX()
{
  return x();
}

int QvkWebcamWindow::getValueY()
{
  return y();
}

int QvkWebcamWindow::getValueWidth()
{
  return width();
}

int QvkWebcamWindow::getValueHeight()
{
  return height();
}

/**
 *  closeEvent wird ausgelößt wenn das webcamfenster geschloßen wird
 */
void QvkWebcamWindow::closeEvent( QCloseEvent * event )
{
  (void)event;
  emit closeWebcamWindow();
}

/**
 * Wird aufgerufen sobald die Kontexttaste betätigt wird
 */
void QvkWebcamWindow::contextMenuEvent( QContextMenuEvent *event )
{
     menu.addAction( action160x120 );
     menu.addAction( action320x240 );
     menu.addAction( action640x480 );
     menu.addSeparator();
     menu.addAction( actionBorder );
     menu.addAction( actionVisibleOverFullscreen );
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
}


void QvkWebcamWindow::set320x240()
{
  setGeometry( x(), y(), 320, 240 );
}


void QvkWebcamWindow::set640x480()
{
  setGeometry( x(), y(), 640, 480 );
}


void QvkWebcamWindow::setBorder()
{
  if ( actionBorder->isChecked() )
  {
    setWindowFlags( Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );
    show();
    setValueBorder( true );
  }
  else
  {
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    show();
    setValueBorder( false );
  }
}


void QvkWebcamWindow::setVisibleOverFullscreen()
{
   setWindowFlags( Qt::WindowTitleHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint );
   activateWindow();
   show();
}


void QvkWebcamWindow::resizeEvent ( QResizeEvent * )
{
  webcamLabel->setGeometry( 0, 0, this->width(), this->height() );
}
