#ifndef QvkWebcamWindow_H 
#define QvkWebcamWindow_H

#include <QMainWindow>
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QX11Info>
#include <QDebug>

#include "QvkSettings.h"

class QvkWebcamWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QvkWebcamWindow();
    virtual ~QvkWebcamWindow();
    QString currentDevice;
    QLabel *webcamLabel;

    QAction *action160x120;
    QAction *action320x240;
    QAction *action640x480;
    QAction *actionUserDefined;
    QAction *actionBorder;
    QAction *actionVisibleOverFullscreen;
    QAction *actionClose;
    QMenu menu;


private:
    bool border;
    bool overFullScreen;
    QvkSettings vkSettings;
    
public slots:
    int getValueX();
    int getValueY();
    int getValueWidth();
    int getValueHeight();
    bool getOverFullScreen();
    bool getValueBorder();
    void setVisibleOverFullscreen();

    
private slots:
    void set160x120();
    void set320x240();
    void set640x480();
    void setActionUserDefined();
    void setBorder();
//    void setVisibleOverFullscreen();
    void closeMenue();
    void setValueBorder( bool value );
    void setOverFullScreen( bool value );
    
    void setVisibleOverFullscreen_New();

protected:
    void closeEvent( QCloseEvent * event );
    void contextMenuEvent( QContextMenuEvent *event );
    void resizeEvent ( QResizeEvent * );
    
signals:
    void closeWebcamWindow();
    void enterEventWebcamWindow();
    void setOverScreen();
};

#endif
