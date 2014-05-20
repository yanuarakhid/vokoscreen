#ifndef QvkWebcamWatcher_H 
#define QvkWebcamWatcher_H

#include <QStringList>
#include <QDir>
#include <QFileSystemWatcher>
#include <QDebug>

class QvkWebcamWatcher: public QObject
{
  
Q_OBJECT

public:
  QvkWebcamWatcher();
  virtual ~QvkWebcamWatcher();


public slots:
  void myfileSystemWatcher( QString path );
  int getWebcamCount();

  
private slots:
  QStringList addedDevices();
  QString removedDevice();

  
signals:
  void changed( QStringList deviceList );
  void removed( QStringList deviceList, QString removedDevice );
  void added( QStringList deviceList, QStringList stringList );

  
protected:  

  
private:
  QStringList deviceList;
  QStringList oldDeviceList;
  int webcamCount;

};

#endif
