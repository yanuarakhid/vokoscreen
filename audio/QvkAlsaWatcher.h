#ifndef QvkAlsaWatcher_H
#define QvkAlsaWatcher_H

#include <QObject>
#include <QStringList>

class QvkAlsaWatcher: public QObject
{ 
Q_OBJECT
public:
  QvkAlsaWatcher();
  virtual ~QvkAlsaWatcher();


public slots:

  
private slots:
  void AlsaWatcherTimer();
  
  
signals:
  /**
   * Dieses Signal wird ausgelöst wenn sich die Anzahl der Alsageräte ändert
   * int count -> Anzahl der Geräte
   */
  void changed( QStringList );

  
protected:  

  
private:
  int AlsaCardCounter;
  
};

#endif
