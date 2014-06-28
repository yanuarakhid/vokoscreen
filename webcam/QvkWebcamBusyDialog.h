#ifndef QvkWebcamBusyDialog_H 
#define QvkWebcamBusyDialog_H

#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QVBoxLayout>


class QvkWebcamBusyDialog: public QDialog
{
Q_OBJECT
public:    
    QvkWebcamBusyDialog( QString Name, QString Device, QString Busyby );
    virtual ~QvkWebcamBusyDialog();

    
public:
 

public slots:

  
private:
    QVBoxLayout *verticalLayout_Dialog;
    QLabel *label_Titel_Busy;
    QHBoxLayout *horizontalLayout_Picture_and_Info;
    QFrame *frame_Picture;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_Picture;
    QLabel *label_Picture;
    QFormLayout *formLayout_Info;
    QLabel *label_Name;
    QLabel *label_Name_Value;
    QLabel *label_Device;
    QLabel *label_Device_Value;
    QLabel *label_UsedBy;
    QLabel *label_UsedBy_Value;
    QHBoxLayout *horizontalLayout_ButtonBox;
    QDialogButtonBox *buttonBox;
  
private slots:

  
protected:


};

#endif // QvkWebcamBusyDialog_H