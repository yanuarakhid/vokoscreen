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

using namespace std;

screencast::screencast()
{
    vkSettings.readAll();
 
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
    QvkAlsaDevice inBox ;
    qDebug() << "[vokoscreen]" << "asoundlib version:" << inBox.getAlsaVersion();
    qDebug() << "[vokoscreen] current icon-theme:" << QIcon::themeName();
    qDebug();

    searchExternalPrograms();
    
    pause = false;
    firststartWininfo = false;
    
    QWidget *centralWidget = new QWidget( this );
    centralWidget->setObjectName( QString::fromUtf8( "centralWidget" ) );
    this->setCentralWidget( centralWidget );

    tabWidget = new QTabWidget( centralWidget );
    tabWidget->setGeometry( 120, 0, 450, 190 );
    tabWidget->setIconSize( QSize( 32, 32 ) );

    // Tab 1 Screen options ***************************************************
    QFrame *frame = new QFrame( this );
    frame->setGeometry( 120, 10, 300, 200 );
    frame->show();
    tabWidget->addTab( frame, "" );
    tabWidget->setTabIcon( 0, QIcon::fromTheme( "video-display", QIcon( ":/pictures/monitor.png" ) ) );
    QFont qfont = frame->font();
    qfont.setPixelSize( 12 );
    frame->setFont( qfont );
 
    FullScreenRadioButton = new QRadioButton( frame );
    FullScreenRadioButton->setGeometry( QRect( 20, 15, 120, 21 ) );
    FullScreenRadioButton->setText( tr( "Fullscreen" ) );
    FullScreenRadioButton->setChecked( true );
    connect( FullScreenRadioButton, SIGNAL( clicked() ), SLOT( clickedScreenSize() ) );

    WindowRadioButton = new QRadioButton( frame );
    WindowRadioButton->setGeometry( QRect( 20, 40, 85, 21 ) );
    WindowRadioButton->setText( tr( "Window" ) );
    connect( WindowRadioButton, SIGNAL( clicked() ), SLOT( clickedScreenSize() ) );

    AreaRadioButton = new QRadioButton( frame );
    AreaRadioButton->setGeometry( QRect( 20, 65, 85, 21 ) );
    AreaRadioButton->setText( tr( "Area" ) );
    connect( AreaRadioButton, SIGNAL( clicked() ), SLOT( clickedScreenSize() ) );
    
    MagnifierCheckBox = new QCheckBox( frame );
    MagnifierCheckBox->setText( tr( "Magnification" ) );
    MagnifierCheckBox->setGeometry( QRect( 160, 40, 120, 21 ) );
    MagnifierCheckBox->setToolTip( "CTRL+SHIFT+F9" );
    MagnifierCheckBox->show();
    connect( MagnifierCheckBox, SIGNAL( clicked() ), SLOT( showMagnifier() ) );

    magnifier = new QvkMagnifier();
    magnifier->close();
    connect( magnifier, SIGNAL( closeMagnifier() ), SLOT( uncheckMagnifier() ) );

    QPushButton *MagnifierDialogPushButton = new QPushButton( frame );
    MagnifierDialogPushButton->setGeometry( 270, 40, 20, 21 );
    MagnifierDialogPushButton->setText( "..." );
    MagnifierDialogPushButton->show();
    connect( MagnifierDialogPushButton, SIGNAL( clicked() ), SLOT( MagnifierDialog() ) );
    
    QLabel *CountdownLabel = new QLabel( frame );
    CountdownLabel->setGeometry( 160, 110, 80, 25 );
    CountdownLabel->setText( tr( "Countdown" ) );
    CountdownLabel->show();
    
    CountdownSpinBox = new QSpinBox( frame );
    CountdownSpinBox->setGeometry( 250, 110, 50, 21 );
    CountdownSpinBox->setMinimum( 0 );
    CountdownSpinBox->setMaximum( 999 );
    CountdownSpinBox->setSingleStep( 1 );
    CountdownSpinBox->setValue( 0 );
    CountdownSpinBox->show();
    
    ScreenComboBox = new QComboBox( frame );
    ScreenComboBox->setGeometry( 160, 15, 200, 21 );
    QDesktopWidget *desk = QApplication::desktop();
    myScreenCountChanged( desk->screenCount() );
    connect( desk, SIGNAL( screenCountChanged(int) ), SLOT( myScreenCountChanged(int) ) );
    connect( desk, SIGNAL( resized(int) ), SLOT( myScreenCountChanged(int) ) );

    // Give the Display :0 or :1 etc.
    qDebug() << "[vokoscreen]" << "---Begin Environment---";
    DISPLAY = qgetenv( "DISPLAY" );
    qDebug() << "[vokoscreen] runs on DISPLAY" << DISPLAY;
    qDebug() << "[vokoscreen]" << "---End Environment---";
    qDebug();
    
    ScreenkeyQCheckBox = new QCheckBox( frame );
    ScreenkeyQCheckBox->setGeometry( 160, 65, 200, 21 );
    ScreenkeyQCheckBox->setText( tr( "Showkey" ) );
    screenkey = new QvkScreenkey();
    screenkeyWindow = new QvkScreenkeyWindow();
    connect( ScreenkeyQCheckBox, SIGNAL( clicked() ), SLOT( screenkeyReadKey() ) );
    connect( screenkey, SIGNAL( pressedKey( QString ) ), SLOT( showScreenkeyWindow( QString ) ) );
    
    
    // Tab 2 Audio options ****************************************
    TabWidgetAudioFrame = new QFrame( this );
    TabWidgetAudioFrame->setGeometry( 120, 0, 300, 290 );
    TabWidgetAudioFrame->show();
    tabWidget->addTab( TabWidgetAudioFrame, "" );
    tabWidget->setTabIcon( 1, QIcon::fromTheme( "audio-input-microphone", QIcon( ":/pictures/micro.png" ) ) );
    qfont = TabWidgetAudioFrame->font();
    qfont.setPixelSize( 12 );
    TabWidgetAudioFrame->setFont( qfont );

    AudioOnOffCheckbox = new QCheckBox( TabWidgetAudioFrame );
    AudioOnOffCheckbox->setGeometry( 10, 0, 100, 25 );
    AudioOnOffCheckbox->setText( tr( "Audio" ) );
    AudioOnOffCheckbox->show();
    connect( AudioOnOffCheckbox,  SIGNAL( stateChanged( int ) ), SLOT( stateChangedAudio( int ) ) );

    AlsaRadioButton= new QRadioButton( TabWidgetAudioFrame );
    AlsaRadioButton->setGeometry( 25, 110, 100, 25 );
    AlsaRadioButton->setText( tr( "Alsa" ) );
    AlsaRadioButton->show();
    connect( AlsaRadioButton,  SIGNAL( clicked( bool )  ), SLOT( clickedAudioAlsa( bool ) ) );
    
    AlsaHwComboBox = new QComboBox( TabWidgetAudioFrame );
    AlsaHwComboBox->setGeometry( 90, 110, 345, 25 );
    AlsaHwComboBox->show();
    
    Pulseframe = new QFrame();

    PulseDeviceRadioButton = new QRadioButton( TabWidgetAudioFrame );
    PulseDeviceRadioButton->setGeometry( 25, 20, 345, 25 );
    PulseDeviceRadioButton->setText( tr( "Pulse" ) );
    PulseDeviceRadioButton->show();
    connect( PulseDeviceRadioButton,  SIGNAL( clicked( bool )  ), SLOT( clickedAudioPulse( bool ) ) );

    // Tab 3 Video options **************************************************
    TabWidgetVideoOptionFrame = new QFrame( this );
    TabWidgetVideoOptionFrame->setGeometry( 120, 0, 300, 200 );
    TabWidgetVideoOptionFrame->show();
    tabWidget->addTab( TabWidgetVideoOptionFrame, "" );
    tabWidget->setTabIcon( 2, QIcon::fromTheme( "applications-multimedia", QIcon( ":/pictures/videooptionen.png" ) ) );

    qfont = TabWidgetVideoOptionFrame->font();
    qfont.setPixelSize( 12 );
    TabWidgetVideoOptionFrame->setFont( qfont );

    QLabel *VideoOptionLabel = new QLabel( TabWidgetVideoOptionFrame );
    VideoOptionLabel->setGeometry( 20, 10, 50, 25 );
    VideoOptionLabel->setText( tr( "Frames" ) );
    VideoOptionLabel->show();

    FrameSpinBox = new QSpinBox( TabWidgetVideoOptionFrame );
    FrameSpinBox->setGeometry( QRect( 100, 10, 50, 25 ) );
    FrameSpinBox->setMinimum( 1 );
    FrameSpinBox->setMaximum( 200 );
    FrameSpinBox->setSingleStep( 1 );
    FrameSpinBox->setValue( 25 );
    FrameSpinBox->show();
    connect( FrameSpinBox, SIGNAL( valueChanged( int ) ), SLOT( valueChangedFrames( int ) ) );

    QPushButton *FrameStandardButton = new QPushButton( TabWidgetVideoOptionFrame );
    FrameStandardButton->setIcon ( QIcon::fromTheme( "edit-undo", QIcon( ":/pictures/undo.png" ) ) );
    FrameStandardButton->setText( "" );
    FrameStandardButton->setGeometry( 350, 10, 50, 25 );
    FrameStandardButton->setToolTip( tr( "Default" ) );
    FrameStandardButton->show();
    connect( FrameStandardButton, SIGNAL( clicked() ), SLOT( setFrameStandardSpinbox() ) );

    QLabel *VideocodecOptionLabel = new QLabel( TabWidgetVideoOptionFrame );
    VideocodecOptionLabel->setGeometry( 20, 40, 80, 25 );
    VideocodecOptionLabel->setText( tr( "Videocodec" ) );
    VideocodecOptionLabel->show();

    VideocodecComboBox = new QComboBox( TabWidgetVideoOptionFrame );
    VideocodecComboBox->setGeometry( 100, 40, 100, 25 );
    VideocodecComboBox->show();
    VideocodecComboBox->addItem( "libx264" );
    VideocodecComboBox->addItem( "mpeg4" );
    connect( VideocodecComboBox, SIGNAL( currentIndexChanged( int ) ), SLOT( currentIndexChangedCodec( int ) ) );
    
    QLabel *VideoContainerLabel = new QLabel(TabWidgetVideoOptionFrame );
    VideoContainerLabel->setGeometry( 210, 40, 50, 25 );
    VideoContainerLabel->setText( tr( "Format" ) );
    VideoContainerLabel->show();
    
    VideoContainerComboBox = new QComboBox( TabWidgetVideoOptionFrame );
    VideoContainerComboBox->setGeometry( 260, 40, 70, 25 );
    VideoContainerComboBox->addItem( "avi" );
    VideoContainerComboBox->addItem( "mkv" );
    VideoContainerComboBox->show();
    connect( VideoContainerComboBox, SIGNAL( currentIndexChanged( int ) ), SLOT( currentIndexChangedFormat( int ) ) );
    
    QPushButton *VideocodecStandardButton = new QPushButton( TabWidgetVideoOptionFrame );
    VideocodecStandardButton->setIcon ( QIcon::fromTheme( "edit-undo", QIcon( ":/pictures/undo.png" ) ) );
    VideocodecStandardButton->setText( "" );
    VideocodecStandardButton->setGeometry( 350, 40, 50, 25 );
    VideocodecStandardButton->setToolTip( tr( "Default" ) );
    VideocodecStandardButton->show();
    connect( VideocodecStandardButton, SIGNAL( clicked() ), SLOT( setVideocodecStandardComboBox() ) );
    
    QLabel *AudiocodecLabel = new QLabel( TabWidgetVideoOptionFrame );
    AudiocodecLabel->setGeometry( 20, 70, 80, 25 );
    AudiocodecLabel->setText( tr( "Audiocodec" ) );
    AudiocodecLabel->show();
    
    AudiocodecComboBox = new QComboBox( TabWidgetVideoOptionFrame );
    AudiocodecComboBox->setGeometry( 100, 70, 100, 25 );
    AudiocodecComboBox->show();
    AudiocodecComboBox->addItem( "libmp3lame" );
    AudiocodecComboBox->addItem( "libvorbis" );
    
    QPushButton *AudiocodecStandardButton = new QPushButton( TabWidgetVideoOptionFrame );
    AudiocodecStandardButton->setIcon ( QIcon::fromTheme( "edit-undo", QIcon( ":/pictures/undo.png" ) ) );
    AudiocodecStandardButton->setText( "" );
    AudiocodecStandardButton->setGeometry( 350, 70, 50, 25 );
    AudiocodecStandardButton->setToolTip( tr( "Default" ) );
    AudiocodecStandardButton->show();
    connect( AudiocodecStandardButton, SIGNAL( clicked() ), SLOT( setAudiocodecStandardComboBox() ) );
    
    HideMouseCheckbox = new QCheckBox( TabWidgetVideoOptionFrame );
    HideMouseCheckbox->setGeometry( 20, 100, 300, 25 );
    HideMouseCheckbox->setText( tr( "Do not record mouse cursor" ) );
    HideMouseCheckbox->show();

    // Tab 4 Miscellaneous options **************************************************
    TabWidgetMiscellaneousFrame = new QFrame( this );
    TabWidgetMiscellaneousFrame->setGeometry( 120, 0, 300, 200 );
    TabWidgetMiscellaneousFrame->show();
    tabWidget->addTab(TabWidgetMiscellaneousFrame, "" );
    tabWidget->setTabIcon( 3, QIcon::fromTheme( "preferences-system", QIcon( ":/pictures/tools.png" ) ) );
    qfont = TabWidgetMiscellaneousFrame->font();
    qfont.setPixelSize( 12 );
    TabWidgetMiscellaneousFrame->setFont( qfont );

    QLabel *SaveVideoPathLabel = new QLabel( TabWidgetMiscellaneousFrame );
    SaveVideoPathLabel->setGeometry( 30, 10, 100, 25 );
    SaveVideoPathLabel->setText( tr( "Videopath" ) );
    SaveVideoPathLabel->show();

    SaveVideoPathLineEdit = new QLineEdit( TabWidgetMiscellaneousFrame );
    SaveVideoPathLineEdit->setGeometry( 140, 10, 210, 25 );
    SaveVideoPathLineEdit->setReadOnly( true );
    SaveVideoPathLineEdit->show();
    
    QPushButton *SaveVideoPathPushButton = new QPushButton( TabWidgetMiscellaneousFrame );
    SaveVideoPathPushButton->setGeometry( 350, 10, 20, 25);
    SaveVideoPathPushButton->setText( "..." );
    SaveVideoPathPushButton->show();
    connect( SaveVideoPathPushButton, SIGNAL(clicked() ), SLOT( saveVideoPath() ) );
    
    QLabel *VideoPlayerLabel = new QLabel(TabWidgetMiscellaneousFrame);
    VideoPlayerLabel->setGeometry( 30, 35, 100, 25 );
    VideoPlayerLabel->setText( tr( "Player" ) );
    VideoPlayerLabel->show();
    
    VideoplayerComboBox = new QComboBox( TabWidgetMiscellaneousFrame );
    VideoplayerComboBox->setGeometry( 140, 35, 210, 25 );
    VideoplayerComboBox->show();

    MinimizedCheckBox = new QCheckBox( TabWidgetMiscellaneousFrame );
    MinimizedCheckBox->setGeometry( 30, 60, 350, 25 );
    MinimizedCheckBox->setText( tr( "Vokoscreen minimized when recording starts" ) );
    MinimizedCheckBox->show();
    
    SystrayCheckBox = new QCheckBox( TabWidgetMiscellaneousFrame );
    SystrayCheckBox->setGeometry( 30, 85, 350, 25 );
    SystrayCheckBox->setText( tr( "Show in systray" ) );
    SystrayCheckBox->setCheckState( Qt::Checked );
    SystrayCheckBox->show();
    connect( SystrayCheckBox, SIGNAL( stateChanged( int ) ), SLOT( stateChangedSystray( int ) ) );
    
    QLabel *recorderLabel = new QLabel( TabWidgetMiscellaneousFrame );
    recorderLabel->setGeometry( 30, 110, 100, 25 );
    recorderLabel->setText( tr( "Recorder" ) );
    recorderLabel->show();

    RecorderLineEdit = new QLineEdit( TabWidgetMiscellaneousFrame );
    RecorderLineEdit->setGeometry( 140, 110, 210, 25 );
    RecorderLineEdit->setReadOnly( true );
    RecorderLineEdit->show();
    connect( RecorderLineEdit, SIGNAL( textChanged( QString ) ), SLOT( recorderLineEditTextChanged( QString ) ) );
    RecorderLineEdit->setText( getFileWithPath( vkSettings.getRecorder() ) );

    QPushButton *selectRecorderPushButton = new QPushButton( TabWidgetMiscellaneousFrame );
    selectRecorderPushButton->setGeometry( 350, 110, 20, 25);
    selectRecorderPushButton->setText( "..." );
    selectRecorderPushButton->show();
    connect( selectRecorderPushButton, SIGNAL(clicked() ), SLOT( selectRecorder() ) );
    

    // Tab 5 Webcam *******************************************************
    TabWidgetWebcamFrame = new QFrame( this );
    TabWidgetWebcamFrame->setGeometry( 120, 0, 300, 200 );
    TabWidgetWebcamFrame->show();
    tabWidget->addTab( TabWidgetWebcamFrame, "" );
    tabWidget->setTabIcon( 4, QIcon::fromTheme( "camera-web", QIcon( ":/pictures/webcam.png" ) ) );
    qfont = TabWidgetWebcamFrame->font();
    qfont.setPixelSize( 12 );
    TabWidgetWebcamFrame->setFont( qfont );

    webcamCheckBox = new QCheckBox( TabWidgetWebcamFrame );
    webcamCheckBox->setText( tr( "Webcam" ) );
    webcamCheckBox->setToolTip( "CTRL+SHIFT+F8" );
    webcamCheckBox->setGeometry( 20, 5, 120, 25 );
    webcamCheckBox->show();
  
    QComboBox *webcamComboBox = new QComboBox( TabWidgetWebcamFrame );
    webcamComboBox->setGeometry( 120, 5, 220, 25 );
    webcamComboBox->setToolTip( tr ( "Select webcam" ) );
    webcamComboBox->show();

    mirrorCheckBox = new QCheckBox( TabWidgetWebcamFrame );
    mirrorCheckBox->setText( tr( "Mirrored" ) );
    mirrorCheckBox->setGeometry( 20, 70, 200, 25 );
    mirrorCheckBox->show();

    QFrame *dialFrame = new QFrame( TabWidgetWebcamFrame );
    dialFrame->setFrameShape( QFrame::StyledPanel );
    dialFrame->setFrameShadow( QFrame::Sunken );
    dialFrame->setGeometry( 200, 40, 86, 86 );
    dialFrame->show();
  
    int dialWith = 40;
    int dialheight = 40;
    rotateDial = new QDial( dialFrame );
    rotateDial->setWrapping ( true );
    rotateDial->setGeometry( dialFrame->width() / 2 - dialWith / 2, 
			     dialFrame->height() / 2 - dialheight / 2, 
			     dialWith,
			     dialheight );

    int radioButtonwidth = 20;
    int radioButtonheight = 20;
    radioButtonTopMiddle = new QRadioButton( dialFrame );
    radioButtonTopMiddle->setGeometry( rotateDial->x() + rotateDial->width() / 2 - radioButtonwidth / 2,
                                       rotateDial->y() - radioButtonwidth,
				       radioButtonwidth,
				       radioButtonheight );
  
    radioButtonRightMiddle = new QRadioButton( dialFrame );
    radioButtonRightMiddle->setGeometry( rotateDial->x() + rotateDial->width(),
                                         rotateDial->y() + rotateDial->height() / 2 - radioButtonheight / 2,
                                         radioButtonwidth,
				         radioButtonheight );
   
    radioButtonBottomMiddle = new QRadioButton( dialFrame );
    radioButtonBottomMiddle->setGeometry( rotateDial->x() + rotateDial->width() / 2 - radioButtonwidth / 2,
                                       rotateDial->y() + rotateDial->height(),
                                       radioButtonwidth,
	 			       radioButtonheight );
 
    radioButtonLeftMiddle = new QRadioButton( dialFrame );
    radioButtonLeftMiddle->setGeometry( rotateDial->x() - radioButtonwidth,
                                        rotateDial->y() + rotateDial->height() / 2 - radioButtonheight / 2,
                                        radioButtonwidth,
				        radioButtonheight );

    webcamController = new QvkWebcamController( webcamCheckBox, webcamComboBox, mirrorCheckBox, 
						dialFrame, rotateDial, radioButtonTopMiddle, radioButtonRightMiddle, radioButtonBottomMiddle, radioButtonLeftMiddle );
    (void)webcamController;
    
    // Tab 6 About *********************************************************
    QFrame *TabWidgetAboutFrame = new QFrame(this);
    TabWidgetAboutFrame->show();
    tabWidget->addTab( TabWidgetAboutFrame, "" );
    tabWidget->setTabIcon( 5, QIcon::fromTheme( "dialog-information", QIcon( ":/pictures/about.png" ) ) );
    tabWidget->show();
    qfont = TabWidgetAboutFrame->font();
    qfont.setPixelSize( 12 );
    TabWidgetAboutFrame->setFont( qfont );

    int labelWidth = tabWidget->width() / 2;
    int leftSide = 0;
    int rightSide = tabWidget->width() / 2;
    
    QLabel* labelOpensuseBetaUrl = new QLabel( TabWidgetAboutFrame );
    labelOpensuseBetaUrl->setText( "<a href='http://linuxecke.volkoh.de/vokoscreen/vokoscreen.html'>" + tr( "Developer Homepage" ) + "</a>" );
    labelOpensuseBetaUrl->setGeometry( leftSide, 10, labelWidth, 22 );
    labelOpensuseBetaUrl->setOpenExternalLinks( true );
    labelOpensuseBetaUrl->setAlignment( Qt::AlignCenter );    
    labelOpensuseBetaUrl->show();
    
    QLabel* labelWebSite = new QLabel( TabWidgetAboutFrame );
    labelWebSite->setGeometry( leftSide, 30, labelWidth, 22 );
    labelWebSite->setText( homepage );
    labelWebSite->setOpenExternalLinks( true );
    labelWebSite->setAlignment( Qt::AlignCenter );
    labelWebSite->show();

    QLabel* labelMail = new QLabel( TabWidgetAboutFrame );
    labelMail->setGeometry( leftSide, 50, labelWidth, 22 );
    labelMail->setText( email );
    labelMail->setOpenExternalLinks( true );
    labelMail->setAlignment( Qt::AlignCenter );    
    labelMail->show();

    QLabel* labelDeveLoperMail = new QLabel( TabWidgetAboutFrame );
    labelDeveLoperMail->setText( emaildeveloper );
    labelDeveLoperMail->setGeometry( leftSide, 70, labelWidth, 22 );
    labelDeveLoperMail->setOpenExternalLinks( true );
    labelDeveLoperMail->setAlignment( Qt::AlignCenter );    
    labelDeveLoperMail->show();
   
    QLabel* labelSourcecodeUrl = new QLabel( TabWidgetAboutFrame );
    labelSourcecodeUrl->setText( "<a href='https://github.com/vkohaupt/vokoscreen'>" + tr( "Sourcecode" ) + "</a>" );
    labelSourcecodeUrl->setGeometry( rightSide, 10, labelWidth, 22 );
    labelSourcecodeUrl->setOpenExternalLinks( true );
    labelSourcecodeUrl->setAlignment( Qt::AlignCenter );    
    labelSourcecodeUrl->show();
    
    QLabel* labelLanguageUrl = new QLabel( TabWidgetAboutFrame );
    labelLanguageUrl->setText( "<a href='https://www.transifex.com/projects/p/vokoscreen/'>" + tr( "Translations" ) + "</a>" );
    labelLanguageUrl->setGeometry( rightSide, 30, labelWidth, 22 );
    labelLanguageUrl->setOpenExternalLinks( true );
    labelLanguageUrl->setAlignment( Qt::AlignCenter );    
    labelLanguageUrl->show();

    QLabel * labelDonateUrl = new QLabel( TabWidgetAboutFrame );
    labelDonateUrl->setText( "<a href='http://www.kohaupt-online.de/hp/spende.html'>" + tr( "Donate" ) + "</a>" );
    labelDonateUrl->setGeometry( 0, 100, tabWidget->width(), 22 );
    labelDonateUrl->setOpenExternalLinks( true );
    labelDonateUrl->setAlignment( Qt::AlignCenter );    
    labelDonateUrl->show();
    
    creditsQPushButton = new QPushButton( TabWidgetAboutFrame );
    creditsQPushButton->setGeometry( 400, 95, 40, 40 );
    QIcon creditsIcon;
    creditsIcon.addFile( ":/pictures/community.png", QSize(), QIcon::Normal, QIcon::On );
    creditsQPushButton->setIconSize( QSize( 30, 30 ) );
    creditsQPushButton->setIcon( creditsIcon );
    creditsQPushButton->show();
    connect( creditsQPushButton, SIGNAL( clicked() ), SLOT( showCredits() ) );

    // End Tabs *************************************************************

    recordButton = new QPushButton( centralWidget );
    recordButton->setText( tr( "Start" ) );
    recordButton->setToolTip( "CTRL+SHIFT+F10" );
    qfont = recordButton->font();
    qfont.setPixelSize( 14 );
    qfont.setBold( true );
    recordButton->setFont( qfont );
    recordButton->setGeometry( 120, 200, 90, 30 );
    recordButton->show();
    connect( recordButton, SIGNAL( clicked() ), SLOT( preRecord() ) );

    StopButton = new QPushButton( centralWidget );
    StopButton->setText( tr( "Stop" ) );
    StopButton->setToolTip( "CTRL+SHIFT+F11" );
    qfont = StopButton->font();
    qfont.setPixelSize( 14 );
    qfont.setBold( true );
    StopButton->setFont( qfont );
    StopButton->setGeometry( 210, 200, 90, 30 );
    StopButton->setEnabled( false );
    StopButton->show();  
    connect( StopButton, SIGNAL( clicked() ), SLOT( Stop() ) );
    
    PauseButton = new QPushButton( centralWidget );
    PauseButton->setText( tr( "Pause" ) );
    PauseButton->setToolTip( "CTRL+SHIFT+F12" );
    qfont = PauseButton->font();
    qfont.setPixelSize( 14 );
    qfont.setBold( true );
    PauseButton->setFont( qfont );
    PauseButton->setGeometry( 300, 200, 90, 30 );
    PauseButton->setCheckable( true );
    PauseButton->setEnabled( false );
    connect( PauseButton, SIGNAL( clicked() ), SLOT( Pause() ) );

    PlayButton = new QPushButton( centralWidget );
    PlayButton->setText( tr( "Play" ) );
    PlayButton->setToolTip( tr( "Play last Video" ) );
    qfont = PlayButton->font();
    qfont.setPixelSize( 14 );
    qfont.setBold( true );
    PlayButton->setFont( qfont );
    PlayButton->setGeometry( 390, 200, 90, 30 );
    PlayButton->show();
    connect( PlayButton, SIGNAL( clicked() ), SLOT( play() ) );

    sendPushButton = new QPushButton( centralWidget );
    sendPushButton->setGeometry( 480, 200, 90, 30 );
    qfont = sendPushButton->font();
    qfont.setPixelSize( 14 );
    qfont.setBold( true );
    sendPushButton->setFont( qfont );
    sendPushButton->setText( tr( "Send" ) );
    sendPushButton->setToolTip( tr( "Send Video" ) );
    connect( sendPushButton, SIGNAL( clicked() ), SLOT( send() ) );
    if ( searchProgramm( "xdg-email" ) )
      sendPushButton->setEnabled( true );
    else
      sendPushButton->setEnabled( false );

  #ifndef NO_NEW_VERSION_CHECK
    QTimer::singleShot( 15000, &version, SLOT( doDownload() ) );
    connect( &version, SIGNAL( versionDownloadFinish() ), SLOT( buttonVersion() ) );
    updateButton = new QPushButton( frame );
    updateButton->setGeometry( 395, 100, 45, 45 );
    updateButton->setIcon( QIcon( ":/pictures/system-software-update.png" ) );
    updateButton->setIconSize( QSize( 35, 35 ) );
    updateButton->setToolTip( tr( "New version available" ) );
    updateButton->hide();
    connect( updateButton, SIGNAL( clicked() ), SLOT( showHomepage() ) );  
  #endif
    
    QLabel* label = new QLabel( centralWidget );
    label->setText("");
    label->setGeometry( QRect( 10, 0, 100, 190 ) );
    label->setAlignment( Qt::AlignCenter );
    label->show();
    QImage* qImage = new QImage( ":/pictures/VokoCola.png" );
    label->setPixmap(QPixmap::fromImage( *qImage, Qt::AutoColor) );
    label->setScaledContents( true );

    statusBarLabelTime = new QLabel();
    statusBarLabelTime->setText( "00:00:00" );
    statusBarLabelTime->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusBarLabelTime->setToolTip( tr ( "Recording time" ) );

    statusBarLabelFps = new QLabel();
    statusBarLabelFps->setText( "0" );
    statusBarLabelFps->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusBarLabelFps->setToolTip( tr( "Actual frames per second" ) );

    statusBarLabelSize = new QLabel();
    statusBarLabelSize->setText( "0" );
    statusBarLabelSize->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusBarLabelSize->setToolTip( tr( "Size in KB" ) );
    
    statusbarLabelScreenSize = new QLabel();
    statusbarLabelScreenSize->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusbarLabelScreenSize->setToolTip( tr( "Recording screensize" ) );

    statusBarLabelCodec = new QLabel();
    statusBarLabelCodec->setText( VideocodecComboBox->currentText() );
    statusBarLabelCodec->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusBarLabelCodec->setToolTip( tr( "Codec" ) );
    
    statusBarLabelFormat = new QLabel();
    statusBarLabelFormat->setText( VideoContainerComboBox->currentText() );
    statusBarLabelFormat->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusBarLabelFormat->setToolTip( tr( "Format" ) );

    statusBarLabelAudio = new QLabel();
    statusBarLabelAudio->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusBarLabelAudio->setToolTip( tr( "Audio" ) );
    
    statusBarLabelFpsSettings = new QLabel();
    statusBarLabelFpsSettings->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    statusBarLabelFpsSettings->setToolTip( tr( "Settings fps" ) );

    QStatusBar *statusBar = new QStatusBar( this );
    setStatusBar( statusBar );
    
    QLabel * LabelTemp = new QLabel();
    statusBar->addWidget( LabelTemp, 120 );
    
    //statusBar->addWidget( statusBarProgForRecord, 0 );
    statusBar->addWidget( statusBarLabelTime, 0);
    statusBar->addWidget( statusBarLabelFps, 0 );
    statusBar->addWidget( statusBarLabelSize, 0 );
    statusBar->addWidget( statusbarLabelScreenSize, 0 );
    
    statusBar->addWidget( statusBarLabelCodec, 0 );
    statusBar->addWidget( statusBarLabelFormat, 0 );
    statusBar->addWidget( statusBarLabelAudio, 0 );
    statusBar->addWidget( statusBarLabelFpsSettings, 0 );
    
    QLabel * LabelTemp1 = new QLabel();
    statusBar->addWidget( LabelTemp1, 40 );
    
    statusBar->show();
    qfont = statusBar->font();
    qfont.setPixelSize( 12 );
    statusBar->setFont( qfont );
  
    qDebug() << "[vokoscreen]" << "---Begin search Videoplayer---";
    QStringList playerList = QStringList()  << "kaffeine"
                                            << "vlc"
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
					    << "kdenlive";
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
	   QFileInfo playProg( playerName );
	   if ( playProg.fileName() == "kdenlive" )
	     playerName = playerName + " -i";

	   VideoplayerComboBox->addItem( playerList.at( x ), playerName );
           break;
         }
       }
     }
    qDebug() << "[vokoscreen]" << "---End search Videoplayer---";
    qDebug();

      AudioOnOffCheckbox->setCheckState( Qt::CheckState( vkSettings.getAudioOnOff() ) );

      AlsaRadioButton->setChecked( vkSettings.getAlsaSelect() );

      PulseDeviceRadioButton->setChecked( vkSettings.getPulseSelect() );

      FullScreenRadioButton->setChecked( vkSettings.getFullScreenSelect() );
      
      WindowRadioButton->setChecked( vkSettings.getWindowSelect() );
      
      AreaRadioButton->setChecked( vkSettings.getAreaSelect() );

      if ( vkSettings.getVideoPath() > "" )
          SaveVideoPathLineEdit->setText( vkSettings.getVideoPath() );
      else
        PathMoviesLocation();

      int x = VideoplayerComboBox->findText( vkSettings.getVideoPlayer(), Qt::MatchExactly );
      if ( x == -1 )
        VideoplayerComboBox->setCurrentIndex( 0 );
      else
        VideoplayerComboBox->setCurrentIndex( x );
      
      MinimizedCheckBox->setCheckState( Qt::CheckState( vkSettings.getMinimized() ) );
      
      CountdownSpinBox->setValue( vkSettings.getCountdown() );
      
      FrameSpinBox->setValue( vkSettings.getFrames() );

      VideocodecComboBox->setCurrentIndex( VideocodecComboBox->findText( vkSettings.getVideoCodec() ) );
      
      AudiocodecComboBox->setCurrentIndex( AudiocodecComboBox->findText( vkSettings.getAudioCodec() ) );

      VideoContainerComboBox->setCurrentIndex( VideoContainerComboBox->findText( vkSettings.getVideoContainer() ) );

      HideMouseCheckbox->setCheckState( Qt::CheckState( vkSettings.getHideMouse()) );
           
      if ( Qt::CheckState( vkSettings.getWebcamOnOff() ) == Qt::Checked )
        webcamCheckBox->click();
      
      move( vkSettings.getX(),vkSettings.getY() );

      tabWidget->setCurrentIndex( vkSettings.getTab() );
      
      if( Qt::CheckState( vkSettings.getMagnifierOnOff() ) == Qt::Checked )
        MagnifierCheckBox->click();
    
    // Statusbar
    stateChangedAudio( AudioOnOffCheckbox->checkState() );
      statusBarLabelFpsSettings->setText( QString::number( FrameSpinBox->value() ) );
    
    SystemCall = new QProcess( this );

    connect( AudioOnOffCheckbox,     SIGNAL( clicked() ), SLOT( AudioOnOff() ) );
    connect( AlsaRadioButton,        SIGNAL( clicked() ), SLOT( AudioOnOff() ) );
    connect( PulseDeviceRadioButton, SIGNAL( clicked() ), SLOT( AudioOnOff() ) );
    
    connect( SystemCall, SIGNAL( stateChanged ( QProcess::ProcessState) ),this, SLOT( stateChanged( QProcess::ProcessState) ) );
    connect( SystemCall, SIGNAL( error( QProcess::ProcessError) ),        this, SLOT( error( QProcess::ProcessError) ) );
    connect( SystemCall, SIGNAL( readyReadStandardError() ),              this, SLOT( readyReadStandardError() ) );
    connect( SystemCall, SIGNAL( readyReadStandardOutput() ),             this, SLOT( readyReadStandardOutput() ) );

    windowMoveTimer = new QTimer( this );
    connect( windowMoveTimer, SIGNAL( timeout() ), this, SLOT( windowMove() ) );

    // Area ein-ausblenden wenn Radiobutton immer wieder angecklickt wird
    connect( AreaRadioButton,       SIGNAL( clicked() ), SLOT( AreaOnOff() ) );
    connect( FullScreenRadioButton, SIGNAL( clicked() ), SLOT( AreaOnOff() ) );
    connect( WindowRadioButton,     SIGNAL( clicked() ), SLOT( AreaOnOff() ) );
    myregionselection = new regionselection();

    // Clean vokoscreen temp
    QDir dir( PathTempLocation() );
    QStringList stringList = dir.entryList( QDir::Files, QDir::Time | QDir::Reversed );
    for ( int i = 0; i < stringList.size(); ++i )
      dir.remove( PathTempLocation().append( QDir::separator() ).append( stringList.at( i ) ) );

   makeAsoundrc();
   AudioOnOff();
   pulseUnloadModule();

   QAction *vokoscreenAction = new QAction( this );
   vokoscreenAction->setIcon( QIcon( ":/pictures/systray.png" ) );
   vokoscreenAction->setText( "vokoscreen" );
   vokoscreenAction->setEnabled( true );
   qfont = vokoscreenAction->font();
   qfont.setBold( true );
   vokoscreenAction->setFont( qfont );
   
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
   goAction->setIcon( QIcon::fromTheme( "media-skip-forward", QIcon( ":/pictures/go.png" ) ) );
   goAction->setText( tr( "Go" ) );
   goAction->setData( "Go" );
   goAction->setEnabled( false );
   
   hideAction = new QAction( this );
   hideAction->setIcon( QIcon::fromTheme( "video-display", QIcon( ":/pictures/systray.png" ) ) );
   hideAction->setText( tr( "Hide window" ) );
   hideAction->setData( "Hide" );
   
   exitAction = new QAction( this );
   exitAction->setIcon( QIcon::fromTheme( "application-exit"  , QIcon( ":/pictures/systray-exit.png" ) ) );
   exitAction->setText( tr( "Exit" ) );
   exitAction->setData( "Exit" );
   
   SystemTrayMenu = new QMenu();
   SystemTrayMenu->addAction( vokoscreenAction );
   SystemTrayMenu->addSeparator();
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
   
   shortcutWebcam = new QxtGlobalShortcut( this );
   connect( shortcutWebcam, SIGNAL( activated() ), this, SLOT( ShortcutWebcam() ) );
   shortcutWebcam->setShortcut( QKeySequence( "Ctrl+Shift+F8" ) );
   
   shortcutMagnifier = new QxtGlobalShortcut( this );
   connect( shortcutMagnifier, SIGNAL( activated() ), this, SLOT( ShortcutMagnifier() ) );
   shortcutMagnifier->setShortcut( QKeySequence( "Ctrl+Shift+F9" ) );

   shortcutStart = new QxtGlobalShortcut( this );
   connect( shortcutStart, SIGNAL( activated() ), this, SLOT( preRecord() ) );
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
   
   clickedScreenSize();
   AreaOnOff();

   screenkeyTimer = new QTimer( this );
   connect( screenkeyTimer, SIGNAL( timeout() ), this, SLOT( hideScreenkeyWindow() ) );
}


