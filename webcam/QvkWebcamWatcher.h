#ifndef QvkWebcamWatcher_H 
#define QvkWebcamWatcher_H

#include <QStringList>
#include <QObject>
#include <QTimer>

class QvkWebcamWatcher: public QObject
{
  
Q_OBJECT

public:
  QvkWebcamWatcher();
  virtual ~QvkWebcamWatcher();


public slots:
  void startStopCameraTimer( bool value );

  
private slots:
  void getAllCameraDescription();
  void detectCameras();
  QString removedDeviceName( QStringList mydeviceNameList, QStringList myoldDeviceNameList );

  
signals:
  void webcamDescription( QStringList description, QStringList deviceName );
  void removedCamera( QString value );


protected:  

  
private:
  QTimer *timer;
  int oldcount;
  QStringList descriptionList;
  QStringList deviceNameList;
  QStringList oldDescriptionList;
  QStringList oldDeviceNameList;


};

#endif
