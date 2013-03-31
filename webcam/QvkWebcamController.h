#ifndef QvkWebcamController_H 
#define QvkWebcamController_H

#include "QvkWebcam.h"
#include "QvkWebcamWatcher.h"

#include <QCheckBox>
#include <QDebug>
#include <QMessageBox>

class QvkWebcamController : public QObject
{
    Q_OBJECT

public:
  QvkWebcamController( QWidget * frame );
  virtual ~QvkWebcamController();
  QvkWebcamWatcher * myWebcamWatcher;
  //bool ifWebcamShow;

  
public slots:
  void webcamChangedEvent( QStringList deviceList );
  void webcamAddedEvent( QStringList deviceList, QStringList addedDevices );
  void webcamRemovedEvent( QStringList deviceList, QString removedDevice );
  void setGeometry( int x, int y, int width, int height);
  void webcamClose();
  bool isVisible();
  void saveSettings();
  
  
private slots:
  void webcam();
  void webcamCloseEvent();

  
private:
  QvkWebcam * vkWebcam;
  QCheckBox *checkBox;

  
protected:
  
  
};

#endif