screencast::~screencast()
{
}


void screencast::screenkeyReadKey()
{
  if ( ScreenkeyQCheckBox->checkState() == Qt::Checked )
  {
    screenkey->setScreenkeyOn();
    screenkey->readKey();
  }
  
  if ( ScreenkeyQCheckBox->checkState() == Qt::Unchecked )
  {
    screenkeyWindow->keyLabel->setText( "" );
    screenkey->setScreenkeyOff();
    screenkeyWindow->hide();
  }
}


void screencast::showScreenkeyWindow( QString value)
{
   screenkeyTimer->stop();
   screenkeyTimer->start( 3000 );
   screenkeyWindow->show();
   screenkeyWindow->keyLabel->setText( screenkeyWindow->keyLabel->text() + value );
}


void screencast::hideScreenkeyWindow()
{
  screenkeyWindow->keyLabel->setText( "" );
  screenkeyWindow->hide();
  screenkeyTimer->stop();
}


void screencast::SystemTrayKontextMenue( QAction *action )
{
  QString data = action->data().toString();
  
  if ( data == "Start" )
    recordButton->click();

  if ( data == "Stop" )
    StopButton->click();
  
  if ( data == "Pause" )
    PauseButton->click();
  
  if ( data == "Go" )
    PauseButton->click();
  
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
    show();
  }
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


