#ifndef QvkAlsaWatcher_H
#define QvkAlsaWatcher_H

#include <alsa/asoundlib.h>

#include <QObject>
#include <QStringList>
#include <QTimer>
#include <QDebug>

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
