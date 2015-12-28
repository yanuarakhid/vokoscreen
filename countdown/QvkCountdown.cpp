#include "QvkCountdown.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QFrame>
#include <QLabel>
#include <QTest>


QvkCountdown::QvkCountdown()
{
}

QvkCountdown::~QvkCountdown()
{
}

void QvkCountdown::Countdown( int value )
{

    QDesktopWidget *desk = QApplication::desktop();
    int Width = 250;
    int Height = 250;;
    int x = ( desk->screenGeometry().width() / 2 ) - ( Width / 2 );
    int y = ( desk->screenGeometry().height() / 2 ) -( Height / 2 );
    
    QFrame * countdownDialog = new QFrame();
    countdownDialog->setGeometry( x, y, Width, Height );
    countdownDialog->setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    //if( QX11Info::isCompositingManagerRunning() ) // *********************************************** vorerst ersatzlos gestrichen **************************************************
       countdownDialog->setAttribute( Qt::WA_TranslucentBackground, true );
    
    countdownDialog->show();
    
    QFont qfont;;
    qfont.setPixelSize( 200 );
    
    QLabel * label = new QLabel( countdownDialog );
    label->setGeometry( 0, 0, Width, Height );
    label->setAlignment( Qt::AlignCenter );    
    countdownDialog->setFont( qfont );
    label->show();
    
    QPalette pal( label->palette() );
    pal.setColor( QPalette::Foreground, Qt::red);
    label->setPalette( pal );
    
    for ( int i = value; i >= 1; i-- )
    {
      label->setText( "" );
      QCoreApplication::processEvents( QEventLoop::AllEvents );     
      
      label->setText( QString::number( i ) );
      
      QCoreApplication::processEvents( QEventLoop::AllEvents );     
      label->update();
      for ( int ii = 1; ii <= 10; ii++)
      {
        QCoreApplication::processEvents( QEventLoop::AllEvents );     
        QTest::qSleep( 100 );
      }
    }

    // Der Desktopanimation "Langsames ausblenden" entgegenwirken
    label->setText( "" );
    QCoreApplication::processEvents( QEventLoop::AllEvents );     
    label->hide();
    QCoreApplication::processEvents( QEventLoop::AllEvents );     
    countdownDialog->close();
}
