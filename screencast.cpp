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
#include "QvkRegionController.h"
#include <QClipboard>
#include <QLibraryInfo>
#include <QWidgetAction>
#include <QLibrary>

using namespace std;

  #include "log/QvkLog.h"
  #include <QPointer>
  QPointer<QvkLog> myLog;
  void myMessageOutput( QtMsgType type, const QMessageLogContext &context, const QString &msg )
  {
    myLog->outputMessage( type, context, msg );
  }


screencast::screencast()
{
    vkSettings.readAll();
    
    myUi.setupUi( this );
    myUi.ListWidgetLogVokoscreen->setVisible( false );

    myLog = new QvkLog();
    qInstallMessageHandler( myMessageOutput );
    connect( myLog, SIGNAL( newLogText( QString ) ), this, SLOT( addLogVokoscreen( QString ) ) );

    oldMainWindowHeight = height();

    if ( vkSettings.isVokoscreenWithLibs() == true )
    {
      screencast::setWindowTitle( vkSettings.getProgName() + " " + vkSettings.getVersion() + " " + "RUN-Version");
    }
    else
    {
      screencast::setWindowTitle( vkSettings.getProgName() + " " + vkSettings.getVersion() );
    }

    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/vokoscreen.png" ), QSize(), QIcon::Normal, QIcon::Off );
    screencast::setWindowIcon( icon );    

    qDebug() << "[vokoscreen]" << "Version:" << vkSettings.getVersion();
    qDebug() << "[vokoscreen]" << "Locale:" << QLocale::system().name();
    qDebug() << "[vokoscreen]" << "Qt version: " << qVersion();
    qDebug() << "[vokoscreen]" << "Operating system:" << QSysInfo::prettyProductName();
    qDebug() << "[vokoscreen]" << "Desktop:" << qgetenv( "XDG_CURRENT_DESKTOP" );
    QvkAlsaDevice inBox;
    qDebug() << "[vokoscreen]" << "asoundlib version:" << inBox.getAlsaVersion();
    qDebug() << "[vokoscreen] current icon-theme:" << QIcon::themeName();
    qDebug() << "[vokoscreen] Qt-PluginsPath:     " << QLibraryInfo::location( QLibraryInfo::PluginsPath );
    qDebug() << "[vokoscreen] Qt-TranslationsPath:" << QLibraryInfo::location( QLibraryInfo::TranslationsPath );
    qDebug() << "[vokoscreen] Qt-LibraryPath:     " << QLibraryInfo::location( QLibraryInfo::LibrariesPath );
    
    qDebug( " " );

    searchExternalPrograms();

    pause = false;
    firststartWininfo = false;

    // Tab 1 Screen options ***************************************************
    myUi.tabWidget->setTabIcon( 0, QIcon::fromTheme( "video-display", QIcon( ":/pictures/monitor.png" ) ) );
    makeAndSetValidIcon( 0 );

    connect( myUi.FullScreenRadioButton, SIGNAL( clicked() ), SLOT( clickedScreenSize() ) );
    connect( myUi.WindowRadioButton, SIGNAL( clicked() ), SLOT( clickedScreenSize() ) );
    connect( myUi.AreaRadioButton, SIGNAL( clicked() ), SLOT( clickedScreenSize() ) );
    myUi.areaResetButton->setIcon ( QIcon::fromTheme( "edit-undo", QIcon( ":/pictures/undo.png" ) ) );

    QDesktopWidget *desk = QApplication::desktop();
    myScreenCountChanged( desk->screenCount() );
    connect( desk, SIGNAL( screenCountChanged(int) ), SLOT( myScreenCountChanged(int) ) );
    connect( desk, SIGNAL( resized(int) ), SLOT( myScreenCountChanged(int) ) );

    // Give the Display :0 or :1 etc.
    qDebug() << "[vokoscreen]" << "---Begin Environment---";
    DISPLAY = qgetenv( "DISPLAY" );
    qDebug() << "[vokoscreen] runs on DISPLAY" << DISPLAY;
    qDebug() << "[vokoscreen]" << "---End Environment---";
    qDebug( " " );

    myUi.MagnifierCheckBox->setToolTip( "CTRL+SHIFT+F9" );
    connect( myUi.MagnifierCheckBox, SIGNAL( clicked() ), SLOT( showMagnifier() ) );
    magnifier = new QvkMagnifier();
    magnifier->close();
    connect( magnifier, SIGNAL( closeMagnifier() ), SLOT( uncheckMagnifier() ) );
    connect( myUi.MagnifierDialogPushButton, SIGNAL( clicked() ), magnifier,  SLOT( showDialogMagnifier() ) );

    QvkShowkeyController *showkeyController = new QvkShowkeyController( myUi.ShowkeyCheckBox );
    (void)showkeyController;
    if( Qt::CheckState( vkSettings.getShowKeyOnOff() ) == Qt::Checked )
      myUi.ShowkeyCheckBox->click();
    
    // Begin showclick
    QColor color   = vkSettings.getShowClickColor();
    int radiant    = vkSettings.getShowClickRadiant();
    double opacity = vkSettings.getShowClickOpacity();
    
    ShowClickDialog = new QvkShowClickDialog( color, radiant, opacity );
    connect( myUi.pointerDialogPushButton, SIGNAL( clicked() ), ShowClickDialog, SLOT( show() ) );
  
    animateControl = new QvkAnimateControl( (double) ShowClickDialog->myUiDialog.horizontalSliderShowtime->value()/10,
					    ShowClickDialog->myUiDialog.horizontalSliderCircle->value(),
					    ShowClickDialog->myUiDialog.checkBoxRadiant->checkState(),
					    (double) ShowClickDialog->myUiDialog.horizontalSliderOpacity->value()/100,
					    color );
    
    connect( myUi.pointerCheckBox, SIGNAL( clicked( bool ) ), animateControl, SLOT( pointerOnOff( bool ) ) );
     
    connect( ShowClickDialog, SIGNAL( newCircleWidgetValue( int, QColor ) ), animateControl, SLOT( setDiameterColor( int, QColor ) ) );
    connect( ShowClickDialog, SIGNAL( newShowtime( double ) ), animateControl, SLOT( setShowTime( double ) ) );
    connect( ShowClickDialog, SIGNAL( newOpacity( double ) ), animateControl, SLOT( setOpacity( double ) ) );
    connect( ShowClickDialog, SIGNAL( newRadiant( bool ) ), animateControl, SLOT( setRadiant( bool ) ) );
   // End showclick

    // StatusBar
    statusBarLabelTime = new QLabel();
    statusBarLabelTime->setText( "00:00:00" );
    statusBarLabelTime->setToolTip( tr ( "Recording time" ) );

    statusBarLabelFps = new QLabel();
    statusBarLabelFps->setText( "0" );
    statusBarLabelFps->setAlignment(Qt::AlignCenter);
    statusBarLabelFps->setToolTip( tr( "Actual frames per second" ) );

    statusBarLabelSize = new QLabel();
    statusBarLabelSize->setText( "0" );
    statusBarLabelSize->setToolTip( tr( "Size in KB" ) );
    
    statusbarLabelScreenSize = new QLabel();
    statusbarLabelScreenSize->setToolTip( tr( "Recording screensize" ) );

    statusBarLabelCodec = new QLabel();
    statusBarLabelCodec->setText( myUi.VideocodecComboBox->currentText() );
    statusBarLabelCodec->setToolTip( tr( "Codec" ) );
    
    statusBarLabelFormat = new QLabel();
    statusBarLabelFormat->setText( myUi.VideoContainerComboBox->currentText() );
    statusBarLabelFormat->setToolTip( tr( "Format" ) );

    statusBarLabelAudio = new QLabel();
    statusBarLabelAudio->setToolTip( tr( "Audio" ) );
    
    statusBarLabelFpsSettings = new QLabel();
    statusBarLabelFpsSettings->setToolTip( tr( "Settings fps" ) );

    QLabel * LabelTemp = new QLabel();
    myUi.statusBar->addWidget( LabelTemp, 0 );
    
    myUi.statusBar->addWidget( statusBarLabelTime, 2 );
    myUi.statusBar->addWidget( statusBarLabelFps, 2 );
    myUi.statusBar->addWidget( statusBarLabelSize, 2 );
    myUi.statusBar->addWidget( statusbarLabelScreenSize, 4 );
    myUi.statusBar->addWidget( statusBarLabelCodec, 2 );
    myUi.statusBar->addWidget( statusBarLabelFormat, 2 );
    myUi.statusBar->addWidget( statusBarLabelAudio, 2 );
    myUi.statusBar->addWidget( statusBarLabelFpsSettings, 2 );
    
    
    // Tab 2 Audio options ****************************************
    myUi.tabWidget->setTabIcon( 1, QIcon::fromTheme( "audio-input-microphone", QIcon( ":/pictures/micro.png" ) ) );
    makeAndSetValidIcon( 1 );

    connect( myUi.AudioOnOffCheckbox,  SIGNAL( stateChanged( int ) ), SLOT( stateChangedAudio( int ) ) );
    connect( myUi.AudioOnOffCheckbox,  SIGNAL( stateChanged( int ) ), SLOT( AudioOff( int ) ) );
    
    myUi.labelPulsaudio->setText("");
    myUi.labelPulsaudio->setAlignment( Qt::AlignCenter );
    QImage qImage( ":/pictures/pulseaudio.png" );
    qImage = qImage.scaledToWidth( 40, Qt::SmoothTransformation);
    qImage = qImage.scaledToHeight( 40, Qt::SmoothTransformation);
    //myUi.labelPulsaudio->setPixmap( QPixmap::fromImage( qImage, Qt::AutoColor)  );
    connect( myUi.PulseDeviceRadioButton,  SIGNAL( clicked( bool )  ), SLOT( clickedAudioPulse( bool ) ) );

    myUi.labelAlsa->setText("");
    myUi.labelAlsa->setAlignment( Qt::AlignCenter );
    QImage qImageAlsa(  ":/pictures/alsa.png" );
    qImageAlsa = qImageAlsa.scaledToWidth( 40, Qt::SmoothTransformation);
    qImageAlsa = qImageAlsa.scaledToHeight( 40, Qt::SmoothTransformation);
    //myUi.labelAlsa->setPixmap( QPixmap::fromImage( qImageAlsa, Qt::AutoColor)  );
    connect( myUi.AlsaRadioButton,  SIGNAL( clicked( bool )  ), SLOT( clickedAudioAlsa( bool ) ) );
    
    
    // Tab 3 Video options **************************************************
    myUi.tabWidget->setTabIcon( 2, QIcon::fromTheme( "applications-multimedia", QIcon( ":/pictures/videooptionen.png" ) ) );
    makeAndSetValidIcon( 2 );

    connect( myUi.VideocodecComboBox, SIGNAL( currentIndexChanged( int ) ), SLOT( currentIndexChangedCodec( int ) ) );

    connect( myUi.VideoContainerComboBox, SIGNAL( currentIndexChanged( int ) ),          this, SLOT( currentIndexChangedFormat( int ) ) );
    connect( myUi.VideoContainerComboBox, SIGNAL( currentTextChanged( const QString ) ), this, SLOT( currentFormatChanged( const QString  ) ) );

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
    makeAndSetValidIcon( 3 );
    
    connect( myUi.SaveVideoPathPushButton, SIGNAL(clicked() ), SLOT( saveVideoPath() ) );

    // Leeren Konstruktor aufrufen und dann wird über den Slot recorderLineEditTextChanged die Formate und Codecs ermittelt
    // so bald das Feld RecorderLineEdit geändert wird
    formatsAndCodecs = new QvkFormatsAndCodecs();
    connect( myUi.RecorderLineEdit, SIGNAL( textChanged( QString ) ), SLOT( recorderLineEditTextChanged( QString ) ) );

    if ( vkSettings.isVokoscreenWithLibs() == true )
    {
      myUi.RecorderLineEdit->setText( vkSettings.getRecorder() );
      myUi.selectRecorderPushButton->hide();
    }
    else
    {
      myUi.RecorderLineEdit->setText( getFileWithPath( vkSettings.getRecorder() ) );
      connect( myUi.selectRecorderPushButton, SIGNAL(clicked() ), SLOT( selectRecorder() ) );
    }
    
    myUi.SystrayCheckBox->setCheckState( Qt::Checked );
    connect( myUi.SystrayCheckBox, SIGNAL( stateChanged( int ) ), SLOT( stateChangedSystray( int ) ) );

    connect( myUi.shortcutCheckBox, SIGNAL( stateChanged( int ) ), this, SLOT( stateChangedshortcutsOnOff( int ) ) );
    
    
    // Tab 5 Webcam *******************************************************
    myUi.tabWidget->setTabIcon( 4, QIcon::fromTheme( "camera-web", QIcon( ":/pictures/webcam.png" ) ) );
    makeAndSetValidIcon( 4 );
    myUi.webcamCheckBox->setToolTip( "CTRL+SHIFT+F8" );
    myUi.webcamComboBox->setToolTip( tr ( "Select webcam" ) );
    myUi.mirrorCheckBox->setText( tr( "Mirrored" ) );
    myUi.rotateDial->setWrapping ( true );
    webcamController = new QvkWebcamController( myUi.webcamCheckBox, myUi.webcamComboBox, myUi.mirrorCheckBox, 
						myUi.dialFrame, myUi.rotateDial, myUi.radioButtonTopMiddle,
						myUi.radioButtonRightMiddle, myUi.radioButtonBottomMiddle, myUi.radioButtonLeftMiddle );
    (void)webcamController;

    
    // Tab 6 Extensions
    myUi.tabWidget->setTabIcon( 5, QIcon::fromTheme( "applications-other", QIcon( ":/pictures/extension.png" ) ) );
    makeAndSetValidIcon( 5 );
    

    // Tab 7 About *********************************************************
    myUi.tabWidget->setTabIcon( 6, QIcon::fromTheme( "dialog-information", QIcon( ":/pictures/about.png" ) ) );
    makeAndSetValidIcon( 6 );
    myUi.labelOpensuseBetaUrl->setOpenExternalLinks( true );
    myUi.labelOpensuseBetaUrl->setText( "<a href='http://linuxecke.volkoh.de/vokoscreen/vokoscreen.html'>" + tr( "Developer Homepage" ) + "</a>" );
    
    QString homepage = "<a href='http://www.kohaupt-online.de/hp'>" + tr( "Homepage" ) + "</a>";
    myUi.labelWebSite->setOpenExternalLinks( true );
    myUi.labelWebSite->setText( homepage );
    
    QString email = "<a href='mailto:tux@kohaupt-online.de?subject=vokoscreen ";
    email = email.append( vkSettings.getVersion() ).append( "'" ).append( ">" + tr( "Support" ) + "</a>" );
    myUi.labelMail->setOpenExternalLinks( true );
    myUi.labelMail->setText( email );

    QString emaildeveloper = "<a href ='mailto:vkohaupt@freenet.de?subject=vokoscreen ";
    emaildeveloper = emaildeveloper.append( vkSettings.getVersion() ).append( "'" ).append( ">" + tr( "Developer" ) + "</a>" );
    myUi.labelDeveLoperMail->setOpenExternalLinks( true );
    myUi.labelDeveLoperMail->setText( emaildeveloper );
    
    QString Sourcecode = "<a href='https://github.com/vkohaupt/vokoscreen'>" + tr( "Sourcecode" ) + "</a>";
    myUi.labelSourcecodeUrl->setOpenExternalLinks( true );
    myUi.labelSourcecodeUrl->setText( Sourcecode );
       
    myUi.labelLanguageUrl->setOpenExternalLinks( true );
    myUi.labelLanguageUrl->setText( "<a href='https://www.transifex.com/projects/p/vokoscreen/'>" + tr( "Translations" ) + "</a>" );

    if ( vkSettings.isVokoscreenWithLibs() == true )
    {
      myUi.labelDonateUrl->setOpenExternalLinks( true );
      myUi.labelDonateUrl->setText( "<a href='http://linuxecke.volkoh.de/vokoscreen/vokoscreen-donate.html'>" + tr( "Donate" ) + "</a>" );
    }
    else
    {
      myUi.labelDonateUrl->setOpenExternalLinks( true );
      myUi.labelDonateUrl->setText( "<a href='http://www.kohaupt-online.de/hp/spende.html'>" + tr( "Donate" ) + "</a>" );
    }
    
    QIcon creditsIcon;
    creditsIcon.addFile( ":/pictures/community.png", QSize(), QIcon::Normal, QIcon::On );
    myUi.creditsQPushButton->setIcon( creditsIcon );
    connect( myUi.creditsQPushButton, SIGNAL( clicked() ), SLOT( showCredits() ) );
    
    
    // Start Stop Pause etc. Buttons
    myUi.recordButton->setToolTip( "CTRL+SHIFT+F10" );
    connect( myUi.recordButton, SIGNAL( clicked() ), SLOT( preRecord() ) );
    
    myUi.StopButton->setToolTip( "CTRL+SHIFT+F11" );
    myUi.StopButton->setEnabled( false );
    connect( myUi.StopButton, SIGNAL( clicked() ), SLOT( Stop() ) );

    myUi.PauseButton->setToolTip( "CTRL+SHIFT+F12" );
    myUi.PauseButton->setCheckable( true );
    myUi.PauseButton->setEnabled( false );
    connect( myUi.PauseButton, SIGNAL( clicked() ), SLOT( Pause() ) );

    myUi.PlayButton->setToolTip( tr( "Play last Video" ) );
    connect( myUi.PlayButton, SIGNAL( clicked() ), SLOT( play() ) );
    
    myUi.sendPushButton->setToolTip( tr( "Send Video" ) );
    connect( myUi.sendPushButton, SIGNAL( clicked() ), SLOT( send() ) );
    if ( searchProgramm( "xdg-email" ) )
      myUi.sendPushButton->setEnabled( true );
    else
      myUi.sendPushButton->setEnabled( false );
    
    myUi.LogPushButton->setIcon ( QIcon::fromTheme( "dialog-information", QIcon( ":/pictures/about.png" ) ) );
    connect( myUi.LogPushButton, SIGNAL( clicked() ), this, SLOT( VisibleHideKonsole() ) );
    
    myUi.updateButton->hide();
  #ifndef NO_NEW_VERSION_CHECK
    QTimer::singleShot( 15000, &version, SLOT( doDownload() ) );
    connect( &version, SIGNAL( versionDownloadFinish() ), SLOT( buttonVersion() ) );
    myUi.updateButton->setIcon( QIcon( ":/pictures/system-software-update.png" ) );
    myUi.updateButton->setToolTip( tr( "New version available" ) );
    connect( myUi.updateButton, SIGNAL( clicked() ), SLOT( showHomepage() ) );  
  #endif

    searchVideoPlayer();
    searchGIFPlayer();
    
    // Read Settings
    myUi.AudioOnOffCheckbox->setCheckState( Qt::CheckState( vkSettings.getAudioOnOff() ) );
    AudioOff( Qt::CheckState( vkSettings.getAudioOnOff() ) );

    if ( vkSettings.getAlsaSelect() == true )
    {
       myUi.AlsaRadioButton->setChecked( true ) ;
       clickedAudioAlsa( true );
    }

    if ( vkSettings.getPulseSelect() == true )
    {
       myUi.PulseDeviceRadioButton->setChecked( true ) ;
       clickedAudioPulse( true );
    }
    
    myUi.FullScreenRadioButton->setChecked( vkSettings.getFullScreenSelect() );
    myUi.WindowRadioButton->setChecked( vkSettings.getWindowSelect() );
    // Hint: AreaaRadioButton steht in screencast::showEvent(QShowEvent *event)
    
    if ( vkSettings.getVideoPath() > "" )
        myUi.SaveVideoPathLineEdit->setText( vkSettings.getVideoPath() );
    else
      PathMoviesLocation();

    int x = myUi.VideoplayerComboBox->findText( vkSettings.getVideoPlayer(), Qt::MatchExactly );
    if ( x == -1 )
      myUi.VideoplayerComboBox->setCurrentIndex( 0 );
    else
      myUi.VideoplayerComboBox->setCurrentIndex( x );
      
    x = myUi.GIFplayerComboBox->findText( vkSettings.getGIFPlayer(), Qt::MatchExactly );
    if ( x == -1 )
      myUi.GIFplayerComboBox->setCurrentIndex( 0 );
    else
      myUi.GIFplayerComboBox->setCurrentIndex( x );
      
    myUi.x264LosslessCheckBox->setCheckState( Qt::CheckState( vkSettings.getX264Lossless() )  );
    
    myUi.MinimizedCheckBox->setCheckState( Qt::CheckState( vkSettings.getMinimized() ) );
    
    if ( Qt::CheckState( vkSettings.getMinimizedByStart() ) == Qt::Checked )
    {
      myUi.MinimizedByStartCheckBox->click();
      setWindowState( Qt::WindowMinimized );
    }
    
    myUi.CountdownSpinBox->setValue( vkSettings.getCountdown() );

    myUi.FrameSpinBox->setValue( vkSettings.getFrames() );
    statusBarLabelFpsSettings->setText( QString::number( myUi.FrameSpinBox->value() ) );

    myUi.HideMouseCheckbox->setCheckState( Qt::CheckState( vkSettings.getHideMouse()) );
           
    if ( Qt::CheckState( vkSettings.getWebcamOnOff() ) == Qt::Checked )
      myUi.webcamCheckBox->click();
      
    move( vkSettings.getX(),vkSettings.getY() );

    if( Qt::CheckState( vkSettings.getMagnifierOnOff() ) == Qt::Checked )
      myUi.MagnifierCheckBox->click();
   
    SystemCall = new QProcess( this );
    
    connect( myUi.AudioOnOffCheckbox,     SIGNAL( clicked() ), SLOT( AudioOnOff() ) );
    connect( myUi.AlsaRadioButton,        SIGNAL( clicked() ), SLOT( AudioOnOff() ) );
    connect( myUi.PulseDeviceRadioButton, SIGNAL( clicked() ), SLOT( AudioOnOff() ) );

    connect( SystemCall, SIGNAL( stateChanged ( QProcess::ProcessState) ),this, SLOT( stateChanged( QProcess::ProcessState) ) );
    connect( SystemCall, SIGNAL( error( QProcess::ProcessError) ),        this, SLOT( error( QProcess::ProcessError) ) );
    connect( SystemCall, SIGNAL( readyReadStandardError() ),              this, SLOT( readyReadStandardError() ) );

    windowMoveTimer = new QTimer( this );
    connect( windowMoveTimer, SIGNAL( timeout() ), this, SLOT( windowMove() ) );

    myregionselection = new QvkRegionController();
    connect( myUi.areaResetButton,       SIGNAL( clicked() ), this, SLOT( areaReset() ) );
    
    // Area ein-ausblenden wenn Radiobutton immer wieder angecklickt wird
    connect( myUi.AreaRadioButton,       SIGNAL( clicked() ), SLOT( AreaOnOff() ) );
    connect( myUi.FullScreenRadioButton, SIGNAL( clicked() ), SLOT( AreaOnOff() ) );
    connect( myUi.WindowRadioButton,     SIGNAL( clicked() ), SLOT( AreaOnOff() ) );

    
   AudioOnOff();
   QvkPulse::pulseUnloadModule();
    
   startAction = new QAction( this );
   startAction->setIcon( QIcon::fromTheme( "media-playback-start", QIcon( ":/pictures/start.png" ) ) );
   startAction->setText( tr( "Start" ) );
   startAction->setData( "Start" );
   
   stopAction = new QAction( this );
   stopAction->setIcon( QIcon::fromTheme( "media-playback-stop" , QIcon( ":/pictures/stop.png" ) ) );
   stopAction->setText( tr( "Stop" ) );
   stopAction->setData( "Stop" );
   stopAction->setEnabled( false );
   
   pauseAction = new QAction( this );
   pauseAction->setIcon( QIcon::fromTheme( "media-playback-pause", QIcon( ":/pictures/pause.png" ) ) );
   pauseAction->setText( tr( "Pause" ) );
   pauseAction->setData( "Pause" );
   pauseAction->setEnabled( false );
   
   goAction = new QAction( this );
   goAction->setIcon( QIcon::fromTheme( "media-playback-start", QIcon( ":/pictures/go.png" ) ) );
   goAction->setText( tr( "Continue" ) );
   goAction->setData( "Continue" );
   goAction->setEnabled( false );
   
   hideAction = new QAction( this );
   hideAction->setIcon( QIcon::fromTheme( "video-display", QIcon( ":/pictures/systray.png" ) ) );
   hideAction->setText( tr( "Hide window" ) );
   hideAction->setData( "Hide" );
   
   exitAction = new QAction( this );
   exitAction->setIcon( QIcon::fromTheme( "application-exit"  , QIcon( ":/pictures/systray-exit.png" ) ) );
   exitAction->setText( tr( "Exit" ) );
   exitAction->setData( "Exit" );

   SystemTrayMenu = new QMenu( this );
   SystemTrayMenu->addAction( startAction );
   SystemTrayMenu->addAction( stopAction );
   SystemTrayMenu->addAction( pauseAction );
   SystemTrayMenu->addAction( goAction );
   SystemTrayMenu->addSeparator();
   SystemTrayMenu->addAction( hideAction );
   SystemTrayMenu->addSeparator();
   SystemTrayMenu->addAction( exitAction );
   connect( SystemTrayMenu, SIGNAL( triggered( QAction * ) ), this, SLOT( SystemTrayKontextMenue( QAction * ) ) );
   
   SystemTrayIcon = new QSystemTrayIcon( QIcon( ":/pictures/systray.png" ) );
   SystemTrayIcon->setContextMenu ( SystemTrayMenu );
   SystemTrayIcon->setToolTip( "vokoscreen" );
   SystemTrayIcon->show();
   myUi.SystrayCheckBox->setCheckState( Qt::CheckState( vkSettings.getSystray() ) );
 

   shortcutWebcam = new QxtGlobalShortcut( this );
   connect( shortcutWebcam, SIGNAL( activated() ), myUi.webcamCheckBox, SLOT( click() ) );
   shortcutWebcam->setShortcut( QKeySequence( "Ctrl+Shift+F8" ) );
   
   shortcutMagnifier = new QxtGlobalShortcut( this );
   connect( shortcutMagnifier, SIGNAL( activated() ), myUi.MagnifierCheckBox, SLOT( click() ) );
   shortcutMagnifier->setShortcut( QKeySequence( "Ctrl+Shift+F9" ) );

   shortcutStart = new QxtGlobalShortcut( this );
   connect( shortcutStart, SIGNAL( activated() ), myUi.recordButton, SLOT( click() ) );
   shortcutStart->setShortcut( QKeySequence( "Ctrl+Shift+F10" ) );
   
   shortcutStop = new QxtGlobalShortcut( this );
   connect( shortcutStop, SIGNAL( activated() ), myUi.StopButton, SLOT( click() ) );
   shortcutStop->setShortcut( QKeySequence( "Ctrl+Shift+F11" ) );
   
   shortcutPause = new QxtGlobalShortcut( this );
   connect( shortcutPause, SIGNAL( activated() ), myUi.PauseButton, SLOT( click() ) );
   shortcutPause->setShortcut( QKeySequence( "Ctrl+Shift+F12" ) );
   
 
   QvkAlsaWatcher * myAlsaWatcher = new QvkAlsaWatcher();
   connect( myAlsaWatcher, SIGNAL( changed( QStringList ) ), this, SLOT( AlsaWatcherEvent( QStringList ) ) );
   connect( myAlsaWatcher, SIGNAL( changed( QStringList ) ), this, SLOT( commandLineStart() ) );

   VideoFileSystemWatcher = new QFileSystemWatcher();
   VideoFileSystemWatcher->addPath( myUi.SaveVideoPathLineEdit->displayText() );
   connect( VideoFileSystemWatcher, SIGNAL( directoryChanged( const QString& ) ), this, SLOT( myVideoFileSystemWatcher( const QString ) ) );
   myVideoFileSystemWatcher( "" );

   clickedScreenSize();
   AreaOnOff();
   
   qDebug() << "[vokoscreen] ---Begin search devices---";
     QString device = "x11grab";
     if ( formatsAndCodecs->isDeviceAvailable( device ) == true )
     {
       qDebug() << "[vokoscreen] find device" << device;
     }
     else
     {
       qDebug() << "[vokoscreen] not found device" << device;
       QMessageBox msgBox;
       msgBox.setText("Your ffmpeg is not compatible with vokoscreen");
       msgBox.setInformativeText("ffmpeg must copmpiled with option --enable-x11grab");
       msgBox.setStandardButtons( QMessageBox::Ok );
       msgBox.setDefaultButton( QMessageBox::Ok );
     }
   qDebug() << "[vokoscreen] ---End search devices---";
   qDebug( " " );
   
   addVokoscreenExtensions();
   connect( myUi.extensionLoadpushButton, SIGNAL( clicked() ), this, SLOT( extensionLoadpushButtonClicked() ) );
   myUi.tabWidget->setCurrentIndex( vkSettings.getTab() );

  // workDirectory setzen damit die links in about funktionieren.
  QSettings settings( vkSettings.getProgName(), vkSettings.getProgName() );
  QFileInfo settingsPath( settings.fileName() );
  QFile file( settingsPath.absolutePath() );
  QString workDirectory = file.fileName();
  QDir Dir( "" );
  Dir.setCurrent( workDirectory );  
 }


