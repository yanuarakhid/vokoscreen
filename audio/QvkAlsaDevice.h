#ifndef QvkAlsaDevice_H
#define QvkAlsaDevice_H

#include <QObject>

class QvkAlsaDevice: public QObject
{ 
Q_OBJECT
public:
  QvkAlsaDevice( QString cardx );
  virtual ~QvkAlsaDevice();
  QvkAlsaDevice();


public slots:
  QString getCard();
  QString getAlsaHw();
  QString getChannel();
  QString getAlsaName();
  QString getAlsaSample();
  bool isbusy();
  QString getAlsaVersion();

  
private slots:
  void setCard( QString string );
  void setAlsaHw();
  void setChannel();
  void setAlsaName();
  void setAlsaSample();


signals:

  
protected:  

  
private:
  QString cardx;
  QString AlsaHw;  
  QString AlsaCannel;
  QString AlsaName;
  QString AlsaSample;
  
};

#endif
