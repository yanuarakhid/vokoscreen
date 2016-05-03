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

#ifndef VOKOSCREEN_H
#define VOKOSCREEN_H

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
#include <QTest>
#include <QDateTime>
#include <QListWidget>

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
#include "QvkShowkeyController.h"


#include "QvkAnimateControl.h"
#include "QvkShowClickDialog.h"

#include "ui_vokoscreen.h"

class screencast : public QMainWindow
{ 
Q_OBJECT
public:
  screencast();
  virtual ~screencast();
  QString recordApplikation;
  QProcess *SystemCall;
  QString deltaXMove;
  QString deltaYMove;
  QString nameInMoviesLocation;
  QString ffmpegString;
  bool firststartWininfo;
  
public:
#ifndef NO_NEW_VERSION_CHECK
    QvkVersion version;
#endif
    
    
public slots:

  
private slots:
  void closeEvent( QCloseEvent * event );
  
  void addLogVokoscreen( QString value );
  void VisibleHideKonsole();
  void myScreenCountChanged( int newCount );
  void showMagnifier();
  void uncheckMagnifier();
  void searchExternalPrograms();
  QString getOsRelease();
  bool searchProgramm( QString ProgName );
  QString getFfmpegVersion();
  QString getPactlVersion();
  QString getXdgemailVersion();
  QString getLsofVersion();  
  void AudioOff( int state );
  void AlsaWatcherEvent( QStringList CardxList );
  void PulseMultipleChoice();
  QCheckBox *getCheckBoxPulseDevice( int value );
  void AudioOnOff();
  void WindowMinimized();
  void saveSettings();

  void setRecordWidth( QString value );
  QString getRecordWidth();
  void setRecordHeight( QString value );
  QString getRecordHeight();
  
  void stateChangedAudio( int state );
  void clickedAudioAlsa( bool checked );
  void clickedAudioPulse( bool checked );
  void valueChangedFrames( int i );
  void clickedScreenSize();
  
  void currentIndexChangedCodec( int index );
  void currentIndexChangedFormat( int index );
  
  void windowMove();
  void newMovedXYcoordinates();

  // Tab Videooptionen
  void setFrameStandardSpinbox();
  void setVideocodecStandardComboBox();
  void setAudiocodecStandardComboBox();

  void searchGIFPlayer();
  void searchVideoPlayer();
  
  // Tab  Verschiedenes
  QString PathMoviesLocation();
  void saveVideoPath();
  void myVideoFileSystemWatcher( const QString & path );
  QString getFileWithPath( QString ProgName );
  void recorderLineEditTextChanged( QString recorder );
  void selectRecorder();
  void showCredits();
  void creditsCloseEvent();
  void stateChangedSystray( int state );
  void SearchCodec();

  
  QString PathTempLocation();
  QString NameInMoviesLocation();
  QString newPauseNameInTmpLocation();
  QString getPauseNameInTmpLocation();
  void setPauseNameInTmpLocation( QString value );
  QString myAlsa();
  QString myAcodec();
  QString noMouse();
  void AreaOnOff();
  void preRecord();
  void Countdown();
  void record();
  void startRecord( QString RecordPathName );
  void Stop();
  void Pause();
  void play();
  void send();

  void error ( QProcess::ProcessError error );
  void readyReadStandardError();
  void stateChanged( QProcess::ProcessState newState );
  
  void ShortcutPause();
  
  void SystemTrayKontextMenue( QAction *action );
  
  void copyToClipboard();

  
#ifndef NO_NEW_VERSION_CHECK
    void buttonVersion();
#endif
  
    
private:
    QString DISPLAY;
  
    QvkSettings vkSettings;
    Ui_screencast  myUi;
    int oldMainWindowHeight;
    QvkMagnifier *magnifier;
    regionselection *myregionselection;
    
    
    QvkShowClickDialog *ShowClickDialog;
    QvkAnimateControl *animateControl;
    QList<QvkAlsaDevice *> AlsaDeviceList;

    QScrollArea *scrollAreaPulse;
    QCheckBox *namePulse;
    
    QLabel *statusBarLabelTime;
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

    QMenu *SystemTrayMenu;
    QSystemTrayIcon *SystemTrayIcon;
    QAction *startAction;
    QAction *stopAction;
    QAction *pauseAction;
    QAction *goAction;
    QAction *hideAction;
    QAction *exitAction;
    
    QvkWebcamController *webcamController;

    QvkCredits *credits;
    
    QvkWinInfo *vkWinInfo;
    WId moveWindowID;
    
    QxtGlobalShortcut *shortcutWebcam;
    QxtGlobalShortcut *shortcutMagnifier;
    QxtGlobalShortcut *shortcutPause;
    QxtGlobalShortcut *shortcutStart;
    QxtGlobalShortcut *shortcutStop;
    
    QTimer *windowMoveTimer;
    QDateTime beginTime;

    bool pause;
    QString pauseName;
    
protected:
   void contextMenuEvent( QContextMenuEvent *event );
    
};


#endif // screencast_H
