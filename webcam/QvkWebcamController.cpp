#include "QvkWebcamController.h" 

QvkWebcamController::QvkWebcamController( QWidget * value )
{
  //qDebug() << "Begin QvkWebcamController::QvkWebcamController( QWidget * value ) ***************************";
  
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

  //ifWebcamShow = false;
  
  vkWebcam = new QvkWebcam( 0 );
  connect( vkWebcam, SIGNAL( closeWebcam() ), this, SLOT( webcamCloseEvent() ) );
  //qDebug() << "End QvkWebcamController::QvkWebcamController( QWidget * value ) ***************************";
  
}


QvkWebcamController::~QvkWebcamController( void )
{
}


void QvkWebcamController::saveSettings()
{
  vkWebcam->saveSettings(); 
}


void QvkWebcamController::setGeometry( int x, int y, int width, int height )
{
  checkBox->setGeometry( x, y, width, height );
}


/**
 * Wird vom Destruktor von vokoscreen aufgerufen
 */
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
  //qDebug() << "[vokoscreen] webcam added:" << addedDevices;
}


/**
 * Wird aufgerufen wenn ein Gerät entfernt wird
 */
void QvkWebcamController::webcamRemovedEvent( QStringList deviceList, QString removedDevice )
{
  (void)deviceList;
  //qDebug() << "[vokoscreen] webcam removed:" << removedDevice;

  /*
  if ( ifWebcamShow )
    vkWebcam->close();
  */
  if ( vkWebcam->isVisible() )
    vkWebcam->close();
}


bool QvkWebcamController::isVisible()
{
  return vkWebcam->isVisible();
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

  //qDebug() << "[vokoscreen] changed";
}


/**
 * Wird aufgerufen wenn das Webcam Fenster geschloßen und dabei
 * das SIGNAL closeWebcam in der Klasse QvkWebcam ausgelösst wird
 */
void QvkWebcamController::webcamCloseEvent()
{
  //qDebug() << "Begin void QvkWebcamController::webcamCloseEvent() ***************************";
  vkWebcam->setClose();
  checkBox->setCheckState( Qt::CheckState( Qt::Unchecked ) );
  //ifWebcamShow = false;
  //qDebug() << "End   void QvkWebcamController::webcamCloseEvent() ***************************";
}


/**
 * Wird aufgerufen wenn die checkbox betätigt wird
 */
void QvkWebcamController::webcam()
{
  //qDebug() << "Begin void QvkWebcamController::webcam() ***************************";
  
  if ( vkWebcam->isVisible() )
  {
  }
  else
    if ( vkWebcam->isBusy() )
    {
      QMessageBox msgBox;
      QString message;
      message.append( tr( "Device is busy" ) );
      msgBox.setText( message );
      msgBox.exec();

      checkBox->setCheckState( Qt::CheckState( Qt::Unchecked ) );    
      return;
    }
  
  if ( checkBox->isChecked() )
  {
    vkWebcam->showWebcam();
    vkWebcam->show();
    //ifWebcamShow = true;
  }
  else
  {
    vkWebcam->close();
    //ifWebcamShow = false;
  }
  //qDebug() << "End   void QvkWebcamController::webcam() ***************************";
  
}