void screencast::myScreenCountChanged( int newCount )
{
    (void)newCount;
    ScreenComboBox->clear();
    QDesktopWidget *desk = QApplication::desktop();
    qDebug() << "[vokoscreen]" << "---Begin search Screen---";
    qDebug() << "[vokoscreen]" << "Number of screens:" << desk->screenCount();
    qDebug() << "[vokoscreen] Primary screen is: Display" << desk->primaryScreen()+1;
    qDebug() << "[vokoscreen] VirtualDesktop:" << desk->isVirtualDesktop();
    for ( int i = 1; i < desk->screenCount()+1; i++ )
    {
      QString ScreenGeometryX1 = QString::number( desk->screenGeometry( i-1 ).left() );
      QString ScreenGeometryY1 = QString::number( desk->screenGeometry( i-1 ).top() );      
      QString ScreenGeometryX = QString::number( desk->screenGeometry( i-1 ).width() );
      QString ScreenGeometryY = QString::number( desk->screenGeometry( i-1 ).height() );
      ScreenComboBox->addItem( tr( "Display" ) + " " + QString::number( i ) + ":  " + ScreenGeometryX + " x " + ScreenGeometryY, i-1 );
      qDebug() << "[vokoscreen]" << "Display " + QString::number( i ) + ":  " + ScreenGeometryX + " x " + ScreenGeometryY;
    }
    ScreenComboBox->addItem( tr( "All Displays" ), -1 );    
    qDebug() << "[vokoscreen]" << "---End search Screen---";
    qDebug();
}