screencast::~screencast()
{ 
}

void screencast::extensionLoadpushButtonClicked()
{
    QString path = QStandardPaths::writableLocation( QStandardPaths::DownloadLocation );

    QFileDialog dialog( this );
    dialog.setNameFilter( "Extensions(*.tar.gz)" );
    dialog.setDirectory( path );
    dialog.exec();

    QStringList extensionsFile = dialog.selectedFiles();

    if ( not extensionsFile.empty() )
    {
       qDebug() << "[vokoscreen] Load Extensionfile:" << extensionsFile[0];

       QString prog = "tar";

       QStringList arguments = QStringList() << "xfvz"<< extensionsFile[0] << "-C" << QStandardPaths::writableLocation( QStandardPaths::HomeLocation );

       QProcess Process;
       Process.start( prog, arguments );
       Process.waitForFinished( 3000 );
       Process.close();
       qDebug() << "[vokoscreen] Extract Extensionfile with:" << prog << "arguments:" << arguments;
       addVokoscreenExtensions();
    }
}

/*
void screencast::debugCommandInvocation(const QString &description, const QString &program,
                                        const QStringList arguments, const QString &suffix){
	QDebug debug(qDebug());
	
	debug.noquote();
	debug << "[vokoscreen]" << (description + ":");
	
	debug.quote();
	debug << program;
	for(QString argument : arguments) {
		debug << argument;
	}
	
	debug.noquote();
	if(suffix.length() > 0) {
		debug << suffix;
	}
}
*/

