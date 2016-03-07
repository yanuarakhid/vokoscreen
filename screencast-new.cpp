/* vokoscreen - A desktop recorder
 * Copyright (C) 2011-2014 Volker Kohaupt
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301 USA 
 */

#include "screencast.h"  
#include "QvkFormatsAndCodecs.h"
#include "QvkCountdown.h"
#include "QvkPulse.h"

using namespace std;

#ifdef QT5
  #include "QvkLogListWidget.h"
  #include "log/QvkLog.h"
  #include <QPointer>

  QPointer<QvkLog> myLog;

  void myMessageOutput( QtMsgType type, const QMessageLogContext &context, const QString &msg )
  {
    myLog->outputMessage( type, context, msg );
  }
#endif


screencast::screencast()
{
    vkSettings.readAll();
    
    myUi.setupUi( this );
    myUi.ListWidgetLogVokoscreen->setVisible( false );

#ifdef QT5
    // http://qt-project.org/doc/qt-5/qtglobal.html#qInstallMessageHandler
    myLog = new QvkLog();
    qInstallMessageHandler( myMessageOutput );
    connect( myLog, SIGNAL( newLogText( QString ) ), this, SLOT( addLogVokoscreen( QString ) ) );
#endif
    
    oldMainWindowHeight = height();

    homepage = "<a href='http://www.kohaupt-online.de/hp'>" + tr( "Homepage" ) + "</a>";
    
    email = "<a href ='mailto:tux@kohaupt-online.de?subject=vokoscreen ";
    email = email.append( vkSettings.getVersion() ).append( "'" ).append( ">" + tr( "Support" ) + "</a>" );

    QString emaildeveloper = "<a href ='mailto:vkohaupt@freenet.de?subject=vokoscreen ";
    emaildeveloper = emaildeveloper.append( vkSettings.getVersion() ).append( "'" ).append( ">" + tr( "Developer" ) + "</a>" );
    
    screencast::setWindowTitle( vkSettings.getProgName() + " " + vkSettings.getVersion() );

    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/vokoscreen.png" ), QSize(), QIcon::Normal, QIcon::Off );
    screencast::setWindowIcon( icon );    

    qDebug() << "[vokoscreen]" << "Version:" << vkSettings.getVersion();
    qDebug() << "[vokoscreen]" << "Qt version: " << qVersion();
    qDebug() << "[vokoscreen]" << "Operating system:" << getOsRelease();
    qDebug() << "[vokoscreen]" << "Desktop:" << qgetenv( "XDG_CURRENT_DESKTOP" );
    QvkAlsaDevice inBox;
    qDebug() << "[vokoscreen]" << "asoundlib version:" << inBox.getAlsaVersion();
    qDebug() << "[vokoscreen] current icon-theme:" << QIcon::themeName();
    qDebug( " " );

    searchExternalPrograms();

    

    connect( myUi.LogPushButton, SIGNAL( clicked() ), this, SLOT( VisibleHideKonsole() ) );

    // Tab 1 Screen options ***************************************************
    myUi.tabWidget->setTabIcon( 0, QIcon::fromTheme( "video-display", QIcon( ":/pictures/monitor.png" ) ) );
    myUi.updateButton->setIcon( QIcon( ":/pictures/system-software-update.png" ) );

    connect( myUi.FullScreenRadioButton, SIGNAL( clicked() ), SLOT( clickedScreenSize() ) );
    connect( myUi.WindowRadioButton, SIGNAL( clicked() ), SLOT( clickedScreenSize() ) );
    connect( myUi.AreaRadioButton, SIGNAL( clicked() ), SLOT( clickedScreenSize() ) );
    
    QDesktopWidget *desk = QApplication::desktop();
    myScreenCountChanged( desk->screenCount() );
    connect( desk, SIGNAL( screenCountChanged(int) ), SLOT( myScreenCountChanged(int) ) );
    connect( desk, SIGNAL( resized(int) ), SLOT( myScreenCountChanged(int) ) );

    connect( myUi.MagnifierCheckBox, SIGNAL( clicked() ), SLOT( showMagnifier() ) );
    magnifier = new QvkMagnifier();
    magnifier->close();
    connect( magnifier, SIGNAL( closeMagnifier() ), SLOT( uncheckMagnifier() ) );
    connect( myUi.MagnifierDialogPushButton, SIGNAL( clicked() ), magnifier,  SLOT( showDialogMagnifier() ) );

    QvkShowkeyController *showkeyController = new QvkShowkeyController( myUi.ShowkeyQCheckBox );
    (void)showkeyController;
    
    // Begin showclick
    QColor color = Qt::red;
    bool radiant = false;
    double opacity = 0.5;
    ShowClickDialog = new QvkShowClickDialog( color, radiant, opacity );
    connect( myUi.pointerDialogPushButton, SIGNAL( clicked() ), ShowClickDialog, SLOT( show() ) );
  
    animateControl = new QvkAnimateControl( (double) ShowClickDialog->myUiDialog.horizontalSliderShowtime->value()/10,
					    ShowClickDialog->myUiDialog.horizontalSliderCircle->value(),
					    ShowClickDialog->myUiDialog.checkBoxRadiant->checkState(),
					    (double) ShowClickDialog->myUiDialog.horizontalSliderOpacity->value()/100,
					    color );

    connect( myUi.pointerQCheckBox, SIGNAL( clicked( bool ) ), animateControl, SLOT( pointerOnOff( bool ) ) );
  
    connect( ShowClickDialog, SIGNAL( newCircleWidgetValue( int, QColor ) ), animateControl, SLOT( setDiameterColor( int, QColor ) ) );
    connect( ShowClickDialog, SIGNAL( newShowtime( double ) ), animateControl, SLOT( setShowTime( double ) ) );
    connect( ShowClickDialog, SIGNAL( newOpacity( double ) ), animateControl, SLOT( setOpacity( double ) ) );
    connect( ShowClickDialog, SIGNAL( newRadiant( bool ) ), animateControl, SLOT( setRadiant( bool ) ) );
    // End showclick
    
    
    // Tab 2 Audio options ****************************************
    myUi.tabWidget->setTabIcon( 1, QIcon::fromTheme( "audio-input-microphone", QIcon( ":/pictures/micro.png" ) ) );
    connect( myUi.AudioOnOffCheckbox,  SIGNAL( stateChanged( int ) ), SLOT( stateChangedAudio( int ) ) );
    connect( myUi.AudioOnOffCheckbox,  SIGNAL( stateChanged( int ) ), SLOT( AudioOff( int ) ) );
    connect( myUi.AlsaRadioButton,  SIGNAL( clicked( bool )  ), SLOT( clickedAudioAlsa( bool ) ) );
    connect( myUi.PulseDeviceRadioButton,  SIGNAL( clicked( bool )  ), SLOT( clickedAudioPulse( bool ) ) );

    
    // Tab 3 Video options **************************************************
    myUi.tabWidget->setTabIcon( 2, QIcon::fromTheme( "applications-multimedia", QIcon( ":/pictures/videooptionen.png" ) ) );
    
    connect( myUi.VideocodecComboBox, SIGNAL( currentIndexChanged( int ) ), SLOT( currentIndexChangedCodec( int ) ) );
    connect( myUi.VideoContainerComboBox, SIGNAL( currentIndexChanged( int ) ), SLOT( currentIndexChangedFormat( int ) ) );

    myUi.FrameStandardButton->setIcon ( QIcon::fromTheme( "edit-undo", QIcon( ":/pictures/undo.png" ) ) );
    myUi.FrameStandardButton->setToolTip( tr( "Default" ) );
    connect( myUi.FrameStandardButton, SIGNAL( clicked() ), SLOT( setFrameStandardSpinbox() ) );

    connect( myUi.FrameSpinBox, SIGNAL( valueChanged( int ) ), SLOT( valueChangedFrames( int ) ) );// Zeigt Änderungen in Statusbar an

    myUi.VideocodecStandardButton->setIcon ( QIcon::fromTheme( "edit-undo", QIcon( ":/pictures/undo.png" ) ) );
    myUi.VideocodecStandardButton->setToolTip( tr( "Default" ) );
    connect( myUi.VideocodecStandardButton, SIGNAL( clicked() ), SLOT( setVideocodecStandardComboBox() ) );

    myUi.AudiocodecStandardButton->setIcon ( QIcon::fromTheme( "edit-undo", QIcon( ":/pictures/undo.png" ) ) );
    myUi.AudiocodecStandardButton->setToolTip( tr( "Default" ) );
    connect( myUi.AudiocodecStandardButton, SIGNAL( clicked() ), SLOT( setAudiocodecStandardComboBox() ) );


    // Tab 4 Miscellaneous options **************************************************
    myUi.tabWidget->setTabIcon( 3, QIcon::fromTheme( "preferences-system", QIcon( ":/pictures/tools.png" ) ) );
    
    
    // Start Stop Pause etc. Buttons
    myUi.LogPushButton->setIcon ( QIcon::fromTheme( "dialog-information", QIcon( ":/pictures/about.png" ) ) );

    
    
    
    // StatusBar
    statusBarLabelTime = new QLabel();
    statusBarLabelTime->setText( "00:00:00" );
    //statusBarLabelTime->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusBarLabelTime->setToolTip( tr ( "Recording time" ) );

    statusBarLabelFps = new QLabel();
    statusBarLabelFps->setText( "0" );
    //statusBarLabelFps->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusBarLabelFps->setToolTip( tr( "Actual frames per second" ) );

    statusBarLabelSize = new QLabel();
    statusBarLabelSize->setText( "0" );
    //statusBarLabelSize->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusBarLabelSize->setToolTip( tr( "Size in KB" ) );
    
    statusbarLabelScreenSize = new QLabel();
    //statusbarLabelScreenSize->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusbarLabelScreenSize->setToolTip( tr( "Recording screensize" ) );

    statusBarLabelCodec = new QLabel();
    statusBarLabelCodec->setText( myUi.VideocodecComboBox->currentText() );
    //statusBarLabelCodec->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusBarLabelCodec->setToolTip( tr( "Codec" ) );
    
    statusBarLabelFormat = new QLabel();
    statusBarLabelFormat->setText( myUi.VideoContainerComboBox->currentText() );
    //statusBarLabelFormat->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusBarLabelFormat->setToolTip( tr( "Format" ) );

    statusBarLabelAudio = new QLabel();
    //statusBarLabelAudio->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusBarLabelAudio->setToolTip( tr( "Audio" ) );
    
    statusBarLabelFpsSettings = new QLabel();
    //statusBarLabelFpsSettings->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusBarLabelFpsSettings->setToolTip( tr( "Settings fps" ) );

    QLabel * LabelTemp = new QLabel();
    myUi.statusBar->addWidget( LabelTemp, 1 );
    
    myUi.statusBar->addWidget( statusBarLabelTime, 1 );
    myUi.statusBar->addWidget( statusBarLabelFps, 1 );
    myUi.statusBar->addWidget( statusBarLabelSize, 1 );
    myUi.statusBar->addWidget( statusbarLabelScreenSize, 1 );
    myUi.statusBar->addWidget( statusBarLabelCodec, 1 );
    myUi.statusBar->addWidget( statusBarLabelFormat, 1 );
    myUi.statusBar->addWidget( statusBarLabelAudio, 1 );
    myUi.statusBar->addWidget( statusBarLabelFpsSettings, 1 );
    
    searchVideoPlayer();
    searchGIFPlayer();
    
    QvkAlsaWatcher * myAlsaWatcher = new QvkAlsaWatcher();
    connect( myAlsaWatcher, SIGNAL( changed( QStringList ) ), this, SLOT( AlsaWatcherEvent( QStringList ) ) );

    SystemCall = new QProcess( this );
    
    connect( myUi.AudioOnOffCheckbox,     SIGNAL( clicked() ), SLOT( AudioOnOff() ) );
    connect( myUi.AlsaRadioButton,        SIGNAL( clicked() ), SLOT( AudioOnOff() ) );
    connect( myUi.PulseDeviceRadioButton, SIGNAL( clicked() ), SLOT( AudioOnOff() ) );

   qDebug() << "[vokoscreen] ---Begin search video codec---";
   QvkFormatsAndCodecs *formatsAndCodecs = new QvkFormatsAndCodecs( "/usr/bin/ffmpeg" );// RecorderLineEdit->displayText() );*************************************************************************
   QStringList videoCodecList;
   bool experimental = false;
   videoCodecList << "libx264" << "libx265" << "mpeg4" << "huffyuv" << "gif";
   for ( int i = 0; i < videoCodecList.count(); i++ )
   {
     if ( formatsAndCodecs->isCodecAvailable( "Video", videoCodecList[ i ], &experimental ) == true )
     {
       qDebug() << "[vokoscreen] find Videocodec" << videoCodecList[ i ];
       myUi.VideocodecComboBox->addItem( videoCodecList[ i ], experimental );
     }
     else
     {
       qDebug() << "[vokoscreen] not found Videocodec" << videoCodecList[ i ];
     }
   }
   // Fallback
   int x = myUi.VideocodecComboBox->findText( vkSettings.getVideoCodec() );
   if ( x == -1 )
      myUi.VideocodecComboBox->setCurrentIndex( 0 );
   else
      myUi.VideocodecComboBox->setCurrentIndex( x );
   qDebug() << "[vokoscreen] ---End search video codec---";
   qDebug( " " );

   
   qDebug() << "[vokoscreen] ---Begin search audio codec---";
   QStringList audioCodecList;
   audioCodecList << "libmp3lame" << "libvorbis" << "pcm_s16le" << "libvo_aacenc" << "aac";
   for ( int i = 0; i < audioCodecList.count(); i++ )
   {
     if ( formatsAndCodecs->isCodecAvailable( "Audio", audioCodecList[ i ], &experimental ) == true )
     {
       qDebug() << "[vokoscreen] find Audiocodec" << audioCodecList[ i ];
       myUi.AudiocodecComboBox->addItem( audioCodecList[ i ], experimental );
     }
     else
     {
       qDebug() << "[vokoscreen] not found Audiocodec" << audioCodecList[ i ];
     }
   }
   // Fallback
   x = myUi.AudiocodecComboBox->findText( vkSettings.getAudioCodec() );
   if ( x == -1 )
      myUi.AudiocodecComboBox->setCurrentIndex( 0 );
   else
      myUi.AudiocodecComboBox->setCurrentIndex( x );
   qDebug() << "[vokoscreen] ---End search audio codec---";
   qDebug( " " );

   
   qDebug() << "[vokoscreen] ---Begin search formats---";
   QStringList formatList   = ( QStringList() << "mkv"      << "mp4" << "gif" );
   QStringList userDataList = ( QStringList() << "matroska" << "mp4" << "gif" );
   for ( int i = 0; i < formatList.count(); i++ )
   {
     if ( formatsAndCodecs->isFormatAvailable( userDataList[ i ] ) == true )
     {
       qDebug() << "[vokoscreen] find Format" << formatList[ i ];
       myUi.VideoContainerComboBox->addItem( formatList[ i ], userDataList[ i ] );
     }
     else
       qDebug() << "[vokoscreen] not found Format" << formatList[ i ];
   }
   // Fallback
   x = myUi.VideoContainerComboBox->findText( vkSettings.getVideoContainer() );
   if ( x == -1 )
      myUi.VideoContainerComboBox->setCurrentIndex( 0 );
   else
      myUi.VideoContainerComboBox->setCurrentIndex( x );
   qDebug() << "[vokoscreen] ---End search formats---";
   qDebug( " " );
    
}

