
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
  bool getCodec( QString value );

private slots:

  
private:
  QString recordApplikation;
  QString codecs;
  
protected:  

  
};

#endif
