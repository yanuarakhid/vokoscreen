/* vokoscreen - A desktop recorder
 * Copyright (C) 2011-2013 Volker Kohaupt
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include "screencast.h"
#include "regionselection.h"

#include "QvkLupe.h"
#include "QvkWebcam.h"
#include "QvkWinInfo.h"
#include "QvkAlsaDevice.h"
#include "QvkAlsaWatcher.h"
#include "QvkMail.h"

#include "qxtwindowsystem.h"
#include <QxtGlobalShortcut> 

#include <QtGui/QLabel>
#include <QtGui/QAction>
#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QImage>
#include <QtGui/QPicture>
#include <QtGui/QTabWidget>
#include <QtGui/QIcon>
#include <QSettings>
#include <QDesktopServices>
#include <QFileInfo>
#include <QtGlobal>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QWidget>
#include <QLineEdit>
#include <QFileDialog>
#include <Qt>
#include <QSpinBox>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QTimer>
#include <QMouseEvent>
#include <QtGui/QScrollArea>
#include <QListWidget>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QCursor>
#include <QDialog>
#include <QTime>
#include <QDate>
#include <QFileSystemWatcher>

#include <QTest>

using namespace std;

screencast::screencast()
{
    bool beta = true;
    QString Beta;
    if ( beta )
      Beta = "Beta 3";
    else
      Beta = "";

    ProgName = "vokoscreen";
    Version = "1.4.14";
    Version = Version + " " + Beta;
    email = "<a href ='mailto:tux@vodafone.de?subject=vokoscreen ";
    email = email.append( Version ).append( "'" ).append( ">tux@vodafone.de</a>" );
    
    QString emaildeveloper = "<a href ='mailto:vkohaupt@freenet.de?subject=vokoscreen ";
    emaildeveloper = emaildeveloper.append( Version ).append( "'" ).append( ">vkohaupt@freenet.de</a>" );
    
    homepage = "<a href='http://www.kohaupt-online.de/hp'>www.kohaupt-online.de/hp</a>";
    if ( beta )
      screencast::setWindowTitle( ProgName + " " + Version );
    else 
      screencast::setWindowTitle( ProgName + " " + Version );
    
    QString mailingliste = "<a href ='mailto:vokoscreen@googlegroups.com'>vokoscreen@googlegroups.com</a>";
    
    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/vokoscreen.png" ), QSize(), QIcon::Normal, QIcon::Off );
    screencast::setWindowIcon( icon );    

    qDebug() << "[vokoscreen]" << "Version:" << Version;
    qDebug() << "[vokoscreen]" << "Qt Version: " << qVersion();
    QvkAlsaDevice inBox ;
    qDebug() << "[vokoscreen]" << "asoundlib Version:" << inBox.getAlsaVersion();
    qDebug() << "[vokoscreen]" << "ffmpeg Version:" << getFfmpegVersion();
    qDebug();
    
    searchExternalProgramms();
    
    pause = false;
    firststartWininfo = false;
    
    QWidget *centralWidget = new QWidget( this );
    centralWidget->setObjectName( QString::fromUtf8( "centralWidget" ) );
    this->setCentralWidget( centralWidget );

    QTabWidget *tabWidget = new QTabWidget( centralWidget );
    tabWidget->setGeometry( 120, 0, 450, 190 );
    tabWidget->setIconSize( QSize( 40, 40 ) );

    // Tab 1 Screenrecord ***************************************************
    QFrame *frame = new QFrame( this );
    frame->setGeometry( 120, 10, 300, 200 );
    frame->show();
    tabWidget->addTab( frame, "" );
    tabWidget->setTabIcon( 0, QIcon( ":/pictures/monitor.png" ) );

    // Tab 2 Audio recording devices ****************************************
    TabWidgetAudioFrame = new QFrame( this );
    TabWidgetAudioFrame->setGeometry( 120, 0, 300, 290 );
    TabWidgetAudioFrame->show();
    tabWidget->addTab( TabWidgetAudioFrame, "" );
    tabWidget->setTabIcon( 1, QIcon( ":/pictures/micro.png" ) );

    AudioOnOffCheckbox = new QCheckBox( TabWidgetAudioFrame );
    AudioOnOffCheckbox->setGeometry( 10, 0, 100, 25 );
    AudioOnOffCheckbox->setText( tr( "Audio" ) );
    AudioOnOffCheckbox->show();

    AlsaRadioButton= new QRadioButton( TabWidgetAudioFrame );
    AlsaRadioButton->setGeometry( 25, 110, 100, 25 );
    AlsaRadioButton->setText( tr( "Alsa" ) );
    AlsaRadioButton->show();
    
    AlsaHwComboBox = new QComboBox( TabWidgetAudioFrame );
    AlsaHwComboBox->setGeometry( 90, 110, 345, 25 );
    AlsaHwComboBox->show();
    
    Pulseframe = new QFrame();

    PulseDeviceRadioButton = new QRadioButton( TabWidgetAudioFrame );
    PulseDeviceRadioButton->setGeometry( 25, 20, 345, 25 );
    PulseDeviceRadioButton->setText( tr( "Pulse" ) );
    PulseDeviceRadioButton->show();

    // Tab 3 Videooptionen **************************************************
    TabWidgetVideoOptionFrame = new QFrame( this );
    TabWidgetVideoOptionFrame->setGeometry( 120, 0, 300, 200 );
    TabWidgetVideoOptionFrame->show();
    tabWidget->addTab( TabWidgetVideoOptionFrame, "" );
    tabWidget->setTabIcon( 2, QIcon( ":/pictures/videooptionen.png" ) );

    QLabel *VideoOptionLabel = new QLabel( TabWidgetVideoOptionFrame );
    VideoOptionLabel->setGeometry( 20, 10, 80, 25 );
    VideoOptionLabel->setText( tr( "Frames:" ) );
    VideoOptionLabel->show();

    FrameSpinBox = new QSpinBox( TabWidgetVideoOptionFrame );
    FrameSpinBox->setGeometry( QRect( 80, 10, 50, 25 ) );
    FrameSpinBox->setMinimum( 1 );
    FrameSpinBox->setMaximum( 100 );
    FrameSpinBox->setSingleStep( 1 );
    FrameSpinBox->setValue( 25 );
    FrameSpinBox->show();
    
    QPushButton *FrameStandardButton = new QPushButton( TabWidgetVideoOptionFrame );
    FrameStandardButton->setText( tr( "Standard" ) );
    FrameStandardButton->setGeometry( 360, 10, 70, 25 );
    FrameStandardButton->show();

    QLabel *VideocodecOptionLabel = new QLabel( TabWidgetVideoOptionFrame );
    VideocodecOptionLabel->setGeometry( 20, 40, 50, 25 );
    VideocodecOptionLabel->setText( tr( "Codec:" ) );
    VideocodecOptionLabel->show();

    VideocodecComboBox = new QComboBox( TabWidgetVideoOptionFrame );
    VideocodecComboBox->setGeometry( 80, 40, 80, 25 );
    VideocodecComboBox->show();
    VideocodecComboBox->addItem( "mpeg4" );
    VideocodecComboBox->addItem( "libx264" );

    QLabel *VideoContainerLabel = new QLabel(TabWidgetVideoOptionFrame );
    VideoContainerLabel->setGeometry( 175, 40, 50, 25 );
    VideoContainerLabel->setText( "Format:" );
    VideoContainerLabel->show();
    
    VideoContainerComboBox = new QComboBox( TabWidgetVideoOptionFrame );
    VideoContainerComboBox->setGeometry( 230, 40, 70, 25 );
    VideoContainerComboBox->addItem( "avi" );
    VideoContainerComboBox->addItem( "mkv" );
    VideoContainerComboBox->show();
    
    QPushButton *VideocodecStandardButton = new QPushButton( TabWidgetVideoOptionFrame );
    VideocodecStandardButton->setText( tr( "Standard" ) );
    VideocodecStandardButton->setGeometry( 360, 40, 70, 25 );
    VideocodecStandardButton->show();
    
    HideMouseCheckbox = new QCheckBox( TabWidgetVideoOptionFrame );
    HideMouseCheckbox->setGeometry( 20, 70, 250, 25 );
    HideMouseCheckbox->setText( tr( "Mouse cursor not recording" ) );
    HideMouseCheckbox->show();

    // Tab 4 Miscellaneous **************************************************
    TabWidgetMiscellaneousFrame = new QFrame(this);
    TabWidgetMiscellaneousFrame->setGeometry(120,0,300,200);
    TabWidgetMiscellaneousFrame->show();
    tabWidget->addTab(TabWidgetMiscellaneousFrame,"");
    tabWidget->setTabIcon(3,QIcon(":/pictures/tools.png"));

    QLabel *SaveVideoPathLabel = new QLabel(TabWidgetMiscellaneousFrame);
    SaveVideoPathLabel->setGeometry(30,30,210,25);
    SaveVideoPathLabel->setText( tr( "Videopath" ) );
    SaveVideoPathLabel->show();
    
    SaveVideoPathLineEdit = new QLineEdit(TabWidgetMiscellaneousFrame);
    SaveVideoPathLineEdit->setGeometry(110,30,210,25);
    SaveVideoPathLineEdit->setReadOnly(true);
    SaveVideoPathLineEdit->show();
    
    QPushButton *SaveVideoPathPushButton = new QPushButton(TabWidgetMiscellaneousFrame);
    SaveVideoPathPushButton->setGeometry(320,30,20,25);
    SaveVideoPathPushButton->setText("...");
    SaveVideoPathPushButton->show();
    
    QLabel *VideoPlayerLabel = new QLabel(TabWidgetMiscellaneousFrame);
    VideoPlayerLabel->setGeometry(30,60,210,25);
    VideoPlayerLabel->setText( tr( "Player" ) );
    VideoPlayerLabel->show();
    
    VideoplayerComboBox = new QComboBox(TabWidgetMiscellaneousFrame);
    VideoplayerComboBox->setGeometry(110,60,210,25);
    VideoplayerComboBox->show();

    MinimizedCheckBox = new QCheckBox( TabWidgetMiscellaneousFrame );
    MinimizedCheckBox->setGeometry( 30, 90, 350, 25 );
    MinimizedCheckBox->setText( tr( "Vokoscreen minimized when recording starts" ) );
    MinimizedCheckBox->show();
    
    sendPushButton = new QPushButton( centralWidget );
    sendPushButton->setGeometry( 490, 200, 70, 30 );
    sendPushButton->setFont( QFont( "Times", 12, QFont::Bold ) );
    sendPushButton->setText( "Send" );
    connect( sendPushButton, SIGNAL( clicked() ), SLOT( send() ) );
    if ( needProgramm( "xdg-email" ) )
      sendPushButton->setEnabled( true );
    else
      sendPushButton->setEnabled( false );
    
     // Tab 6 About *********************************************************
    QFrame *TabWidgetAboutFrame = new QFrame(this);
    TabWidgetAboutFrame->setGeometry( 120, 0, 300, 200 );
    TabWidgetAboutFrame->show();
    tabWidget->addTab( TabWidgetAboutFrame, "" );
    tabWidget->setTabIcon( 4, QIcon( ":/pictures/about.png" ) );
    tabWidget->show();
    
    QLabel* labelAutor = new QLabel( TabWidgetAboutFrame );
    labelAutor->setText( ProgName + " " + Version );
    labelAutor->setGeometry( QRect( 0, 10, 400, 22 ) );
    labelAutor->setAlignment( Qt::AlignCenter );
    labelAutor->show();

    QLabel* labelMailingliste = new QLabel( TabWidgetAboutFrame );
    labelMailingliste->setText( "Mailinglist" );
    labelMailingliste->setGeometry( 70, 32, 400, 22 );
    labelMailingliste->show();

    QLabel* labelMailinglisteMail = new QLabel( TabWidgetAboutFrame );
    labelMailinglisteMail->setText( mailingliste );
    labelMailinglisteMail->setGeometry( 160, 32, 400, 22 );
    labelMailinglisteMail->setOpenExternalLinks( true );
    labelMailinglisteMail->show();

    
    QLabel* labelWeb = new QLabel( TabWidgetAboutFrame );
    labelWeb->setGeometry( QRect( 70, 54, 400, 22 ) );
    labelWeb->setText( "Homepage" );
    labelWeb->show();
    
    QLabel* labelWebSite = new QLabel( TabWidgetAboutFrame );
    labelWebSite->setGeometry( QRect( 160, 54, 400, 22 ) );
    labelWebSite->setText( homepage );
    labelWebSite->setOpenExternalLinks( true );
    labelWebSite->show();
    
    QLabel* labelMailSupport = new QLabel( TabWidgetAboutFrame );
    labelMailSupport->setGeometry( QRect( 70, 76, 400, 22 ) );
    labelMailSupport->setText( "Support" );
    labelMailSupport->show();
 
    QLabel* labelMail = new QLabel( TabWidgetAboutFrame );
    labelMail->setGeometry( QRect( 160, 76, 400, 22 ) );
    labelMail->setText( email );
    //labelMail->setAlignment( Qt::AlignCenter );
    labelMail->setOpenExternalLinks( true );
    labelMail->show();

    QLabel* labelDeveLoper = new QLabel( TabWidgetAboutFrame );
    labelDeveLoper->setText( "Developer" );
    labelDeveLoper->setGeometry( QRect( 70, 98, 400, 22 ) );
    labelDeveLoper->show();

    QLabel* labelDeveLoperMail = new QLabel( TabWidgetAboutFrame );
    labelDeveLoperMail->setText( emaildeveloper );
    labelDeveLoperMail->setGeometry( QRect( 160, 98, 400, 22 ) );
    labelDeveLoperMail->setOpenExternalLinks( true );
    labelDeveLoperMail->show();

    
    // End Tabs *************************************************************

    FullScreenRadioButton = new QRadioButton( frame );
    FullScreenRadioButton->setGeometry( QRect( 20, 15, 120, 21 ) );
    FullScreenRadioButton->setText( tr( "Fullscreen" ) );
    FullScreenRadioButton->setChecked( true );

    WindowRadioButton = new QRadioButton( frame );
    WindowRadioButton->setGeometry(QRect(20, 40, 85, 21));
    WindowRadioButton->setText( tr( "Window" ) );
  
    AreaRadioButton = new QRadioButton( frame );
    AreaRadioButton->setGeometry(QRect( 20, 65, 85, 21) );
    AreaRadioButton->setText( tr( "Area" ) );
    
    recordButton = new QPushButton( centralWidget );
    recordButton->setText( "Start" );
    recordButton->setToolTip( "CTRL+SHIFT+F10" );
    recordButton->setFont( QFont( "Times", 12, QFont::Bold ) );
    recordButton->setGeometry( 210, 200, 70, 30 );
    recordButton->show();
    if ( needProgramm( "ffmpeg" ) )
      recordButton->setEnabled( true );
    else
      recordButton->setEnabled( false );

    recordTimeLabel = new QLabel(frame);
    recordTimeLabel->setText("00:00:00");
    //recordTime->setFont(QFont("Times", 16, QFont::Bold));
    recordTimeLabel->setGeometry(110, 70, 70, 20);
    recordTimeLabel->setAlignment(Qt::AlignCenter);
    recordTimeLabel->show();
    recordTimeLabel->setVisible(false);

    StopButton = new QPushButton( centralWidget );
    StopButton->setText( "Stop" );
    StopButton->setToolTip( "CTRL+SHIFT+F11" );
    StopButton->setFont( QFont( "Times", 12, QFont::Bold ) );
    StopButton->setGeometry( 280, 200, 70, 30 );
    StopButton->setEnabled( false );
    StopButton->show();  
    
    PauseButton = new QPushButton( centralWidget );
    PauseButton->setText( "Pause" );
    PauseButton->setToolTip( "CTRL+SHIFT+F12" );
    PauseButton->setFont( QFont( "Times", 12, QFont::Bold ) );
    PauseButton->setGeometry( 350, 200, 70, 30 );
    PauseButton->setCheckable( true );
    PauseButton->setEnabled( false );
    PauseButton->show();

    PlayButton = new QPushButton( centralWidget );
    PlayButton->setText( "Play" );
    PlayButton->setFont( QFont( "Times", 12, QFont::Bold ) );
    PlayButton->setGeometry( 420, 200, 70, 30 );
    PlayButton->show();

    LupeCheckBox = new QCheckBox( frame );
    LupeCheckBox->setText( tr( "Magnification" ) );
    LupeCheckBox->setGeometry( QRect( 160, 15, 120, 21 ) );
    LupeCheckBox->setToolTip( "CTRL+SHIFT+F9" );
    LupeCheckBox->show();
    
    webcamCheckBox = new QCheckBox( frame );
    webcamCheckBox->setText( tr( "Webcam" ) );
    webcamCheckBox->setGeometry( QRect( 160, 40, 120, 21 ) );

    QPushButton *webcamDialogPushButton = new QPushButton( frame );
    webcamDialogPushButton->setGeometry( 250, 40, 20, 21 );
    webcamDialogPushButton->setText( "..." );
    webcamDialogPushButton->show();
    
    QLabel* label = new QLabel(centralWidget);
    label->setText("");
    label->setGeometry( QRect( 0, 0, 123, 240) );
    label->setAlignment( Qt::AlignCenter );
    label->show();
    QImage* qImage = new QImage( ":/pictures/VokoCola.png" );
    label->setPixmap(QPixmap::fromImage(*qImage, Qt::AutoColor));
    label->setScaledContents(true);
    
    qDebug() << "[vokoscreen]" << "---Begin search Videoplayer---";
    QStringList playerList = QStringList()  << "kaffeine"
                                            << "vlc" 
					    << "gnome-mplayer" 
					    << "ffplay" 
					    << "totem" 
					    << "pia"
					    << "xine"
					    << "gxine"
					    << "gmplayer"
					    << "kplayer"
					    << "smplayer"
					    << "dragon"
					    << "banshee";
    playerList.sort();
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
	  VideoplayerComboBox->addItem( playerList.at( x ), playerName );
	  break;
	}
      }
    }
    qDebug() << "[vokoscreen]" << "---End search Videoplayer---";
    qDebug();

    // Einstellungen aus .conf einlesen
    QSettings settings( ProgName, ProgName );

    // Dient nur zum anlegen des Profils damit ffmpeglog erstellt werden kann
    settings.beginGroup("vokoscreen");
      settings.setValue("Version", Version);
    settings.endGroup();

    settings.beginGroup( "Audio" );
      AudioOnOffCheckbox->setCheckState( Qt::CheckState( settings.value( "AudioOnOff", 2 ).toUInt() ) );
    settings.endGroup();

    settings.beginGroup("Alsa" );
      AlsaRadioButton->setChecked( settings.value( "Alsa", false ).toBool() );
    settings.endGroup();

    settings.beginGroup("Pulse");
      PulseDeviceRadioButton->setChecked( settings.value( "Pulse", true ).toBool() );
    settings.endGroup();

    settings.beginGroup("Record");
      FullScreenRadioButton->setChecked( settings.value( "FullScreen", true ).toBool() );
      WindowRadioButton->setChecked( settings.value( "Window", false ).toBool() );
    settings.endGroup();

    settings.beginGroup( "Miscellaneous" );
      QDir Dir;
      if ( Dir.exists( settings.value( "VideoPath", PathMoviesLocation() ).toString() ) )
        SaveVideoPathLineEdit->setText( settings.value( "VideoPath", PathMoviesLocation() ).toString() );
      else
	PathMoviesLocation();

      int x = VideoplayerComboBox->findText( settings.value( "Videoplayer" ).toString(),Qt::MatchExactly );
      if ( x == -1 )
        VideoplayerComboBox->setCurrentIndex( 0 );
      else
        VideoplayerComboBox->setCurrentIndex( x );
      
      MinimizedCheckBox->setCheckState( Qt::CheckState( settings.value( "Minimized", 0 ).toUInt() ) );
    settings.endGroup();
    
    settings.beginGroup( "Videooptions" );
      FrameSpinBox->setValue( settings.value( "Frames", 25 ).toInt() );
      VideocodecComboBox->setCurrentIndex( VideocodecComboBox->findText( settings.value( "Videocodec", "libx264" ).toString() ) );
      VideoContainerComboBox->setCurrentIndex( VideoContainerComboBox->findText( settings.value( "Format", "avi" ).toString() ) );
      HideMouseCheckbox->setCheckState( Qt::CheckState( settings.value( "HideMouse").toUInt() ) );
    settings.endGroup();

    SystemCall = new QProcess( this );
    
    connect( recordButton, SIGNAL( clicked() ), SLOT( preRecord() ) );
    connect( StopButton,   SIGNAL( clicked() ), SLOT( Stop() ) );
    connect( PauseButton,  SIGNAL( clicked() ), SLOT( Pause() ) );
    connect( PlayButton,   SIGNAL( clicked() ), SLOT( play() ) );

    connect( AudioOnOffCheckbox,     SIGNAL( clicked() ),              SLOT( AudioOnOff() ) );
    connect( AlsaRadioButton,        SIGNAL( clicked() ),              SLOT( AudioOnOff() ) );
    connect( PulseDeviceRadioButton, SIGNAL( clicked() ),              SLOT( AudioOnOff() ) );
    
    connect( SaveVideoPathPushButton, SIGNAL(clicked() ), SLOT( saveVideoPath() ) );

    connect( FrameStandardButton,      SIGNAL( clicked() ), SLOT( setFrameStandardSpinbox() ) );
    connect( VideocodecStandardButton, SIGNAL( clicked() ), SLOT( setVideocodecStandardComboBox() ) );

    connect( SystemCall, SIGNAL( stateChanged ( QProcess::ProcessState) ),this, SLOT( stateChanged( QProcess::ProcessState) ) );
    connect( SystemCall, SIGNAL( error(QProcess::ProcessError) ),         this, SLOT( error( QProcess::ProcessError) ) );
    connect( SystemCall, SIGNAL( readyReadStandardError() ),              this, SLOT( readyReadStandardError() ) );
    connect( SystemCall, SIGNAL( readyReadStandardOutput() ),             this, SLOT( readyReadStandardOutput() ) );
    
    windowMoveTimer = new QTimer( this );
    connect( windowMoveTimer, SIGNAL( timeout() ), this, SLOT( windowMove() ) );

    // Area ein-ausblenden wenn Radiobutton immer wieder angecklickt wird
    connect( AreaRadioButton,       SIGNAL( clicked() ), SLOT( AreaOnOff() ) );
    connect( FullScreenRadioButton, SIGNAL( clicked() ), SLOT( AreaOnOff() ) );
    connect( WindowRadioButton,     SIGNAL( clicked() ), SLOT( AreaOnOff() ) );
    myregionselection = new regionselection( 200, 200, 400, 400, 5 );
    myregionselection->close();
    
    connect( LupeCheckBox, SIGNAL( clicked() ), SLOT( showLupe() ) );
    lupe = new QvkLupe();
    lupe->close();

    webcam = new QvkWebcam( webcamCheckBox, webcamDialogPushButton );
    connect( webcamCheckBox, SIGNAL( clicked() ), this, SLOT( showWebcam() ) );
    connect( webcamDialogPushButton, SIGNAL( clicked() ), this, SLOT( showWebcamDialog() ) );
   
    // Clean vokoscreen temp
    QDir dir( PathTempLocation() );
    QStringList stringList = dir.entryList( QDir::Files, QDir::Time | QDir::Reversed );
    for ( int i = 0; i < stringList.size(); ++i )
      dir.remove( PathTempLocation().append( QDir::separator() ).append( stringList.at( i ) ) );

   makeAsoundrc();
   AudioOnOff();
   pulseUnloadModule();

   // Abfrage einbauen ob Systemtray existiert

   SystemTrayIconGreen = new QSystemTrayIcon( QIcon ( ":/pictures/start.png" ) );
   SystemTrayIconGreen->show();
   SystemTrayIconGreen->setToolTip( tr( "Start" ) );

   SystemTrayIconRed = new QSystemTrayIcon( QIcon ( ":/pictures/stop.png" ) );
   SystemTrayIconRed->hide();
   SystemTrayIconRed->setToolTip( tr( "Stop" ) );
   
   SystemTrayIconYellow = new QSystemTrayIcon( QIcon ( ":/pictures/pause.png" ) );
   SystemTrayIconYellow->hide();
   SystemTrayIconYellow->setToolTip( "Pause" );

   SystemTrayIconBlue = new QSystemTrayIcon( QIcon ( ":/pictures/go.png" ) );
   SystemTrayIconBlue->hide();
   SystemTrayIconBlue->setToolTip( "Go" );

   connect( SystemTrayIconGreen,  SIGNAL( activated ( QSystemTrayIcon::ActivationReason ) ), this, SLOT( SystemTrayStart( QSystemTrayIcon::ActivationReason ) ) );
   connect( SystemTrayIconRed,    SIGNAL( activated ( QSystemTrayIcon::ActivationReason ) ), this, SLOT( SystemTrayStop( QSystemTrayIcon::ActivationReason ) ) );
   connect( SystemTrayIconYellow, SIGNAL( activated ( QSystemTrayIcon::ActivationReason ) ), this, SLOT( SystemTrayPause( QSystemTrayIcon::ActivationReason ) ) );
   connect( SystemTrayIconBlue,   SIGNAL( activated ( QSystemTrayIcon::ActivationReason ) ), this, SLOT( SystemTrayGo( QSystemTrayIcon::ActivationReason ) ) ); 
   
   shortcutLupe = new QxtGlobalShortcut( this );
   connect( shortcutLupe, SIGNAL( activated() ), this, SLOT( ShortcutLupe() ) );
   shortcutLupe->setShortcut( QKeySequence( "Ctrl+Shift+F9" ) );

   shortcutStart = new QxtGlobalShortcut( this );
   connect( shortcutStart, SIGNAL( activated() ), this, SLOT( preRecord() ) );// record()
   shortcutStart->setShortcut( QKeySequence( "Ctrl+Shift+F10" ) );
   
   shortcutStop = new QxtGlobalShortcut( this );
   connect( shortcutStop, SIGNAL( activated() ), this, SLOT( Stop() ) );
   shortcutStop->setShortcut( QKeySequence( "Ctrl+Shift+F11" ) );
   shortcutStop->setEnabled( false );
   
   shortcutPause = new QxtGlobalShortcut( this );
   connect( shortcutPause, SIGNAL( activated() ), this, SLOT( ShortcutPause() ) );
   shortcutPause->setShortcut( QKeySequence( "Ctrl+Shift+F12" ) );
   
   QvkAlsaWatcher * myAlsaWatcher = new QvkAlsaWatcher();
   connect( myAlsaWatcher, SIGNAL( changed( QStringList ) ), this, SLOT( AlsaWatcherEvent( QStringList ) ) );

   QFileSystemWatcher * VideoFileSystemWatcher = new QFileSystemWatcher();
   VideoFileSystemWatcher->addPath( SaveVideoPathLineEdit->displayText() );
   connect( VideoFileSystemWatcher, SIGNAL( directoryChanged( const QString& ) ), this, SLOT( myVideoFileSystemWatcher( const QString ) ) );
   myVideoFileSystemWatcher( "" );
 
}


screencast::~screencast()
{
}


void screencast::send()
{
  QvkMail *vkMail = new QvkMail( this );
  (void)vkMail;
}


void screencast::myVideoFileSystemWatcher( const QString & path )
{
  (void)path;
  QDir Dira( PathMoviesLocation() );
  QStringList filters;
  filters << "screencast*";
  QStringList List = Dira.entryList( filters, QDir::Files, QDir::Time );
  
  if ( List.isEmpty() )
    PlayButton->setEnabled( false );
  else
    PlayButton->setEnabled( true );
}



/*
QString boolToStr( bool boo )
{
  return ( ( true == boo ) ? "true" : "false" );
}
*/


