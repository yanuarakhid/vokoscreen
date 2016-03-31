
#ifndef QvkPulse_H
#define QvkPulse_H

#include <QObject>
#include <QFrame>
#include <QVBoxLayout>

class QvkPulse: public QObject
{
    Q_OBJECT
    
public:
    QvkPulse();
    virtual ~QvkPulse();
    static QString getPulseDeviceName( int value, QWidget *Pulseframe );
    static int getCountCheckedPulseDevices( QWidget *Pulseframe );
    static void pulseUnloadModule();
    static const QString myPulseDevice( QWidget *Pulseframe );
    
    static QString getPulseInputDevices( int value );
    static QString getPulseInputName( int value );
    static int getPulseInputDevicesCount();

    void grepPactlOutput();
    QStringList grepListSourceNumberX( int value );
    QString getIconName( int value );

    
signals:

    
private slots:
  
  
private:
    QStringList listPactlOutput;
    QString getNameOfSourceNumber( int value );

  
protected:  

};

#endif