screencast::~screencast()
{ 
}

#ifndef NO_NEW_VERSION_CHECK
void screencast::buttonVersion()
{
  QString localVersion = vkSettings.getVersion();
  if ( version.isNewVersionAvailable( localVersion, version.getRemoteVersion() ) )
    myUi.updateButton->show();
  else
    myUi.updateButton->hide();
}
#endif


void screencast::addLogVokoscreen( QString value )
{
  myUi.ListWidgetLogVokoscreen->addItem( value );
  myUi.ListWidgetLogVokoscreen->scrollToBottom();
}

void screencast::VisibleHideKonsole()
{
  if ( myUi.ListWidgetLogVokoscreen->isVisible() )
  {
    myUi.ListWidgetLogVokoscreen->setVisible( false );
    resize( width(), oldMainWindowHeight );
  }
  else
  {
    myUi.ListWidgetLogVokoscreen->setVisible( true );
    resize( width(), 600 );
  }
}

#ifdef QT5
#include <QScreen>
#endif 
void screencast::myScreenCountChanged( int newCount )
{
    (void)newCount;
    myUi.ScreenComboBox->clear();
    QDesktopWidget *desk = QApplication::desktop();
    qDebug() << "[vokoscreen]" << "---Begin search Screen---";
    qDebug() << "[vokoscreen]" << "Number of screens:" << desk->screenCount();
    qDebug() << "[vokoscreen] Primary screen is: Display" << desk->primaryScreen()+1;
    qDebug() << "[vokoscreen] VirtualDesktop:" << desk->isVirtualDesktop();
  #ifdef QT5
      //QList < QScreen *> screens = QGuiApplication::screens();
      QScreen *screen = QGuiApplication::primaryScreen();    
      qDebug() << "[vokoscreen] DevicePixelRatio:" << screen->devicePixelRatio() << " (On normal displays is 1 and on Retina is 2)";
  #endif
    
    for ( int i = 1; i < desk->screenCount()+1; i++ )
    {
      QString ScreenGeometryX1 = QString::number( desk->screenGeometry( i-1 ).left() );
      QString ScreenGeometryY1 = QString::number( desk->screenGeometry( i-1 ).top() );
      #ifdef QT4
        QString ScreenGeometryX = QString::number( desk->screenGeometry( i-1 ).width() );
        QString ScreenGeometryY = QString::number( desk->screenGeometry( i-1 ).height() );
      #endif
      #ifdef QT5
        QString ScreenGeometryX = QString::number( desk->screenGeometry( i-1 ).width() * screen->devicePixelRatio() ); // devicePixelRatio() for Retina Displays
        QString ScreenGeometryY = QString::number( desk->screenGeometry( i-1 ).height() * screen->devicePixelRatio() );
      #endif
      myUi.ScreenComboBox->addItem( tr( "Display" ) + " " + QString::number( i ) + ":  " + ScreenGeometryX + " x " + ScreenGeometryY, i-1 );
      qDebug() << "[vokoscreen]" << "Display " + QString::number( i ) + ":  " + ScreenGeometryX + " x " + ScreenGeometryY;
    }
    myUi.ScreenComboBox->addItem( tr( "All Displays" ), -1 );    
    qDebug() << "[vokoscreen]" << "---End search Screen---";
    qDebug( " " );
}

