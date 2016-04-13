
#include "QvkMail.h" 

using namespace std;

QvkMail::QvkMail()
{
  
}


QvkMail::QvkMail( QWidget *object )
{
  (void)object;
  
  newDialog = new QDialog;
  newDialog->setModal( true );

  myUiMailDialog.setupUi( newDialog );
  newDialog->show();  
  
  connect( myUiMailDialog.sendPushButton, SIGNAL( clicked() ), this, SLOT( selection() ) );
  connect( myUiMailDialog.cancelPushButton, SIGNAL( clicked() ), this, SLOT( closeDialog() ) ) ;
}


QvkMail::~QvkMail()
{
}


void QvkMail::closeDialog()
{
  newDialog->close();
}


void QvkMail::selection()
{
  if ( myUiMailDialog.mailRadioButton->isChecked() )
    startMailClientWithLastFile( lastMovie() );

  if ( myUiMailDialog.mailSelectedRadioButton->isChecked() )
    startMailClientWithSelectedFiles( selectedMail() );
}


void QvkMail::startMailClientWithLastFile( QString file )
{
  if ( file > "" )
  {
    // The body String like not Space
    QString bodyString = tr( "You find the Video as attachment" );
  
    QString processString = "xdg-email --utf8 --attach ";
            processString.append( "\"" );
	    processString.append( file );
            processString.append( "\"" );
            processString.append( " --body ");
	    processString.append( "\"" );
	    processString.append( bodyString );
	    processString.append( "\"" );

    qDebug() << "[vokoscreen]" << "startMailClientWithLastFile:" << processString;
  
    QProcess Process;
    Process.startDetached( processString );
    Process.close();
  }

  newDialog->close();
}


void QvkMail::startMailClientWithSelectedFiles( QStringList fileList )
{
   // The body String like not Space
  QString bodyString = tr( "You find the Video as attachment" );

  QString files;
  for ( int i = 1; i <= fileList.count(); i++ )
  {
    files = files + " --attach ";
    files.append( "\"" );
    files = files + fileList[ i - 1 ] ;
    files.append( "\"" );
  }
  
  if ( not fileList.empty())
  {
    QString processString = "xdg-email --utf8" + files + " --body \"" + bodyString + "\"";
  
    qDebug() << "[vokoscreen]" << "startMailClientWithSelectedFiles:" << processString;
  
    QProcess Process;
    Process.startDetached( processString );
    Process.close();
  }
  
  newDialog->close();
}


QString QvkMail::lastMovie()
{
#ifdef QT4
  QDir dir( QDesktopServices::storageLocation( QDesktopServices::MoviesLocation ) );
#endif
#ifdef QT5
  QDir dir( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );
#endif
  
  QStringList filters;
  filters << "vokoscreen*";
  QStringList List = dir.entryList( filters, QDir::Files, QDir::Time );
  if ( List.isEmpty() )
     return "";
  else
     return dir.filePath( List.first() );
}


QStringList QvkMail::selectedMail()
{
#ifdef QT4
  QString a = QDesktopServices::storageLocation( QDesktopServices::MoviesLocation );
  
  QStringList files = QFileDialog::getOpenFileNames( newDialog,
                                                     tr( "Select one or more files" ),
                                                     QDesktopServices::storageLocation( QDesktopServices::MoviesLocation ) , 
                                                     "*.*" );
#endif

#ifdef QT5
  QString a = QStandardPaths::writableLocation( QStandardPaths::MoviesLocation );
  
  QStringList files = QFileDialog::getOpenFileNames( newDialog,
                                                     tr( "Select one or more files" ),
                                                     QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) , 
                                                     "*.*" );
#endif
  
  return files;
}
