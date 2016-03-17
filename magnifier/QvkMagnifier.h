#ifndef QvkMagnifier_H 
#define QvkMagnifier_H

#include <QScreen>
#include <QPixmap>
#include <QDialog>
#include <QLabel>
#include <QDesktopWidget>
#include <QDebug>
#include <QApplication>
#include <QTimer>
#include <QObject>
#include <QRadioButton>
#include <QPropertyAnimation>
#include <QPushButton>

#include "ui_QvkMagnifierDialog.h"
#include "QvkSettings.h"

class QvkMagnifier: public QDialog
{ 
Q_OBJECT
public:
  QvkMagnifier();
  virtual ~QvkMagnifier();
  
public slots:
  void showDialogMagnifier();
  void magnifierShow();
  int getFormValue();
  
private slots:
  void closeEvent( QCloseEvent * event );
  int NewDistanceXLeft();
  int NewDistanceXRight();
  void mytimer();  
  void setMagnifier();
  void Magnifier200x200();
  void Magnifier400x200();
  void Magnifier600x200();
  
  int getDistanceX();
  int getDistanceY();

  void closeDialog();
  
protected:  

  
signals:
    void closeMagnifier();
  
  
private:
  QLabel *label;
  int border;
  int distanceX;
  int distanceY;
  QRadioButton *radioButton1;
  QRadioButton *radioButton2;
  QRadioButton *radioButton3;
  int faktor;
  int formValue;
  QDialog *newDialog;
  QTimer *timer;
  QvkSettings vkSettings;
  
};


#endif