/**
 * CardxList beinhaltet "card0", "card1" ...
 * */
void screencast::AlsaWatcherEvent( QStringList CardxList )
{
  qDebug() << "[vokoscreen] ---Begin search Alsa capture device---";

  AlsaHwComboBox->clear();
  AlsaDeviceList.clear();
  // Für jede card wird eine Instanz erzeugt und in AlsaDeviceList abgelegt
  for( int i = 0; i <= CardxList.count() - 1; i++ )
  {
    QvkAlsaDevice * alsaDevice = new QvkAlsaDevice( CardxList[ i ] );
    AlsaDeviceList.append( alsaDevice );
    AlsaHwComboBox->addItem( AlsaDeviceList.at( i )->getAlsaName() , i );
  }

  QSettings settings( ProgName, ProgName );
  settings.beginGroup("Alsa" );
    int x = AlsaHwComboBox->findText( settings.value( "NameCaptureCard" ).toString(),Qt::MatchExactly );
    AlsaHwComboBox->setCurrentIndex( x );
  settings.endGroup();
  qDebug() << "[vokoscreen] ---End search Alsa capture device---";
  qDebug();

  settings.beginGroup( "Pulse" );
    PulseMultipleChoice();
    for ( int x = 0; x < 10; x++ )
       for ( int i = 0; i < getPulseInputDevicesCount(); i++ )
       {
          QCheckBox *aa = getCheckBoxPulseDevice( i );
          if ( aa->text() == settings.value( "NameCaptureCard-" + QString::number( x + 1 ) ).toString() )
            aa->setCheckState( Qt::Checked );
       }  
  settings.endGroup();
}


