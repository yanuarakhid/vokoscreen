#ifndef QvkWebcamController_H 
#define QvkWebcamController_H

#include "QvkWebcamWatcher.h"
#include "QvkCapturethread.h"
#include "QvkWebcamWatcher.h"
#include "QvkWebcamWindow.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QMessageBox>
#include <QMainWindow>
#include <QLabel>

class QvkWebcamController : public QObject
{
    Q_OBJECT

public:
  QvkWebcamController( QCheckBox *checkBox, QComboBox *comboBoxCount );
  virtual ~QvkWebcamController();
  
  
public slots:
  void webcamCloseEvent();

  
private slots:
  void webcamChangedEvent( QStringList deviceList );
  void webcamAddedEvent( QStringList deviceList, QStringList addedDevices );
  void webcamRemovedEvent( QStringList deviceList, QString removedDevice );
  void setWebcamOnOff( bool value );
  void setNewImage( QImage image );

  
private:
  QCheckBox *checkBox;
  QComboBox *comboBox;

  QvkWebcamWatcher * myWebcamWatcher;
  CaptureThread *captureThread;
  QvkWebcamWindow *webcamWindow;
 
protected:
  
  
};

#endif