// Only for testing no funktion
void screencast::styleChange( QStyle &oldStyle )
{
    qDebug() << "[vokoscreen] old styleSheet:" <<  &oldStyle; 
}

// Only for testing no funktion
void screencast::changeEvent( QEvent *event )
{
  switch ( event->type() ) 
  {
    case QEvent::StyleChange:
        qDebug() << "[vokoscreen] current icon-theme" << QIcon::themeName();
        tabWidget->setTabIcon( 0, QIcon::fromTheme( "video-display", QIcon( ":/pictures/monitor.png" ) ) );      
        tabWidget->setTabIcon( 1, QIcon::fromTheme( "audio-input-microphone", QIcon( ":/pictures/micro.png" ) ) );
	break;
    default:
        break;
    }
}


#ifndef NO_NEW_VERSION_CHECK
void screencast::buttonVersion()
{
  QString localVersion = vkSettings.getVersion();
  if ( version.isNewVersionAvailable( localVersion, version.getRemoteVersion() ) )
    updateButton->show();
  else
    updateButton->hide();
}
#endif

void screencast::showHomepage()
{
   QDesktopServices::openUrl( QUrl( "http://www.kohaupt-online.de/hp", QUrl::StrictMode ) );
}


void screencast::showCredits()
{
   creditsQPushButton->setEnabled( false );
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
   creditsQPushButton->setEnabled( true );
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
  QvkMail *vkMail = new QvkMail( this );
  (void)vkMail;
}


