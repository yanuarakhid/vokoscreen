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

#ifndef screencast_H 
#define screencast_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QScrollArea>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QSettings>
#include <QLineEdit>
#include <QFileDialog>
#include <QDate>
#include <QStatusBar>
#include <QDesktopWidget>
#include <QDial>
#include <QAction>

#include "QvkSettings.h"
#include "regionselection.h"
#include "QvkMagnifier.h"
#include "QvkWebcamController.h"
#include "QvkAlsaDevice.h"
#include "QvkMail.h"
#include "QvkAlsaWatcher.h"
#include "QvkWinInfo.h"
#include "QvkCredits.h"
#include "QvkVersion.h"

#include "ui_QvkNoPlayerDialog.h"

#include <QxtGlobalShortcut> 

#include <QTest>
#include <QDateTime>

class screencast : public QMainWindow
{ 
Q_OBJECT
public:
    screencast();
    virtual ~screencast();
    QString getFfmpegVersion();
    QString getPactlVersion();
    QString getOsRelease();

    QString email;
    QString homepage;
    QString ffmpegString;
    QString nameInMoviesLocation;
    QString winID;
    QString deltaXMove;
    QString deltaYMove;
    bool pause;
    QStringList MoveWindowlist;
    bool firststartWininfo;
    bool moveWindow;
    
    QString recordApplikation;
    
    QvkCredits * credits;
    
    QMenu *SystemTrayMenu;
    QSystemTrayIcon *SystemTrayIcon;

public:
#ifndef NO_NEW_VERSION_CHECK
    QvkVersion version;
#endif
    
public slots:

private slots:
    void Stop(void);
    void record(void);
    void play();

    void Countdown();

    void PulseMultipleChoice();

    void AudioOnOff();
    
    void Pause();
    void startRecord( QString RecordPathName );
    QString myAlsa();
    
    QString myAcodec();
    void searchExternalPrograms();
    QString getFileWithPath( QString ProgName );
    void preRecord();

    const QString myPulseDevice();
    int getCountCheckedPulseDevices();
    QString getPulseDeviceName( int value );
    int getPulseInputDevicesCount();
    QCheckBox * getCheckBoxPulseDevice( int value );
    QString getPulseInputName( int value );
    QString getPulseInputDevices( int value );
    QString getPulseSample( int value );
    void pulseUnloadModule();
    QString mySample();

    QString noMouse();
    
    void windowMove();
    void moveWindowPause();
    void moveWindowGo();
    
    // Tab Videooptionen
    void setFrameStandardSpinbox();
    void setVideocodecStandardComboBox();
    void setAudiocodecStandardComboBox();
    
    QString PathMoviesLocation();
    QString PathTempLocation();
    QString NameInMoviesLocation();
    QString PauseNameInTmpLocation();
    void saveVideoPath();
    void selectRecorder();
    void recorderLineEditTextChanged( QString recorder );
    void makeAsoundrc();
    
    bool searchProgramm( QString ProgName );
    void stateChanged( QProcess::ProcessState newState );
    void error ( QProcess::ProcessError error );
    void readyReadStandardError();
    void readyReadStandardOutput();
    
    void saveSettings();
    
    void WindowMinimized();

    void stateChangedSystray( int );
    void SystemTrayKontextMenue( QAction *action );

    void AreaOnOff();
    void showMagnifier();
    void MagnifierDialog();
    
    void ShortcutWebcam();
    void ShortcutPause();
    void ShortcutMagnifier();
    void uncheckMagnifier();
    
    void send();

    void AlsaWatcherEvent( QStringList CardxList );
    void myVideoFileSystemWatcher( const QString & path );
    
    void currentIndexChangedCodec( int index );
    void currentIndexChangedFormat( int index );

    void stateChangedAudio( int state );
    void clickedAudioAlsa( bool checked );
    void clickedAudioPulse( bool checked );
    void valueChangedFrames( int i );
    void clickedScreenSize();

    void setRecordWidth( QString value );
    QString getRecordWidth();
    void setRecordHeight( QString value );
    QString getRecordHeight();
    
