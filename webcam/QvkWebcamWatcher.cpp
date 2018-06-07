#include "QvkWebcamWatcher.h" 
#include "QvkAllLoaded.h"

#include <QCameraInfo>

QvkWebcamWatcher::QvkWebcamWatcher()
{
    oldcount = 0;
    descriptionList.clear();
    deviceNameList.clear();
    oldDescriptionList.clear();
    oldDeviceNameList.clear();

    timer = new QTimer(this);
    connect( timer, SIGNAL( timeout() ), this, SLOT( detectCameras() ) );
}


QvkWebcamWatcher::~QvkWebcamWatcher()
{
}


void QvkWebcamWatcher::getAllCameraDescription()
{
    qDebug() << "[vokoscreen]" << "---Begin search cameras---";
    oldDescriptionList = descriptionList;
    oldDeviceNameList = deviceNameList;
    descriptionList.clear();
    deviceNameList.clear();

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach ( const QCameraInfo &cameraInfo, cameras )
    {
      if ( cameraInfo.description() != "screen-capture-recorder" )
      {
        qDebug() << "[vokoscreen]" << cameraInfo.description() << cameraInfo.deviceName();
        descriptionList << cameraInfo.description();
        deviceNameList << cameraInfo.deviceName();
      }
    }
    qDebug() << "[vokoscreen]" << "---End search cameras---";
    qDebug(" ");
    emit webcamDescription( descriptionList, deviceNameList );
}

/*
 * Return removed device
 */
QString QvkWebcamWatcher::removedDeviceName( QStringList mydeviceNameList, QStringList myoldDeviceNameList )
{
  QStringList removedList;
  int x;
  for ( x = 0; x < myoldDeviceNameList.count(); x++ )
  {
    removedList = mydeviceNameList.filter( myoldDeviceNameList[ x ] );
    if ( removedList.empty() )
      break;
  }
  return myoldDeviceNameList[ x ];
}


void QvkWebcamWatcher::startStopCameraTimer( bool value )
{
    if ( value == true )
    {
        timer->start(1000);
    }

    if ( value == false )
    {
        timer->stop();
    }
}


/*
 * Is called periodically by the timer
 */
void QvkWebcamWatcher::detectCameras()
{
    timer->stop();
    int newcount = QCameraInfo::availableCameras().count();

    if ( newcount == 0 )
    {
        cameraLoaded = true;
    }

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach ( const QCameraInfo &cameraInfo, cameras )
    {
        if ( cameraInfo.description() == "screen-capture-recorder" )
        {
            newcount--;
        }
    }

    if ( newcount > oldcount )
    {
        getAllCameraDescription();
        oldcount = newcount;
        timer->start(1000);
        return;
    }

    if ( newcount < oldcount )
    {
        getAllCameraDescription();
        oldcount = newcount;

        // detected which camera was removed
        QString cameraDevice = removedDeviceName( deviceNameList , oldDeviceNameList );
        emit removedCamera( cameraDevice );
        timer->start(1000);
        return;
    }

    timer->start(1000);
}