void screencast::WindowMinimized()
{
  setWindowState( Qt::WindowMinimized );
}


/**
 * Wird beim beenden von vokoscreen aufgerufen
 */
void screencast::closeEvent( QCloseEvent * event )
{
  (void)event;
  Stop();
  saveSettings();
  myregionselection->close();
  lupe->close();
}


void screencast::saveSettings()
{
  QSettings settings(ProgName, ProgName);
  
  settings.clear();

  settings.beginGroup( "vokoscreen" );
    settings.setValue( "Version", Version );
  settings.endGroup();

  settings.beginGroup( "Audio" );
    settings.setValue( "AudioOnOff", AudioOnOffCheckbox->checkState() );
  settings.endGroup();

  settings.beginGroup( "Alsa" );
    settings.setValue( "Alsa", AlsaRadioButton->isChecked() );
    settings.setValue( "NameCaptureCard", AlsaHwComboBox->currentText() );
  settings.endGroup();

  settings.beginGroup( "Pulse" );
    settings.setValue( "Pulse", PulseDeviceRadioButton->isChecked() );
    for ( int i = 1; i < getCountCheckedPulseDevices() + 1; i++ )
      settings.setValue( "NameCaptureCard-" + QString::number( i ), getPulseDeviceName( i ) );
  settings.endGroup();

  settings.beginGroup( "Record" );
    settings.setValue( "FullScreen", FullScreenRadioButton->isChecked() );
    settings.setValue( "Window", WindowRadioButton->isChecked() );
  settings.endGroup();

  settings.beginGroup( "Miscellaneous" );
    settings.setValue( "VideoPath", SaveVideoPathLineEdit->displayText() );
    settings.setValue( "Videoplayer", VideoplayerComboBox->currentText() );
    settings.setValue( "Minimized", MinimizedCheckBox->checkState() );
  settings.endGroup();

  settings.beginGroup( "Videooptions" );
    settings.setValue( "Frames", FrameSpinBox->value() );
    settings.setValue( "Videocodec", VideocodecComboBox->currentText() );
    settings.setValue( "Format", VideoContainerComboBox->currentText() );
    settings.setValue( "HideMouse", HideMouseCheckbox->checkState() );    
  settings.endGroup();
}