void screencast::showMagnifier()
{
  if ( myUi.MagnifierCheckBox->isChecked() )
    magnifier-> magnifierShow();
  else
    magnifier->close(); 
}


void screencast::uncheckMagnifier()
{
  if ( myUi.MagnifierCheckBox->checkState() == Qt::Checked )
    myUi.MagnifierCheckBox->click();
}


QString screencast::getOsRelease()
{
  QString OS;
  QString ID;
  QString VersionID;
  QString content;
  QFile file("/etc/os-release");

  if ( file.exists() )
  {
    file.open(QIODevice::ReadOnly);
      content = file.readAll().constData();
      QStringList list = content.split( "\n" );
      QStringList listID = list.filter( QRegExp( "^ID=" ) );
      if ( !listID.empty() )
        ID = listID[0].remove(0, 3).replace("\"", ""  );
      
      QStringList listVersionID = list.filter( QRegExp( "^VERSION_ID=" ) );
      if ( !listVersionID.empty() )
	VersionID = listVersionID[0].remove( 0, 11 ).replace("\"", ""  );
    
    OS = ID + " " + VersionID;
      
    file.close();
  }
  else
  {
    OS = "/etc/os-release not found";
  }
 
  return OS;
}


/**
 * Looking for external programs
 */
void screencast::searchExternalPrograms()
{
  qDebug() << "[vokoscreen]" << "---Begin Search external tools---";
  
  if ( searchProgramm( vkSettings.getRecorder() ) )
    qDebug() << "[vokoscreen]" << "Search ffmpeg ..... found" << vkSettings.getRecorder() << "Version:" << getFfmpegVersion();
  else
    qDebug() << "[vokoscreen]" << "Search ffmpeg ..... not found";
  
  if ( searchProgramm("pactl") )
     qDebug() << "[vokoscreen]" << "Search pactl  ..... found Version:" << getPactlVersion();
  else
     qDebug() << "[vokoscreen]" << "Error: pactl is not found, this is an PulseAudio-utils tool. Please install pactl";

  qDebug() << "[vokoscreen]" << "---End search external tools---";
  qDebug( " " );
}


