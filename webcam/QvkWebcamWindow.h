#ifndef QvkWebcamWindow_H 
#define QvkWebcamWindow_H

#include <QMainWindow>
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>

class QvkWebcamWindow : public QMainWindow
{
    Q_OBJECT

    
public:
    QvkWebcamWindow();
    virtual ~QvkWebcamWindow();
    QString currentDevice;
    QLabel *webcamLabel;

    
private:
    bool border;
    int X;
    int Y;
    int Width;
    int Height;

    
public slots:

    
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
    void closeEvent( QCloseEvent * event );
    void contextMenuEvent( QContextMenuEvent *event );
    void resizeEvent ( QResizeEvent * );

    
signals:
    void closeWebcamWindow();

  
};

#endif