void screencast::ShortcutLupe()
{
  LupeCheckBox->click();
}


void screencast::ShortcutPause()
{
  PauseButton->click();
}


void screencast::SystemTrayStart( QSystemTrayIcon::ActivationReason reason )
{
  if ( reason == QSystemTrayIcon::Trigger )
  {
    SystemTrayIconGreen->hide();
    recordButton->click();
  }
}


void screencast::SystemTrayStop( QSystemTrayIcon::ActivationReason reason )
{
  if ( reason == QSystemTrayIcon::Trigger )
    StopButton->click();
}


void screencast::SystemTrayPause( QSystemTrayIcon::ActivationReason reason )
{
  if ( reason == QSystemTrayIcon::Trigger )
    PauseButton->click();
}


void screencast::SystemTrayGo( QSystemTrayIcon::ActivationReason reason )
{
  if ( reason == QSystemTrayIcon::Trigger )
    PauseButton->click();
}


void screencast::showLupe()
{
  if ( LupeCheckBox->isChecked() )
    lupe->show();
  else
    lupe->close(); 
}


void screencast::showWebcam()
{
  if ( webcamCheckBox->isChecked() )
    webcam->startWebcam(); 
  else
    webcam->stopWebcam();
}


void screencast::showWebcamDialog()
{
  webcam->showWebcamDialog();
}


void screencast::AreaOnOff()
{
  if ( FullScreenRadioButton->isChecked() or WindowRadioButton->isChecked() )
    myregionselection->close();

  if ( AreaRadioButton->isChecked() )
  {
    myregionselection->close();
    myregionselection->show();
  }
}


/**
 * Looking for external programs
 */
void screencast::searchExternalProgramms()
{
  qDebug() << "[vokoscreen]" << "---Begin Search external tools---";
  if ( needProgramm("ffmpeg") )
     qDebug() << "[vokoscreen]" << "Find ffmpeg";
  else
     qDebug() << "[vokoscreen]" << "Error: ffmpeg is not found, this is an ffmpeg tool. Please install ffmpeg";

  if ( needProgramm("pactl") )
     qDebug() << "[vokoscreen]" << "Find pactl";
  else
     qDebug() << "vokoscreen" << "Error: pactl is not found, this is an PulseAudio-utils tool. Please install pactl";
  
  if ( needProgramm("v4l2-ctl") )
     qDebug() << "[vokoscreen]" << "Find v4l2-ctl";
  else
     qDebug() << "[vokoscreen]" << "Error: v4l2-ctl is not found, this is an v4l-utils tool. Please install v4l2-ctl";
  
  if ( needProgramm("ffplay") )
     qDebug() << "[vokoscreen]" << "Find ffplay";
  else
     qDebug() << "[vokoscreen]" << "Error: ffplay is not found, this is an ffmpeg tool. Please install ffplay";

  if ( needProgramm("mkvmerge") )
     qDebug() << "[vokoscreen]" << "Find mkvmerge";
  else
     qDebug() << "[vokoscreen]" << "Error: mkvmerge is not found, this is an mkvtoolnix tool. Please install mkvmerge";
  
  qDebug() << "[vokoscreen]" << "---End search external tools---";
  qDebug();
}


/**
 * Set standard video codec and options
 */
void screencast::setVideocodecStandardComboBox()
{
  VideocodecComboBox->setCurrentIndex( VideocodecComboBox->findText( "libx264", Qt::MatchExactly ) );
  VideoContainerComboBox->setCurrentIndex( VideoContainerComboBox->findText( "avi", Qt::MatchExactly ) );
}


/**
 * Set standard fps
 */
void screencast::setFrameStandardSpinbox()
{
  FrameSpinBox->setValue( 25 );
}


/**
 * Erstellt ~/.asoundrc wenn nicht im Userhome vorhanden
 * -Die .asoundrc sollte nur erstellt werden wenn der Start von ffmpeg fehlschägt.
 * Hint: Abfrage muß noch eingebaut werden.
 */
void screencast::makeAsoundrc()
{
  QString homeLocation = QDesktopServices::storageLocation( QDesktopServices:: QDesktopServices::HomeLocation );
  QString asoundrc = homeLocation + "/.asoundrc";
  
  qDebug() << "[vokoscreen]" << "---Begin search PulseAudio Plugin---";
  QFile qFile( asoundrc );
  if ( not qFile.exists() )
  {
    qFile.open( QIODevice::WriteOnly );
    qDebug() << "[vokoscreen] File not found .asound for PulseAudio Plugin";
    qDebug() << "[vokoscreen] Create file .asound for PulseAudio Plugin";
    QTextStream out( &qFile );
    out << "# create by vokoscreen " << Version << "\n";
    out << "\n";
    out << "pcm.pulse {\n";
    out << "  type pulse\n";
    out << "}\n";
    out << "\n";
    out << "ctl.pulse {\n";
    out << "  type pulse\n";
    out << "}\n";
    out << "\n";
    qFile.flush();
    qFile.close();
  }
  else
    qDebug() << "[vokoscreen] Found file .asound for PulseAudio Plugin";
  
  qDebug() << "[vokoscreen]" << "---End search PulseAudio Plugin---";
  qDebug();

}


