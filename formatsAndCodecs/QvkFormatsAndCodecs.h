
#ifndef QvkFormatsAndCodecs_H
#define QvkFormatsAndCodecs_H

#include <QApplication>
#include <QMainWindow> 
#include <QLabel>

class QvkFormatsAndCodecs: public QObject
{
    Q_OBJECT
    
public:
    QvkFormatsAndCodecs( QString recordApplikation );
    virtual ~QvkFormatsAndCodecs();
  
    
signals:

 
public slots:
  bool isCodecAvailable( QString typeOfCodec, QString nameOfCodec, bool *experimental );
  bool isFormatAvailable( QString format);


private slots:

  
private:
  QString recordApplikation;
  QStringList ListCodecs;
  
  enum Codec{ Video, Audio };
  
protected:  

  
};

#endif
