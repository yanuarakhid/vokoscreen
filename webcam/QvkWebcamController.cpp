#include "QvkWebcamController.h" 

#include <QTest>

//QvkWebcamController::QvkWebcamController( QCheckBox *myCheckBox, QComboBox *myComboBox, QCheckBox *myMirrorCheckBox,
//					  QFrame *myRotateFrame ,QDial *myRotateDial, QRadioButton *myRadioButtonTopMiddle,
//					  QRadioButton *myRadioButtonRightMiddle, QRadioButton *myRadioButtonBottomMiddle, QRadioButton *myRadioButtonLeftMiddle )
QvkWebcamController::QvkWebcamController( Ui_screencast value )
{
  vkSettings.readAll();

  myUi = value;
  //checkBox = myCheckBox;
  myUi.webcamCheckBox->setEnabled( false );
  connect( myUi.webcamCheckBox, SIGNAL( clicked( bool ) ), this, SLOT( setWebcamOnOff( bool ) ) );
  
  //rotateFrame = myRotateFrame;
  
  //comboBox = myComboBox;
  
  mirrored = false;
  //mirrorCheckBox = myMirrorCheckBox;
  
  if ( myUi.webcamCheckBox->checkState() == Qt::Unchecked )
  {
    myUi.mirrorCheckBox->setEnabled( false );
    myUi.dialFrame->setEnabled( false );
  }
  else
  {
    myUi.mirrorCheckBox->setEnabled( true );
    myUi.dialFrame->setEnabled( false );
  }
  connect( myUi.mirrorCheckBox, SIGNAL( clicked( bool ) ), this, SLOT( setMirrorOnOff( bool ) ) );
  if ( vkSettings.getWebcamMirrored() == Qt::Checked )
  {
    myUi.mirrorCheckBox->setEnabled( true );
    myUi.mirrorCheckBox->click();
    myUi.mirrorCheckBox->setEnabled( false );
  }

  //rotateDial = myRotateDial;
  myUi.rotateDial->setMinimum( 0 );
  myUi.rotateDial->setMaximum ( 360 );
  myUi.rotateDial->setValue( 0 );
  connect( myUi.rotateDial, SIGNAL( sliderPressed () ), this, SLOT( rotateDialclicked() ) );
  
//  radioButtonTopMiddle = myRadioButtonTopMiddle;
  myUi.radioButtonTopMiddle->setChecked( vkSettings.getWebcamButtonTopMiddle() );
  
  //radioButtonRightMiddle = myRadioButtonRightMiddle;
  myUi.radioButtonRightMiddle->setChecked( vkSettings.getWebcamButtonRightMiddle() );
  
  //radioButtonBottomMiddle = myRadioButtonBottomMiddle;
  myUi.radioButtonBottomMiddle->setChecked( vkSettings.getWebcamButtonBottomMiddle() );
  
  //radioButtonLeftMiddle = myRadioButtonLeftMiddle;
  myUi.radioButtonLeftMiddle->setChecked( vkSettings.getWebcamButtonLeftMiddle() );
  
  captureThread = new CaptureThread();
  connect( captureThread, SIGNAL( newPicture( QImage ) ), this, SLOT( setNewImage( QImage ) ) );
  
  myWebcamWatcher = new QvkWebcamWatcher();
  connect( myWebcamWatcher, SIGNAL( changed( QStringList ) ), this, SLOT( webcamChangedEvent( QStringList ) ) );
  connect( myWebcamWatcher, SIGNAL( removed( QStringList, QString ) ), this, SLOT( webcamRemovedEvent( QStringList, QString ) ) );
  myWebcamWatcher->myfileSystemWatcher( "/dev/" );
  
  webcamWindow = new QvkWebcamWindow();
  connect( webcamWindow, SIGNAL( setOverScreen() ), this, SLOT( checkBox_OnOff() ) );
  connect( webcamWindow, SIGNAL( closeWebcamWindow() ), SLOT( webcamCloseEvent() ) );

  connect( myWebcamWatcher, SIGNAL( readWebcamNames( QStringList ) ), this, SLOT( readWebcams( QStringList ) ) );
  
  if ( vkSettings.getWebcamOnOff() == Qt::Unchecked )
  {
    webcamCloseEvent();
  }
}


QvkWebcamController::~QvkWebcamController( void )
{
}

void QvkWebcamController::checkBox_OnOff()
{
  myUi.webcamCheckBox->click();
  QCoreApplication::processEvents( QEventLoop::AllEvents );     
  webcamWindow->setVisibleOverFullscreen();
  myUi.webcamCheckBox->click();
  myUi.webcamCheckBox->setChecked( Qt::Checked );
}