void screencast::saveVideoPath()
{
  QString dir = QFileDialog::getExistingDirectory( this, tr( "Open Directory" ),
                QDesktopServices::storageLocation( QDesktopServices::HomeLocation ), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

  if ( dir > "" )
    SaveVideoPathLineEdit->setText( dir );
}


void screencast::readyReadStandardError()
{
  QString recordTime = SystemCall->readAllStandardError();
  if ( recordTime.contains( "time", Qt::CaseInsensitive ) )
  {
    int x = recordTime.indexOf( "time" );
    recordTime = recordTime.mid( x + 5, 8 );
    recordTimeLabel->setText( recordTime );
  }
}


void screencast::readyReadStandardOutput()
{
//   qDebug() << SystemCall->readAllStandardOutput();
}


/**
 * Je nach Prozessstaus von ffmpeg werden die Widget enabled bzw. disabled
 */
void screencast::stateChanged ( QProcess::ProcessState newState )
{
    if ( ( newState == QProcess::Running ) and ( pause == false) )
    {
      StopButton->setEnabled(true);
      PauseButton->setEnabled(true);
      recordButton->setEnabled(false);
      PlayButton->setEnabled(false);
      FullScreenRadioButton->setEnabled( false );
      WindowRadioButton->setEnabled( false );
      AreaRadioButton->setEnabled( false );
      
      TabWidgetAudioFrame->setEnabled(false);
      TabWidgetMiscellaneousFrame->setEnabled(false);
      TabWidgetVideoOptionFrame->setEnabled( false );

      SystemTrayIconGreen->hide();
      SystemTrayIconYellow->show();      
      SystemTrayIconRed->show();
      SystemTrayIconBlue->hide();
    }
  
    if ((newState == QProcess::NotRunning) and (pause == false))
    {
      StopButton->setEnabled(false);
      PauseButton->setEnabled(false);
      recordButton->setEnabled(true);
      FullScreenRadioButton->setEnabled( true );
      WindowRadioButton->setEnabled( true );
      AreaRadioButton->setEnabled( true );
      
      TabWidgetAudioFrame->setEnabled(true);
      TabWidgetMiscellaneousFrame->setEnabled(true);
      TabWidgetVideoOptionFrame->setEnabled( true );
      
      SystemTrayIconGreen->show();
      SystemTrayIconYellow->hide();
      SystemTrayIconRed->hide();      
      SystemTrayIconBlue->hide();
      
      if ( LupeCheckBox->isChecked() )
	LupeCheckBox->click();
    }
    
    if ((newState == QProcess::NotRunning) and (pause == true) and ( PauseButton->isChecked() ))
    {
      StopButton->setEnabled(false);
      PauseButton->setEnabled(true);
      recordButton->setEnabled(false);
      PlayButton->setEnabled(false);
      FullScreenRadioButton->setEnabled( false );
      WindowRadioButton->setEnabled( false );
      AreaRadioButton->setEnabled( false );
      
      TabWidgetAudioFrame->setEnabled(false);
      TabWidgetMiscellaneousFrame->setEnabled(false);
      TabWidgetVideoOptionFrame->setEnabled( false );

      SystemTrayIconGreen->hide();
      SystemTrayIconRed->hide();
      SystemTrayIconYellow->hide();
      SystemTrayIconBlue->show();
    }

    if ((newState == QProcess::Running) and (pause == true) and ( not PauseButton->isChecked() ))
    {
      StopButton->setEnabled(true);
      PauseButton->setEnabled(true);
      recordButton->setEnabled(false);
      PlayButton->setEnabled(false);
      FullScreenRadioButton->setEnabled( false );
      WindowRadioButton->setEnabled( false );
      AreaRadioButton->setEnabled( false );
      
      TabWidgetAudioFrame->setEnabled(false);
      TabWidgetMiscellaneousFrame->setEnabled(false);
      TabWidgetVideoOptionFrame->setEnabled( false );
      
      SystemTrayIconGreen->hide();
      SystemTrayIconYellow->show();      
      SystemTrayIconRed->show();
      SystemTrayIconBlue->hide();
    }
      
    if ( ( newState == QProcess::NotRunning ) and ( pause == true ) and ( not PauseButton->isChecked() ) )
    {
      StopButton->setEnabled(false);
      PauseButton->setEnabled(false);
      recordButton->setEnabled(true);
      FullScreenRadioButton->setEnabled( true );
      WindowRadioButton->setEnabled( true );
      AreaRadioButton->setEnabled( true );
      
      TabWidgetAudioFrame->setEnabled(true);
      TabWidgetMiscellaneousFrame->setEnabled(true);
      TabWidgetVideoOptionFrame->setEnabled( true );
      
      SystemTrayIconGreen->show();
      SystemTrayIconRed->hide();
      SystemTrayIconYellow->hide();
      SystemTrayIconBlue->hide();
    }
       
    if (newState == QProcess::Running)
    {
      qDebug();
      qDebug() << "[vokoscreen] ffmpeg is running and is ready for reading and writing";
    }
   
    if (newState == QProcess::NotRunning)
    {
      qDebug();
      qDebug() << "[vokoscreen] ffmpeg is not running";
    }
}


void screencast::error( QProcess::ProcessError error )
{
  // Kann mann testen in dem man den ffmpeg aus der Processliste tötet
  if ( error == QProcess::Crashed )
  {
    // crashed Ordner löschen    
    QSettings settings1( ProgName, ProgName );
    QFileInfo settingsPath1( settings1.fileName() );
    QFile file1( settingsPath1.absolutePath() );
    QString workDirectory1 = file1.fileName();
    QDir Dir1( "" );
    Dir1.setCurrent( workDirectory1 );
    if ( Dir1.cd( "crashed" ) )
    {
      QStringList stringList1 = Dir1.entryList( QDir::Files, QDir::NoSort );
      for ( int i = 0; i < stringList1.count(); ++i )
        Dir1.remove( stringList1[ i ] );
    }
    
    // Sichern von ffmpeg.log, vokoscreen.log, vokoscreen.conf im Ordner crashed
    QSettings settings( ProgName, ProgName );
    QFileInfo settingsPath( settings.fileName() );
    QFile file( settingsPath.absolutePath() );
    QString workDirectory = file.fileName();
    QDir Dir( "" );
    Dir.setCurrent( workDirectory );
    Dir.mkdir( "crashed" );
    
    QStringList stringList = Dir.entryList( QDir::Files, QDir::NoSort );
    for ( int i = 0; i < stringList.count(); ++i )
      QFile::copy( workDirectory + QDir::separator() + stringList[ i ], workDirectory + QDir::separator() + "crashed" + QDir::separator() + stringList[ i ]);
    
    QString crashedtext = "[vokoscreen] ffmpeg is crashed on ";
    QDateTime dateTime = QDateTime::currentDateTime();
    QString time = dateTime.toString( "hh:mm" );
    QString day = dateTime.toString( "dddd" );
    qDebug();
    qDebug() << "********************************************************";
    qDebug() << crashedtext + day + ", " + time;
    qDebug() << "********************************************************";
    QMessageBox msgBox;
    msgBox.setIcon( QMessageBox::Critical );
    msgBox.setText( crashedtext + day + ", " + time );
    msgBox.exec();
  }

  // Noch nicht getestet
  if ( error == QProcess::FailedToStart )
  {
    qDebug() << "Der Prozess konnte nicht gestartet werden. Entweder ist das aufgerufene Programm nicht installiert, oder der ffmpeg Aufruf ist Fehlerhaft oder Sie haben nicht über ausreichende Berechtigungen um das Programm aufzurufen.";
    QMessageBox msgBox;
    msgBox.setText("Der Prozess konnte nicht gestartet werden. Entweder ist das aufgerufene Programm nicht installiert, oder der ffmpeg Aufruf ist Fehlerhaft oder Sie haben nicht über ausreichende Berechtigungen um das Programm aufzurufen.");
    msgBox.exec();
  }
}


/**
 * Search program foo in PATH
 */
bool screencast::needProgramm( QString ProgName )
{
    bool find = false;
    QString prog;
    QString resultString( qgetenv( "PATH" ) );
    QStringList pathList = resultString.split( ":" );
      for ( int i = 0; i < pathList.size(); ++i )
      {
        prog = pathList.at( i ) + QDir::separator() + ProgName;
        if ( QFile::exists(prog ) )
        {
          find = true;
          break;
        }
      }
    return find;
}


void screencast::moveWindowPause()
{
  pause = true;
  PauseButton->setChecked( true );
  PauseButton->setText( "Go" );
  SystemCall->terminate();
  SystemCall->waitForFinished();
  pulseUnloadModule();
}


void screencast::moveWindowGo()
{
  PauseButton->setChecked( false );  
  PauseButton->setText( "Pause" );
  startRecord( PathTempLocation() + QDir::separator() + PauseNameInTmpLocation() );
}


void screencast::Pause()
{
  if ( FullScreenRadioButton->isChecked() or AreaRadioButton->isChecked() )
  {
    pause = true;
    if ( PauseButton->isChecked() )
    {
      shortcutStop->setEnabled( false );
      windowMoveTimer->stop();
      PauseButton->setText( "Go" );
      SystemCall->terminate();
      SystemCall->waitForFinished();
      pulseUnloadModule();
    }
    else
    {
      QVariant aa = AlsaHwComboBox->itemData( AlsaHwComboBox->currentIndex() );
      QvkAlsaDevice *inBox = AlsaDeviceList.at( aa.toInt() );
      if ( inBox->isbusy() and AlsaRadioButton->isChecked() )
      {
        QMessageBox msgBox;
        QString message;
        message.append( tr( "Device " ) );
        message.append( inBox->getAlsaHw() );
        message.append( tr( " is busy" ) );
        msgBox.setText( message );
        msgBox.exec();
	PauseButton->click();
        return;
      }
      
      shortcutStop->setEnabled( true );
      PauseButton->setText( "Pause" );
      startRecord( PathTempLocation() + QDir::separator() + PauseNameInTmpLocation() );
    }
  }
  
  
  if ( WindowRadioButton->isChecked() )
  {
    pause = true;
    if ( PauseButton->isChecked() )
    {
      shortcutStop->setEnabled( false );
      windowMoveTimer->stop();
      PauseButton->setText( "Go" );
      SystemCall->terminate();
      SystemCall->waitForFinished();
      pulseUnloadModule();
    }
    else
    {
      QVariant aa = AlsaHwComboBox->itemData( AlsaHwComboBox->currentIndex() );
      QvkAlsaDevice *inBox = AlsaDeviceList.at( aa.toInt() );
      if ( inBox->isbusy() and AlsaRadioButton->isChecked() )
      {
        QMessageBox msgBox;
        QString message;
        message.append( tr( "Device " ) );
        message.append( inBox->getAlsaHw() );
        message.append( tr( " is busy" ) );
        msgBox.setText( message );
        msgBox.exec();
	PauseButton->click();
        return;
      }
      shortcutStop->setEnabled( true );
      PauseButton->setText( "Pause" );
      startRecord( PathTempLocation() + QDir::separator() + PauseNameInTmpLocation() );
      windowMoveTimer->start();
    }
  }
}


/**
 * Play last screencast with Videoplayer
 */
void screencast::play()
{
  
  if ( LupeCheckBox->isChecked() )
	LupeCheckBox->click();
 
  QVariant aa = VideoplayerComboBox->itemData( VideoplayerComboBox->currentIndex() ); // get userdata from ComboBox
  QString player = aa.toString();
  player = player.replace( "\n", "" ); 
  
  QDir Dira( PathMoviesLocation() );
  QStringList filters;
  filters << "screencast*";
  QStringList List = Dira.entryList( filters, QDir::Files, QDir::Time );
  if ( List.isEmpty() )
  {
    QProcess *SystemCall = new QProcess( this );
    SystemCall->start( player );
  }
  else
  {
    QProcess *SystemCall = new QProcess( this );
    SystemCall->start( player + " " + PathMoviesLocation() + QDir::separator() + List.at( 0 ) );
  }
}


/**
 * Versionsnummer von ffmpeg aufbereiten so diese mit "kleiner gleich" bzw. "größer gleich" ausgewertet werden kann
 */
QString screencast::getFfmpegVersion()
{
  QProcess Process;
  Process.start("ffmpeg -version");
  Process.waitForFinished();
  QString ffmpegversion = Process.readAllStandardOutput();
  Process.close();
  
  QStringList list = ffmpegversion.split( "\n" );
  ffmpegversion = list[ 0 ];
  ffmpegversion.replace(QRegExp( "[A-Z a-z ]" ), "" );
 
  // major
  list.clear();
  list = ffmpegversion.split( "." );

  if ( list.count() == 1 )
    list.append( "00" );
    
  if ( list.count() == 2 )
    list.append( "00" );
  
  // major
  QString major;
  if ( list[ 0 ].size() < 2 )
    major = list[ 0 ].prepend( "0" );
  else
    major = list[ 0 ];
  
  //minor
  QString minor;
  if ( list.count() > 1 )
  {
    if ( list[ 1 ].size() < 2 )
      minor = list[ 1 ].prepend( "0" );
    else
      minor = list[ 1 ];
  }
  
  //patch
  QString patch;
  if ( list.count() > 2 )
  {
    if ( list[ 2 ].size() < 2 ) 
     patch = list[ 2 ].prepend( "0" ); 
    else
      patch = list[ 2 ];
  }

  return major + "." + minor + "." + patch;
}


void screencast::windowMove()
{
  QString x = QString::number( QxtWindowSystem::windowGeometry( moveWindowID ).x() );
  QString y = QString::number( QxtWindowSystem::windowGeometry( moveWindowID ).y() );
  
  if ( ( deltaXMove != x ) or ( deltaYMove != y ) )
    if ( SystemCall->state() == QProcess::Running ) 
      moveWindowPause();
  
  if ( ( deltaXMove == x ) and ( deltaYMove == y ) )
    if ( ( SystemCall->state() == QProcess::NotRunning ) )
    {
      QStringList result = ffmpegString.split( ":0." );
      QString str1 = result[ 0 ];
      QString str2 = result[ 1 ];
      result.clear();
      result = str2.split( " " );
      result[ 0 ] = ":0.0+" + x + "," + y;
      str2 = "";
      for ( int i = 0; i < result.count(); i++ )
        str2.append( result.at( i ) + " " );

      ffmpegString = str1 + str2.trimmed() + " ";
      moveWindowGo();
    }

  deltaXMove = x;
  deltaYMove = y; 
}


/**
 * 
 */
int screencast::getPulseInputDevicesCount()
{
  QProcess *Process = new QProcess( this );
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "c");
  Process->setProcessEnvironment(env);
  Process->start( "pactl list" );
  Process->waitForFinished();
  QString output = Process->readAllStandardOutput();
  Process->close();
  delete Process;
  
  QStringList list = output.split( "\n" );

  QStringList result = list.filter( "Source #", Qt::CaseInsensitive );
  return result.count();
}


