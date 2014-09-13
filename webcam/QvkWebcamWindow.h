#ifndef QvkWebcamWindow_H 
#define QvkWebcamWindow_H

#include <QMainWindow>
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QX11Info>

class QvkWebcamWindow : public QMainWindow
{
    Q_OBJECT

    
public:
    QvkWebcamWindow();
    virtual ~QvkWebcamWindow();
    QString currentDevice;
    QLabel *webcamLabel;
    bool getValueBorder();
    int getValueX();
    int getValueY();
    int getValueWidth();
    int getValueHeight();

    
private:
    bool border;

    
public slots:

    
private slots:
    void set160x120();
    void set320x240();
    void set640x480();
    void setNoBorder();
    void setBorder();
    void setVisibleOverFullscreen();
    void closeMenue();
    void setValueBorder( bool value );

    
protected:
    void closeEvent( QCloseEvent * event );
    void contextMenuEvent( QContextMenuEvent *event );
    void resizeEvent ( QResizeEvent * );

    
signals:
    void closeWebcamWindow();

  
};

#endif