/**
 * Search program foo in PATH
 */
bool screencast::searchProgramm( QString ProgName )
{
    bool find = false;
    QString prog;
    
    // if ProgName with path
    if ( ProgName.contains("/", Qt::CaseInsensitive) and ( QFile::exists( ProgName ) ) )
      return true;
      
    QString resultString( qgetenv( "PATH" ) );
    QStringList pathList = resultString.split( ":" );
      for ( int i = 0; i < pathList.size(); ++i )
      {
        prog = pathList.at( i ) + QDir::separator() + ProgName;
        if ( QFile::exists( prog ) )
        {
          find = true;
          break;
        }
      }
    return find;
}


QString screencast::getFfmpegVersion()
{
  QProcess Process;
  Process.start( vkSettings.getRecorder() + " -version");
  Process.waitForFinished();
  QString ffmpegversion = Process.readAllStandardOutput();
  Process.close();
  
  QStringList list = ffmpegversion.split( "\n" );
  if ( list.empty() )
    ffmpegversion = "";
  else
    ffmpegversion = list[ 0 ];

  return ffmpegversion;
}


QString screencast::getPactlVersion()
{
  QProcess Process;
  Process.start("pactl --version");
  Process.waitForFinished();
  QString pactlVersion = Process.readAllStandardOutput();
  Process.close();

  QStringList list = pactlVersion.split( "\n" );
  list = list[ 0 ].split( " " );
  return list[ 1 ];
}


