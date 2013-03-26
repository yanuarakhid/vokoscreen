#ifndef QvkMail_H
#define QvkMail_H

#include <QObject>
#include <QRadioButton>
#include <QProcess>
#include <QPushButton>
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>


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


signals:

  
protected:  

  
private:
  QDialog * dialog;
  QRadioButton * mailRadioButton;
  QRadioButton * mailSelectedRadioButton;
};

#endif
