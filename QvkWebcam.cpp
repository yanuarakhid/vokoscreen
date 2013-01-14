#include "QvkWebcam.h"
#include <QDebug>
#include <QProcess>
#include <QLabel>
#include <QFileSystemWatcher>
#include <QDir>
#include <QCheckBox>
#include <QDialog>
#include <QComboBox>
#include <QStringList>

using namespace std;

QvkWebcam::QvkWebcam( QCheckBox *widget )
{
  //ist das Widget im vokoscreen Fenster
  myWidget = widget;
  
  setWindowTitle( tr( "Webcam Settings" ) );
  setModal( true );
  setFixedHeight( 150 );
  setFixedWidth ( 350 );
  
  ProcessCammera1 = new QProcess();
  ProcessCammera2 = new QProcess();
  ProcessCammera3 = new QProcess();

  label1 = new QLabel( this );
  label1->setGeometry( 10, 40, 100, 30 );
  label1->setText( "Webcam 1:" );
  label1->show();
  
  label2 = new QLabel( this );
  label2->setGeometry( 10, 70, 100, 30 );
  label2->setText( "Webcam 2:" );
  label2->show();

  label3 = new QLabel( this );
  label3->setGeometry( 10, 100, 100, 30 );
  label3->setText( "Webcam 3:" );
  label3->show();
  
  webcam1QCheckBox = new QCheckBox( this );
  webcam1QCheckBox->setGeometry( 100, 40, 200, 30 );
  webcam1QCheckBox->show();

  webcam2QCheckBox = new QCheckBox( this );
  webcam2QCheckBox->setGeometry( 100, 70, 200, 30 );
  webcam2QCheckBox->show();
   
  webcam3QCheckBox = new QCheckBox( this );
  webcam3QCheckBox->setGeometry( 100, 100, 200, 30 );
  webcam3QCheckBox->show();
  
  webcam1QComboBox = new QComboBox( this );
  webcam1QComboBox->setGeometry( 220, 40, 120, 30 );
  webcam1QComboBox->show();
  
  webcam2QComboBox = new QComboBox( this );
  webcam2QComboBox->setGeometry( 220, 70, 120, 30 );
  webcam2QComboBox->show();

  webcam3QComboBox = new QComboBox( this );
  webcam3QComboBox->setGeometry( 220, 100, 120, 30 );
  webcam3QComboBox->show();

  
  
  QFileSystemWatcher * fileSystemWatcher = new QFileSystemWatcher();
  fileSystemWatcher->addPath( "/dev/" );
  connect( fileSystemWatcher, SIGNAL( directoryChanged( const QString& ) ), this, SLOT( myfileSystemWatcher( const QString ) ) );
  myfileSystemWatcher( "/dev/" );
  
  connect( ProcessCammera1, SIGNAL( stateChanged ( QProcess::ProcessState) ),this, SLOT( stateChanged1( QProcess::ProcessState ) ) );
  connect( ProcessCammera2, SIGNAL( stateChanged ( QProcess::ProcessState) ),this, SLOT( stateChanged2( QProcess::ProcessState ) ) );
  connect( ProcessCammera3, SIGNAL( stateChanged ( QProcess::ProcessState) ),this, SLOT( stateChanged3( QProcess::ProcessState ) ) );

  connect( webcam1QCheckBox, SIGNAL( clicked() ),this, SLOT( startWebcam1() ) );
  connect( webcam2QCheckBox, SIGNAL( clicked() ),this, SLOT( startWebcam2() ) );
  connect( webcam2QCheckBox, SIGNAL( clicked() ),this, SLOT( startWebcam3() ) );
  
  connect( webcam1QComboBox, SIGNAL( activated( int ) ),this, SLOT( reStartWebcam1( int ) ) );
  connect( webcam2QComboBox, SIGNAL( activated( int ) ),this, SLOT( reStartWebcam2( int ) ) );
  connect( webcam3QComboBox, SIGNAL( activated( int ) ),this, SLOT( reStartWebcam3( int ) ) );

}


QvkWebcam::~QvkWebcam()
{
}


QStringList QvkWebcam::resolution( QString device)
{
    QProcess Process;
    Process.start("/usr/bin/v4l2-ctl --list-formats-ext --device=" + device );
    Process.waitForFinished();
    QString myString = Process.readAllStandardOutput();
    Process.close();

    QStringList list = myString.split( "\n" );
    list = list.filter( "Size:", Qt::CaseInsensitive );  
      
    for ( int i = 0; i < list.count(); i++ )
    {
      list[ i ] = list[ i ].trimmed();
      list[ i ] = list[ i ].section ( " ", 2, 2 );
    }
    
    list.removeDuplicates();

  return list;
}


