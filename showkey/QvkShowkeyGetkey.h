#ifndef QvkShowkeyGetkey_H 
#define QvkShowkeyGetkey_H

#include <QDebug>
#include <QObject>
#include <QThread>
#include <QStringList>

class QvkShowkeyGetkey: public QThread
{
Q_OBJECT
public:    
  QvkShowkeyGetkey();
  virtual ~QvkShowkeyGetkey();
  QStringList PrintKeyTable();

public:

  
public slots:
  QString getKey( QStringList list, int code );
  QString getKeyShift( QStringList list, int code );
  //QString getKeyCaps( QStringList list, int code );

  
private:
  
  
private slots:
  
  
signals:
  void pressedKey( QString value );

  
protected:
  void run();
  
};

#endif // QvkXev_H