/**
 * 
 */
QString screencast::getPulseInputName( int value )
{
  QProcess *Process = new QProcess( this );
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "c");
  Process->setProcessEnvironment(env);
  Process->start( "pactl list" );
  Process->waitForFinished();
  QString output = Process->readAllStandardOutput();
  Process->close();
  delete Process;
  
  QStringList list = output.split( "\n" );

  QStringList result = list.filter( "Source #", Qt::CaseInsensitive );
  QString SourceNummber = result[ value - 1 ];
  
  int index = list.indexOf( SourceNummber );
  QString name = list[ index + 3 ];
  QStringList resultList = name.split( ":" );
  name = resultList[ 1 ].trimmed();
  return name;
}


/**
 * Get Pulse Devicname
 */
QString screencast::getPulseInputDevices( int value )
{
  QProcess *Process = new QProcess( this );
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "c");
  Process->setProcessEnvironment(env);
  Process->start( "pactl list" );
  Process->waitForFinished();
  QString output = Process->readAllStandardOutput();
  Process->close();
  delete Process;
 
  QStringList list = output.split( "\n" );

  QStringList result = list.filter( "Source #", Qt::CaseInsensitive );
  QString SourceNummber = result[ value - 1 ];

  int index = list.indexOf( SourceNummber );
  QString device = list[ index + 2 ];
  QStringList resultList = device.split( ":" );
  device = resultList[ 1 ].trimmed();
  return device;
}


/**
 * Get Pulse Samplerate
 */
QString screencast::getPulseSample( int value )
{
  QProcess *Process = new QProcess( this );
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "c");
  Process->setProcessEnvironment(env);
  Process->start( "pactl list" );
  Process->waitForFinished();
  QString output = Process->readAllStandardOutput();
  Process->close();
  delete Process;
  
  QStringList list = output.split( "\n" );
  
  QStringList result = list.filter( "Source #", Qt::CaseInsensitive );
  QString SourceNummber = result[ value - 1 ];
  
  int index = list.indexOf( SourceNummber );
  QString device = list[ index + 5 ];
  QStringList resultList = device.split( ":" );
 
  list.clear();
  QString aa = resultList[ 1 ].trimmed();
  list = aa.split( " " );
  
  result.clear();
  result = list.filter( "Hz", Qt::CaseInsensitive );

  QString sample = result[ 0 ].replace("Hz","");

  return sample;
}


/**
 * Englisch:
 * Return Samplerate from checked Device
 * The lowest rate sampler for the selected device is used
 * 
 * German:
 * Gibt Samplerrate von ausgewählten Geräten zurück.
 * Wenn mehrere Geräte ausgewählt wurden wird die niedrigste Samplerrate herangezogen
 */
QString screencast::mySample()
{
  QString ret = "48000";
  if ( PulseDeviceRadioButton->isChecked() )
  {
    QList<QCheckBox *> listQFrame = Pulseframe->findChildren<QCheckBox *>();
    QCheckBox *inBox;
    QString Sample;

    QStringList list;
    for ( int i = 0; i < listQFrame.count(); i++ )
    {
      inBox = listQFrame.at( i );
      if ( inBox->checkState() == Qt::Checked  )
        list.append( getPulseSample( i + 1 ) );
    }
  
    list.sort();

    if ( not list.empty() )
      ret = list.first();
   }
    
  return ret;
}


void screencast::AudioOnOff()
{
  if ( AudioOnOffCheckbox->checkState() == Qt::Checked )
  {
    AlsaRadioButton->setEnabled( true );
    PulseDeviceRadioButton->setEnabled( true );
    
    if ( PulseDeviceRadioButton->isChecked() )
      Pulseframe->setEnabled( true );
    else
      Pulseframe->setEnabled( false );
    
    if ( AlsaRadioButton->isChecked() )
      AlsaHwComboBox->setEnabled( true );
    else
      AlsaHwComboBox->setEnabled( false );
  }
  else
  {
    AlsaRadioButton->setEnabled( false );
    AlsaHwComboBox->setEnabled( false );
//    PulseRadioButton->setEnabled( false );
    Pulseframe->setEnabled( false );
    PulseDeviceRadioButton->setEnabled( false );
  }
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
  
  QList<QScrollArea *> listQScrollArea = TabWidgetAudioFrame->findChildren<QScrollArea *>();
  
  if ( listQScrollArea.count() > 0 )
  {
    delete scrollAreaPulse;
    listQScrollArea.clear();
  }
  
  if ( listQScrollArea.count() == 0 )
  {
    Pulseframe = new QFrame();
    scrollAreaPulse = new QScrollArea( TabWidgetAudioFrame ); 
    scrollAreaPulse->setWidget( Pulseframe );
    scrollAreaPulse->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    scrollAreaPulse->setGeometry( 90, 20, 345, 80 );
    scrollAreaPulse->show();
    Pulseframe->setGeometry( 90, 20, 320, 100 ); // Mit den letzten Zahlen werden die scrollAreaPulsebalken geregelt // 90, 45, 320, 100
    Pulseframe->show();

    for ( int i = 0; i < getPulseInputDevicesCount(); ++i )
    {
      namePulse = new QCheckBox( Pulseframe );
      namePulse->setGeometry( QRect( 0,  i * 20, 400, 21 ) );
      namePulse->setText( getPulseInputName( i + 1 ) );
      namePulse->setAccessibleName( getPulseInputDevices( i + 1 ) );
      namePulse->setToolTip("Select one or more devices");
      namePulse->show();
      qDebug() << "[vokoscreen]" << "Find CaptureCard:" << namePulse->text() << "with device:" << namePulse->accessibleName();
    }  
  }
  
  AudioOnOff();
  
  qDebug() << "[vokoscreen]" << "---End search PulseAudio Capture Devices---";
  qDebug();
}


