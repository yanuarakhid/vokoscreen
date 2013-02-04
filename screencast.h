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

#ifndef screencast_H
#define screencast_H
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QToolButton>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QProcess>
#include <QList>
#include <QSpinBox>
#include <QRadioButton>
#include <QFrame>
#include <QScrollArea>
#include <QSystemTrayIcon>

#include "regionselection.h"
#include "QvkLupe.h"
#include "QvkWebcam.h"
#include "QvkAlsaDevice.h"

#include <QxtGlobalShortcut> 

#include "QvkWinInfo.h"


class screencast : public QMainWindow
{ 
Q_OBJECT
public:
    screencast();
    virtual ~screencast();
    QString getFfmpegVersion();
    QString ProgName;
    QString Version;
    QString email;
    QString homepage;
    //QString channels;
    QString ffmpegString;
    QString nameInMoviesLocation;
    QString winID;
    QString deltaXMove;
    QString deltaYMove;
    bool pause;
    QStringList MoveWindowlist;
    bool firststartWininfo;
    bool moveWindow;
    
    
public slots:
  
private slots:
    void Stop(void);
    void record(void);
    void play();
    //bool isScreencastFileAvailable();
    
    //void alsaDevice();
    void PulseMultipleChoice();

    void AudioOnOff();
    //void getChannelCount( int value );
    //QString getChannelCount( QString device );
    
    void Pause();
    void startRecord( QString RecordPathName );
    QString myAlsa();
    
    QString myAcodec();
    void searchExternalProgramms();
    //int getCountAlsaCaptureDevice();
   // QString getFfmpegVersionMinor();
   // QString winInfo();
    //QString winInfo(); //***************************************************************************
    void preRecord(); //********************************************************************

    //QString getAlsaHwCaptureDevice(int value);
    //QString getNameCaptureCard(int value);
    
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

    //void usb();

    QString noMouse();
    
    void windowMove();
    void moveWindowPause();
    void moveWindowGo();
    
    // Tab Videooptionen
    void setFrameStandardSpinbox();
    void setVideocodecStandardComboBox();

    QString PathMoviesLocation();
    QString PathTempLocation();
    QString NameInMoviesLocation();
    QString PauseNameInTmpLocation();
    void saveVideoPath();
    void makeAsoundrc();
    
    bool needProgramm( QString ProgName );
    void stateChanged( QProcess::ProcessState newState );
    void error ( QProcess::ProcessError error );
    void readyReadStandardError();
    void readyReadStandardOutput();
    
    void saveSettings();
    
    void WindowMinimized();
    
    void SystemTrayStart( QSystemTrayIcon::ActivationReason );
    void SystemTrayStop( QSystemTrayIcon::ActivationReason );
    void SystemTrayPause( QSystemTrayIcon::ActivationReason reason );
    void SystemTrayGo( QSystemTrayIcon::ActivationReason reason );

    void AreaOnOff();
    void showLupe();
    void showWebcam();
    void showWebcamDialog();

    void ShortcutPause();
    void ShortcutLupe();

    
    void mailSend();

    void AlsaWatcherEvent( QStringList CardxList );
    void myVideoFileSystemWatcher( const QString & path );
    

    
private:
    QPushButton *StopButton;
    QPushButton *recordButton;
    QProcess *SystemCall;
    QRadioButton *AlsaRadioButton;
    QComboBox *AlsaHwComboBox;
    QCheckBox *AudioOnOffCheckbox;
    QRadioButton * FullScreenRadioButton;
    QRadioButton * WindowRadioButton;
    QRadioButton *AreaRadioButton;
    QCheckBox *LupeCheckBox;
    QCheckBox *webcamCheckBox;
    QLabel *ChannelLabel;
    QComboBox *VideoplayerComboBox;
    QPushButton *PlayButton;
    QPushButton *PauseButton;
    QFrame *TabWidgetAudioFrame;
    QLabel *recordTimeLabel;
    QLineEdit *SaveVideoPathLineEdit;
    QRadioButton *PulseDeviceRadioButton;
    
    QFrame *TabWidgetMiscellaneousFrame;
    QFrame *TabWidgetVideoOptionFrame;
    QFrame *TabWidgetSocialFrame;
    QSpinBox *FrameSpinBox;
    QComboBox *VideocodecComboBox;
    QComboBox *VideoContainerComboBox;
    QCheckBox *HideMouseCheckbox;
    QCheckBox *MinimizedCheckBox;
    
    QTimer *windowMoveTimer;
    
    QFrame *Pulseframe;
    QScrollArea *scrollAreaPulse;
    QCheckBox *namePulse;
    
    QSystemTrayIcon *SystemTrayIconGreen;
    QSystemTrayIcon *SystemTrayIconRed;
    QSystemTrayIcon *SystemTrayIconYellow;
    QSystemTrayIcon *SystemTrayIconBlue;
    
    QvkLupe *lupe;
    regionselection *myregionselection;
    QvkWebcam *webcam;
    
    QxtGlobalShortcut *shortcutLupe;
    QxtGlobalShortcut *shortcutPause;
    QxtGlobalShortcut *shortcutStart;
    QxtGlobalShortcut *shortcutStop;
    
    QvkWinInfo *vkWinInfo;
    WId moveWindowID;
    
    QPushButton *mailPushButton;
    QCheckBox *mailOnOffCheckbox;

    QList<QvkAlsaDevice *> AlsaDeviceList;

 protected:
    void closeEvent( QCloseEvent * event );
	
};


#endif // screencast_H
