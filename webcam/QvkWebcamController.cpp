#include "QvkWebcamController.h"

QvkWebcamController::QvkWebcamController( QWidget * value )
{
  checkBox = new QCheckBox( value );
  checkBox->setText( "Webcam" );
  checkBox->show();

  connect( checkBox, SIGNAL( clicked() ), SLOT( webcam() ) );
  
  myWebcamWatcher = new QvkWebcamWatcher();
  connect( myWebcamWatcher, SIGNAL( changed( QStringList ) ), this, SLOT( webcamChangedEvent( QStringList ) ) );
  connect( myWebcamWatcher, SIGNAL( added( QStringList, QStringList ) ), this, SLOT( webcamAddedEvent( QStringList, QStringList ) ) );
  connect( myWebcamWatcher, SIGNAL( removed( QStringList, QString ) ), this, SLOT( webcamRemovedEvent( QStringList, QString ) ) );
  myWebcamWatcher->myfileSystemWatcher( "/dev/" );
  
  if ( myWebcamWatcher->getWebcamCount() == 0 )
    checkBox->setEnabled( false );

  ifWebcamShow = false;
}


QvkWebcamController::~QvkWebcamController(void)
{
}


void QvkWebcamController::setGeometry( int x, int y, int width, int height )
{
  checkBox->setGeometry( x, y, width, height );
}


void QvkWebcamController::webcamClose()
{
  checkBox->click();
}


/**
 * Wird aufgerufen wenn ein Gerät hinzugefügt wird
 */
void QvkWebcamController::webcamAddedEvent( QStringList deviceList, QStringList addedDevices )
{
  (void)deviceList;
  qDebug() << "[vokoscreen] webcam added:" << addedDevices;
}

/**
 * Wird aufgerufen wenn ein Gerät entfernt wird
 */
void QvkWebcamController::webcamRemovedEvent( QStringList deviceList, QString removedDevice )
{
  (void)deviceList;
  qDebug() << "[vokoscreen] webcam removed:" << removedDevice;
  
  if ( ifWebcamShow )
    vkWebcam->close();
}


/**
 * Wird aufgerufen wenn Gerät hinzugefügt oder entfernt wird
 */
void QvkWebcamController::webcamChangedEvent( QStringList deviceList )
{
  if ( deviceList.empty() )
  {
    checkBox->setEnabled( false );
    checkBox->setCheckState( Qt::CheckState( Qt::Unchecked ) );
  }
  else
    checkBox->setEnabled( true );

  qDebug() << "[vokoscreen] changed";
}


/**
 * Wird aufgerufen wenn das Fenster geschloßen wird
 * siehe Methode wecam()
 */
void QvkWebcamController::webcamCloseEvent()
{
  checkBox->setCheckState( Qt::CheckState( Qt::Unchecked ) );
  delete vkWebcam;
  ifWebcamShow = false;
}


/**
 * Wird aufgerufen wenn die checkbox betätigt wird
 */
void QvkWebcamController::webcam()
{
  if ( checkBox->isChecked() )
  {
    vkWebcam = new QvkWebcam( 0 );
    connect( vkWebcam, SIGNAL( closeWebcam() ), this, SLOT( webcamCloseEvent() ) );
    vkWebcam->showWebcam();
    vkWebcam->show();
    ifWebcamShow = true;
  }
  else
  {
    vkWebcam->close();
    ifWebcamShow = false;
  }
}
