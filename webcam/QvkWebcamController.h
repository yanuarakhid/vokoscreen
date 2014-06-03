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
#include <QDial>
#include <QRadioButton>

class QvkWebcamController : public QObject
{
    Q_OBJECT

public:
   QvkWebcamController( QCheckBox *myCheckBox, QComboBox *myComboBox, QCheckBox *myMirrorCheckBox, 
					  QFrame *myRotateFrame ,QDial *myRotateDial, QRadioButton *myRadioButtonTopMiddle, QRadioButton *myRadioButtonRightMiddle, QRadioButton *myRadioButtonBottomMiddle, QRadioButton *myRadioButtonLeftMiddle );
  
  //QvkWebcamController( QCheckBox *checkBox, QComboBox *comboBoxCount, QCheckBox *myMirrorCheckBox, QFrame *myRotateFrame,QDial *myRotateDial );
  virtual ~QvkWebcamController();
  
  
public slots:
  void webcamCloseEvent();

  
private slots:
  void webcamChangedEvent( QStringList deviceList );
  void webcamAddedEvent( QStringList deviceList, QStringList addedDevices );
  void webcamRemovedEvent( QStringList deviceList, QString removedDevice );
  void setWebcamOnOff( bool value );
  void setNewImage( QImage image );
  void setMirrorOnOff( bool value );
  void rotateDialclicked();

  
private:
  QFrame *rotateFrame;
  QCheckBox *checkBox;
  QComboBox *comboBox;
  QCheckBox *mirrorCheckBox;
  QDial *rotateDial;
  QRadioButton *radioButtonTopMiddle;
  QRadioButton *radioButtonRightMiddle;
  QRadioButton *radioButtonBottomMiddle;
  QRadioButton *radioButtonLeftMiddle;
  QvkWebcamWatcher * myWebcamWatcher;
  CaptureThread *captureThread;
  QvkWebcamWindow *webcamWindow;
  bool mirrored;

  
protected:
  
  
};

#endif