/*
 * Setzt neues Icon um aufzuzeigen das Audio abgeschaltet ist
 */
void screencast::AudioOff( int state )
{
  if ( state == Qt::Unchecked )  
  {
    QIcon myIcon = myUi.tabWidget->tabIcon( 1 );
    QSize size = myUi.tabWidget->iconSize();
    QPixmap workPixmap( myIcon.pixmap( size ) );
    QPainter painter;
    QPen pen;
    painter.begin( &workPixmap );
      pen.setColor( Qt::red );
      pen.setWidth( 2 );
      painter.setPen( pen );
      painter.drawLine ( 5, 5, size.width()-5, size.height()-5 );
      painter.drawLine ( 5, size.height()-5, size.width()-5, 5 );
    painter.end();
    myUi.tabWidget->setTabIcon( 1, QIcon( workPixmap ) );
  }
  else{
    myUi.tabWidget->setTabIcon( 1, QIcon::fromTheme( "audio-input-microphone", QIcon( ":/pictures/micro.png" ) ) );
  }
}


/**
 * Returns Checkbox from Pulse device
 * 
 */
QCheckBox * screencast::getCheckBoxPulseDevice( int value )
{
//  QList<QCheckBox *> listQFrame = Pulseframe->findChildren<QCheckBox *>();
  QList<QCheckBox *> listQFrame = PulseFrame->findChildren<QCheckBox *>();  
  QCheckBox *inBox;  
  inBox = listQFrame.at( value );
  return inBox;
}


