
#include "QvkMail.h"

#include <QDialog>
#include <QProcess>
#include <QPushButton>
#include <QRadioButton>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QDesktopServices>
#include <QIcon>
#include <QFileDialog>

using namespace std;

QvkMail::QvkMail()
{
  
}


QvkMail::QvkMail( QWidget *object )
{
  dialog = new QDialog( object );
  dialog->resize( 500, 200 );
  
  mailRadioButton = new QRadioButton( dialog );
  mailRadioButton->setGeometry( QRect( 20, 15, 200, 21 ) );
  mailRadioButton->setText( tr( "Mail last Video" ) );
  mailRadioButton->setChecked( true );
  
  mailSelectedRadioButton = new QRadioButton( dialog );
  mailSelectedRadioButton->setGeometry( QRect( 20, 36, 300, 21 ) );
  mailSelectedRadioButton->setText( tr( "Mail on or more selected Video" ) );
  
  QPushButton * sendPushbutton = new QPushButton( dialog );
  sendPushbutton->setGeometry( 200, 170, 80, 30 );
  sendPushbutton->setText( "Send" );
  sendPushbutton->show();
  connect( sendPushbutton, SIGNAL( clicked() ), this, SLOT( selection() ) );
  dialog->exec();
}


QvkMail::~QvkMail()
{
}


void QvkMail::selection()
{
  if ( mailRadioButton->isChecked() )
    startMailClientWithLastFile( lastMovie() );

  if ( mailSelectedRadioButton->isChecked() )
    startMailClientWithSelectedFiles( selectedMail() );
}


void QvkMail::startMailClientWithLastFile( QString file )
{
  if ( file > "" )
  {
    // The body String like not Space
    QString bodyString;
    bodyString = tr("You find the Video as attachment");
    bodyString.replace(" ","_");
  
    QString processString;
    processString = "xdg-email --attach " + file + " --body " + bodyString;
  
    qDebug() << "[vokoscreen]" << "startMailClientWithLastFile:" << processString;
  
    QProcess Process;
    Process.startDetached( processString );
    Process.close();
  }

  dialog->close();
}


void QvkMail::startMailClientWithSelectedFiles( QStringList fileList )
{
   // The body String like not Space
  QString bodyString;
  bodyString = tr("You find the Video as attachment");
  bodyString.replace(" ","_");

  QString files;
  for ( int i = 1; i <= fileList.count(); i++ )
  {
    files = files + " --attach ";
    files = files + fileList[ i - 1 ] ;
  }
  
  if ( not fileList.empty())
  {
    QString processString;
    processString = "xdg-email " + files + " --body " + bodyString;
  
    qDebug() << "[vokoscreen]" << "startMailClientWithSelectedFiles:" << processString;
  
    QProcess Process;
    Process.startDetached( processString );
    Process.close();
  }
  
  dialog->close();
}


QString QvkMail::lastMovie()
{
  QDir dir( QDesktopServices::storageLocation( QDesktopServices::MoviesLocation ) );
  QStringList filters;
  filters << "screencast*";
  QStringList List = dir.entryList( filters, QDir::Files, QDir::Time );
  if ( List.isEmpty() )
     return "";
  else
     return dir.filePath( List.first() );
}


QStringList QvkMail::selectedMail()
{

  QString a = QDesktopServices::storageLocation( QDesktopServices::MoviesLocation );
  
  QStringList files = QFileDialog::getOpenFileNames( dialog,
                                                     tr( "Select one or more files" ),
                                                     QDesktopServices::storageLocation( QDesktopServices::MoviesLocation ) , 
                                                     "*.*" );
  
  return files;
}