QString screencast::PathMoviesLocation()
{
  QString Path;
  if ( SaveVideoPathLineEdit->displayText() > "" )
     Path = SaveVideoPathLineEdit->displayText();
  else
  {
    if ( QDesktopServices::storageLocation(QDesktopServices::MoviesLocation).isEmpty() )
    {
       Path = QDesktopServices::storageLocation(QDesktopServices:: QDesktopServices::HomeLocation);
       SaveVideoPathLineEdit->setText(Path);
    }
    else
    {
      Path = QDesktopServices::storageLocation(QDesktopServices::MoviesLocation);
      SaveVideoPathLineEdit->setText(Path);
    }
  }
  return Path;
}



QString screencast::PathTempLocation()
{
  // Doppelter ProgName um Eindeutigkeit in tmp zuerreichen
  QString tmpName = ProgName + "-" + ProgName;
  QString tempPathProg = QDesktopServices::storageLocation ( QDesktopServices::TempLocation ) + QDir::separator() + tmpName;
  QDir dirTempPathProg( tempPathProg );
  if ( not dirTempPathProg.exists() )
  {
      QString tempPath = QDesktopServices::storageLocation ( QDesktopServices::TempLocation );
      QDir dirTempPath( tempPath );
      dirTempPath.mkdir( tmpName );
  }
  return tempPathProg;  
}


/**
 * Return the new screencastname
 */
QString screencast::NameInMoviesLocation()
{
  QString myFilename = "screencast";
  QString myFilenameExtension = "." + VideoContainerComboBox->currentText();
  QString myName = PathMoviesLocation() + QDir::separator() + myFilename + myFilenameExtension;

  QFile *myFile = new QFile( myName );
  uint x = 0;
  do
  {
    x++;
    myFile->setFileName( PathMoviesLocation() + QDir::separator() + myFilename + "-" + QString().number(x) + myFilenameExtension );
    myName = myFile->fileName();
  } while ( myFile->exists() );

  return myFilename + "-" + QString().number(x) + myFilenameExtension;
}


/**
 * Return the new pausename
 */
QString screencast::PauseNameInTmpLocation()
{
  QString myFilename = "screencast-pause";
  QString myFilenameExtension = "." + VideoContainerComboBox->currentText();
  QString myName = PathTempLocation() + QDir::separator() + myFilename + myFilenameExtension;

  QFile *myFile = new QFile( myName );
  uint x = 0;
  do
  {
    x++;
    myFile->setFileName( PathTempLocation() + QDir::separator() + myFilename + "-" + QString().number( x ) + myFilenameExtension );
    myName = myFile->fileName();
  } while ( myFile->exists() );

  return myFilename + "-" + QString().number( x ) + myFilenameExtension;
}


QString screencast::myAlsa()
{
  QString value;
  if ( AudioOnOffCheckbox->checkState() == Qt::Checked )
  {
    if ( AlsaRadioButton->isChecked() )  
    {
      QVariant aa = AlsaHwComboBox->itemData( AlsaHwComboBox->currentIndex() );
      QvkAlsaDevice *inBox = AlsaDeviceList.at( aa.toInt() );
      if ( AlsaHwComboBox->currentIndex() > -1 )
        value = "-f alsa -ac " + inBox->getChannel() + " -i " + inBox->getAlsaHw() + " ";
      else
        value = "";
    }
    
    if ( PulseDeviceRadioButton->isChecked() )      
    {
      QCheckBox *box;
      int counter = 0;
      QList<QCheckBox *> listQFrame = Pulseframe->findChildren<QCheckBox *>();
      if ( listQFrame.count() > 0 )
        for ( int i = 0; i < listQFrame.count(); i++ )
        {
          box = listQFrame.at( i );
          if ( box->checkState() == Qt::Checked  )
            counter++;
        }

      if ( counter > 0 )
        value = "-f alsa -i pulse ";
      else
	value = "";
    }

  }
  else
      value = "";

  return value;
}


void screencast::pulseUnloadModule()
{
  qDebug() << "";
  qDebug() << "[vokoscreen] ---Begin Pulse unload Module---";
  QProcess Process;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "c");
  Process.setProcessEnvironment(env);
  Process.start( "pactl list" );
  Process.waitForFinished();
  QString output = Process.readAllStandardOutput();
  Process.close();
  
  QStringList list = output.split( "\n" );
  
  QStringList modulNumberList;
  QString str;
  QString strLog;
  for ( int i = 0; i < list.count(); i++ )
  {
     str = list[ i ];
     if ( str.contains("vokoscreenMix", Qt::CaseInsensitive) )
     {
       str = list[ i - 2 ];
       if ( str.contains("Module", Qt::CaseInsensitive) )
       {
	 str.replace("Module #", "");
	 str.replace("\n", "");
	 modulNumberList.insert ( 0, str );
       }
     }
  }
  
  for ( int i = 0; i < modulNumberList.count(); i++ )
  {
    Process.start("pactl unload-module " + modulNumberList[ i ] );
    Process.waitForFinished();
    Process.close();
    qDebug() << "[vokoscreen] pactl unload-module number" << modulNumberList[ i ] ;
  }
  qDebug() << "[vokoscreen] ---End Pulse unload Module---";
  qDebug() << "";
}


/**
 * Returns Checkbox from Pulse device
 * 
 */
QCheckBox * screencast::getCheckBoxPulseDevice( int value )
{
  QList<QCheckBox *> listQFrame = Pulseframe->findChildren<QCheckBox *>();  
  QCheckBox *inBox;  
  inBox = listQFrame.at( value );
  return inBox;
}


/**
 * Returns xte checked pulse devicename
 */
QString screencast::getPulseDeviceName( int value )
{
  QList<QCheckBox *> listQFrame = Pulseframe->findChildren<QCheckBox *>();
  QCheckBox *inBox;
  int x = 1;
  QString name;
  
  for ( int i = 0; i < listQFrame.count(); i++ )
  {
    inBox = listQFrame.at( i );
    if ( inBox->checkState() == Qt::Checked  )
    {
      if  ( x == value )
        name = inBox->text();
      x++;
    }
  }
  return name;
}


/**
 *  Returns count checked pulse devices
 */
int screencast::getCountCheckedPulseDevices()
{
  QList<QCheckBox *> listQFrame = Pulseframe->findChildren<QCheckBox *>();
  QCheckBox *inBox;
  int x = 0;
  
  for ( int i = 0; i < listQFrame.count(); i++ )
  {
    inBox = listQFrame.at( i );
    if ( inBox->checkState() == Qt::Checked  )
      x++;
  }
  return x;
}


/**
 * Return checked Pulse Device
 */
const QString screencast::myPulseDevice()
{
  QList<QCheckBox *> listQFrame = Pulseframe->findChildren<QCheckBox *>();
  QCheckBox *box;
  QList<int> integerList;
  QString ret;
  
  for ( int i = 0; i < listQFrame.count(); i++ )
  {
    box = listQFrame.at( i );
    if ( box->checkState() == Qt::Checked  )
      integerList.append( i );
  }

  if ( integerList.count() == 0 )
    ret = "";

  if ( integerList.count() == 1 )
  {
    box = listQFrame[ integerList[ 0 ] ];
    ret = box->accessibleName();
  }

  if ( integerList.count() > 1 )
    ret = "vokoscreenMix.monitor";
    
  return ret;
}


QString screencast::myAcodec()
{
  QString acodec;
  if ( ( AudioOnOffCheckbox->checkState() == Qt::Checked ) and ( AlsaRadioButton->isChecked() ) and ( AlsaHwComboBox->currentText() > "" ) )
     return " -acodec libmp3lame";

  
  if ( ( AudioOnOffCheckbox->checkState() == Qt::Checked ) and ( PulseDeviceRadioButton->isChecked() ) and ( myPulseDevice() > "" ) )
     return " -acodec libmp3lame";
 
  return "";
}


QString screencast::noMouse()
{
  if ( HideMouseCheckbox->checkState() == Qt::Checked  )
    return "+nomouse";
  else
    return "";
}


void screencast::preRecord()
{
  if ( AlsaRadioButton->isChecked() )
  {
    qDebug() << "*******************************************";
    QVariant aa = AlsaHwComboBox->itemData( AlsaHwComboBox->currentIndex() );
    QvkAlsaDevice *inBox = AlsaDeviceList.at( aa.toInt() );
    if ( inBox->isbusy() )
    {
      QMessageBox msgBox;
      QString message;
      message.append( tr( "Device " ) );
      message.append( inBox->getAlsaHw() );
      message.append( tr( " is busy" ) );
      msgBox.setText( message );
      msgBox.exec();
      return;
    }
    else
    {
      // Kanäle werden kurz vor der Aufnahme ermittelt
      inBox->setChannel();
    }
  }
  
  
/*  
  QVariant aa = AlsaHwComboBox->itemData( AlsaHwComboBox->currentIndex() );
  QvkAlsaDevice *inBox = AlsaDeviceList.at( aa.toInt() );
  if ( inBox->isbusy() and AlsaRadioButton->isChecked() )
  {
    QMessageBox msgBox;
    QString message;
    message.append( tr( "Device " ) );
    message.append( inBox->getAlsaHw() );
    message.append( tr( " is busy" ) );
    msgBox.setText( message );
    msgBox.exec();
    return;
  }
  else
  {
    // Kanäle werden kurz vor der Aufnahme ermittelt
    inBox->setChannel();
  }
*/

  if ( WindowRadioButton->isChecked() )
    if ( firststartWininfo == false )
    {
      vkWinInfo = new QvkWinInfo();
      connect( vkWinInfo, SIGNAL( windowChanged() ), this, SLOT( record() ) );
      return;
    }
   record();
}