/**
 * CardxList beinhaltet "card0", "card1" ...
 * */
void screencast::AlsaWatcherEvent( QStringList CardxList )
{
  qDebug() << "[vokoscreen] ---Begin search Alsa capture device---";

  myUi.AlsaHwComboBox->clear();
  AlsaDeviceList.clear();
  // Für jede card wird eine Instanz erzeugt und in AlsaDeviceList abgelegt
  for( int i = 0; i <= CardxList.count() - 1; i++ )
  {
    QvkAlsaDevice * alsaDevice = new QvkAlsaDevice( CardxList[ i ] );
    AlsaDeviceList.append( alsaDevice );
    myUi.AlsaHwComboBox->addItem( AlsaDeviceList.at( i )->getAlsaName() , i );
  }

  QSettings settings( vkSettings.getProgName(), vkSettings.getProgName() );
  settings.beginGroup( "Alsa" );
    int x = myUi.AlsaHwComboBox->findText( settings.value( "NameCaptureCard" ).toString(),Qt::MatchExactly );
    myUi.AlsaHwComboBox->setCurrentIndex( x );
  settings.endGroup();
  qDebug() << "[vokoscreen] ---End search Alsa capture device---";
  qDebug( " " );

  settings.beginGroup( "Pulse" );
    PulseMultipleChoice();
    /*for ( int x = 0; x < 10; x++ )
       for ( int i = 0; i < QvkPulse::getPulseInputDevicesCount(); i++ )
       {
          QCheckBox *aa = getCheckBoxPulseDevice( i );
          if ( aa->text() == settings.value( "NameCaptureCard-" + QString::number( x + 1 ) ).toString() )
            aa->setCheckState( Qt::Checked );
       }  */
  settings.endGroup();
}

/**
 * Erstellt eine Scrollarea mit einem Frame
 * in dem die Checkboxen gesetzt werden
 * 
 * In setAccessibleName steht das Pulse Device
 */
void screencast::PulseMultipleChoice()
{
  qDebug() << "[vokoscreen]" << "---Begin search PulseAudio Capture Devices---";
 
  QList<QCheckBox *> listQScrollArea = myUi.scrollAreaWidgetContents->findChildren<QCheckBox *>();
  for ( int i = 0; i < listQScrollArea.count(); ++i )
  {
    delete listQScrollArea[ i ];
  }
  
  for ( int i = 1; i <= QvkPulse::getPulseInputDevicesCount(); ++i )
  {
    namePulse = new QCheckBox();
    myUi.verticalLayout_3->addWidget( namePulse );
    namePulse->setText( QvkPulse::getPulseInputName( i ) );
    namePulse->setAccessibleName( QvkPulse::getPulseInputDevices( i  ) );
    namePulse->setToolTip( tr ( "Select one or more devices" ) );
    qDebug() << "[vokoscreen]" << "Find CaptureCard:" << namePulse->text() << "with device:" << namePulse->accessibleName();
  }

  AudioOnOff();

  qDebug() << "[vokoscreen]" << "---End search PulseAudio Capture Devices---";
  qDebug( " " );
}


