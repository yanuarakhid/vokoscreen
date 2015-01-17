#ifndef QVKSETTINGS_H 
#define QVKSETTINGS_H

#include <QObject>
#include <QSettings>

class QvkSettings: public QObject
{ 
Q_OBJECT
public:
  QvkSettings();
  virtual ~QvkSettings();
  void readAll();

  QString getVersion();
  QString getProgName();
  
  // Audio
  int getAudioOnOff();
  
  // Alsa
  bool getAlsaSelect();
  
  // Pulse
  bool getPulseSelect();
  
  // Record
  bool getFullScreenSelect();
  bool getWindowSelect();
  bool getAreaSelect();
  
  // Miscellaneous
  QString getVideoPath();
  QString getVideoPlayer();
  int getMinimized();
  int getCountdown();
  
  // Videooptions
  int getFrames();
  QString getVideoCodec();
  QString getAudioCodec();
  QString getVideoContainer();
  int getHideMouse();

  // Gui
  int getX();
  int getY();
  int getTab();
  int getSystray();
  
  // Area
  int getAreaX();
  int getAreaY();
  int getAreaWidth();
  int getAreaHeight();
  
  // webcam
  int getWebcamX();
  int getWebcamY();
  int getWebcamHeight();
  int getWebcamWidth();
  bool getWebcamMirrored();
  bool getWebcamBorder();
  bool getWebcamOverFullScreen();
  
  // Magnifier
  int getMagnifierOnOff();
  
public slots:

  
private slots:


signals:

  
protected:  

  
private:
  QString ProgName;
  QString Version;

  int AudioOnOff;
  bool AlsaSelect;
  bool PulseSelect;
  bool FullScreenSelect;
  bool WindowSelect;
  bool AreaSelect;
  QString VideoPath;
  QString VideoPlayer;
  int Minimized;
  int Countdown;
  int Frames;
  QString VideoCodec;
  QString AudioCodec;
  QString VideoContainer;
  int HideMouse;
  
  // Gui
  int X;
  int Y;
  int Tab;
  int Systray;
  
  // Area
  int AreaX;
  int AreaY;
  int AreaWidth;
  int AreaHeight;

  // webcam
  int webcamX;
  int webcamY;
  int webcamHeight;
  int webcamWidth;
  bool webcamMirrored;
  bool webcamBorder;
  bool webcamOverFullScreen;
  
  // Magnifier
  int magnifierOnOff;
};

#endif