void screencast::record()
{
  shortcutStart->setEnabled( false );
  shortcutStop->setEnabled( true );
   
  if ( MinimizedCheckBox->checkState() == Qt::Checked )
    WindowMinimized();
  
  QString RecordX;
  QString RecordY;
  if ( FullScreenRadioButton->isChecked() )
  {
    QDesktopWidget *desk = QApplication::desktop();
    RecordX = tr( "%1" ).arg(desk->screenGeometry().width());
    RecordY = tr( "%1" ).arg(desk->screenGeometry().height());
  }

  QString deltaX = "0";
  QString deltaY = "0";
  if ( WindowRadioButton->isChecked() )
    if ( firststartWininfo == false )
    {
      RecordX = vkWinInfo->width();;
      RecordY = vkWinInfo->height();
      deltaX  = vkWinInfo->x();
      deltaY  = vkWinInfo->y();
      winID   = vkWinInfo->WinID();

      moveWindowID = vkWinInfo->getWinID();
      
      deltaXMove = deltaX;
      deltaYMove = deltaY;

      windowMoveTimer->start( 120 );
      firststartWininfo = true;
    }
  
  if ( AreaRadioButton->isChecked() )
  {
    RecordX = QString().number( myregionselection->getWidth() );
    RecordY = QString().number( myregionselection->getHeight() );
    deltaX  = QString().number( myregionselection->getX() );
    deltaY  = QString().number( myregionselection->getY() );
  }
  
  // -report wird erst ab ffmpeg version 0.9 unterstützt
  QString myReport = "";
  if ( getFfmpegVersion() >= "00.09.00" )
    myReport = "-report ";
  else
    myReport = "";

  qDebug() << "[vokoscreen]" << "Report :" << myReport;
  //qDebug() << "[vokoscreen]" << "ffmpegVersion :" << getFfmpegVersion();

  // set working directory for writing and delete the ffmpegLog from Profil directory
  QSettings settings( ProgName, ProgName );
  QFileInfo settingsPath( settings.fileName() );
  QFile file( settingsPath.absolutePath() );
  QString workDirectory = file.fileName();
  QDir Dir( "" );
  Dir.setCurrent( workDirectory );  
  
  // bestehendes ffmpegLog löschen
  QDir Dira("");
  QStringList filters;
  filters << "ffmpeg*";
  QStringList List = Dira.entryList(filters, QDir::Files, QDir::Name);
  QFile FfmpegFile;
  for (int i = 0; i < List.size(); ++i)
     FfmpegFile.remove( List.at(i) );

  // evtl. müßte hier erst mal nachgeschaut werden ob Temp leer ist.
  // Clean vokoscreen temp 
  QDir dir( PathTempLocation() );
  QStringList stringList = dir.entryList( QDir::Files, QDir::Time | QDir::Reversed );
  for ( int i = 0; i < stringList.size(); ++i )
     dir.remove( PathTempLocation().append(QDir::separator() ).append(stringList.at( i ) ) );

  // frame rate
  QString frame = QString().number( FrameSpinBox->value() );

  // Videocodec
  QString myVcodec = VideocodecComboBox->currentText();
  if ( myVcodec == "libx264" )
  {
    // Number of pixels must be divisible by two
    int intRecordX = RecordX.toInt();
    if ( ( intRecordX % 2 ) == 1 )
      RecordX = QString().number( --intRecordX );

    // Number of pixels must be divisible by two
    int intRecordY = RecordY.toInt();
    if ( ( intRecordY % 2 ) == 1 )
      RecordY = QString().number( --intRecordY );
    
    myVcodec = "libx264 -preset medium";
  }  

  nameInMoviesLocation = NameInMoviesLocation();

  QString quality;
  if ( getFfmpegVersion() < "01.01.00" )
    quality = " -sameq ";
  else
    quality = " -qscale 0 ";

  ffmpegString = "/usr/bin/ffmpeg "
               + myReport
               + myAlsa()
	       + "-f x11grab "
	       //+ "-isync " 
	       + "-r " 
	       + frame
	       + " -s "
	       + RecordX
	       + "x"
	       + RecordY
	       + " -i :0.0+"
	       + deltaX + "," 
	       + deltaY
               + noMouse()
	       + " -vcodec "
	       + myVcodec
       	       + myAcodec()
      	       + " -ar " + mySample()
	       + quality;
	       	            
  startRecord( PathTempLocation() + QDir::separator() + nameInMoviesLocation );
  
  QFile FileVokoscreenLog(settingsPath.absolutePath() + QDir::separator() + ProgName + ".log");
  if (!FileVokoscreenLog.open (QIODevice::WriteOnly)) 
    qDebug() << "Datei konnte nicht angelegt werden: " << FileVokoscreenLog.errorString();
  
  QTextStream stream(&FileVokoscreenLog);
  stream << ProgName << " Version: " << Version << "\n";
  stream << "Record resolution: " << RecordX << "x" << RecordY << "\n";
  stream << "Alsa string: " << myAlsa() << "\n";
  stream << "Qt Version: " << qVersion() << "\n";
  stream << "ffmpeg Version: " << getFfmpegVersion() << "\n";
  stream << "Record String: " << ffmpegString << PathMoviesLocation() << QDir::separator() << nameInMoviesLocation << "\n";
}


void screencast::startRecord( QString RecordPathName )
{
  
 
  qDebug() << "[vokoscreen]"<< "ffmpegcommand :" << ffmpegString + RecordPathName;

  if ( PulseDeviceRadioButton->isChecked() )
  {
    QProcess Process;
    QString value = myPulseDevice();
    if ( value == "vokoscreenMix.monitor" )
    {
      Process.start("pactl load-module module-null-sink sink_name=vokoscreenMix");
      Process.waitForFinished();
      QString modulNumber = Process.readAllStandardOutput();
      Process.close();
      qDebug();
      qDebug() << "[vokoscreen] ---Begin Pulse loade module---";
      modulNumber.replace("\n", "");    
      qDebug() << "[vokoscreen] pactl load-module module-null-sink sink_name=vokoscreenMix " << modulNumber;
    
      QList<QCheckBox *> listQFrame = Pulseframe->findChildren<QCheckBox *>();
      QCheckBox *box;
      QList<int> integerList;
      for ( int i = 0; i < listQFrame.count(); i++ )
      {
        box = listQFrame.at( i );
        if ( box->checkState() == Qt::Checked  )
          integerList.append( i );
      }
      
      for ( int i = 0; i < integerList.count(); i++ )
      {
        box = listQFrame[ integerList[ i ] ];
        Process.start("pactl load-module module-loopback source=" + box->accessibleName() + " sink=vokoscreenMix");
        Process.waitForFinished();
        QString modulNumber = Process.readAllStandardOutput();
        modulNumber.replace("\n", "");
        Process.close();
        qDebug() << "[vokoscreen] pactl load-module module-loopback source=" << box->accessibleName() << "sink=vokoscreenMix number" << modulNumber;
      }
      qDebug() << "[vokoscreen] ---End Pulse loade module---";
      qDebug();
    }
  
    if ( value > "" )
    {
      QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
      env.insert( "PULSE_SOURCE", value );
      SystemCall->setProcessEnvironment( env );
    }
  }
  
  SystemCall->start( ffmpegString + RecordPathName );

}


void screencast::Stop()
{
  shortcutStart->setEnabled( true );
  shortcutStop->setEnabled( false );
  
  if ( SystemCall->state() == QProcess::Running )
  {
    SystemCall->terminate();
    SystemCall->waitForFinished();
  }
  
  if ( pause )
  {
    QString mergeString = "/usr/bin/mkvmerge ";
    QDir dir( PathTempLocation() );
    QStringList stringList = dir.entryList(QDir::Files, QDir::Time | QDir::Reversed);
    for ( int i = 0; i < stringList.size(); ++i )
      if ( i == 0 )    
        mergeString.append( PathTempLocation() ).append( QDir::separator() ).append( stringList.at( i ) );
      else
        mergeString.append( " + " ).append( PathTempLocation() ).append( QDir::separator() ).append( stringList.at( i ) );

    mergeString.append( " -o " ).append( PathMoviesLocation() + QDir::separator() + nameInMoviesLocation );
    SystemCall->start( mergeString );
    SystemCall->waitForFinished();
    for ( int i = 0; i < stringList.size(); ++i )
      dir.remove( PathTempLocation().append( QDir::separator() ).append( stringList.at( i ) ) );

    qDebug() << "[vokoscreen]" << "Mergestring :" << mergeString;
  }
  else
  {
    QString FileInTemp = PathTempLocation() + QDir::separator() + nameInMoviesLocation;
    QFile::copy ( FileInTemp, PathMoviesLocation() + QDir::separator() + nameInMoviesLocation );
    QFile::remove ( FileInTemp );
  }
  
  QDir dir;
  dir.rmdir( PathTempLocation() );
  
  pause = false;
  windowMoveTimer->stop();
  firststartWininfo = false;

  pulseUnloadModule();
  
}

//#include "screencast.moc"
