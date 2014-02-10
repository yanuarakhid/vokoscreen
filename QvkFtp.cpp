
#include "QvkFtp.h"
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDebug>


QvkFtp::QvkFtp()
{
  setGeometry( 500, 200, 500, 500 );
  
  QLabel *userLabel = new QLabel( this );
  userLabel->setGeometry( 100, 100, 100, 30 );
  userLabel->setText( "User" );
  userLabel->show();
  
  userLineEdit = new QLineEdit( this );
  userLineEdit->setGeometry( 200, 100, 100, 30 );
  userLineEdit->show();
  
  QLabel *passLabel = new QLabel( this );
  passLabel->setGeometry( 100, 150, 100, 30 );
  passLabel->setText( "Password" );
  passLabel->show();

  passLineEdit = new QLineEdit( this );
  passLineEdit->setGeometry( 200, 150, 100, 30 );
  passLineEdit->show();
  
  QLabel *serverLabel = new QLabel( this );
  serverLabel->setGeometry( 100, 200, 100, 30 );
  serverLabel->setText( "Server" );
  serverLabel->show();

  serverLineEdit = new QLineEdit( this );
  serverLineEdit->setGeometry( 200, 200, 100, 30 );
  serverLineEdit->show();
  
  fileLineEdit = new QLineEdit( this );
  fileLineEdit->setGeometry( 100, 250, 200, 30 );
  fileLineEdit->show();
  
  QPushButton *pushButton = new QPushButton( this );
  pushButton->setGeometry( 300, 250, 100, 30 );
  pushButton->setText( "File..." );
  pushButton->show();
  connect( pushButton, SIGNAL( clicked() ), SLOT( getLocalFile() ) );

  QPushButton *uploadButton = new QPushButton( this );
  uploadButton->setGeometry( 100, 300, 300, 30 );
  uploadButton->setText( "Upload" );
  connect( uploadButton, SIGNAL( clicked() ), SLOT( uploadLocalFile() ) );

  connect( this, SIGNAL( finished( int )), SLOT( myDestroyed() ) );

  ftp = new QFtp( this );

}

QvkFtp::~QvkFtp()
{
}


void QvkFtp::getLocalFile()
{
  QString videoFile = QFileDialog::getOpenFileName ( this, 
						    "Open Directory",
						     QDesktopServices::storageLocation( QDesktopServices::MoviesLocation ),
						     "*.*" );
						     
  fileLineEdit->setText( videoFile );
  
}


void QvkFtp::uploadLocalFile()
{
  connect( ftp, SIGNAL( stateChanged( int ) ), SLOT( myStateChanged( int ) ) );
  
  ftp->connectToHost( serverLineEdit->text() );
  ftp->login( userLineEdit->text(), passLineEdit->text() );
}


void QvkFtp::myStateChanged( int state )
{
  if ( state == QFtp::Unconnected )
  {
    qDebug() << "[vokoscreen] There is no connection to the host";
  }

  if ( state == QFtp::Connected )
  {
    qDebug() << "[vokoscreen] Connection to the host has been achieved";
  }
  
  if ( state == QFtp::LoggedIn )
  {
    qDebug() << "[vokoscreen] Connection and user login have been achieved";
  }
}

void QvkFtp::myDestroyed()
{
  qDebug() << "[vokoscreen] close ftp connection";
  ftp->close();
}




