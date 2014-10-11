#include "QvkWebcamController.h" 
#include <QTimer>
#include <QSettings>

QvkWebcamController::QvkWebcamController( QCheckBox *myCheckBox, QComboBox *myComboBox, QCheckBox *myMirrorCheckBox, 
					  QFrame *myRotateFrame ,QDial *myRotateDial, QRadioButton *myRadioButtonTopMiddle, QRadioButton *myRadioButtonRightMiddle, QRadioButton *myRadioButtonBottomMiddle, QRadioButton *myRadioButtonLeftMiddle )
{
  
  vkSettings.readAll();
  
  checkBox = myCheckBox;
  checkBox->setEnabled( false );
  connect( checkBox, SIGNAL( clicked( bool ) ), this, SLOT( setWebcamOnOff( bool ) ) );
  
  rotateFrame = myRotateFrame;
  
  comboBox = myComboBox;
  
  mirrored = false;
  mirrorCheckBox = myMirrorCheckBox;
  
  if ( checkBox->checkState() == Qt::Unchecked )
  {
    mirrorCheckBox->setEnabled( false );
    rotateFrame->setEnabled( false );
  }
  else
  {
    mirrorCheckBox->setEnabled( true );
    rotateFrame->setEnabled( false );
  }
  connect( mirrorCheckBox, SIGNAL( clicked( bool ) ), this, SLOT( setMirrorOnOff( bool ) ) );
  if ( vkSettings.getWebcamMirrored() == true )
  {
    mirrorCheckBox->setEnabled( true );
    mirrorCheckBox->click();
    mirrorCheckBox->setEnabled( false );
  }

  rotateDial = myRotateDial;
  rotateDial->setMinimum( 0 );
  rotateDial->setMaximum ( 360 );
  rotateDial->setValue( 0 );
  connect( rotateDial, SIGNAL( sliderPressed () ), this, SLOT( rotateDialclicked() ) );
  
  radioButtonTopMiddle = myRadioButtonTopMiddle;
  radioButtonRightMiddle = myRadioButtonRightMiddle;
  radioButtonBottomMiddle = myRadioButtonBottomMiddle;
  radioButtonBottomMiddle->setChecked( true );
  radioButtonLeftMiddle = myRadioButtonLeftMiddle;
 
  captureThread = new CaptureThread();
  connect( captureThread, SIGNAL( newPicture( QImage ) ), this, SLOT( setNewImage( QImage ) ) );
  
  myWebcamWatcher = new QvkWebcamWatcher();
  connect( myWebcamWatcher, SIGNAL( changed( QStringList ) ), this, SLOT( webcamChangedEvent( QStringList ) ) );
  connect( myWebcamWatcher, SIGNAL( removed( QStringList, QString ) ), this, SLOT( webcamRemovedEvent( QStringList, QString ) ) );
  myWebcamWatcher->myfileSystemWatcher( "/dev/" );
  
  webcamWindow = new QvkWebcamWindow();
  connect( webcamWindow, SIGNAL( closeWebcamWindow() ), SLOT( webcamCloseEvent() ) );
  webcamWindow->setGeometry( vkSettings.getWebcamX(), vkSettings.getWebcamY(), vkSettings.getWebcamWidth(), vkSettings.getWebcamHeight() );
  
  connect( myWebcamWatcher, SIGNAL( readWebcamNames( QStringList ) ), this, SLOT( readWebcams( QStringList ) ) );
}


QvkWebcamController::~QvkWebcamController( void )
{
}


void QvkWebcamController::saveSettings()
{
  QSettings settings( "vokoscreen", "vokoscreen" );
  settings.beginGroup( "Webcam" );
    settings.setValue( "X", webcamWindow->getValueX() );
    settings.setValue( "Y", webcamWindow->getValueY() );
    settings.setValue( "Width", webcamWindow->getValueWidth() );
    settings.setValue( "Height", webcamWindow->getValueHeight() );
    settings.setValue( "Border", webcamWindow->getValueBorder() );
  settings.endGroup();
}


void QvkWebcamController::rotateDialclicked()
{
  // Diese drei Befehle müssen sein damit der Radiobutton unchecked ist
  radioButtonTopMiddle->setCheckable( false );
  radioButtonTopMiddle->setChecked( false );
  radioButtonTopMiddle->setCheckable ( true );

  radioButtonRightMiddle->setCheckable ( false );
  radioButtonRightMiddle->setChecked( false );
  radioButtonRightMiddle->setCheckable ( true );

  radioButtonBottomMiddle->setCheckable ( false );
  radioButtonBottomMiddle->setChecked( false );
  radioButtonBottomMiddle->setCheckable ( true );
 
  radioButtonLeftMiddle->setCheckable ( false );
  radioButtonLeftMiddle->setChecked( false );
  radioButtonLeftMiddle->setCheckable ( true );
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
    comboBox->setEnabled( true );
    mirrorCheckBox->setEnabled( false );
    rotateFrame->setEnabled( false );
    return;
  }
 
  QString index = comboBox->currentText().left( 2 ).right( 1 );
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
    
    QvkWebcamBusyDialog *WebcamBusyDialog = new QvkWebcamBusyDialog( comboBox->currentText().remove( 0, 4 ), "/dev/video" + index, usedBy );
    (void) WebcamBusyDialog;
  
    qDebug() << "[vokoscreen] webcam device /dev/video" + index + " " + comboBox->currentText().remove( 0, 4 ) + " is busy by " + usedBy;
    checkBox->setCheckState( Qt::CheckState( Qt::Unchecked ) );
    return;
  }
  
  if ( value == true )
  {
    comboBox->setEnabled( false );
    mirrorCheckBox->setEnabled( true );
    rotateFrame->setEnabled( true );
    webcamWindow->show();
    webcamWindow->currentDevice = "/dev/video" + index;
    captureThread->start( "/dev/video" + index );
    if ( not vkSettings.getWebcamBorder( ) ) 
      webcamWindow->actionBorder->trigger();
  }
}


void QvkWebcamController::setNewImage( QImage image )
{
  if ( mirrored == true )
    image = image.mirrored ( true, false );

  if (radioButtonLeftMiddle->isChecked() == true )
      rotateDial->setValue( 90 );
  
  if ( radioButtonTopMiddle->isChecked() == true )
    rotateDial->setValue( 180 );

  if ( radioButtonRightMiddle->isChecked() == true )
    rotateDial->setValue( 270 );
  
  if ( radioButtonBottomMiddle->isChecked() == true )
    rotateDial->setValue( 360 );


  QTransform transform;
  transform.rotate( rotateDial->value() );
  QImage transformedImage = image.transformed( transform );
  
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
  comboBox->clear();
  for( int x = 0; x < myWebcamWatcher->getWebcamCount(); x++ )
    comboBox->addItem( "[" + deviceList[x].right( 1 ) + "]" + " " + captureThread->getNameFromDevice( "/dev/video" + deviceList[x].right( 1 ) ) );

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
