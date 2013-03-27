#ifndef QvkWebcam_H 
#define QvkWebcam_H

#include <cv.h>
#include <highgui.h>

#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QContextMenuEvent>
#include <QDebug>

class QvkWebcam : public QMainWindow
{
    Q_OBJECT

    
public:
    QvkWebcam( int value );
    virtual ~QvkWebcam();
    void showWebcam();
    int getDeviceNumber();
    
private:
    QLabel *label;
    QImage ConvertImage( IplImage *source );
    IplImage *image;
    CvCapture *source;
    int deviceNumber;
    void setDeviceNumber( int value );

    
private slots:
    void set160x120();
    void set320x240();
    void set640x480();
 

protected:
    void timerEvent( QTimerEvent* );
    void resizeEvent( QResizeEvent * );
    
    // closeEvent wird ausgelößt wenn Videofenster geschloßen wird
    void closeEvent( QCloseEvent * event );
    void contextMenuEvent( QContextMenuEvent *event );

    
signals:
    void closeWebcam();

  
};

#endif
