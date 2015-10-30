#ifndef QvkShowkeyGetkey_H 
#define QvkShowkeyGetkey_H

#include <QDebug>
#include <QObject>
#include <QThread>

class QvkShowkeyGetkey: public QThread
{
Q_OBJECT
public:    
    QvkShowkeyGetkey();
    virtual ~QvkShowkeyGetkey();

public:

  
public slots:

  
private:
  
  
private slots:
  
  
signals:
  void pressedKey( QString value );

  
protected:
  void run();
  
};

#endif // QvkXev_H