// range-based ‘for’ loops are not allowed in C++98 mode

void screencast::debugCommandInvocation(const QString &description, const QString &program,
                                        const QStringList arguments, const QString &suffix)
{
    QString value = "[vokoscreen] " + description + ": " + program;
    for( int i = 0; i < arguments.count(); i++ )
    {
       value = value.append( " " );
       value = value.append( arguments[i] );
    }

    if( suffix.length() > 0 )
    {
       value = value + suffix;
    }
    qDebug().noquote() << value;
}


void screencast::addVokoscreenExtensions()
{
  QString vokoscreenExtensionsPath = QStandardPaths::writableLocation( QStandardPaths::HomeLocation ) + QDir::separator() + "vokoscreen_extensions" + QDir::separator();
  qDebug() << "[vokoscreen] ExtensionsPath:" << vokoscreenExtensionsPath;

  QDir libDir( vokoscreenExtensionsPath );
  QStringList filter = QStringList()  << "*.so.*";
  QStringList libStringList = libDir.entryList( filter, QDir::Files | QDir::NoSymLinks );

  // Testen ob eine lib im Ordner ist
  if ( !libStringList.empty() )
  {

    QLibrary *library = new QLibrary( vokoscreenExtensionsPath + libStringList[ 0 ] );

    if ( library->load() )
    {
      qDebug() << "[vokoscreen]" << libStringList[ 0 ] << "Library has been loaded";

      typedef QWidget*(*CreateWidgetFunction)( QWidget *parent, Ui_screencast GUI );
      CreateWidgetFunction cwf = ( CreateWidgetFunction )library->resolve( "vokoscreen_extensions" );

      if ( cwf )
      {
        cwf( this, myUi );
        qDebug() << "[vokoscreen]" << "Extension was loaded";
        myUi.extensionLoadpushButton->hide();
      }
      else
      {
        qDebug() << "[vokoscreen]" << "Extension can not be displayed from the loaded library";
      }
    }
    else
    {
      qDebug() << "[vokoscreen]" << "Library found but not loaded" << library->errorString();
    }
  }
  else
  {
    qDebug() << "[vokoscreen] No library for extension found";
  }

  qDebug( " " );
}


void screencast::stateChangedshortcutsOnOff( int state )
{
  if ( state == Qt::Checked )
  {
    shortcutWebcam->setEnabled( true );
    shortcutMagnifier->setEnabled( true );
    shortcutStart->setEnabled( true );
    shortcutStop->setEnabled( true );
    shortcutPause->setEnabled( true );
  }
    
  if ( state == Qt::Unchecked )
  {
    shortcutWebcam->setEnabled( false );
    shortcutMagnifier->setEnabled( false );
    shortcutStart->setEnabled( false );
    shortcutStop->setEnabled( false );
    shortcutPause->setEnabled( false );
  }
}


void screencast::showEvent(QShowEvent *event)
{
   (void)event;
   // Unter Plasma5 wird Area erst angezeigt wenn GUI vollständig aufgebaut ist, daher muß es im showEvent stehen
   if ( vkSettings.getAreaSelect() == true )
      myUi.AreaRadioButton->click();
}


void screencast::areaReset()
{
    myregionselection->regionChoise->areaReset();
}


bool commandLine_Start = false;
void screencast::commandLineStart( bool value )
{
  commandLine_Start = value;
  qDebug() << "[vokoscreen] ---Begin commandLine_Start---";
  qDebug() << "[vokoscreen] Start:" << value;
  qDebug() << "[vokoscreen] ---End commandLine_Start---";
  qDebug( " " );
}

void screencast::commandLineStart()
{
  if ( commandLine_Start == true )
  {
    commandLine_Start = false;
    preRecord();
  }
}