    void showCredits();
    void creditsCloseEvent();

    void showHomepage();
    
    void myScreenCountChanged ( int newCount );
    
#ifndef NO_NEW_VERSION_CHECK
    void buttonVersion();
#endif
    
private:
    QString DISPLAY;
    
    QTabWidget *tabWidget;
  
    QPushButton *recordButton;
    QPushButton *StopButton;
    QPushButton *PauseButton;
    QPushButton *PlayButton;
    QPushButton *sendPushButton;
    QCheckBox *webcamCheckBox;
    QCheckBox *mirrorCheckBox;
    QDial *rotateDial;
    QRadioButton *radioButtonTopMiddle;
    QRadioButton *radioButtonRightMiddle;
    QRadioButton *radioButtonBottomMiddle;
    QRadioButton *radioButtonLeftMiddle;
        
    QProcess *SystemCall;
    QRadioButton *AlsaRadioButton;
    QComboBox *AlsaHwComboBox;
    QCheckBox *AudioOnOffCheckbox;
    QRadioButton * FullScreenRadioButton;
    QRadioButton * WindowRadioButton;
    QRadioButton *AreaRadioButton;
    QCheckBox *MagnifierCheckBox;
    QSpinBox * CountdownSpinBox;
    QComboBox * ScreenComboBox;
    
    QLabel *ChannelLabel;
    QComboBox *VideoplayerComboBox;
    QLabel *recordTimeLabel;
    QLineEdit *SaveVideoPathLineEdit;
    QLineEdit *RecorderLineEdit;
    QRadioButton *PulseDeviceRadioButton;
    
    QFrame *TabWidgetAudioFrame;
    QFrame *TabWidgetVideoOptionFrame;
    QFrame *TabWidgetMiscellaneousFrame;
    QFrame *TabWidgetWebcamFrame;
    QFrame *TabWidgetSocialFrame;
    
    QSpinBox *FrameSpinBox;
    QComboBox *VideocodecComboBox;
    QComboBox *VideoContainerComboBox;
    QComboBox *AudiocodecComboBox;
    QCheckBox *HideMouseCheckbox;
    QCheckBox *MinimizedCheckBox;
    
    QTimer *windowMoveTimer;
    
    QFrame *Pulseframe;
    QScrollArea *scrollAreaPulse;
    QCheckBox *namePulse;
    
    QCheckBox *SystrayCheckBox;
    QAction *startAction;
    QAction *stopAction;
    QAction *pauseAction;
    QAction *goAction;
    QAction *hideAction;
    QAction *exitAction;
    
    QvkMagnifier *magnifier;
    regionselection *myregionselection;
    QvkWebcamController *webcamController;

//#ifdef QT4
    QxtGlobalShortcut *shortcutWebcam;
    QxtGlobalShortcut *shortcutMagnifier;
    QxtGlobalShortcut *shortcutPause;
    QxtGlobalShortcut *shortcutStart;
    QxtGlobalShortcut *shortcutStop;
//#endif

    QvkWinInfo *vkWinInfo;
    WId moveWindowID;
    

    QList<QvkAlsaDevice *> AlsaDeviceList;

    QLabel * statusBarLabelTime;
    QLabel * statusBarLabelFps;
    QLabel * statusBarLabelSize;
    QLabel * statusBarLabelCodec;
    QLabel * statusBarLabelFormat;
    QLabel * statusBarLabelAudio;
    QLabel * statusBarLabelFpsSettings;
    QLabel * statusbarLabelScreenSize;
    QLabel * statusBarProgForRecord;
    
    QString screenRecordWidth;
    QString screenRecordHeight;
    
    QDateTime beginTime;
    
    QPushButton * creditsQPushButton;
    QPushButton * updateButton;

    QvkSettings vkSettings;

 protected:
    void closeEvent( QCloseEvent * event );
    void changeEvent(QEvent *e);
    void styleChange( QStyle &asd );
};


#endif // screencast_H
