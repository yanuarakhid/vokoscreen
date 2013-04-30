#ifndef QvkWebcam_H 
#define QvkWebcam_H

#include <cv.h>
#include <highgui.h>

#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QContextMenuEvent>
#include <QDebug>
#include <QSettings>
#include <QComboBox>

class QvkWebcam : public QMainWindow
{
    Q_OBJECT

    
public:
//    QvkWebcam( int value );
    QvkWebcam();
    virtual ~QvkWebcam();
    void showWebcam();
    int getDeviceNumber();
    void setDeviceNumber( int value );

private:
    QLabel *label;
    QImage ConvertImage( IplImage *source );
    IplImage *image;
    CvCapture *source;
    int deviceNumber;
    int idTimer;
    
    bool border;
    int X;
    int Y;
    int Width;
    int Height;
    
    
public slots:
    void setClose();
    bool isBusy();
    void saveSettings();

    
private slots:
    void set160x120();
    void set320x240();
    void set640x480();
    void setNoBorder();
    void setBorder();
    void closeMenue();

    void setValueBorder( bool value );
    void setValueX( int value );
    void setValueY( int value );
    void setValueWidth( int value );
    void setValueHeight( int value );
    
    bool getValueBorder();
    int getValueX();
    int getValueY();
    int getValueWidth();
    int getValueHeight();
    

protected:
    void timerEvent( QTimerEvent* );
    void resizeEvent( QResizeEvent * );
    
    // closeEvent wird ausgelößt wenn Videofenster geschloßen wird
    void closeEvent( QCloseEvent * event );
    void contextMenuEvent( QContextMenuEvent *event );

    void moveEvent( QMoveEvent * event );

signals:
    void closeWebcam();

  
};

#endif
