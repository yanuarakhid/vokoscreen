#ifndef QvkMsgInWebcamWindow_H 
#define QvkMsgInWebcamWindow_H

#include "QvkWebcamWindow.h"

#include <QLabel>

class QvkMsgInWebcamWindow: public QLabel
{
  
Q_OBJECT

public:
  QvkMsgInWebcamWindow( QObject *parent, QWidget *webcamWindow, QString value );
  virtual ~QvkMsgInWebcamWindow();


public slots:

  
private slots:
  void setNewSize(QSize size);

  
signals:

    
protected:  

  
private:


};

#endif