void screencast::currentFormatChanged( const QString value )
{
   /*
    * Jedes Format kann nur mit bestimmte Codecs umgehen
    */
   QStringList MKV_videoCodecList = ( QStringList() << "libx264" << "libx265" << "mpeg4" << "huffyuv");
   QStringList MKV_AudioCodecLIst = ( QStringList() << "libmp3lame" << "libvorbis" << "pcm_s16le" << "libvo_aacenc" << "aac" );
   
   QStringList MP4_videoCodecList = ( QStringList() << "libx264" << "libx265" << "mpeg4");
   QStringList MP4_AudioCodecList = ( QStringList() << "libmp3lame" << "libvorbis" << "libvo_aacenc" << "aac");

   // https://de.wikipedia.org/wiki/QuickTime
   QStringList MOV_videoCodecList = ( QStringList() << "libx264" << "mpeg4" );
   QStringList MOV_AudioCodecList = ( QStringList() << "alac" << "aac" << "libmp3lame" );
   
   QStringList GIF_videoCodecList = ( QStringList() << "gif" );
   QStringList GIF_AudioCodecList = ( QStringList() << "" );
   
   // https://trac.ffmpeg.org/wiki/Encode/VP8
   // /usr/bin/ffmpeg -y -report  -threads 4 -f x11grab -draw_mouse 1 -framerate 25 -video_size 1680x1050 -i :0+0,0 -pix_fmt yuv420p -c:v libvpx -quality realtime -vpre libvpx-720p  -s 1680x1050 -f webm vokoscreen-2016-05-27_08-32-31.webm
   //QStringList WEBM_videoCodecList = ( QStringList() << "libvpx" );
   //QStringList WEBM_AudioCodecList = ( QStringList() << "vorbis" );
   
  
  if ( value == "mkv" )
  {
    searchVideoCodec( MKV_videoCodecList );
    searchAudioCodec( MKV_AudioCodecLIst );
  }
  
  if ( value == "mp4" )
  {
    searchVideoCodec( MP4_videoCodecList );
    searchAudioCodec( MP4_AudioCodecList );
  }
  
  if ( value == "gif" )
  {
    searchVideoCodec( GIF_videoCodecList );
    searchAudioCodec( GIF_AudioCodecList );
  }
  
  if ( value == "mov" )
  {
    searchVideoCodec( MOV_videoCodecList );
    searchAudioCodec( MOV_AudioCodecList );
  }
  
/*  
  if ( value == "webm" )
  {
    searchVideoCodec( WEBM_videoCodecList );
    searchAudioCodec( WEBM_AudioCodecList );
  }
*/  
}

void screencast::searchAudioCodec( QStringList audioCodecList )
{
   qDebug() << "[vokoscreen] ---Begin search audio codec---";
   bool experimental = false;
   myUi.AudiocodecComboBox->clear();
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
   int x = myUi.AudiocodecComboBox->findText( vkSettings.getAudioCodec() );
   if ( x == -1 )
      myUi.AudiocodecComboBox->setCurrentIndex( 0 );
   else
      myUi.AudiocodecComboBox->setCurrentIndex( x );
   qDebug() << "[vokoscreen] ---End search audio codec---";
   qDebug( " " );
}

void screencast::searchVideoCodec( QStringList videoCodecList )
{
   qDebug() << "[vokoscreen] ---Begin search video codec---";
   bool experimental = false;
   myUi.VideocodecComboBox->clear();
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
}


void screencast::SearchFormats()
{
   qDebug() << "[vokoscreen] ---Begin search formats---";
   myUi.VideoContainerComboBox->clear();
   QStringList formatList   = ( QStringList() << "mkv"      << "mp4" << "gif" << "mov" );
   QStringList userDataList = ( QStringList() << "matroska" << "mp4" << "gif" << "mov" );
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
   int x = myUi.VideoContainerComboBox->findText( vkSettings.getVideoContainer() );
   if ( x == -1 )
      myUi.VideoContainerComboBox->setCurrentIndex( 0 );
   else
      myUi.VideoContainerComboBox->setCurrentIndex( x );
   qDebug() << "[vokoscreen] ---End search formats---";
   qDebug( " " );
}


void screencast::contextMenuEvent( QContextMenuEvent *event )
{
    if ( myUi.ListWidgetLogVokoscreen->underMouse() == true )
    {
      QAction *actionCopyAll = new QAction( "Copy all", this );
      connect( actionCopyAll, SIGNAL( triggered() ), this, SLOT( copyToClipboard() ) );
      QMenu *menu = new QMenu();
      menu->addAction( actionCopyAll );
      menu->exec( event->globalPos() );
      delete actionCopyAll;
      delete menu;
    }
}


void screencast::copyToClipboard()
{
  QClipboard *clipboard = QApplication::clipboard();  
  QString string;
  for ( int i = 0; i < myUi.ListWidgetLogVokoscreen->count(); ++i )
  {
    QListWidgetItem *listWidgetItem;
    listWidgetItem = myUi.ListWidgetLogVokoscreen->item( i );
    string = string + listWidgetItem->text() + "\n";
  }
  clipboard->setText( string );
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
  if ( myUi.pointerCheckBox->checkState() == Qt::Checked )
    myUi.pointerCheckBox->click();
  myregionselection->close();
  magnifier->close();
  webcamController->webcamCloseEvent();
  SystemTrayIcon->hide();  
}