/**
 * Wird aufgerufen wenn device /dev/video* hinzugefügt oder entfernt wird
 */
void QvkWebcam::myfileSystemWatcher( const QString & path )
{
  QDir dir( path );
  QStringList filters;
  filters << "video*";
  deviceList = dir.entryList( filters, QDir::System, QDir::Time );

  if ( deviceList.isEmpty() )
  {
    ProcessCammera1->terminate();
    myWidget->setCheckState( Qt::Unchecked );
    myWidget->setEnabled( false );
  }
  else
    myWidget->setEnabled( true );

 
  if ( deviceList.count() >= 1 )
  {
    label1->setVisible( true );
    webcam1QCheckBox->setVisible( true );
    webcam1QCheckBox->setText( "/dev/" + deviceList[ 0 ] );
    webcam1QComboBox->setVisible( true );
    webcam1QComboBox->clear();
    webcam1QComboBox->insertItems( 0, resolution( "/dev/" + deviceList[ 0 ] ) );
    webcam1QComboBox->setCurrentIndex( webcam1QComboBox->findText( "320x240" ) );
  }
  else
  {
    label1->setVisible( false );
    webcam1QCheckBox->setVisible( false );
    webcam1QCheckBox->setText( "" );
    webcam1QComboBox->setVisible( false );
  }
    
  if ( deviceList.count() >= 2 )
  {
    label2->setVisible( true );
    webcam2QCheckBox->setVisible( true );
    webcam2QCheckBox->setText( "/dev/" + deviceList[ 1 ] );
    webcam2QComboBox->setVisible( true );
    webcam2QComboBox->clear();
    webcam2QComboBox->insertItems(0, resolution( "/dev/" + deviceList[ 1 ] ) );
    webcam2QComboBox->setCurrentIndex( webcam2QComboBox->findText( "320x240" ) );
  }
  else
  {
    label2->setVisible( false );
    webcam2QCheckBox->setVisible( false );
    webcam2QCheckBox->setText( "" );
    webcam2QComboBox->setVisible( false );
  }

  if ( deviceList.count() > 2 )
  {
    label3->setVisible( true );
    webcam3QCheckBox->setVisible( true );
    webcam3QCheckBox->setText( "/dev/" + deviceList[ 2 ] );
    webcam3QComboBox->setVisible( true );
    webcam3QComboBox->clear();
    webcam3QComboBox->insertItems( 0, resolution( "/dev/" + deviceList[ 2 ] ) );
    webcam3QComboBox->setCurrentIndex( webcam3QComboBox->findText( "320x240" ) );
  }
  else
  {
    label3->setVisible( false );
    webcam3QCheckBox->setVisible( false );
    webcam3QCheckBox->setText( "" );
    webcam3QComboBox->setVisible( false );
  }

  
}

/**
 * Betr: Webcamdialog
 * 
 * Starte alle eingestellten Cammeras
 * 
 * Es sollte aber mindestens eine Camera gestartet werden
 * 
 * Und es sollte nur die Camera gestartet werden die gecheckt ist z.b die zweite
 */
void QvkWebcam::startWebcam()
{
  // Wird genommen wenn nur eine Camera eingesteckt ist
  if ( deviceList.count() == 1 )
  {
    webcam1QCheckBox->setChecked( true );
    startWebcam1();
    return;
  }
  
  // Wenn mehr als eine Kamere angeschlossen aber alle checkboxen Unchecked dann die erste Kamera checked
  if ( ( deviceList.count() > 1 ) and ( webcam1QCheckBox->checkState() == Qt::Unchecked )
                                  and ( webcam2QCheckBox->checkState() == Qt::Unchecked ) 
				  and ( webcam3QCheckBox->checkState() == Qt::Unchecked ) )
  {
    webcam1QCheckBox->setChecked( true );
    startWebcam1();
    return;
  }
    
  if ( webcam1QCheckBox->checkState() == Qt::Checked )
    startWebcam1();
  
  if ( webcam2QCheckBox->checkState() == Qt::Checked )
    startWebcam2();  

  if ( webcam3QCheckBox->checkState() == Qt::Checked )
    startWebcam3();  
}


/**
 * framerate ist fix auf 30 eingestellt, falls das nicht von ffplay unterstützt wird regelt das ffplay automatisch runter
 * */
void QvkWebcam::startWebcam1()
{
  if ( webcam1QCheckBox->checkState() == Qt::Checked )
  {
    QString device;
    device = "/dev/" + deviceList[ 0 ];
    QString resolution = webcam1QComboBox->currentText();
    ProcessCammera1->start( "ffplay -f video4linux2 -framerate 30 -video_size " + resolution + " " + device );
    qDebug() << "[vokoscreen] ffplay resolution:" << resolution << " device:" << device;
  }
  else
    ProcessCammera1->close();
}