void QvkWebcamController::rotateDialclicked()
{
  // Diese drei Befehle müssen sein damit der Radiobutton unchecked ist
  myUi.radioButtonTopMiddle->setCheckable( false );
  myUi.radioButtonTopMiddle->setChecked( false );
  myUi.radioButtonTopMiddle->setCheckable ( true );
  myUi.radioButtonTopMiddle->update();
  
  myUi.radioButtonRightMiddle->setCheckable ( false );
  myUi.radioButtonRightMiddle->setChecked( false );
  myUi.radioButtonRightMiddle->setCheckable ( true );
  myUi.radioButtonRightMiddle->update();
  
  myUi.radioButtonBottomMiddle->setCheckable ( false );
  myUi.radioButtonBottomMiddle->setChecked( false );
  myUi.radioButtonBottomMiddle->setCheckable ( true );
  myUi.radioButtonBottomMiddle->update();
  
  myUi.radioButtonLeftMiddle->setCheckable ( false );
  myUi.radioButtonLeftMiddle->setChecked( false );
  myUi.radioButtonLeftMiddle->setCheckable ( true );
  myUi.radioButtonLeftMiddle->update();
}


/**
 * Wird aufgerufen wenn das Webcam Fenster geschloßen und dabei
 * das SIGNAL closeWebcamWindow in der Klasse QvkWebcamWindow ausgelösst wird
 */
void QvkWebcamController::webcamCloseEvent()
{
  myUi.webcamCheckBox->setCheckState( Qt::CheckState( Qt::Unchecked ) );
  myUi.webcamComboBox->setEnabled( true );
  myUi.mirrorCheckBox->setEnabled( false );
  myUi.dialFrame->setEnabled( false );
  
  if ( captureThread->running )
    captureThread->stop();
  
  webcamWindow->close();
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
    return;
  }

  QString index = myUi.webcamComboBox->currentText().left( 2 ).right( 1 );
  QProcess Process;
  Process.start( "lsof /dev/video" + index );
  Process.waitForFinished();
  QString standardOutput = Process.readAllStandardOutput();
  Process.close();
  
  if ( captureThread->busy( "/dev/video" + index ) == true )
  {
    QString usedBy = "";
    if ( standardOutput > "" )
    {
      QStringList list = standardOutput.split( "\n" );
      if ( not list.empty() )
      {
        list = list[ 1 ].split( " " );
        usedBy = list[ 0 ];
      }
    }
    
    QvkWebcamBusyDialog *WebcamBusyDialog = new QvkWebcamBusyDialog( myUi.webcamComboBox->currentText().remove( 0, 4 ), "/dev/video" + index, usedBy );
    (void) WebcamBusyDialog;
  
    qDebug() << "[vokoscreen] webcam device /dev/video" + index + " " + myUi.webcamComboBox->currentText().remove( 0, 4 ) + " is busy by " + usedBy;
    myUi.webcamCheckBox->setChecked( Qt::Unchecked );
    return;
  }
  
  if ( value == true )    
  {
    myUi.webcamComboBox->setEnabled( false );
    myUi.mirrorCheckBox->setEnabled( true );
    myUi.dialFrame->setEnabled( true );
    webcamWindow->show();
    webcamWindow->currentDevice = "/dev/video" + index;
    captureThread->start( "/dev/video" + index );
  }
}


void QvkWebcamController::setNewImage( QImage image )
{
  if ( mirrored == true )
    image = image.mirrored ( true, false );

  if (myUi.radioButtonLeftMiddle->isChecked() == true )
    myUi.rotateDial->setValue( 90 );
  
  if ( myUi.radioButtonTopMiddle->isChecked() == true )
    myUi.rotateDial->setValue( 180 );

  if ( myUi.radioButtonRightMiddle->isChecked() == true )
    myUi.rotateDial->setValue( 270 );
  
  if ( myUi.radioButtonBottomMiddle->isChecked() == true )
    myUi.rotateDial->setValue( 360 );


  QTransform transform;
  transform.rotate( myUi.rotateDial->value() );
  QImage transformedImage = image.transformed( transform );

  // Passt Bild beim resizen des Fensters an
  transformedImage = transformedImage.scaled( webcamWindow->webcamLabel->width(), webcamWindow->webcamLabel->height(), Qt::KeepAspectRatio, Qt::FastTransformation);
  webcamWindow->webcamLabel->setPixmap( QPixmap::fromImage( transformedImage, Qt::AutoColor) );
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
 * This routine read the webcam name 5 seconds after the webcam device
 */
void QvkWebcamController::readWebcams( QStringList deviceList )
{
  webcamList = deviceList;
  QTimer::singleShot( 5000, this, SLOT( webcams() ) );
}


void QvkWebcamController::webcams()
{
  webcamChangedEvent( webcamList );
}


/**
 * Wird aufgerufen wenn ein Gerät hinzugefügt oder entfernt wird
 */
void QvkWebcamController::webcamChangedEvent( QStringList deviceList )
{
  myUi.webcamComboBox->clear();
  for( int x = 0; x < myWebcamWatcher->getWebcamCount(); x++ )
    myUi.webcamComboBox->addItem( "[" + deviceList[x].right( 1 ) + "]" + " " + captureThread->getNameFromDevice( "/dev/video" + deviceList[x].right( 1 ) ) );

  if ( deviceList.empty() )
  {
    myUi.webcamCheckBox->setEnabled( false );
    myUi.webcamCheckBox->setCheckState( Qt::CheckState( Qt::Unchecked ) );
    myUi.webcamComboBox->setEnabled( false );
  }
  
  if ( not deviceList.empty() )
  {
    myUi.webcamCheckBox->setEnabled( true );
    myUi.webcamComboBox->setEnabled( true );
  }
}

