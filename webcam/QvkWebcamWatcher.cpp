#include "QvkWebcamWatcher.h" 

using namespace std;

/**
 * Die Metohoden werden nur aufgerufen wenn sich in /dev/ was geändert hat
 */

QvkWebcamWatcher::QvkWebcamWatcher()
{
  webcamCount = 0;
  QFileSystemWatcher * fileSystemWatcher = new QFileSystemWatcher();
  fileSystemWatcher->addPath( "/dev/" );
  connect( fileSystemWatcher, SIGNAL( directoryChanged( QString ) ), this, SLOT( myfileSystemWatcher( QString ) ) );
}


QvkWebcamWatcher::~QvkWebcamWatcher()
{
}


int QvkWebcamWatcher::getWebcamCount()
{
  return webcamCount;
}


/**
 * Return added devices
 */
QStringList QvkWebcamWatcher::addedDevices()
{
  QStringList addedList;
  int x;
  for ( x = 0; x < deviceList.count(); x++ )
    if ( not oldDeviceList.contains( deviceList[ x ] ) )
      addedList << deviceList[ x ];

  return addedList;
}


/**
 * Return removed device
 */
QString QvkWebcamWatcher::removedDevice()
{
  QStringList removedList;
  int x;
  for ( x = 0; x < oldDeviceList.count(); x++ )
  {
    removedList = deviceList.filter( oldDeviceList[ x ] );
    if ( removedList.empty() )
      break;
  }
  
  return oldDeviceList[ x ];
}


void QvkWebcamWatcher::myfileSystemWatcher( QString path )
{
  QDir dir( path );
  QStringList filters;
  filters << "video*";
  deviceList = dir.entryList( filters, QDir::System, QDir::Time );
  deviceList.sort();
  
  // removed device
  if ( deviceList.count() <  webcamCount )
  {
    qDebug();
    qDebug() << "[vokoscreen] removed device" << removedDevice();
    qDebug() << "[vokoscreen] connected devices" <<  deviceList;
    
    webcamCount = deviceList.count();
    emit changed( deviceList );
    emit removed( deviceList, removedDevice() );
  }

  // added device
  if ( deviceList.count() >  webcamCount )
  {
    qDebug();
    qDebug() << "[vokoscreen] added device" << addedDevices();
    qDebug() << "[vokoscreen] connected devices" <<  deviceList;
    
    webcamCount = deviceList.count();
    emit changed( deviceList );
    emit added( deviceList, addedDevices() );
    emit readWebcamNames( deviceList );
  }
  
  if ( oldDeviceList.count() != deviceList.count() )
    oldDeviceList = deviceList;
  
}
