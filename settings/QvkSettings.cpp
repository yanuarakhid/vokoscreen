#include "QvkSettings.h" 

using namespace std;

QvkSettings::QvkSettings(){}
QvkSettings::~QvkSettings(){}

void QvkSettings::readAll()
{
    // Read from file VERSION progname and versionsnumber
    QSettings versionSettings(":/VERSION", QSettings::IniFormat );
    versionSettings.beginGroup("Info");
      bool beta = versionSettings.value( "Beta" ).toBool();
      QString Beta;
      if ( beta )
        Beta = " Beta"; 
      else
        Beta = "";
      
      ProgName = versionSettings.value( "Progname" ).toString();
      Version = versionSettings.value( "Version" ).toString() + Beta;
    versionSettings.endGroup();
    
    // Einstellungen aus .conf einlesen
    QSettings settings( getProgName(), getProgName() );
    
    // Dient nur zum anlegen des Profils damit ffmpeglog erstellt werden kann
    settings.beginGroup("vokoscreen");
      settings.setValue("Version", getVersion());
    settings.endGroup();
  
    settings.beginGroup( "Audio" );
      AudioOnOff = settings.value( "AudioOnOff", 2 ).toUInt();
    settings.endGroup();
    
    settings.beginGroup("Alsa" );
      AlsaSelect = settings.value( "Alsa", false ).toBool();
    settings.endGroup();

    settings.beginGroup("Pulse");
      PulseSelect = settings.value( "Pulse", true ).toBool();
    settings.endGroup();

    settings.beginGroup("Record");
      FullScreenSelect = settings.value( "FullScreen", true ).toBool();
      WindowSelect = settings.value( "Window", false ).toBool();
      AreaSelect = settings.value( "Area", false ).toBool();
    settings.endGroup();
    
    settings.beginGroup( "Miscellaneous" );
      VideoPath = settings.value( "VideoPath" ).toString();
      VideoPlayer = settings.value( "Videoplayer" ).toString();
      Minimized = settings.value( "Minimized", 0 ).toUInt();
      Countdown = settings.value( "Countdown", 0 ).toUInt();
    settings.endGroup();
    
    settings.beginGroup( "Videooptions" );
      Frames = settings.value( "Frames", 25 ).toInt();
      VideoCodec = settings.value( "Videocodec", "libx264" ).toString();
      AudioCodec = settings.value( "Audiocodec", "libmp3lame" ).toString();
      VideoContainer = settings.value( "Format", "mkv" ).toString();
      HideMouse = settings.value( "HideMouse").toUInt();
    settings.endGroup();

    settings.beginGroup( "GUI" );
      X = settings.value( "X", "100" ).toUInt();
      Y = settings.value( "Y", "100" ).toUInt();
      Tab = settings.value( "Tab", "0" ).toUInt();
      Systray = settings.value( "Systray", 2 ).toUInt();
    settings.endGroup();
    
    settings.beginGroup( "Area" );
        AreaX = settings.value( "X", 200 ).toUInt();
        AreaY = settings.value( "Y", 200 ).toUInt();
        AreaWidth = settings.value( "Width", 200 ).toUInt();
        AreaHeight = settings.value( "Height", 200 ).toUInt();
    settings.endGroup();

    settings.beginGroup( "Webcam" );
	webcamX = settings.value( "X", 400 ).toUInt();
	webcamY = settings.value( "Y", 400 ).toUInt();
	webcamWidth = settings.value( "Width", 320 ).toUInt();
	webcamHeight = settings.value( "Height", 240 ).toUInt();
	webcamMirrored = settings.value( "Mirrored", false ).toBool();
	webcamBorder = settings.value( "Border", true ).toBool();
	webcamOverFullScreen = settings.value( "OverFullScreen", false ).toBool();
    settings.endGroup();
    
    settings.beginGroup( "Magnifier" );
        magnifierOnOff = settings.value( "OnOff", 0 ).toUInt();
	magnifierFormValue = settings.value( "FormValue", 2 ).toUInt();
    settings.endGroup();
}

QString QvkSettings::getVersion()
{
  return Version; 
}

QString QvkSettings::getProgName()
{
  return ProgName; 
}

int QvkSettings::getAudioOnOff()
{
  return AudioOnOff; 
}

bool QvkSettings::getAlsaSelect()
{
  return AlsaSelect;
}

bool QvkSettings::getPulseSelect()
{
  return PulseSelect;
}

bool QvkSettings::getFullScreenSelect()
{
  return FullScreenSelect;
}

bool QvkSettings::getWindowSelect()
{
  return WindowSelect;
}

bool QvkSettings::getAreaSelect()
{
  return AreaSelect; 
}

QString QvkSettings::getVideoPath()
{
  return VideoPath;
}

QString QvkSettings::getVideoPlayer()
{
  return VideoPlayer; 
}

int QvkSettings::getMinimized()
{
  return Minimized; 
}

int QvkSettings::getCountdown()
{
  return Countdown; 
}

int QvkSettings::getFrames()
{
  return Frames; 
}

QString QvkSettings::getVideoCodec()
{
  return VideoCodec; 
}

QString QvkSettings::getAudioCodec()
{
  return AudioCodec; 
}

QString QvkSettings::getVideoContainer()
{
  return VideoContainer; 
}

int QvkSettings::getHideMouse()
{
  return HideMouse; 
}


// Gui
int QvkSettings::getX()
{
  return X; 
}

int QvkSettings::getY()
{
  return Y;  
}

int QvkSettings::getTab()
{
  return Tab; 
}

int QvkSettings::getSystray()
{
  return Systray;  
}


// Area
int QvkSettings::getAreaX()
{
  return AreaX;
}

int QvkSettings::getAreaY()
{
  return AreaY;
}

int QvkSettings::getAreaWidth()
{
  return AreaWidth;
}

int QvkSettings::getAreaHeight()
{
  return AreaHeight;
}

// Webcam
int QvkSettings::getWebcamX()
{
  return webcamX;
}

int QvkSettings::getWebcamY()
{
  return webcamY;
}

int QvkSettings::getWebcamHeight()
{
 return webcamHeight; 
}
 
int QvkSettings::getWebcamWidth()
{
  return webcamWidth;
}

bool QvkSettings::getWebcamMirrored()
{
  return webcamMirrored;
}

bool QvkSettings::getWebcamBorder()
{
  return webcamBorder;  
}

bool QvkSettings::getWebcamOverFullScreen()
{
  return webcamOverFullScreen;
}

// Magnifier
int QvkSettings::getMagnifierOnOff()
{
  return magnifierOnOff;
}

int QvkSettings::getMagnifierFormValue()
{
  return magnifierFormValue; 
}