/**
 * Statusbar
 */
void screencast::clickedScreenSize()
{
  if ( FullScreenRadioButton->isChecked() )
  {
    if ( SystemCall->state() == QProcess::Running )
      statusbarLabelScreenSize->setText( "F:" + getRecordWidth() + "x" + getRecordHeight() );
    else
      statusbarLabelScreenSize->setText( "F" );
    
    if ( ( SystemCall->state() == QProcess::Running ) or ( PauseButton->isChecked() ) )
      ScreenComboBox->setEnabled( false );
    else
      ScreenComboBox->setEnabled( true );
  }
  
  if ( WindowRadioButton->isChecked() )
  {
    if ( SystemCall->state() == QProcess::Running )
      statusbarLabelScreenSize->setText( "W:" + getRecordWidth() + "x" + getRecordHeight() );
    else
      statusbarLabelScreenSize->setText( "W" );
    
    ScreenComboBox->setEnabled( false );
  }
  
  if ( AreaRadioButton->isChecked() )
  {
    if ( SystemCall->state() == QProcess::Running )
      statusbarLabelScreenSize->setText( "A:" + getRecordWidth() + "x" + getRecordHeight() );
    else
      statusbarLabelScreenSize->setText( "A" );
    
    ScreenComboBox->setEnabled( false );
  }
}


/**
 * Statusbar
 */
void screencast::valueChangedFrames( int i ) 
{
  (void)i;
  statusBarLabelFpsSettings->setText( QString::number( FrameSpinBox->value() ) );
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
     if ( AlsaRadioButton->isChecked() )    
       statusBarLabelAudio->setText( "Alsa" );
     
     if ( PulseDeviceRadioButton->isChecked() )
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
  statusBarLabelCodec->setText( VideocodecComboBox->currentText() );  
}


/**
 * Statusbar
 */
void screencast::currentIndexChangedFormat( int index )
{
 (void)index;
 statusBarLabelFormat->setText( VideoContainerComboBox->currentText() );
}


void screencast::MagnifierDialog()
{
  magnifier->getDialogMagnifier( this );
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
    PlayButton->setEnabled( false );
    sendPushButton->setEnabled( false );
  }
  else
  {
    PlayButton->setEnabled( true );
    sendPushButton->setEnabled( true );
  }
}


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

  QSettings settings( vkSettings.getProgName(), vkSettings.getProgName() );
  settings.beginGroup( "Alsa" );
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
  screenkey->setScreenkeyOff();
  Stop();
  saveSettings();
  myregionselection->close();
  magnifier->close();
  webcamController->webcamCloseEvent();  
}


