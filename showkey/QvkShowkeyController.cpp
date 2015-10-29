#include "QvkShowkeyController.h"

QvkShowkeyController::QvkShowkeyController()
{
}

QvkShowkeyController::QvkShowkeyController( QCheckBox *value )
{
  checkBox = value;
  
  // Fenster das den Key anzeigt  
  showkeyWindow = new QvkShowkeyWindow();
  connect( checkBox, SIGNAL( stateChanged( int ) ), this, SLOT( showkeyReadKey( int ) ) );

  pThread = new QThread();
  xev = new QvkShowkeyGetkey();
  
  xev->moveToThread( pThread );
  connect( xev, SIGNAL( pressedKey( QString ) ), this, SLOT( showScreenkeyWindow( QString ) ) );
  connect( xev, SIGNAL( finished() ), this, SLOT( finished() ) );
  
  screenkeyTimer = new QTimer( this );
  connect( screenkeyTimer, SIGNAL( timeout() ), this, SLOT( hideScreenkeyWindow() ) );
}

QvkShowkeyController::~QvkShowkeyController()
{
}

void QvkShowkeyController::finished()
{
  if ( checkBox->checkState() == Qt::Checked )
    xev->start();
}

void QvkShowkeyController::showkeyReadKey( int value )
{
  if ( value == Qt::Checked )
    xev->start();
  
  if ( value == Qt::Unchecked )
    xev->terminate();
}

void QvkShowkeyController::showScreenkeyWindow( QString value)
{
   screenkeyTimer->stop();
   screenkeyTimer->start( 3000 );
   showkeyWindow->show();
   showkeyWindow->keyLabel->setText( showkeyWindow->keyLabel->text() + value );
}


void QvkShowkeyController::hideScreenkeyWindow()
{
  showkeyWindow->keyLabel->setText( "" );
  showkeyWindow->hide();
  screenkeyTimer->stop();
}
