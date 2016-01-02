
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
  bool getCodec( QString typeOfCodec, QString nameOfCodec, bool *experimental );
  

private slots:

  
private:
  QString recordApplikation;
  QStringList ListCodecs;
  
  enum Codec{ Video, Audio };
  
protected:  

  
};

#endif