void screencast::saveSettings()
{
  QSettings settings( vkSettings.getProgName(), vkSettings.getProgName() );
  
  settings.clear();

  settings.beginGroup( "vokoscreen" );
    settings.setValue( "Version", vkSettings.getVersion() );
  settings.endGroup();

  settings.beginGroup( "Audio" );
    settings.setValue( "AudioOnOff", myUi.AudioOnOffCheckbox->checkState() );
  settings.endGroup();

  settings.beginGroup( "Alsa" );
    settings.setValue( "Alsa", myUi.AlsaRadioButton->isChecked() );
    settings.setValue( "NameCaptureCard", myUi.AlsaHwComboBox->currentText() );
  settings.endGroup();

  settings.beginGroup( "Pulse" );
    settings.setValue( "Pulse", myUi.PulseDeviceRadioButton->isChecked() );
    for ( int i = 1; i < QvkPulse::getCountCheckedPulseDevices( myUi.scrollAreaWidgetContents ) + 1; i++ )
      settings.setValue( "NameCaptureCard-" + QString::number( i ), QvkPulse::getPulseDeviceName( i, myUi.scrollAreaWidgetContents ).replace( "&", "" ) );
  settings.endGroup();

  settings.beginGroup( "Record" );
    settings.setValue( "FullScreen", myUi.FullScreenRadioButton->isChecked() );
    settings.setValue( "Window", myUi.WindowRadioButton->isChecked() );
    settings.setValue( "Area", myUi.AreaRadioButton->isChecked() );
  settings.endGroup();

  // Have to time no function
  settings.beginGroup( "Screen" );
    QDesktopWidget *desk = QApplication::desktop();
    settings.setValue( "Count", desk->screenCount() );
  settings.endGroup();
  
  settings.beginGroup( "Miscellaneous" );
    settings.setValue( "VideoPath", myUi.SaveVideoPathLineEdit->displayText() );
    settings.setValue( "Videoplayer", myUi.VideoplayerComboBox->currentText() );
    settings.setValue( "GIFplayer", myUi.GIFplayerComboBox->currentText() );
    settings.setValue( "Minimized", myUi.MinimizedCheckBox->checkState() );
    settings.setValue( "MinimizedByStart", myUi.MinimizedByStartCheckBox->checkState() );
    settings.setValue( "Countdown", myUi.CountdownSpinBox->value() );
    settings.setValue( "Reset", myUi.ResetCheckBox->checkState() );
    if ( vkSettings.isVokoscreenWithLibs() == false )
      settings.setValue( "Recorder", myUi.RecorderLineEdit->displayText() );
  settings.endGroup();

  settings.beginGroup( "Videooptions" );
    settings.setValue( "Frames", myUi.FrameSpinBox->value() );
    settings.setValue( "Videocodec", myUi.VideocodecComboBox->currentText() );
    settings.setValue( "X264Lossless", myUi.x264LosslessCheckBox->checkState() );
    settings.setValue( "Audiocodec", myUi.AudiocodecComboBox->currentText() );
    settings.setValue( "Format", myUi.VideoContainerComboBox->currentText() );
    settings.setValue( "HideMouse", myUi.HideMouseCheckbox->checkState() );    
  settings.endGroup();
  
  settings.beginGroup( "GUI" );
    settings.setValue( "X", x() );
    settings.setValue( "Y", y() );
    settings.setValue( "Tab", myUi.tabWidget->currentIndex() );
    settings.setValue( "Systray", myUi.SystrayCheckBox->checkState() );
  settings.endGroup();
  
  settings.beginGroup( "Area" );
    settings.setValue( "X", myregionselection->getX() );
    settings.setValue( "Y", myregionselection->getY() );
    settings.setValue( "Width", myregionselection->getWidth() );
    settings.setValue( "Height", myregionselection->getHeight() );
  settings.endGroup();

  settings.beginGroup( "Webcam" );
    settings.setValue( "OnOff", myUi.webcamCheckBox->checkState() );
    settings.setValue( "Mirrored", myUi.mirrorCheckBox->checkState() );
    settings.setValue( "Rotate", myUi.rotateDial->value() );
    settings.setValue( "Top", myUi.radioButtonTopMiddle->isChecked() );
    settings.setValue( "Right", myUi.radioButtonRightMiddle->isChecked() );
    settings.setValue( "Bottom", myUi.radioButtonBottomMiddle->isChecked() );
    settings.setValue( "Left", myUi.radioButtonLeftMiddle->isChecked() );
    settings.setValue( "X", webcamController->webcamWindow->getValueX() );
    settings.setValue( "Y", webcamController->webcamWindow->getValueY() );
    settings.setValue( "Width", webcamController->webcamWindow->getValueWidth() );
    settings.setValue( "Height", webcamController->webcamWindow->getValueHeight() );
    settings.setValue( "Border", webcamController->webcamWindow->getValueBorder() );
    settings.setValue( "OverFullScreen", webcamController->webcamWindow->getOverFullScreen() );
  settings.endGroup();
  
  settings.beginGroup( "Magnifier" );
    settings.setValue( "OnOff", myUi.MagnifierCheckBox->checkState() );
    settings.setValue( "FormValue", magnifier->getFormValue() );
  settings.endGroup();
  
  settings.beginGroup( "ShowClick" );
    settings.setValue( "OnOff", myUi.pointerCheckBox->checkState() );
    settings.setValue( "Showtime", (double) ShowClickDialog->myUiDialog.horizontalSliderShowtime->value() );
    settings.setValue( "Circle", ShowClickDialog->myUiDialog.horizontalSliderCircle->value() );
    settings.setValue( "Radiant", ShowClickDialog->myUiDialog.checkBoxRadiant->isChecked() );
    settings.setValue( "Opacity", (double) ShowClickDialog->myUiDialog.horizontalSliderOpacity->value()/100 );
    settings.setValue( "Color", (QVariant)ShowClickDialog->getColor() );
  settings.endGroup();
  
  settings.beginGroup( "ShowKey" );
    settings.setValue(  "OnOff", myUi.ShowkeyCheckBox->checkState() );
  settings.endGroup();
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

#include <QScreen>
void screencast::myScreenCountChanged( int newCount )
{
    (void)newCount;
    myUi.ScreenComboBox->clear();
    QDesktopWidget *desk = QApplication::desktop();
    qDebug() << "[vokoscreen]" << "---Begin search Screen---";
    qDebug() << "[vokoscreen]" << "Number of screens:" << desk->screenCount();
    qDebug() << "[vokoscreen] Primary screen is: Display" << desk->primaryScreen()+1;
    qDebug() << "[vokoscreen] VirtualDesktop:" << desk->isVirtualDesktop();

      //QList < QScreen *> screens = QGuiApplication::screens();
      QScreen *screen = QGuiApplication::primaryScreen();    
      qDebug() << "[vokoscreen] DevicePixelRatio:" << screen->devicePixelRatio() << " (Normal displays is 1, Retina display is 2)";
    
    for ( int i = 1; i < desk->screenCount()+1; i++ )
    {
      QString ScreenGeometryX1 = QString::number( desk->screenGeometry( i-1 ).left() );
      QString ScreenGeometryY1 = QString::number( desk->screenGeometry( i-1 ).top() );
      QString ScreenGeometryX = QString::number( desk->screenGeometry( i-1 ).width() * screen->devicePixelRatio() ); // devicePixelRatio() for Retina Displays
      QString ScreenGeometryY = QString::number( desk->screenGeometry( i-1 ).height() * screen->devicePixelRatio() );
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


void screencast::AreaOnOff()
{
  if ( myUi.FullScreenRadioButton->isChecked() or myUi.WindowRadioButton->isChecked() )
  {
    myregionselection->hide();
    myUi.areaResetButton->setEnabled( false );
  }

  if ( myUi.AreaRadioButton->isChecked() )
  {
    myUi.areaResetButton->setEnabled( true );
    myregionselection->show();
  }
}


/**
 * Looking for external programs
 */
void screencast::searchExternalPrograms()
{
  qDebug() << "[vokoscreen]" << "---Begin Search external tools---";
  
  if ( searchProgramm( vkSettings.getRecorder() ) )
     qDebug() << "[vokoscreen]" << "Search ffmpeg     ..... found" << vkSettings.getRecorder() << "Version:" << getFfmpegVersion();
  else
     qDebug() << "[vokoscreen]" << "Search ffmpeg     ..... not found. Please install ffmpeg";

#ifdef Q_OS_LINUX
  if ( searchProgramm("pactl") )
     qDebug() << "[vokoscreen]" << "Search pactl      ..... found Version:" << getPactlVersion();
  else
     qDebug() << "[vokoscreen]" << "Search pactl      ..... pactl not found, this is an pulseaudio-utils tool. Please install pulseaudio-utils";
#endif 
#ifdef Q_OS_WIN
#endif  
  
#ifdef Q_OS_LINUX
  if ( searchProgramm("xdg-email") )
     qDebug() << "[vokoscreen]" << "Search xdg-email  ..... found Version:" << getXdgemailVersion();
  else
     qDebug() << "[vokoscreen]" << "Search xdg-email  ..... xdg-email not found, this is an xdg-utils tool. Please install xdg-utils";
#endif  
#ifdef Q_OS_WIN
#endif  
  
#ifdef Q_OS_LINUX
  if ( searchProgramm("lsof") )
     qDebug() << "[vokoscreen]" << "Search lsof       ..... found Version:" << getLsofVersion();
  else
     qDebug() << "[vokoscreen]" << "Search lsof       ..... lsof not found. Please install lsof";
#endif  
#ifdef Q_OS_WIN
#endif  
  
  
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
  Process.start(vkSettings.getRecorder(), QStringList("-version"));
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

/*
 * Zum überprüfen ob der gesetzte FFmpeg der richtige ist
 */
QString screencast::getFfmpegVersionFullOutput()
{
  QProcess Process;
  Process.start(myUi.RecorderLineEdit->displayText(), QStringList("-version"));
  Process.waitForFinished();
  QString ffmpegversion = Process.readAllStandardOutput();
  Process.close();
  return ffmpegversion;
}


QString screencast::getPactlVersion()
{
  QProcess Process;
  Process.start("pactl", QStringList("--version"));
  Process.waitForFinished();
  QString pactlVersion = Process.readAllStandardOutput();
  Process.close();

  QStringList list = pactlVersion.split( "\n" );
  
  if ( list.empty() )
    pactlVersion = "";
  else
    pactlVersion = list[ 0 ];
  
  return pactlVersion  ;
}


QString screencast::getXdgemailVersion()
{
  QProcess Process;
  Process.start("xdg-email", QStringList("--version"));
  Process.waitForFinished();
  QString xdgemailVersion = Process.readAllStandardOutput();
  Process.close();

  QStringList list = xdgemailVersion.split( "\n" );
  
  if ( list.empty() )
    xdgemailVersion = "";
  else
    xdgemailVersion = list[ 0 ];
  
  return xdgemailVersion;
}


QString screencast::getLsofVersion()
{
  QProcess Process;
  Process.start("lsof", QStringList("-v"));
  Process.waitForFinished();
  QString lsofVersion = Process.readAllStandardError();
  Process.close();

  QStringList list = lsofVersion.split( "\n" );
  if ( list.empty() )
    lsofVersion = "";
  else
  {
    if ( list.count() >= 2 )
      lsofVersion = list[ 1 ].trimmed();
  }

  return lsofVersion;
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
    makeAndSetValidIcon( 1 );
  }
}

/*
 * In der run version werden die svg icons versetzt nach unten dargestellt.
 * Dies soll das berichtigen.
 */
void screencast::makeAndSetValidIcon( int index )
{
  QIcon myIcon = myUi.tabWidget->tabIcon( index );
  QSize size = myUi.tabWidget->iconSize();
  QPixmap workPixmap( myIcon.pixmap( size ) );
  myUi.tabWidget->setTabIcon( index, QIcon( workPixmap ) );
}


void screencast::SystemTrayKontextMenue( QAction *action )
{
  QString data = action->data().toString();
  
  if ( data == "Start" )
    myUi.recordButton->click();

  if ( data == "Stop" )
    myUi.StopButton->click();
  
  if ( data == "Pause" )
    myUi.PauseButton->click();
  
  if ( data == "Continue" )
    myUi.PauseButton->click();
  
  if ( data == "Exit" )
    close();

  if ( data == "Hide" )
  {
    hideAction->setText( tr( "Show window" ) );
    hideAction->setData( "NoHide" );
    hide();
  }
  
  if ( data == "NoHide" )
  {
    hideAction->setText( tr( "Hide window" ) );
    hideAction->setData( "Hide" );
    showNormal();
  }
}


/**
 * Returns Checkbox from Pulse device
 * 
 */
QCheckBox * screencast::getCheckBoxPulseDevice( int value )
{
  QList<QCheckBox *> listQFrame = myUi.scrollAreaWidgetContents->findChildren<QCheckBox *>();  
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
  AlsaCardList.clear();
  // Für jede card wird eine Instanz erzeugt und in AlsaDeviceList abgelegt
  for( int i = 0; i < CardxList.count(); i++ )
  {
    QvkAlsaDevice * alsaCard = new QvkAlsaDevice( CardxList[ i ] );
    AlsaCardList.append( alsaCard );
    myUi.AlsaHwComboBox->addItem( AlsaCardList.at( i )->getAlsaName() , i );
    myUi.AlsaHwComboBox->setItemIcon( i , QIcon::fromTheme( "audio-input-microphone", QIcon( ":/pictures/micro.png" ) ) );
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
    int countPulse = QvkPulse::getPulseInputDevicesCount();
    for ( int x = 1; x <= countPulse; x++ )
    {
       for ( int i = 0; i < countPulse; i++ )
       {
          QCheckBox *aa = getCheckBoxPulseDevice( i );
          if ( aa->text() == settings.value( "NameCaptureCard-" + QString::number( x  ) ).toString() )
	  {
            aa->setCheckState( Qt::Checked );
	  }
       }
    }
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

  QvkPulse vkPulse;
  vkPulse.grepPactlOutput();
  for ( int i = 1; i <= QvkPulse::getPulseInputDevicesCount(); ++i )
  {
    namePulse = new QCheckBox();
    myUi.verticalLayout_3->addWidget( namePulse );
    namePulse->setText( QvkPulse::getPulseInputName( i ) );
    namePulse->setAccessibleName( QvkPulse::getPulseInputDevices( i  ) );
    namePulse->setToolTip( tr ( "Select one or more devices" ) );
    namePulse->setIcon( QIcon::fromTheme( vkPulse.getIconName( i-1 ), QIcon( ":/pictures/micro.png" ) ) );
    qDebug() << "[vokoscreen]" << "Find CaptureCard:" << namePulse->text() << "with device:" << namePulse->accessibleName();
  }

  AudioOnOff();

  qDebug() << "[vokoscreen]" << "---End search PulseAudio Capture Devices---";
  qDebug( " " );
}

#include <X11/Xlib.h>
void screencast::windowMove()
{
  Window root_return, child_return;
  int root_x_return, root_y_return;
  int win_x_return, win_y_return;
  unsigned int mask_return;
  XQueryPointer( QX11Info::display(), moveWindowID, &root_return, &child_return, &root_x_return, &root_y_return, 
                                          &win_x_return, &win_y_return, &mask_return );

  if ( ( root_y_return < QxtWindowSystem::windowGeometryWithoutFrame( moveWindowID ).y() ) )
  {
    if ( SystemCall->state() == QProcess::Running )
    {
      if ( ( QxtWindowSystem::activeWindow() == moveWindowID ) and ( mask_return == 272 ) )
      {
        SystemCall->terminate();
        SystemCall->waitForFinished();
        pause = true;
        QvkPulse::pulseUnloadModule();
        return;
      }
    }
  }
  
  // Window have new position
  if ( ( pause == true ) and ( SystemCall->state() == QProcess::NotRunning ) )
  {
    if ( ( QxtWindowSystem::activeWindow() == moveWindowID ) and ( mask_return == 16 ) )
    {
      newMovedXYcoordinates();
      myUi.PauseButton->setChecked( false );  
      myUi.PauseButton->setText( tr ( "Pause" ) );
      startRecord((PathTempLocation() + QDir::separator() + newPauseNameInTmpLocation()), deltaXMove, deltaYMove);
      return;
    }
  }
  
  // Window is closed
  QStringList stringList;
  QList<WId> list = QxtWindowSystem::windows() ;
  for( int i = 0; i < list.count(); i++)
    stringList << QString::number( list[ i ] );

  if ( !stringList.contains( QString::number( moveWindowID ) ) )
  {
    windowMoveTimer->stop();
    Stop();
  } 
}

void screencast::newMovedXYcoordinates()
{
      // Wenn Versatz kleiner null ist
      QString x = QString::number( QxtWindowSystem::windowGeometryWithoutFrame( moveWindowID ).x() );
      int xx = x.toInt();
      if ( xx < 0 )
        x = "0";
  
      QString y = QString::number( QxtWindowSystem::windowGeometryWithoutFrame( moveWindowID ).y() );
      int yy = y.toInt();
      if ( yy < 0 )
        y = "0";
      
      deltaXMove = x;
      deltaYMove = y; 
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
 * Statusbar and widgets
 */
void screencast::currentIndexChangedCodec( int index )
{
  (void)index;
  statusBarLabelCodec->setText( myUi.VideocodecComboBox->currentText() );

  if ( myUi.VideocodecComboBox->currentText() == "libx264" )
  {
     myUi.x264LosslessCheckBox->setEnabled( true );
  }
  else
  {
     myUi.x264LosslessCheckBox->setEnabled( false );
  }
}


/**
 * Statusbar and widgets
 */
void screencast::currentIndexChangedFormat( int index )
{
  (void)index;
  statusBarLabelFormat->setText( myUi.VideoContainerComboBox->currentText() );
  if ( myUi.VideoContainerComboBox->currentText() == "gif" )
  {
    myUi.GIFplayerComboBox->show();
    myUi.VideoplayerComboBox->hide();
    myUi.VideocodecComboBox->setEnabled( false );
    myUi.AudiocodecLabel->setEnabled( false );
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
    myUi.AudiocodecLabel->setEnabled( true );
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
 * Statusbar and widgets
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
  myUi.x264LosslessCheckBox->setChecked( false );
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
    qDebug() << "[vokoscreen]" << "---End search GIFplayer---";
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


void screencast::saveVideoPath()
{
  QString dir = QFileDialog::getExistingDirectory( this, tr( "Open Directory" ),
                QStandardPaths::writableLocation( QStandardPaths::HomeLocation ), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

  if ( dir > "" )
  {
      VideoFileSystemWatcher->removePath( myUi.SaveVideoPathLineEdit->displayText() );
        myUi.SaveVideoPathLineEdit->setText( dir );
      VideoFileSystemWatcher->addPath( myUi.SaveVideoPathLineEdit->displayText() );
      myVideoFileSystemWatcher( "" );
  }
}


void screencast::readyReadStandardError()
{
  int s = beginTime.secsTo( QDateTime::currentDateTime() );
  int HH = s / 3600;
  int MM = ( s- ( s / 3600 ) * 3600 ) / 60;
  int SS = s % 60;
  QTime myTime( HH, MM, SS);
  QString time = myTime.toString ( "hh:mm:ss");
  statusBarLabelTime->setText( time );

  QString output = SystemCall->readAllStandardError();
  if ( output.contains( "fps=", Qt::CaseInsensitive ) )
  {
    int x = output.indexOf( "fps" );
    statusBarLabelFps->setText( output.mid( x + 4, 3 ).replace( " ", "" ) );
  }
  
  qint64 summFileSize = 0;
  QFileInfo fileInfo;
  if ( pause == true )
  {
    QDir dir( PathTempLocation() + QDir::separator() );
    QStringList stringList = dir.entryList(QDir::Files, QDir::Time | QDir::Reversed);
    
    for ( int i = 0; i < stringList.size(); ++i )
    {
      fileInfo.setFile( PathTempLocation() + QDir::separator() + stringList[ i ] );
      summFileSize = summFileSize + fileInfo.size() / 1024;
    }
  }
  else
  {
    fileInfo.setFile( PathTempLocation() + QDir::separator() + nameInMoviesLocation );
    summFileSize = fileInfo.size() / 1024 ;
   
  }

  statusBarLabelSize->setText( QString::number( summFileSize ) );
}


/**
 * Je nach Prozessstaus von ffmpeg werden die Widget enabled bzw. disabled
 */
void screencast::stateChanged ( QProcess::ProcessState newState )
{
    if ( ( newState == QProcess::Running ) and ( pause == false) )
    {
      myUi.StopButton->setEnabled(true);
      myUi.PauseButton->setEnabled(true);
      myUi.recordButton->setEnabled(false);
      myUi.PlayButton->setEnabled(false);
      myUi.FullScreenRadioButton->setEnabled( false );
      myUi.WindowRadioButton->setEnabled( false );
      myUi.AreaRadioButton->setEnabled( false );
      if ( myUi.AreaRadioButton-> isChecked() ) 
           myUi.areaResetButton->setEnabled( false );
      clickedScreenSize();

      myUi.TabWidgetAudioFrame->setEnabled(false);
      myUi.tab_2->setEnabled( false );
      myUi.TabWidgetVideoOptionFrame->setEnabled( false );

      if ( myUi.SystrayCheckBox->checkState() == Qt::Checked )
      {
        SystemTrayIcon->setIcon( QIcon::fromTheme( "media-record", QIcon( ":/pictures/systray-record" ) ) );
        startAction->setEnabled( false );
        stopAction->setEnabled( true );
        pauseAction->setEnabled( true );
        goAction->setEnabled( false );
      } 
    }
  
    if ((newState == QProcess::NotRunning) and (pause == false))
    {
      myUi.StopButton->setEnabled(false);
      myUi.PauseButton->setEnabled(false);
      myUi.recordButton->setEnabled(true);
      myUi.FullScreenRadioButton->setEnabled( true );
      myUi.WindowRadioButton->setEnabled( true );
      myUi.AreaRadioButton->setEnabled( true );
      if ( myUi.AreaRadioButton-> isChecked() ) 
           myUi.areaResetButton->setEnabled( true );
      clickedScreenSize();

      myUi.TabWidgetAudioFrame->setEnabled(true);
      myUi.tab_2->setEnabled(true);

      myUi.TabWidgetVideoOptionFrame->setEnabled( true );
      
      if ( myUi.SystrayCheckBox->checkState() == Qt::Checked )
      {
        SystemTrayIcon->setIcon( QIcon(":/pictures/systray.png" ) );
        startAction->setEnabled( true );
        stopAction->setEnabled( false );
        pauseAction->setEnabled( false );
        goAction->setEnabled( false );
      }
      
      if ( myUi.MagnifierCheckBox->isChecked() )
        myUi.MagnifierCheckBox->click();
    }
    
    if ((newState == QProcess::NotRunning) and (pause == true) and ( myUi.PauseButton->isChecked() ))
    {
      myUi.StopButton->setEnabled(false);
      myUi.PauseButton->setEnabled(true);
      myUi.recordButton->setEnabled(false);
      myUi.PlayButton->setEnabled(false);
      myUi.FullScreenRadioButton->setEnabled( false );
      myUi.WindowRadioButton->setEnabled( false );
      myUi.AreaRadioButton->setEnabled( false );
      if ( myUi.AreaRadioButton-> isChecked() ) 
           myUi.areaResetButton->setEnabled( false );
      clickedScreenSize();
      
      myUi.TabWidgetAudioFrame->setEnabled(false);
      myUi.tab_2->setEnabled(false);
      myUi.TabWidgetVideoOptionFrame->setEnabled( false );

      if ( myUi.SystrayCheckBox->checkState() == Qt::Checked )
      {
        SystemTrayIcon->setIcon( QIcon::fromTheme( "media-playback-pause", QIcon( ":/pictures/pause.png" ) ) );
        startAction->setEnabled( false );
        stopAction->setEnabled( false );
        pauseAction->setEnabled( false );
        goAction->setEnabled( true );
      } 
    }

    if ((newState == QProcess::Running) and (pause == true) and ( not myUi.PauseButton->isChecked() ))
    {
      myUi.StopButton->setEnabled(true);
      myUi.PauseButton->setEnabled(true);
      myUi.recordButton->setEnabled(false);
      myUi.PlayButton->setEnabled(false);
      myUi.FullScreenRadioButton->setEnabled( false );
      myUi.WindowRadioButton->setEnabled( false );
      myUi.AreaRadioButton->setEnabled( false );
      if ( myUi.AreaRadioButton-> isChecked() ) 
           myUi.areaResetButton->setEnabled( false );
      clickedScreenSize();
      
      myUi.TabWidgetAudioFrame->setEnabled(false);
      myUi.tab_2->setEnabled(false);
      myUi.TabWidgetVideoOptionFrame->setEnabled( false );

      if ( myUi.SystrayCheckBox->checkState() == Qt::Checked )
      {
        SystemTrayIcon->setIcon( QIcon::fromTheme( "media-record", QIcon( ":/pictures/systray-record" ) ) );
        startAction->setEnabled( false );
        stopAction->setEnabled( true );
        pauseAction->setEnabled( true );
        goAction->setEnabled( false );
      } 
    } 
      
    if ( ( newState == QProcess::NotRunning ) and ( pause == true ) and ( not myUi.PauseButton->isChecked() ) )
    {
      myUi.StopButton->setEnabled(false);
      myUi.PauseButton->setEnabled(false);
      myUi.recordButton->setEnabled(true);
      myUi.FullScreenRadioButton->setEnabled( true );
      myUi.WindowRadioButton->setEnabled( true );
      myUi.AreaRadioButton->setEnabled( true );
      if ( myUi.AreaRadioButton-> isChecked() ) 
           myUi.areaResetButton->setEnabled( true );
      clickedScreenSize()      ;
      
      myUi.TabWidgetAudioFrame->setEnabled(true);
      myUi.tab_2->setEnabled(true);
      myUi.TabWidgetVideoOptionFrame->setEnabled( true );

      if ( myUi.SystrayCheckBox->checkState() == Qt::Checked )
      {
        SystemTrayIcon->setIcon( QIcon(":/pictures/systray.png" ) );
        startAction->setEnabled( true );
        stopAction->setEnabled( false );
        pauseAction->setEnabled( false );
        goAction->setEnabled( false );
      } 
    }
       
    if ( newState == QProcess::Running )
    {
      qDebug() << "[vokoscreen]" << ffmpegProgram << "is running and is ready for reading and writing";
      qDebug( " " );
    }
   
    if ( newState == QProcess::NotRunning )
    {
      qDebug() << "[vokoscreen]" << ffmpegProgram << "is not running";
      qDebug( " " );

      //Enables the customarea rectangle again. (Is diabled in record() )
      if ( !myUi.PauseButton->isChecked() and myUi.AreaRadioButton->isChecked() )
      {
        myregionselection->lockFrame( false );
      }
    }

    if ( ( myUi.VideocodecComboBox->currentText() == "gif" ) or ( myUi.VideoContainerComboBox->currentText() == "gif" ) )
    {
      myUi.PauseButton->setEnabled( false );
      pauseAction->setEnabled( false );
    } 

}


void screencast::error( QProcess::ProcessError error )
{
  // Kann mann testen in dem man den ffmpeg aus der Processliste tötet
  if ( error == QProcess::Crashed )
  {
    // remove crashed directory
    QSettings settings1( vkSettings.getProgName(), vkSettings.getProgName() );
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
    
    // Move ffmpeg.log, vokoscreen.log, vokoscreen.conf in directory crashed
    QSettings settings( vkSettings.getProgName(), vkSettings.getProgName() );
    QFileInfo settingsPath( settings.fileName() );
    QFile file( settingsPath.absolutePath() );
    QString workDirectory = file.fileName();
    QDir Dir( "" );
    Dir.setCurrent( workDirectory );
    Dir.mkdir( "crashed" );
    
    QStringList stringList = Dir.entryList( QDir::Files, QDir::NoSort );
    for ( int i = 0; i < stringList.count(); ++i )
      QFile::copy( workDirectory + QDir::separator() + stringList[ i ], workDirectory + QDir::separator() + "crashed" + QDir::separator() + stringList[ i ]);
    
    QString crashedtext = "[vokoscreen] " + ffmpegProgram + " is crashed on ";
    QDateTime dateTime = QDateTime::currentDateTime();
    QString time = dateTime.toString( "hh:mm" );
    QString day = dateTime.toString( "dddd" );
    qDebug( " " );
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
    qDebug() << "The process could not be started. Either the is called program is not installed, or the ffmpeg call Faulty or you have not over sufficient permissions to the program.";
    QMessageBox msgBox;
    msgBox.setText( "The process could not be started. Either the is called program is not installed, or the ffmpeg call Faulty or you have not over sufficient permissions to the program." );
    msgBox.exec();
  }
}

void screencast::Pause()
{
  if ( myUi.FullScreenRadioButton->isChecked() or myUi.AreaRadioButton->isChecked() )
  {
    pause = true;
    if ( myUi.PauseButton->isChecked() )
    {
      windowMoveTimer->stop();
      myUi.PauseButton->setText( tr ( "Continue" ) );
      SystemCall->terminate();
      SystemCall->waitForFinished();
      QvkPulse::pulseUnloadModule();
    }
    else
    {
      QVariant aa = myUi.AlsaHwComboBox->itemData( myUi.AlsaHwComboBox->currentIndex() );
      QvkAlsaDevice *inBox = AlsaCardList.at( aa.toInt() );
      if ( inBox->isbusy() and myUi.AlsaRadioButton->isChecked() )
      {
        inBox->busyDialog( inBox->getAlsaHw(), inBox->getPurAlsaName() );
        myUi.PauseButton->click();
        return;
      }
      Countdown();
      myUi.PauseButton->setText( tr( "Pause" ) );
      startRecord( PathTempLocation() + QDir::separator() + newPauseNameInTmpLocation(), deltaX, deltaY );
    }
  }
  
  
  if ( myUi.WindowRadioButton->isChecked() )
  {
    pause = true;
    if ( myUi.PauseButton->isChecked() )
    {
      windowMoveTimer->stop();
      myUi.PauseButton->setText( tr ( "Continue" ) );
      SystemCall->terminate();
      SystemCall->waitForFinished();
      QvkPulse::pulseUnloadModule();
    }
    else
    {
      QVariant aa = myUi.AlsaHwComboBox->itemData( myUi.AlsaHwComboBox->currentIndex() );
      QvkAlsaDevice *inBox = AlsaCardList.at( aa.toInt() );
      if ( inBox->isbusy() and myUi.AlsaRadioButton->isChecked() )
      {
        inBox->busyDialog( inBox->getAlsaHw(), inBox->getPurAlsaName() );
        myUi.PauseButton->click();
        return;
      }
      Countdown();
      myUi.PauseButton->setText( tr( "Pause" ) );
      newMovedXYcoordinates();
      startRecord( PathTempLocation() + QDir::separator() + newPauseNameInTmpLocation(), deltaXMove, deltaYMove );
      windowMoveTimer->start();
    }
  }
}


/**
 * Play last screencast with Videoplayer
 */
void screencast::play()
{
  // This part must have for VLC for vokoscreen-with-libs
  // I start vokoscreen and as first "Play"
  // Without this part VLC do not start
  QSettings settings( vkSettings.getProgName(), vkSettings.getProgName() );
  QFileInfo settingsPath( settings.fileName() );
  QFile file( settingsPath.absolutePath() );
  QString workDirectory = file.fileName();
  QDir Dir( "" );
  Dir.setCurrent( workDirectory );  
  // --------------------------
  
  if ( myUi.MagnifierCheckBox->isChecked() )
    myUi.MagnifierCheckBox->click();
  
  if ( myUi.VideoplayerComboBox->count() == 0 )
  {
    QDialog *newDialog = new QDialog;
    newDialog->setModal( true );
    Ui_NoPlayerDialog myUiDialog;
    myUiDialog.setupUi( newDialog );
    newDialog->show();
    return;
  }
  
  QDir Dira( PathMoviesLocation() );
  QStringList filters;
  filters << "vokoscreen*";
  QStringList List = Dira.entryList( filters, QDir::Files, QDir::Time );
  QString player;
  QFileInfo fileinfo( List.at( 0 ) );
  if ( fileinfo.suffix() == "gif" )
  {
    QVariant aa = myUi.GIFplayerComboBox->itemData( myUi.GIFplayerComboBox->currentIndex() ); // get userdata from ComboBox
    player = aa.toString();
    player = player.replace( "\n", "" ); 
  }
  else
  {
    QVariant aa = myUi.VideoplayerComboBox->itemData( myUi.VideoplayerComboBox->currentIndex() ); // get userdata from ComboBox
    player = aa.toString();
    player = player.replace( "\n", "" ); 
  }
    
  QProcess *SystemCall = new QProcess();
  SystemCall->systemEnvironment();
  QString path = PathMoviesLocation() + QDir::separator() + List.at(0);
  qDebug() << "[vokoscreen] play video: " << player << path;

  SystemCall->start(player, QStringList(path));
  SystemCall->waitForFinished( 3000 );
  QString output = SystemCall->readAllStandardError();
  qDebug() << output;
}


QString screencast::PathMoviesLocation()
{
  QString Path;
  if ( myUi.SaveVideoPathLineEdit->displayText() > "" )
     Path = myUi.SaveVideoPathLineEdit->displayText();
  else
  { 
    if ( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ).isEmpty() )
    {
       Path = QStandardPaths::writableLocation( QStandardPaths::HomeLocation );
       myUi.SaveVideoPathLineEdit->setText(Path);
    }
    else
    {
      Path = QStandardPaths::writableLocation( QStandardPaths::MoviesLocation );
      myUi.SaveVideoPathLineEdit->setText( Path );
    }
  }
  return Path;
}


void screencast::myVideoFileSystemWatcher( const QString & path )
{
  (void)path;
  QDir Dira( PathMoviesLocation() );
  QStringList filters;
  filters << "vokoscreen*";
  QStringList List = Dira.entryList( filters, QDir::Files, QDir::Time );
  
  if ( List.isEmpty() )
  {
    myUi.PlayButton->setEnabled( false );
    myUi.sendPushButton->setEnabled( false );
  }
  else
  {
    myUi.PlayButton->setEnabled( true );
    myUi.sendPushButton->setEnabled( true );
  }
}


QString screencast::getFileWithPath( QString ProgName )
{
   if ( ProgName.contains("/", Qt::CaseInsensitive) and ( QFile::exists( ProgName ) ) )
    return ProgName; 
     
    QString find;
    QString prog;
    QString resultString( qgetenv( "PATH" ) );
    QStringList pathList = resultString.split( ":" );
    for ( int i = 0; i < pathList.size(); ++i )
    {
      prog = pathList.at( i ) + QDir::separator() + ProgName;
      if ( QFile::exists( prog ) )
      {
        find = prog;
        break;
      }
    }
    return find;
}

void screencast::recorderLineEditTextChanged( QString recorder )
{
   (void)recorder;
   formatsAndCodecs->getFormatsAndCodecs( myUi.RecorderLineEdit->displayText() );
   SearchFormats();
}


void screencast::selectRecorder()
{
    QString recorder = QFileDialog::getOpenFileName( this,
                                 tr( "Select recorder" ),
                                 QStandardPaths::writableLocation( QStandardPaths::HomeLocation ) );

    if ( recorder > "" )
    {
      myUi.RecorderLineEdit->setText( recorder );
    }
}


void screencast::showHomepage()
{
   QDesktopServices::openUrl( QUrl( "http://www.kohaupt-online.de/hp", QUrl::StrictMode ) );
}


void screencast::showCredits()
{
   myUi.creditsQPushButton->setEnabled( false );
   credits = new QvkCredits();
   credits->show();
   connect( credits, SIGNAL( closeCredits() ), SLOT( creditsCloseEvent() ) );
}


/**
 * Wird aufgerufen wenn das credits Fenster geschloßen und dabei
 * das SIGNAL closeCredits in der Klasse QvkCredits ausgelösst wird
 */
void screencast::creditsCloseEvent()
{
   myUi.creditsQPushButton->setEnabled( true );
   delete credits;
}


void screencast::stateChangedSystray( int state )
{
  if ( state == Qt::Unchecked )
    SystemTrayIcon->hide();

  if ( state == Qt::Checked )
    SystemTrayIcon->show();
}


void screencast::send()
{
  QvkMail *vkMail = new QvkMail( myUi.SaveVideoPathLineEdit->displayText() );
  (void)vkMail;
}



QString screencast::PathTempLocation()
{
  QString tmpName = "." + vkSettings.getProgName() + "-recording";  

  QString tempPathProg = moviePath + QDir::separator() + tmpName;
  
  QDir dirTempPathProg( tempPathProg );
  if ( not dirTempPathProg.exists() )
  {
      QDir dirTempPath( moviePath );
      dirTempPath.mkdir( tmpName );
  }
  return tempPathProg;  
}


/**
 * Return the new screencastname
 */
QString screencast::NameInMoviesLocation()
{
  return "vokoscreen-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + myUi.VideoContainerComboBox->currentText();
}


/**
 * Return the new pausename
 */
QString screencast::newPauseNameInTmpLocation()
{
  QString myFilename = "screencast-pause";
  QString myFilenameExtension = "." + myUi.VideoContainerComboBox->currentText();
  QString myName = PathTempLocation() + QDir::separator() + myFilename + myFilenameExtension;

  QFile *myFile = new QFile( myName );
  uint x = 0;
  do
  {
    x++;
    myFile->setFileName( PathTempLocation() + QDir::separator() + myFilename + "-" + QString().number( x ) + myFilenameExtension );
    myName = myFile->fileName();
  } while ( myFile->exists() );

  setPauseNameInTmpLocation( myFilename + "-" + QString().number( x ) + myFilenameExtension );
  
  return myFilename + "-" + QString().number( x ) + myFilenameExtension;
}

void screencast::setPauseNameInTmpLocation( QString value )
{
  pauseName = value;
}

QString screencast::getPauseNameInTmpLocation()
{
  return pauseName;
}

QStringList screencast::myAlsa()
{
  QStringList value;
  if ( myUi.AudioOnOffCheckbox->checkState() == Qt::Checked )
  {
    if ( myUi.AlsaRadioButton->isChecked() )  
    {
      QVariant aa = myUi.AlsaHwComboBox->itemData( myUi.AlsaHwComboBox->currentIndex() );
      QvkAlsaDevice *inBox = AlsaCardList.at( aa.toInt() );
      if ( myUi.AlsaHwComboBox->currentIndex() > -1 )
      {
        value << "-f"  << "alsa";
        value << "-ac" << inBox->getChannel();
        value << "-i"  << inBox->getAlsaHw();
      }
    }
    else if ( myUi.PulseDeviceRadioButton->isChecked() )      
    {
      QCheckBox *box;
      int counter = 0;
      QList<QCheckBox *> listQFrame = myUi.scrollAreaWidgetContents->findChildren<QCheckBox *>();
      
      if ( listQFrame.count() > 0 )
      {
        for ( int i = 0; i < listQFrame.count(); i++ )
        {
          box = listQFrame.at( i );
          if (box->checkState() == Qt::Checked)
          {
            counter++;
          }
        }

        if (counter > 1)
        { 
          value << "-f" << "pulse";
          value << "-i" << "vokoscreenMix.monitor";
        }
        else if ( counter > 0 )
        {
          for ( int i = 0; i < listQFrame.count(); i++ )
          {
            box = listQFrame.at( i );
            if (box->checkState() == Qt::Checked)
            {
              value << "-f" << "pulse";
              value << "-name" << "vokoscreen";
              value << "-i" << box->accessibleName();
            }
          }
        }
      }
    }
  }

  return value;
}


QStringList screencast::myAcodec()
{
  QStringList result;
  if ( ( myUi.AudioOnOffCheckbox->checkState() == Qt::Checked ) and ( myUi.AlsaRadioButton->isChecked() ) and ( myUi.AlsaHwComboBox->currentText() > "" ) )
  {
    if ( myUi.AudiocodecComboBox->itemData( myUi.AudiocodecComboBox->currentIndex() ) == true )
      result << "-c:a" << myUi.AudiocodecComboBox->currentText() << "-strict" << "experimental";
    else
      result << "-c:a" << myUi.AudiocodecComboBox->currentText();
  }
  if ( ( myUi.AudioOnOffCheckbox->checkState() == Qt::Checked ) and ( myUi.PulseDeviceRadioButton->isChecked() ) and ( QvkPulse::myPulseDevice( myUi.scrollAreaWidgetContents ) > "" ) )
  {
    if ( myUi.AudiocodecComboBox->itemData( myUi.AudiocodecComboBox->currentIndex() ) == true )
      result << "-c:a" << myUi.AudiocodecComboBox->currentText() << "-strict" << "experimental";
    else
      result << "-c:a" << myUi.AudiocodecComboBox->currentText();
  }

  return result;
}


void screencast::preRecord()
{
  if ( myUi.AlsaRadioButton->isChecked() and myUi.AudioOnOffCheckbox->isChecked() )
  {
    QVariant aa = myUi.AlsaHwComboBox->itemData( myUi.AlsaHwComboBox->currentIndex() );
    QvkAlsaDevice *inBox = AlsaCardList.at( aa.toInt() );
    if ( inBox->isbusy() )
    {
      inBox->busyDialog( inBox->getAlsaHw(), inBox->getPurAlsaName() );
      return;
    }
    else
    {
      // Kanäle werden kurz vor der Aufnahme ermittelt
      inBox->setChannel();
    }
  }
  
  if ( myUi.WindowRadioButton->isChecked() )
    if ( firststartWininfo == false )
    {
      vkWinInfo = new QvkWinInfo();
      connect( vkWinInfo, SIGNAL( windowChanged() ), this, SLOT( record() ) );
      return;
    }
   record();
}


void screencast::Countdown()
{
  if ( myUi.CountdownSpinBox->value() > 0 )
  {
    myUi.recordButton->setEnabled( false );
    myUi.PauseButton->setEnabled( false );
    QvkCountdown *countdown = new QvkCountdown( myUi.CountdownSpinBox->value() );
    (void)countdown;
  } 
}

void screencast::record()
{
  Countdown();
  if ( myUi.MinimizedCheckBox->checkState() == Qt::Checked )
  {
    WindowMinimized();
    hideAction->setText( tr( "Show window" ) );
    hideAction->setData( "NoHide" );
  }

  deltaX = "0";
  deltaY = "0";
  
  if ( myUi.WindowRadioButton->isChecked() and ( firststartWininfo == false) )
  {
      qDebug() << "[vokoscreen]" << "recording window";
      setRecordWidth( vkWinInfo->width() );
      setRecordHeight( vkWinInfo->height() );
      deltaX = QString::number( vkWinInfo->x().toUInt() );
      deltaY = QString::number( vkWinInfo->y().toUInt() );

      moveWindowID = vkWinInfo->getWinID();
      
      deltaXMove = deltaX;
      deltaYMove = deltaY;

      windowMoveTimer->start( 100 );
      firststartWininfo = true;
  }

  if( myUi.FullScreenRadioButton->isChecked() )
  {
      int screen = myUi.ScreenComboBox->itemData( myUi.ScreenComboBox->currentIndex() ).toInt();
      qDebug() << "[vokoscreen]" << "recording fullscreen Display: " << screen;

      int fullScreenWidth = 0;
      int fullScreenHeight = 0;

      QDesktopWidget *desk = QApplication::desktop();
      for ( int i = 0; i < desk->screenCount(); i++ )
      {
          // skip if we are recording a specific screen and this isn't it.
          if( screen != -1 && screen != i ) continue;
          // set the offset if we are recording a specific screen.
          if( screen != -1 )
          {
              deltaX = QString::number( desk->screenGeometry( i ).left() );
              deltaY = QString::number( desk->screenGeometry( i ).top() );
          }
          fullScreenWidth += desk->screenGeometry( i ).width();
          fullScreenHeight = std::max( fullScreenHeight, desk->screenGeometry( i ).height() );
      }
      QScreen *qscreen = QGuiApplication::primaryScreen();    
      setRecordWidth( QString::number( fullScreenWidth * qscreen->devicePixelRatio() ) );
      setRecordHeight( QString::number( fullScreenHeight * qscreen->devicePixelRatio() ) );
  }  

  if ( myUi.AreaRadioButton->isChecked() )
  {
    setRecordWidth( QString().number( myregionselection->getWidth() ) );
    setRecordHeight( QString().number( myregionselection->getHeight() ) );
    deltaX  = QString().number( myregionselection->getXRecordArea() );
    deltaY  = QString().number( myregionselection->getYRecordArea() );
    //Makes the rectangle unmovable and unresizeable (Is enabled yet again when process finished)
    myregionselection->lockFrame( true );

    qDebug() << "[vokoscreen]" << "recording area";
  }
  
  ffmpegProgram = myUi.RecorderLineEdit->displayText();
 
  // set working directory for writing and delete the ffmpegLog from Profil directory
  QSettings settings( vkSettings.getProgName(), vkSettings.getProgName() );
  QFileInfo settingsPath( settings.fileName() );
  QFile file( settingsPath.absolutePath() );
  QString workDirectory = file.fileName();
  QDir Dir( "" );
  Dir.setCurrent( workDirectory );  
  
  // bestehendes ffmpegLog löschen
  QDir Dira( "" );
  QStringList filters( QStringList() << "ffmpeg*" );
  QStringList List = Dira.entryList( filters, QDir::Files, QDir::Name );
  QFile FfmpegFile;
  for (int i = 0; i < List.size(); ++i)
     FfmpegFile.remove( List.at(i) );
  
  // Make sure the video is saved in one of the following three folders.
  // Rank order:
  // 1. Path set in vokoscreen GUI
  // 2. Then system path from desktop
  // 3. Then home
  QDir dir_0( myUi.SaveVideoPathLineEdit->displayText() );
  if ( dir_0.exists() )
  {
    moviePath = myUi.SaveVideoPathLineEdit->displayText();
  }
  else
  {
    QDir dir_1( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );
    if ( dir_1.exists() )
    {
      moviePath = QStandardPaths::writableLocation( QStandardPaths::MoviesLocation );
    }
    else
    {
      moviePath = QStandardPaths::writableLocation( QStandardPaths::HomeLocation );
    }
  }
  
  nameInMoviesLocation = NameInMoviesLocation();

  // evtl. müßte hier erst mal nachgeschaut werden ob Temp leer ist.
  // Clean vokoscreen temp 
  QDir dir( PathTempLocation() );
  QStringList stringList = dir.entryList( QDir::Files, QDir::Time | QDir::Reversed );
  for ( int i = 0; i < stringList.size(); ++i )
     dir.remove( PathTempLocation().append(QDir::separator() ).append(stringList.at( i ) ) );

  // framerate
  QString framerate = "-framerate " + QString().number( myUi.FrameSpinBox->value() );

  QString videoCodec = myUi.VideocodecComboBox->currentText();
  QStringList videoFlags;
  if ( videoCodec == "libx264" )
  {
    videoFlags << "-preset" << "veryfast";
  }  

  // https://trac.ffmpeg.org/wiki/Encode/H.265
  if ( videoCodec == "libx265" )
  {
    videoFlags << "-preset" << "veryfast";
    videoFlags << "-x265-params" << "crf=20";
  }  
  
  // Number of pixels must be divisible by two
  int intRecordX = getRecordWidth().toInt();
  if ( ( intRecordX % 2 ) == 1 )
    setRecordWidth( QString().number( --intRecordX ) );

  // Number of pixels must be divisible by two
  int intRecordY = getRecordHeight().toInt();
  if ( ( intRecordY % 2 ) == 1 )
    setRecordHeight( QString().number( --intRecordY ) );
  
  ffmpegInputArguments.clear();
  ffmpegInputArguments << "-report";
  ffmpegInputArguments << "-f" << "x11grab";
  ffmpegInputArguments << "-draw_mouse" << ((myUi.HideMouseCheckbox->checkState() == Qt::Checked) ? "0" : "1");
  ffmpegInputArguments << "-framerate" << QString().number(myUi.FrameSpinBox->value());
  ffmpegInputArguments << "-video_size" << (getRecordWidth() + "x" + getRecordHeight());
  
  ffmpegOutputArguments.clear();
  ffmpegOutputArguments << myAlsa();
  ffmpegOutputArguments << "-pix_fmt" << "yuv420p";
  ffmpegOutputArguments << "-c:v" << videoCodec << videoFlags;
  if(myUi.x264LosslessCheckBox->isChecked())
  {
  	ffmpegOutputArguments << "-qp" << "0";
  }
  ffmpegOutputArguments << myAcodec();
  ffmpegOutputArguments << "-q:v" << "1";
  ffmpegOutputArguments << "-s" << (getRecordWidth() + "x" + getRecordHeight());
  ffmpegOutputArguments << "-f" << myUi.VideoContainerComboBox->itemData(myUi.VideoContainerComboBox->currentIndex()).toString();
  
  startRecord((PathTempLocation() + QDir::separator() + nameInMoviesLocation), deltaX, deltaY);
}


void screencast::startRecord(QString RecordPathName, QString x, QString y)
{
  
  if ( myUi.PulseDeviceRadioButton->isChecked() )
  {
    QProcess Process;
    QString value = QvkPulse::myPulseDevice( myUi.scrollAreaWidgetContents );
    if ( value == "vokoscreenMix.monitor" )
    {
      Process.start("pactl", QStringList() << "load-module" << "module-null-sink" << "sink_name=vokoscreenMix");
      Process.waitForFinished( 3000 );
      QString modulNumber = Process.readAllStandardOutput();
      Process.close();
      qDebug();
      qDebug() << "[vokoscreen] ---Begin Pulse loade module---";
      modulNumber.replace("\n", "");
      debugCommandInvocation("Pulse Command", Process.program(), Process.arguments(), " number: " + modulNumber);
    
      QList<QCheckBox *> listQFrame = myUi.scrollAreaWidgetContents->findChildren<QCheckBox *>();
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
        Process.start("pactl", QStringList() << "load-module" << "module-loopback" << ("source=" + box->accessibleName()) << "sink=vokoscreenMix");
        Process.waitForFinished( 3000 );
        QString modulNumber = Process.readAllStandardOutput();
        modulNumber.replace("\n", "");
        Process.close();
        debugCommandInvocation("Pulse Command", Process.program(), Process.arguments(), " number: " + modulNumber);
      }
      qDebug() << "[vokoscreen] ---End Pulse loade module---";
      qDebug( " " );
    }
  
    if ( value > "" )
    {
      QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
      env.insert( "PULSE_SOURCE", value );
      SystemCall->setProcessEnvironment( env );
    }
  }

  // Add invocation paramters that may be different after pausing
  QStringList arguments;
  arguments << ffmpegInputArguments;
  arguments << "-i" << (DISPLAY + "+" + x + "," + y);
  arguments << ffmpegOutputArguments;
  arguments << RecordPathName;

  debugCommandInvocation("Executing command", ffmpegProgram, arguments);
  qDebug( " " );

  SystemCall->start(ffmpegProgram, arguments);

  beginTime  = QDateTime::currentDateTime();
}


void screencast::Stop()
{
  if ( SystemCall->state() == QProcess::Running )
  {
    SystemCall->terminate();
    SystemCall->waitForFinished( 3000 );
  }
  
  if ( ( pause == true ) and (  myUi.VideocodecComboBox->currentText() != "gif" ) )
  {
    QDir dir( PathTempLocation() );
    QStringList stringList = dir.entryList(QDir::Files, QDir::Time | QDir::Reversed);
    QString mergeFile = QStandardPaths::writableLocation( QStandardPaths::TempLocation ) + QDir::separator() + "mergeFile.txt";
    QFile file( mergeFile );
    file.open( QIODevice::WriteOnly | QIODevice::Text );
      QString videoFiles;
      for ( int i = 0; i < stringList.size(); ++i )
      {
        QString filepath = PathTempLocation() + QDir::separator() + stringList[i];
        videoFiles.append( "file " ).append("'" + filepath.replace("'", "\\'") + "'").append( "\n" );
        file.write( videoFiles.toLatin1() );
        videoFiles = "";
      }
    file.close();

	QStringList mergeArguments;
	mergeArguments << "-report";
	mergeArguments << "-safe" << "0";
	mergeArguments << "-f" << "concat";
	mergeArguments << "-i" << mergeFile;
	mergeArguments << "-c" << "copy";
	mergeArguments << (moviePath + QDir::separator() + nameInMoviesLocation);
    SystemCall->start(ffmpegProgram, mergeArguments);
    SystemCall->waitForFinished(8000);

    for ( int i = 0; i < stringList.size(); ++i )
      dir.remove( PathTempLocation().append( QDir::separator() ).append( stringList.at( i ) ) );

    file.remove();

    debugCommandInvocation("Mergestring", ffmpegProgram, mergeArguments);
   }
   else    
  {
    QString FileInTemp = PathTempLocation() + QDir::separator() + nameInMoviesLocation;
    QFile::copy ( FileInTemp, moviePath + QDir::separator() + nameInMoviesLocation );
    QFile::remove ( FileInTemp );
  }
  
  QDir dir_1;
  dir_1.rmdir( PathTempLocation() );
  
  pause = false;
  windowMoveTimer->stop();
  firststartWininfo = false;

  QvkPulse::pulseUnloadModule();
  
}
