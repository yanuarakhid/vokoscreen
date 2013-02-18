#ifndef QvkAlsaDevice_H
#define QvkAlsaDevice_H

#include <QObject>


#include <stdlib.h>
#include <alsa/asoundlib.h>


struct AlsaDevice_ {
   char *device_name;
   int channels;
   snd_pcm_t *capture_handle;
   snd_pcm_t *playback_handle;
   int readN, writeN;
   struct pollfd *read_fd, *write_fd;
};


class QvkAlsaDevice: public QObject
{ 
Q_OBJECT
public:
  QvkAlsaDevice( QString cardx );
  virtual ~QvkAlsaDevice();
  QvkAlsaDevice();


public slots:
  QString getCard();        // libasound
  QString getAlsaHw();      // libasound
  QString getChannel();     // libasound
  QString getAlsaName();    // libasound
  QString getAlsaSample();
  QString getAlsaVersion(); // libasound
  bool isbusy();            // libasound
  
  
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