void screencast::AudioOnOff()
{
  if ( myUi.AudioOnOffCheckbox->checkState() == Qt::Checked )
  {
    myUi.AlsaRadioButton->setEnabled( true );
    myUi.PulseDeviceRadioButton->setEnabled( true );
    
    if ( myUi.PulseDeviceRadioButton->isChecked() )
      myUi.scrollArea->setEnabled( true );
    else
      myUi.scrollArea->setEnabled( false );
    
    if ( myUi.AlsaRadioButton->isChecked() )
      myUi.AlsaHwComboBox->setEnabled( true );
    else
      myUi.AlsaHwComboBox->setEnabled( false );
    
    myUi.AudiocodecComboBox->setEnabled( true );
  }
  else
  {
    myUi.AlsaRadioButton->setEnabled( false );
    myUi.AlsaHwComboBox->setEnabled( false );
    myUi.scrollArea->setEnabled( false );
    myUi.PulseDeviceRadioButton->setEnabled( false );
    myUi.AudiocodecComboBox->setEnabled( false );
  }
}

/**
 * Statusbar
 */
void screencast::valueChangedFrames( int i ) 
{
  (void)i;
  statusBarLabelFpsSettings->setText( QString::number( myUi.FrameSpinBox->value() ) );
}
  

/**
 * Statusbar
 */
void screencast::stateChangedAudio( int state )
{
  if ( state == Qt::Unchecked )
     statusBarLabelAudio->setText( "off" );

  if ( state == Qt::Checked )
  {
     if ( myUi.AlsaRadioButton->isChecked() )    
       statusBarLabelAudio->setText( "Alsa" );
     
     if ( myUi.PulseDeviceRadioButton->isChecked() )
       statusBarLabelAudio->setText( "Pulse" );
  }
}


/**
 * Statusbar
 */
void screencast::clickedAudioAlsa( bool checked ) 
{
  if ( checked )
     statusBarLabelAudio->setText( "Alsa" );
}


/**
 * Statusbar
 */
void screencast::clickedAudioPulse( bool checked )
{
  if ( checked == true ) 
    statusBarLabelAudio->setText( "Pulse" );
}


/**
 * Statusbar
 */
void screencast::currentIndexChangedCodec( int index )
{
  (void)index;
  statusBarLabelCodec->setText( myUi.VideocodecComboBox->currentText() );

  if ( myUi.VideocodecComboBox->currentText() == "gif" )
  {
    myUi.VideoContainerComboBox->setCurrentIndex( myUi.VideoContainerComboBox->findText( "gif" ) );
    if ( myUi.AudioOnOffCheckbox->checkState() == Qt::Checked )
    {
      myUi.AudioOnOffCheckbox->click();
    }
    
    myUi.AudioOnOffCheckbox->setEnabled( false );
  }

  if ( ( myUi.VideocodecComboBox->currentText() != "gif" ) and ( myUi.VideoContainerComboBox->currentText() != "gif" ) )
  {
    myUi.AudioOnOffCheckbox->setEnabled( true );
  } 
}


/**
 * Statusbar
 */
void screencast::currentIndexChangedFormat( int index )
{
  (void)index;
  statusBarLabelFormat->setText( myUi.VideoContainerComboBox->currentText() );
 
  if ( myUi.VideoContainerComboBox->currentText() == "gif" )
  {
    myUi.GIFplayerComboBox->show();
    myUi.VideoplayerComboBox->hide();
    myUi.VideocodecComboBox->setCurrentIndex( myUi.VideocodecComboBox->findText( "gif" ) );
    myUi.VideocodecComboBox->setEnabled( false );
    if ( myUi.AudioOnOffCheckbox->checkState() == Qt::Checked )
    {
      myUi.AudioOnOffCheckbox->click();
    }
    
    myUi.AudioOnOffCheckbox->setEnabled( false );
  }
  else
  {
    myUi.GIFplayerComboBox->hide();
    myUi.VideoplayerComboBox->show();
    myUi.VideocodecComboBox->setEnabled( true );
    if ( myUi.VideocodecComboBox->currentText() == "gif" )
      myUi.VideocodecComboBox->setCurrentIndex( 0 ); 
  }
 
  if ( ( myUi.VideocodecComboBox->currentText() != "gif" ) and ( myUi.VideoContainerComboBox->currentText() != "gif" ) )
  {
    myUi.AudioOnOffCheckbox->setEnabled( true );
  }
}


void screencast::setRecordWidth( QString value )
{
  screenRecordWidth = value; 
}


QString screencast::getRecordWidth()
{
  return screenRecordWidth; 
}