void screencast::saveSettings()
{
  QSettings settings( vkSettings.getProgName(), vkSettings.getProgName() );
  
  settings.clear();

  settings.beginGroup( "vokoscreen" );
    settings.setValue( "Version", vkSettings.getVersion() );
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
    settings.setValue( "Area", AreaRadioButton->isChecked() );
  settings.endGroup();

  settings.beginGroup( "Miscellaneous" );
    settings.setValue( "VideoPath", SaveVideoPathLineEdit->displayText() );
    settings.setValue( "Videoplayer", VideoplayerComboBox->currentText() );
    settings.setValue( "Minimized", MinimizedCheckBox->checkState() );
    settings.setValue( "Countdown", CountdownSpinBox->value() );
    settings.setValue( "Recorder", RecorderLineEdit->displayText() );
  settings.endGroup();

  settings.beginGroup( "Videooptions" );
    settings.setValue( "Frames", FrameSpinBox->value() );
    settings.setValue( "Videocodec", VideocodecComboBox->currentText() );
    settings.setValue( "Audiocodec", AudiocodecComboBox->currentText() );
    settings.setValue( "Format", VideoContainerComboBox->currentText() );
    settings.setValue( "HideMouse", HideMouseCheckbox->checkState() );    
  settings.endGroup();
  
  settings.beginGroup( "GUI" );
    settings.setValue( "X", x() );
    settings.setValue( "Y", y() );
    settings.setValue( "Tab", tabWidget->currentIndex() );
    settings.setValue( "Systray", SystrayCheckBox->checkState() );
  settings.endGroup();
  
  settings.beginGroup( "Area" );
    settings.setValue( "X", myregionselection->getX() );
    settings.setValue( "Y", myregionselection->getY() );
    settings.setValue( "Width", myregionselection->getWidth() );
    settings.setValue( "Height", myregionselection->getHeight() );
  settings.endGroup();

  settings.beginGroup( "Webcam" );
    settings.setValue( "OnOff", webcamCheckBox->checkState() );
    settings.setValue( "Mirrored", mirrorCheckBox->checkState() );
    settings.setValue( "Rotate", rotateDial->value() );
    settings.setValue( "Top", radioButtonTopMiddle->isChecked() );
    settings.setValue( "Right", radioButtonRightMiddle->isChecked() );
    settings.setValue( "Bottom", radioButtonBottomMiddle->isChecked() );
    settings.setValue( "Left", radioButtonLeftMiddle->isChecked() );
  settings.endGroup();
  webcamController->saveSettings();
  
  settings.beginGroup( "Magnifier" );
    settings.setValue( "OnOff", MagnifierCheckBox->checkState());
    settings.setValue( "FormValue", magnifier->getFormValue() );
  settings.endGroup();
  
}


void screencast::ShortcutWebcam()
{
  webcamCheckBox->click(); 
}

void screencast::ShortcutMagnifier()
{
  MagnifierCheckBox->click();
}


void screencast::showMagnifier()
{
  if ( MagnifierCheckBox->isChecked() )
    magnifier-> magnifierShow();
  else
    magnifier->close(); 
}


void screencast::uncheckMagnifier()
{
  if ( MagnifierCheckBox->checkState() == Qt::Checked )
    MagnifierCheckBox->click();
}


void screencast::ShortcutPause()
{
  PauseButton->click();
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
  qDebug();
}


/**
 * Set standard video codec and options
 */
void screencast::setVideocodecStandardComboBox()
{
  VideocodecComboBox->setCurrentIndex( VideocodecComboBox->findText( "libx264", Qt::MatchExactly ) );
  VideoContainerComboBox->setCurrentIndex( VideoContainerComboBox->findText( "mkv", Qt::MatchExactly ) );
}


/**
 * Set standard Audio codec
 */