void QvkWebcam::startWebcam2()
{
  if ( webcam2QCheckBox->checkState() == Qt::Checked )
  {
    QString device;
    device = "/dev/" + deviceList[ 1 ];
    QString resolution = webcam2QComboBox->currentText();
    ProcessCammera2->start( "ffplay -f video4linux2 -framerate 30 -video_size " + resolution + " " + device );
  }
  else
    ProcessCammera2->close();
}


void QvkWebcam::startWebcam3()
{
  if ( webcam3QCheckBox->checkState() == Qt::Checked )
  {
    QString device;
    device = "/dev/" + deviceList[ 2 ];
    QString resolution = webcam3QComboBox->currentText();
    ProcessCammera3->start( "ffplay -f video4linux2 -framerate 30 -video_size " + resolution + " " + device );
  }
  else
    ProcessCammera3->close();
}


void QvkWebcam::reStartWebcam1( int index )
{
  (void)index;
  ProcessCammera1->close();
  webcam1QCheckBox->setCheckState( Qt::Checked );
  startWebcam1();
}


void QvkWebcam::reStartWebcam2( int index )
{
  (void)index;
  ProcessCammera2->close();
  webcam2QCheckBox->setCheckState( Qt::Checked );
  startWebcam2();
}


void QvkWebcam::reStartWebcam3( int index )
{
  (void)index;
  ProcessCammera3->close();
  webcam3QCheckBox->setCheckState( Qt::Checked );
  startWebcam3();
}


/**
 * Stopt alle Webcams wenn Widget Checkbox im Hauptprogramm unchecked ist.
 */
void QvkWebcam::stopWebcam()
{
   ProcessCammera1->close();
   ProcessCammera2->close();
   ProcessCammera3->close();
}


/**
 * Setzte das Widget(CheckBox) checked im vokoscreen Fenster
 * 
 * Hinweis: Ein abziehen des USb-Kabels löst diesen Event nicht aus, der Prozess läuft weiter.
 */
void QvkWebcam::stateChanged1 ( QProcess::ProcessState newState )
{
  if ( newState == QProcess::Running ) 
    if ( ( webcam1QCheckBox->checkState() == Qt::Checked ) or ( webcam2QCheckBox->checkState() == Qt::Checked ) or ( webcam3QCheckBox->checkState() == Qt::Checked ) )
      myWidget->setCheckState( Qt::Checked );
    
  if ( newState == QProcess::NotRunning )
  {
    webcam1QCheckBox->setCheckState( Qt::Unchecked );
    if ( ( webcam1QCheckBox->checkState() == Qt::Unchecked ) and ( webcam2QCheckBox->checkState() == Qt::Unchecked ) and ( webcam3QCheckBox->checkState() == Qt::Unchecked ) )
      myWidget->setCheckState( Qt::Unchecked );
  }
}


void QvkWebcam::stateChanged2 ( QProcess::ProcessState newState )
{
  if ( newState == QProcess::Running ) 
    if ( ( webcam1QCheckBox->checkState() == Qt::Checked ) or ( webcam2QCheckBox->checkState() == Qt::Checked ) or ( webcam3QCheckBox->checkState() == Qt::Checked ) )
      myWidget->setCheckState( Qt::Checked );

  if ( newState == QProcess::NotRunning ) 
  {
    webcam2QCheckBox->setCheckState( Qt::Unchecked ); 
    if ( ( webcam1QCheckBox->checkState() == Qt::Unchecked ) and ( webcam2QCheckBox->checkState() == Qt::Unchecked ) and ( webcam3QCheckBox->checkState() == Qt::Unchecked ) )
      myWidget->setCheckState( Qt::Unchecked );
  }
}


void QvkWebcam::stateChanged3 ( QProcess::ProcessState newState )
{
  if ( newState == QProcess::Running ) 
    if ( ( webcam1QCheckBox->checkState() == Qt::Checked ) or ( webcam2QCheckBox->checkState() == Qt::Checked ) or ( webcam3QCheckBox->checkState() == Qt::Checked ) )
      myWidget->setCheckState( Qt::Checked );

  if ( newState == QProcess::NotRunning )
  {
    webcam3QCheckBox->setCheckState( Qt::Unchecked ); 
    if ( ( webcam1QCheckBox->checkState() == Qt::Unchecked ) and ( webcam2QCheckBox->checkState() == Qt::Unchecked ) and ( webcam3QCheckBox->checkState() == Qt::Unchecked ) )
      myWidget->setCheckState( Qt::Unchecked );
  }
}


void QvkWebcam::showWebcamDialog()
{
  show();
}
 