void screencast::setRecordHeight( QString value )
{
  screenRecordHeight = value; 
}


QString screencast::getRecordHeight()
{
  return screenRecordHeight; 
}

/**
 * Statusbar
 */
void screencast::clickedScreenSize()
{
  if ( myUi.FullScreenRadioButton->isChecked() )
  {
    if ( SystemCall->state() == QProcess::Running )
      statusbarLabelScreenSize->setText( "F:" + getRecordWidth() + "x" + getRecordHeight() );
    else
      statusbarLabelScreenSize->setText( "F" );
    
    if ( ( SystemCall->state() == QProcess::Running ) or ( myUi.PauseButton->isChecked() ) )
      myUi.ScreenComboBox->setEnabled( false );
    else
      myUi.ScreenComboBox->setEnabled( true );
  }
  
  if ( myUi.WindowRadioButton->isChecked() )
  {
    if ( SystemCall->state() == QProcess::Running )
      statusbarLabelScreenSize->setText( "W:" + getRecordWidth() + "x" + getRecordHeight() );
    else
      statusbarLabelScreenSize->setText( "W" );
    
    myUi.ScreenComboBox->setEnabled( false );
  }
  
  if ( myUi.AreaRadioButton->isChecked() )
  {
    if ( SystemCall->state() == QProcess::Running )
      statusbarLabelScreenSize->setText( "A:" + getRecordWidth() + "x" + getRecordHeight() );
    else
      statusbarLabelScreenSize->setText( "A" );
    
    myUi.ScreenComboBox->setEnabled( false );
  }
}


/**
 * Set standard video codec and options
 */
void screencast::setVideocodecStandardComboBox()
{
  myUi.VideocodecComboBox->setCurrentIndex( myUi.VideocodecComboBox->findText( "libx264", Qt::MatchExactly ) );
  myUi.VideoContainerComboBox->setCurrentIndex( myUi.VideoContainerComboBox->findText( "mkv", Qt::MatchExactly ) );
}


/**
 * Set standard Audio codec
 */
void screencast::setAudiocodecStandardComboBox()
{
  myUi.AudiocodecComboBox ->setCurrentIndex( myUi.AudiocodecComboBox->findText( "libmp3lame", Qt::MatchExactly )  );
}


/**
 * Set standard fps
 */
void screencast::setFrameStandardSpinbox()
{
  myUi.FrameSpinBox->setValue( 25 );
}


void screencast::searchGIFPlayer()
{
    qDebug() << "[vokoscreen]" << "---Begin search GIFplayer---";
    QStringList GIFList = QStringList()  << "firefox"
                                         << "mpv"
					 << "chromium"
					 << "konqueror";
    for ( int x = 0; x < GIFList.size(); ++x )
    {
      if ( searchProgramm( GIFList[ x ] ) == true )
      {
        qDebug() << "[vokoscreen]" << "Find GIFplayer :" << GIFList[ x ];
	myUi.GIFplayerComboBox->addItem( GIFList.at( x ), GIFList.at( x ) );
      }	
    }
    qDebug() << "[vokoscreen]" << "---End search GIFoplayer---";
    qDebug( " " );
}


void screencast::searchVideoPlayer()
{
    qDebug() << "[vokoscreen]" << "---Begin search Videoplayer---";
    QStringList playerList = QStringList()  << "vlc"
                                            << "kaffeine"
                                            << "gnome-mplayer"
                                            << "totem"
                                            << "pia"
                                            << "xine"
                                            << "gxine"
                                            << "gmplayer"
                                            << "kplayer"
                                            << "smplayer"
                                            << "smplayer2"
                                            << "dragon"
                                            << "banshee"
					    << "openshot"
					    << "kdenlive"
					    << "mpv";

    QString playerName;
    QString resultString( qgetenv( "PATH" ) );
    QStringList pathList = resultString.split( ":" );
    for ( int x = 0; x < playerList.size(); ++x )
     {
       for ( int i = 0; i < pathList.size(); ++i )
       {
         playerName = pathList.at( i );
         playerName = playerName.append( QDir::separator() ).append( playerList.at( x ) );
         if ( QFile::exists( playerName ) )
         {
           qDebug() << "[vokoscreen]" << "Find Videoplayer :" << playerName;
	   QFileInfo playProg( playerName );
	   if ( playProg.fileName() == "kdenlive" )
	     playerName = playerName + " -i";

	   myUi.VideoplayerComboBox->addItem( playerList.at( x ), playerName );
           break;
         }
       }
     }
    qDebug() << "[vokoscreen]" << "---End search Videoplayer---";
    qDebug( " " );
}