void screencast::setAudiocodecStandardComboBox()
{
  AudiocodecComboBox ->setCurrentIndex( AudiocodecComboBox->findText( "libmp3lame", Qt::MatchExactly )  );
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
  #ifdef QT4
  QString homeLocation = QDesktopServices::storageLocation( QDesktopServices:: QDesktopServices::HomeLocation );
  #endif
  
  #ifdef QT5
  QString homeLocation = QStandardPaths::writableLocation( QStandardPaths::HomeLocation );
  #endif  
  
  QString asoundrc = homeLocation + "/.asoundrc";
  
  qDebug() << "[vokoscreen]" << "---Begin search PulseAudio Plugin---";
  QFile qFile( asoundrc );
  if ( not qFile.exists() )
  {
    qFile.open( QIODevice::WriteOnly );
    qDebug() << "[vokoscreen] File not found .asound for PulseAudio Plugin";
    qDebug() << "[vokoscreen] Create file .asound for PulseAudio Plugin";
    QTextStream out( &qFile );
    out << "# create by vokoscreen " << vkSettings.getVersion() << "\n";
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


void screencast::recorderLineEditTextChanged( QString recorder )
{
   recordApplikation = recorder;
}


#ifdef QT4
void screencast::selectRecorder()
{
  QString recorder = QFileDialog::getOpenFileName( this,
					           tr( "Select recorder" ),
					           QDesktopServices::storageLocation( QDesktopServices::HomeLocation ) );

  if ( recorder > "" )
    RecorderLineEdit->setText( recorder );
}
#endif

#ifdef QT5
void screencast::selectRecorder()
{
    QString recorder = QFileDialog::getOpenFileName( this,
                                 tr( "Select recorder" ),
                                 QStandardPaths::writableLocation( QStandardPaths::HomeLocation ) );

    if ( recorder > "" )
      RecorderLineEdit->setText( recorder );
}
#endif

#ifdef QT4
void screencast::saveVideoPath()
{
  QString dir = QFileDialog::getExistingDirectory( this, tr( "Open Directory" ),
                QDesktopServices::storageLocation( QDesktopServices::HomeLocation ), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

  if ( dir > "" )
    SaveVideoPathLineEdit->setText( dir );
}
#endif

#ifdef QT5
void screencast::saveVideoPath()
{
  QString dir = QFileDialog::getExistingDirectory( this, tr( "Open Directory" ),
                QStandardPaths::writableLocation( QStandardPaths::HomeLocation ), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

  if ( dir > "" )
      SaveVideoPathLineEdit->setText( dir );
}
#endif

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

  QFileInfo fileInfo( PathTempLocation() + QDir::separator() + nameInMoviesLocation );
  statusBarLabelSize->setText( QString::number( fileInfo.size() / 1024 ) );
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
      clickedScreenSize();

      TabWidgetAudioFrame->setEnabled(false);
      TabWidgetMiscellaneousFrame->setEnabled(false);
      TabWidgetVideoOptionFrame->setEnabled( false );
      
      if ( SystrayCheckBox->checkState() == Qt::Checked )
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
      StopButton->setEnabled(false);
      PauseButton->setEnabled(false);
      recordButton->setEnabled(true);
      FullScreenRadioButton->setEnabled( true );
      WindowRadioButton->setEnabled( true );
      AreaRadioButton->setEnabled( true );
      clickedScreenSize();

      TabWidgetAudioFrame->setEnabled(true);
      TabWidgetMiscellaneousFrame->setEnabled(true);
      TabWidgetVideoOptionFrame->setEnabled( true );
      
      if ( SystrayCheckBox->checkState() == Qt::Checked )
      {
        SystemTrayIcon->setIcon( QIcon(":/pictures/systray.png" ) );
	startAction->setEnabled( true );
	stopAction->setEnabled( false );
	pauseAction->setEnabled( false );
	goAction->setEnabled( false );
      }
      
      if ( MagnifierCheckBox->isChecked() )
	MagnifierCheckBox->click();
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
      clickedScreenSize();
      
      TabWidgetAudioFrame->setEnabled(false);
      TabWidgetMiscellaneousFrame->setEnabled(false);
      TabWidgetVideoOptionFrame->setEnabled( false );

      if ( SystrayCheckBox->checkState() == Qt::Checked )
      {
	startAction->setEnabled( false );
	stopAction->setEnabled( false );
	pauseAction->setEnabled( false );
	goAction->setEnabled( true );
      } 
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
      clickedScreenSize();
      
      TabWidgetAudioFrame->setEnabled(false);
      TabWidgetMiscellaneousFrame->setEnabled(false);
      TabWidgetVideoOptionFrame->setEnabled( false );

      if ( SystrayCheckBox->checkState() == Qt::Checked )
      {
	startAction->setEnabled( false );
	stopAction->setEnabled( true );
	pauseAction->setEnabled( true );
	goAction->setEnabled( false );
      } 
    } 
      
    if ( ( newState == QProcess::NotRunning ) and ( pause == true ) and ( not PauseButton->isChecked() ) )
    {
      StopButton->setEnabled(false);
      PauseButton->setEnabled(false);
      recordButton->setEnabled(true);
      FullScreenRadioButton->setEnabled( true );
      WindowRadioButton->setEnabled( true );
      AreaRadioButton->setEnabled( true );
      clickedScreenSize()      ;
      
      TabWidgetAudioFrame->setEnabled(true);
      TabWidgetMiscellaneousFrame->setEnabled(true);
      TabWidgetVideoOptionFrame->setEnabled( true );

      if ( SystrayCheckBox->checkState() == Qt::Checked )
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
      qDebug();
      qDebug() << "[vokoscreen]" << recordApplikation << "is running and is ready for reading and writing";
    }
   
    if ( newState == QProcess::NotRunning )
    {
      qDebug();
      qDebug() << "[vokoscreen]" << recordApplikation << "is not running";

      //Enables the customarea rectangle again. (Is diabled in record() )
      if ( !PauseButton->isChecked() )
         myregionselection->lockFrame( false );
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
    
    QString crashedtext = "[vokoscreen] " + recordApplikation + " is crashed on ";
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
    qDebug() << "The process could not be started. Either the is called program is not installed, or the ffmpeg call Faulty or you have not over sufficient permissions to to the program.";
    QMessageBox msgBox;
    msgBox.setText( "The process could not be started. Either the is called program is not installed, or the ffmpeg call Faulty or you have not over sufficient permissions to to the program." );
    msgBox.exec();
  }
}


void screencast::moveWindowPause()
{
  pause = true;
  PauseButton->setChecked( true );
  PauseButton->setText( tr( "Go" ) );
  SystemCall->terminate();
  SystemCall->waitForFinished();
  pulseUnloadModule();
}


void screencast::moveWindowGo()
{
  PauseButton->setChecked( false );  
  PauseButton->setText( tr ( "Pause" ) );
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
      PauseButton->setText( tr ( "Go" ) );
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
        inBox->busyDialog( inBox->getAlsaHw(), inBox->getPurAlsaName() );
	PauseButton->click();
        return;
      }
      Countdown();
      shortcutStop->setEnabled( true );
      PauseButton->setText( tr( "Pause" ) );
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
      PauseButton->setText( tr ( "Go" ) );
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
        inBox->busyDialog( inBox->getAlsaHw(), inBox->getPurAlsaName() );
        PauseButton->click();
        return;
      }
      Countdown();
      shortcutStop->setEnabled( true );
      PauseButton->setText( tr( "Pause" ) );
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
  if ( MagnifierCheckBox->isChecked() )
	MagnifierCheckBox->click();
  
  if ( VideoplayerComboBox->count() == 0 )
  {
    QDialog *newDialog = new QDialog;
    newDialog->setModal( true );
    Ui_NoPlayerDialog myUiDialog;
    myUiDialog.setupUi( newDialog );
    newDialog->show();
    return;
  }
  
  QVariant aa = VideoplayerComboBox->itemData( VideoplayerComboBox->currentIndex() ); // get userdata from ComboBox
  QString player = aa.toString();
  player = player.replace( "\n", "" ); 
  
  QDir Dira( PathMoviesLocation() );
  QStringList filters;
  filters << "vokoscreen*";
  QStringList List = Dira.entryList( filters, QDir::Files, QDir::Time );
  if ( List.isEmpty() )
  {
    QProcess *SystemCall = new QProcess();
    SystemCall->start( player );
  }
  else
  {
    QProcess *SystemCall = new QProcess();
    SystemCall->start( player + " " + PathMoviesLocation() + QDir::separator() + List.at( 0 ) );
  }
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


QString screencast::getFfmpegVersion()
{
  QProcess Process;
//  Process.start("ffmpeg -version");
  Process.start( vkSettings.getRecorder() + " -version");
  Process.waitForFinished();
  QString ffmpegversion = Process.readAllStandardOutput();
  Process.close();
  
  QStringList list = ffmpegversion.split( "\n" );
  ffmpegversion = list[ 0 ];
  list = ffmpegversion.split( " " );
  
  return list[ 2 ];
}


void screencast::windowMove()
{
  // Begin Window is closed
  QStringList stringList;
  QList<WId> list = QxtWindowSystem::windows() ;
  for( int i = 0; i < list.count(); i++)
    stringList << QString::number( list[ i ] );

  if ( !stringList.contains( QString::number( moveWindowID ) ) )
  {
    windowMoveTimer->stop();
    StopButton->click();
    return;
  } 
  // End Window is closed

  // Wenn Versatzwert kleiner null ist
  QString x = QString::number( QxtWindowSystem::windowGeometry( moveWindowID ).x() );
  int xx = x.toInt();
  if ( xx < 0 )
    x = "0";
  
  QString y = QString::number( QxtWindowSystem::windowGeometry( moveWindowID ).y() );
  int yy = y.toInt();
  if ( yy < 0 )
    y = "0";

  if ( ( deltaXMove != x ) or ( deltaYMove != y ) )
    if ( SystemCall->state() == QProcess::Running ) 
      moveWindowPause();
  
  if ( ( deltaXMove == x ) and ( deltaYMove == y ) )
    if ( ( SystemCall->state() == QProcess::NotRunning ) )
    {
//      QStringList result = ffmpegString.split( ":0." );
      QStringList result = ffmpegString.split( DISPLAY );
      QString str1 = result[ 0 ];
      QString str2 = result[ 1 ];
      result.clear();
      result = str2.split( " " );
//      result[ 0 ] = ":0.0+" + x + "," + y;
      result[ 0 ] = DISPLAY + "+" + x + "," + y;
      
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
  QString output = QString::fromUtf8(Process->readAllStandardOutput());
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
 * Pulse: The lowest rate sampler for the selected device is used
 * 
 * German:
 * Gibt Samplerrate von ausgewählten Geräten zurück.
 * Pulse: Wenn mehrere Geräte ausgewählt wurden wird die niedrigste Samplerrate herangezogen
 */
QString screencast::mySample()
{
  QString ret = "41000";
  
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
   
  if ( AlsaRadioButton->isChecked() )
  {
    // Aus der ComboBox das ausgewählte Device holen
    QVariant aa = AlsaHwComboBox->itemData( AlsaHwComboBox->currentIndex() );
    QvkAlsaDevice *inBox = AlsaDeviceList.at( aa.toInt() );
    ret = inBox->getAlsaSample();
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
    
    AudiocodecComboBox->setEnabled( true );
  }
  else
  {
    AlsaRadioButton->setEnabled( false );
    AlsaHwComboBox->setEnabled( false );
    Pulseframe->setEnabled( false );
    PulseDeviceRadioButton->setEnabled( false );
    AudiocodecComboBox->setEnabled( false );
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
    Pulseframe->setGeometry( 90, 20, 320, getPulseInputDevicesCount() * 20);
    Pulseframe->show();

    for ( int i = 0; i < getPulseInputDevicesCount(); ++i )
    {
      namePulse = new QCheckBox( Pulseframe );
      namePulse->setGeometry( QRect( 0,  i * 20, 400, 21 ) );
      namePulse->setText( getPulseInputName( i + 1 ) );
      namePulse->setAccessibleName( getPulseInputDevices( i + 1 ) );
      namePulse->setToolTip( tr ( "Select one or more devices" ) );
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
    #ifdef QT4
    if ( QDesktopServices::storageLocation( QDesktopServices::MoviesLocation).isEmpty() )
    #endif
    #ifdef QT5
    if ( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ).isEmpty() )
    #endif 
    {
       #ifdef QT4
       Path = QDesktopServices::storageLocation( QDesktopServices::HomeLocation );
       #endif
       #ifdef QT5
       Path = QStandardPaths::writableLocation( QStandardPaths::HomeLocation );
       #endif
       SaveVideoPathLineEdit->setText(Path);
    }
    else
    {
      #ifdef QT4
      Path = QDesktopServices::storageLocation( QDesktopServices::MoviesLocation );
      #endif
      #ifdef QT5
      Path = QStandardPaths::writableLocation( QStandardPaths::MoviesLocation );
      #endif
      SaveVideoPathLineEdit->setText( Path );
    }
  }
  return Path;
}




QString screencast::PathTempLocation()
{
  // Doppelter ProgName um Eindeutigkeit in tmp zuerreichen
  QString tmpName = vkSettings.getProgName() + "-" + vkSettings.getProgName();

  #ifdef QT4
  QString tempPathProg = QDesktopServices::storageLocation ( QDesktopServices::TempLocation ) + QDir::separator() + tmpName;
  #endif

  #ifdef QT5
  QString tempPathProg = QStandardPaths::writableLocation( QStandardPaths::TempLocation ) + QDir::separator() + tmpName;
  #endif
  
  QDir dirTempPathProg( tempPathProg );
  if ( not dirTempPathProg.exists() )
  {
      #ifdef QT4
      QString tempPath = QDesktopServices::storageLocation ( QDesktopServices::TempLocation );
      #endif
      #ifdef QT5
      QString tempPath = QStandardPaths::writableLocation( QStandardPaths::TempLocation );
      #endif      
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
  return "vokoscreen-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + VideoContainerComboBox->currentText();
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
     return "-c:a " + AudiocodecComboBox->currentText(); //"-c:a libmp3lame";
  
  if ( ( AudioOnOffCheckbox->checkState() == Qt::Checked ) and ( PulseDeviceRadioButton->isChecked() ) and ( myPulseDevice() > "" ) )
     return "-c:a " + AudiocodecComboBox->currentText(); //"-c:a libmp3lame";

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
  if ( AlsaRadioButton->isChecked() and AudioOnOffCheckbox->isChecked() )
  {
    QVariant aa = AlsaHwComboBox->itemData( AlsaHwComboBox->currentIndex() );
    QvkAlsaDevice *inBox = AlsaDeviceList.at( aa.toInt() );
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
  
  if ( WindowRadioButton->isChecked() )
    if ( firststartWininfo == false )
    {
      vkWinInfo = new QvkWinInfo();
      connect( vkWinInfo, SIGNAL( windowChanged() ), this, SLOT( record() ) );
      return;
    }
   record();
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


void screencast::Countdown()
{
  if ( CountdownSpinBox->value() > 0 )
  {
    recordButton->setEnabled( false );

    QDesktopWidget *desk = QApplication::desktop();
    int Width = 250;
    int Height = 250;;
    int x = ( desk->screenGeometry().width() / 2 ) - ( Width / 2 );
    int y = ( desk->screenGeometry().height() / 2 ) -( Height / 2 );
    
    QFrame * countdownDialog = new QFrame();
    countdownDialog->setGeometry( x, y, Width, Height );
    countdownDialog->setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    //if( QX11Info::isCompositingManagerRunning() ) // *********************************************** vorerst ersatzlos gestrichen **************************************************
       countdownDialog->setAttribute( Qt::WA_TranslucentBackground, true );
    
    countdownDialog->show();
    
    QFont qfont;;
    qfont.setPixelSize( 200 );
    
    QLabel * label = new QLabel( countdownDialog );
    label->setGeometry( 0, 0, Width, Height );
    label->setAlignment( Qt::AlignCenter );    
    countdownDialog->setFont( qfont );
    label->show();
    
    QPalette pal( label->palette() );
    pal.setColor( QPalette::Foreground, Qt::red);
    label->setPalette( pal );
    
    for ( int i = CountdownSpinBox->value(); i >= 1; i-- )
    {
      label->setText( "" );
      QCoreApplication::processEvents( QEventLoop::AllEvents );     
      
      label->setText( QString::number( i ) );
      
      QCoreApplication::processEvents( QEventLoop::AllEvents );     
      label->update();
      for ( int ii = 1; ii <= 10; ii++)
      {
        QCoreApplication::processEvents( QEventLoop::AllEvents );     
        QTest::qSleep( 100 );
      }
    }

    // Der Desktopanimation "Langsames ausblenden" entgegenwirken
    label->setText( "" );
    QCoreApplication::processEvents( QEventLoop::AllEvents );     
    label->hide();
    QCoreApplication::processEvents( QEventLoop::AllEvents );     
    countdownDialog->close();
  } 
}


void screencast::record()
{
  Countdown();
//#ifdef QT4
  shortcutStart->setEnabled( false );
  shortcutStop->setEnabled( true );
//#endif   
  if ( MinimizedCheckBox->checkState() == Qt::Checked )
    WindowMinimized();

  QString deltaX = "0";
  QString deltaY = "0";
  
  if ( WindowRadioButton->isChecked() and ( firststartWininfo == false) )
  {
      qDebug() << "[vokoscreen]" << "recording window";
      setRecordWidth( vkWinInfo->width() );
      setRecordHeight( vkWinInfo->height() );
      deltaX = QString::number( vkWinInfo->x().toUInt() );
      deltaY = QString::number( vkWinInfo->y().toUInt() );

      moveWindowID = vkWinInfo->getWinID();
      
      deltaXMove = deltaX;
      deltaYMove = deltaY;

      windowMoveTimer->start( 120 );
      firststartWininfo = true;
  }

  if( FullScreenRadioButton->isChecked() )
  {
      int screen = ScreenComboBox->itemData( ScreenComboBox->currentIndex() ).toInt();
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

      setRecordWidth( QString::number( fullScreenWidth ) );
      setRecordHeight( QString::number( fullScreenHeight) );
  }  

  if ( AreaRadioButton->isChecked() )
  {
    setRecordWidth( QString().number( myregionselection->getWidth() ) );
    setRecordHeight( QString().number( myregionselection->getHeight() ) );
    deltaX  = QString().number( myregionselection->getXRecordArea() );
    deltaY  = QString().number( myregionselection->getYRecordArea() );

    //Makes the rectangle unmovable and unresizeable (Is enabled yet again when process finished)
    myregionselection->lockFrame( true );

    qDebug() << "[vokoscreen]" << "recording area";
  }
  
  QString myReport = "-report ";
  
  // set working directory for writing and delete the ffmpegLog from Profil directory
  QSettings settings( vkSettings.getProgName(), vkSettings.getProgName() );
  QFileInfo settingsPath( settings.fileName() );
  QFile file( settingsPath.absolutePath() );
  QString workDirectory = file.fileName();
  QDir Dir( "" );
  Dir.setCurrent( workDirectory );  
  
  // bestehendes ffmpegLog löschen
  QDir Dira( "" );
  QStringList filters;
  filters << "ffmpeg*";
  QStringList List = Dira.entryList(filters, QDir::Files, QDir::Name );
  QFile FfmpegFile;
  for (int i = 0; i < List.size(); ++i)
     FfmpegFile.remove( List.at(i) );

  // evtl. müßte hier erst mal nachgeschaut werden ob Temp leer ist.
  // Clean vokoscreen temp 
  QDir dir( PathTempLocation() );
  QStringList stringList = dir.entryList( QDir::Files, QDir::Time | QDir::Reversed );
  for ( int i = 0; i < stringList.size(); ++i )
     dir.remove( PathTempLocation().append(QDir::separator() ).append(stringList.at( i ) ) );

  // framerate
  QString framerate;
  framerate = "-framerate " + QString().number( FrameSpinBox->value() );

  QString myVcodec = VideocodecComboBox->currentText();
  if ( myVcodec == "libx264" )
  {
    // Number of pixels must be divisible by two
    int intRecordX = getRecordWidth().toInt();
    if ( ( intRecordX % 2 ) == 1 )
      setRecordWidth( QString().number( --intRecordX ) );

    // Number of pixels must be divisible by two
    int intRecordY = getRecordHeight().toInt();
    if ( ( intRecordY % 2 ) == 1 )
      setRecordHeight( QString().number( --intRecordY ) );
    
    myVcodec = "libx264 -preset veryfast";
  }  

  nameInMoviesLocation = NameInMoviesLocation();

  QString quality = " -q:v 1 ";
  
  ffmpegString = recordApplikation + " "
               + myReport + " "
               + "-f x11grab" + " "
               + framerate + " "
               + "-video_size" + " " + getRecordWidth() + "x" + getRecordHeight() + " "
               + "-i " + DISPLAY + "+" + deltaX + "," + deltaY
               + noMouse() + " "
               + myAlsa() + " "
               + "-pix_fmt yuv420p" + " "
               + "-c:v" + " " + myVcodec + " "
               + myAcodec() + " "
               + quality + " "
               + "-s" + " " + getRecordWidth() + "x" + getRecordHeight() + " ";
  
  startRecord( PathTempLocation() + QDir::separator() + nameInMoviesLocation );
  
  QFile FileVokoscreenLog(settingsPath.absolutePath() + QDir::separator() + vkSettings.getProgName() + ".log");
  if ( !FileVokoscreenLog.open( QIODevice::WriteOnly ) ) 
    qDebug() << "Datei konnte nicht angelegt werden: " << FileVokoscreenLog.errorString();
  
  QTextStream stream( &FileVokoscreenLog );
  stream << vkSettings.getProgName() << " Version: " << vkSettings.getVersion() << "\n";
  stream << "Record resolution: " << getRecordWidth() << "x" << getRecordHeight() << "\n";
  stream << "Alsa string: " << myAlsa() << "\n";
  stream << "Qt Version: " << qVersion() << "\n";
  stream << "ffmpeg Version: " << getFfmpegVersion() << "\n";
  stream << "Record String: " << ffmpegString << PathMoviesLocation() << QDir::separator() << nameInMoviesLocation << "\n";
}


void screencast::startRecord( QString RecordPathName )
{
  qDebug() << "[vokoscreen]"<< "Executive command :" << ffmpegString + RecordPathName;

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

  beginTime  = QDateTime::currentDateTime();
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
    QDir dir( PathTempLocation() );
    QStringList stringList = dir.entryList(QDir::Files, QDir::Time | QDir::Reversed);
#ifdef QT4    
    QString mergeFile = QDesktopServices::storageLocation ( QDesktopServices::TempLocation ) + QDir::separator() + "mergeFile.txt";
#endif
#ifdef QT5
    QString mergeFile = QStandardPaths::writableLocation( QStandardPaths::TempLocation ) + QDir::separator() + "mergeFile.txt";
#endif
    QFile file( mergeFile );
    file.open( QIODevice::WriteOnly | QIODevice::Text );
      QString videoFiles;
      for ( int i = 0; i < stringList.size(); ++i )
      {
        videoFiles.append( "file " ).append( PathTempLocation() ).append( QDir::separator() ).append( stringList[ i ] ).append( "\n" );
#ifdef QT4	
        file.write( videoFiles.toAscii() );
#endif
#ifdef QT5
        file.write( videoFiles.toLatin1() );
#endif
        videoFiles = "";
      }
    file.close();

    QString mergeString = recordApplikation + " -report -f concat -i " + mergeFile + " -c copy " + PathMoviesLocation() + QDir::separator() + nameInMoviesLocation;
    SystemCall->start( mergeString );
    SystemCall->waitForFinished();
    
    for ( int i = 0; i < stringList.size(); ++i )
      dir.remove( PathTempLocation().append( QDir::separator() ).append( stringList.at( i ) ) );

    file.remove();

    qDebug() << "[vokoscreen]" << "Mergestring :" << mergeString;
   }
   else    
  {
    QString FileInTemp = PathTempLocation() + QDir::separator() + nameInMoviesLocation;
    QFile::copy ( FileInTemp, PathMoviesLocation() + QDir::separator() + nameInMoviesLocation );
    QFile::remove ( FileInTemp );
  }
  
  QDir dir_1;
  dir_1.rmdir( PathTempLocation() );
  
  pause = false;
  windowMoveTimer->stop();
  firststartWininfo = false;

  pulseUnloadModule();
  
}

//#include "screencast.moc"
