#include "QvkMsgInWebcamWindow.h" 

QvkMsgInWebcamWindow::QvkMsgInWebcamWindow( QObject *parent, QWidget *webcamWindow, QString value )
{
      connect( parent, SIGNAL( webcamBusy() ), this, SLOT( close() ) );
      connect( webcamWindow, SIGNAL( webcamWindoResize( QSize ) ), this, SLOT( setNewSize( QSize ) ) );

      setParent( webcamWindow );
      setStyleSheet( "background-color: lightgray;" );
      setGeometry( 0, 0, webcamWindow->width(), webcamWindow->height() );
      setWordWrap( true );
      setAlignment( Qt::AlignCenter );
      QFont font;
      font.setPointSize( 14 );
      setFont( font );
      setText( value );
      show();
}


QvkMsgInWebcamWindow::~QvkMsgInWebcamWindow()
{
}


void QvkMsgInWebcamWindow::setNewSize( QSize size )
{
    setGeometry( 0, 0, size.width(), size.height() );
}
