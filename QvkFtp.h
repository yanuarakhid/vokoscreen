#ifndef QVKFTP_H
#define QVKFTP_H

#include <QDialog> 
#include <QLineEdit>
#include <QFtp>

class QvkFtp: public QDialog
{
    Q_OBJECT
    
public:
    QvkFtp();
    virtual ~QvkFtp();
    
    
signals:

    
private slots:
  void getLocalFile();
  void uploadLocalFile();
  void myStateChanged( int state );
  void myDestroyed();


private:
    QLineEdit *fileLineEdit;
    QLineEdit *userLineEdit;
    QLineEdit *passLineEdit;
    QLineEdit *serverLineEdit;
    QFtp *ftp;

  
protected:  

};

#endif
