#ifndef QvkMail_H 
#define QvkMail_H

#include <QObject>
#include <QRadioButton>
#include <QProcess>
#include <QPushButton>
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QLabel>

#include "ui_QvkMailDialog.h"

class QvkMail: public QObject
{ 
Q_OBJECT
public:
  QvkMail();
  virtual ~QvkMail();
  QvkMail( QWidget *object );


public slots:

  
private slots:
  void startMailClientWithLastFile( QString file );
  void startMailClientWithSelectedFiles( QStringList fileList );

  QString lastMovie();
  void selection();
  QStringList selectedMail();
  void closeDialog();


signals:

  
protected:  

  
private:
  QDialog *newDialog;
  Ui_MailDialog myUiMailDialog;

};

#endif
