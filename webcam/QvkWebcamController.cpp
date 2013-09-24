#include "QvkWebcamController.h" 

QvkWebcamController::QvkWebcamController( QWidget * value )
{
  //qDebug() << "Begin QvkWebcamController::QvkWebcamController( QWidget * value ) ***************************";
  
  checkBox = new QCheckBox( value );
  checkBox->setText( tr( "Webcam" ) );
  checkBox->show();
  connect( checkBox, SIGNAL( clicked() ), SLOT( webcam() ) );

  comboBoxCount = new QComboBox( value );
  comboBoxCount->setGeometry( 250, 40, 40, 21 );
  comboBoxCount->setToolTip( "Select webcam" );
  comboBoxCount->show();
  
  myWebcamWatcher = new QvkWebcamWatcher();
  connect( myWebcamWatcher, SIGNAL( changed( QStringList ) ), this, SLOT( webcamChangedEvent( QStringList ) ) );
  connect( myWebcamWatcher, SIGNAL( added( QStringList, QStringList ) ), this, SLOT( webcamAddedEvent( QStringList, QStringList ) ) );
  connect( myWebcamWatcher, SIGNAL( removed( QStringList, QString ) ), this, SLOT( webcamRemovedEvent( QStringList, QString ) ) );
  myWebcamWatcher->myfileSystemWatcher( "/dev/" );

  if ( myWebcamWatcher->getWebcamCount() == 0 )
  {
    checkBox->setEnabled( false );
    comboBoxCount->setEnabled( false );
  }

  vkWebcam = new QvkWebcam();
  QSettings settings( "vokoscreen", "vokoscreen" );   
  settings.beginGroup( "Webcam" );
    vkWebcam->setDeviceNumber( settings.value( "Number", 0 ).toUInt() );
    comboBoxCount->setCurrentIndex( vkWebcam->getDeviceNumber() );
  settings.endGroup();
  
  //vkWebcam->setDeviceNumber( comboBoxCount->currentText().toUInt() );
  //comboBoxCount->setCurrentIndex( vkWebcam->getDeviceNumber() );
  
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
  (void)addedDevices;
  //qDebug() << "[vokoscreen] webcam added:" << addedDevices;
}


/**
 * Wird aufgerufen wenn ein Gerät entfernt wird
 */
void QvkWebcamController::webcamRemovedEvent( QStringList deviceList, QString removedDevice )
{
  (void)deviceList;
  (void)removedDevice;
  //qDebug() << "[vokoscreen] webcam removed:" << removedDevice;

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
  comboBoxCount->clear();
  for( int x = 0; x < myWebcamWatcher->getWebcamCount(); x++ )
  {
    comboBoxCount->addItem( QString::number( x ) );
  }

  if ( deviceList.empty() )
  {
    checkBox->setEnabled( false );
    checkBox->setCheckState( Qt::CheckState( Qt::Unchecked ) );
    comboBoxCount->setEnabled( false );
  }
  else
  {
    checkBox->setEnabled( true );
    comboBoxCount->setEnabled( true );
  }

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
  //qDebug() << "End   void QvkWebcamController::webcamCloseEvent() ***************************";
  comboBoxCount->setEnabled( true );
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
    comboBoxCount->setEnabled( false );
    vkWebcam->setDeviceNumber( comboBoxCount->currentText().toUInt() );
    vkWebcam->showWebcam();
    vkWebcam->show();
  }
  else
  {
    comboBoxCount->setEnabled( true );
    vkWebcam->close();
  }
  //qDebug() << "End   void QvkWebcamController::webcam() ***************************";
  
}
