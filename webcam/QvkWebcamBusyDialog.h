#ifndef QvkWebcamBusyDialog_H 
#define QvkWebcamBusyDialog_H

#include "ui_QvkWebcamBusyDialog.h"

class QvkWebcamBusyDialog: public QDialog
{
Q_OBJECT
public:    
    QvkWebcamBusyDialog( QString Name, QString Device, QString Busyby );
    virtual ~QvkWebcamBusyDialog();

    
public:
 

public slots:

  
private:
    QDialog *newDialog;

    
private slots:
    void closeDialog();

  
protected:


};

#endif // QvkWebcamBusyDialog_H