#ifndef QvkLupe_H 
#define QvkLupe_H

#include <QDialog>
#include <QLabel>
#include <QPixmap>
#include <QDesktopWidget>
#include <QDebug>
#include <QApplication>
#include <QTimer>
#include <QObject>
#include <QRadioButton>

class QvkLupe: public QDialog
{ 
Q_OBJECT
public:
  QvkLupe();
  virtual ~QvkLupe();
  
   //QDialog *dialog;
   
public slots:
  void getDialogLupe( QWidget *parent );

  
private slots:
  void closeEvent( QCloseEvent * event );
  int NewDistanzXLeft();
  int NewDistanzXRight();
  void mytimer();  
  void setLupe();
  void Lupe200x200();
  void Lupe400x200();
  void Lupe600x200();
  
  int getDistanzX();
  int getDistanzY();


protected:  

  
signals:
    void closeLupe();
  
  
private:
  QLabel *label;
  int border;
  int distanzX;
  int distanzY;
  QRadioButton *radioButton1;
  QRadioButton *radioButton2;
  QRadioButton *radioButton3;
  int faktor;
  int formValue;
};


#endif
