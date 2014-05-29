#include "QvkWebcamController.h" 

QvkWebcamController::QvkWebcamController( QCheckBox *myCheckBox, QComboBox *myComboBox, QCheckBox *myMirrorCheckBox )
{
  checkBox = myCheckBox;
  checkBox->setEnabled( false );
  connect( checkBox, SIGNAL( clicked( bool ) ), this, SLOT( setWebcamOnOff( bool ) ) );
  
  comboBox = myComboBox;

  mirrorCheckBox = myMirrorCheckBox;
  connect( mirrorCheckBox, SIGNAL( clicked( bool ) ), this, SLOT( setMirrorOnOff( bool ) ) );

  
  captureThread = new CaptureThread();
  connect( captureThread, SIGNAL( newPicture( QImage ) ), this, SLOT( setNewImage( QImage ) ) );
  
  myWebcamWatcher = new QvkWebcamWatcher();
  connect( myWebcamWatcher, SIGNAL( changed( QStringList ) ), this, SLOT( webcamChangedEvent( QStringList ) ) );
  connect( myWebcamWatcher, SIGNAL( removed( QStringList, QString ) ), this, SLOT( webcamRemovedEvent( QStringList, QString ) ) );
  myWebcamWatcher->myfileSystemWatcher( "/dev/" );
  
  webcamWindow = new QvkWebcamWindow();
  connect( webcamWindow, SIGNAL( closeWebcamWindow() ), SLOT( webcamCloseEvent() ) );  
  (void) webcamWindow;
  
}

QvkWebcamController::~QvkWebcamController( void )
{
}


/**
 * Wird aufgerufen wenn das Webcam Fenster geschloßen und dabei
 * das SIGNAL closeWebcamWindow in der Klasse QvkWebcamWindow ausgelösst wird
 */
void QvkWebcamController::webcamCloseEvent()
{
  checkBox->setCheckState( Qt::CheckState( Qt::Unchecked ) );
  comboBox->setEnabled( true );
  if ( captureThread->running )
    captureThread->stop();
}


void QvkWebcamController::setMirrorOnOff( bool value )
{
  if ( value == true )
    mirrored = true;
  else
    mirrored = false;
}

/*
 * Wird ausgelößt wenn checkbox getätigt wird
 */
void QvkWebcamController::setWebcamOnOff( bool value )
{
  if ( value == false )
  {
    captureThread->stop();
    webcamWindow->close();
    comboBox->setEnabled( true );
    return;
  }
 
  QString index = comboBox->currentText().left( 2 ).right( 1 );
  //index = index.right( 1 );
  if ( captureThread->busy( "/dev/video" + index ) == true )
  {
    qDebug() << "[vokoscreen] webcam device /dev/video" + comboBox->currentText() << "is busy";
    QMessageBox messageBox( QMessageBox::Warning,
                            "vokoscren webcam",
                            "Webcam /dev/video" + comboBox->currentText() + " " + tr( "is busy") );
    
    messageBox.exec();
    checkBox->setCheckState( Qt::CheckState( Qt::Unchecked ) );
    return;
  }
  
  if ( value == true )
  {
    comboBox->setEnabled( false );
    webcamWindow->show();
    webcamWindow->currentDevice = "/dev/video" + index;
    captureThread->start( "/dev/video" + index );
  }
}


void QvkWebcamController::setNewImage( QImage image )
{
  if ( mirrored == true )
    image = image.mirrored ( true, false );
  
  webcamWindow->webcamLabel->setPixmap( QPixmap::fromImage( image, Qt::AutoColor) );
}


/**
 * Wird aufgerufen wenn ein Gerät hinzugefügt wird
 */
void QvkWebcamController::webcamAddedEvent( QStringList deviceList, QStringList addedDevices )
{
  (void)deviceList;
  (void)addedDevices;
}


/**
 * Wird aufgerufen wenn ein Gerät entfernt wird
 */
void QvkWebcamController::webcamRemovedEvent( QStringList deviceList, QString removedDevice )
{
  (void)deviceList;

  if ( removedDevice.right( 1 ) == webcamWindow->currentDevice.right( 1 ) )
    webcamWindow->close();
}


/**
 * Wird aufgerufen wenn ein Gerät hinzugefügt oder entfernt wird
 */
void QvkWebcamController::webcamChangedEvent( QStringList deviceList )
{
  comboBox->clear();
  for( int x = 0; x < myWebcamWatcher->getWebcamCount(); x++ )
  {
    //comboBox->addItem( deviceList[x].right( 1 ) );
//    comboBox->addItem( "[" + QString::number( x ) + "]" + " " + captureThread->getNameFromDevice( "/dev/video" + deviceList[x].right( 1 ) ) );
    comboBox->addItem( "[" + deviceList[x].right( 1 ) + "]" + " " + captureThread->getNameFromDevice( "/dev/video" + deviceList[x].right( 1 ) ) );
  }

  if ( deviceList.empty() )
  {
    checkBox->setEnabled( false );
    checkBox->setCheckState( Qt::CheckState( Qt::Unchecked ) );
    comboBox->setEnabled( false );
  }
  
  if ( not deviceList.empty() )
  {
    checkBox->setEnabled( true );
    comboBox->setEnabled( true );
  }
}
