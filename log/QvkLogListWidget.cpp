
#include "QvkLogListWidget.h" 

#include <QDebug>
#include <QMenu>
#include <QContextMenuEvent>
#include <QClipboard>
#include <QApplication>

QvkLogListWidget::QvkLogListWidget()
{
}


QvkLogListWidget::~QvkLogListWidget()
{
}


void QvkLogListWidget::contextMenuEvent( QContextMenuEvent *event )
{
  QAction *actionCopy  = new QAction( "Copy all", this );
  connect( actionCopy, SIGNAL( triggered() ), this, SLOT( copyAll() ) );

  QMenu menu;
  menu.addAction( actionCopy );
  menu.addSeparator();
  menu.exec( event->globalPos() );
}


void QvkLogListWidget::copyAll()
{
  QClipboard *clipboard = QApplication::clipboard();  
  QString string;
  for ( int i = 0; i < count(); ++i )
  {
    QListWidgetItem *listWidgetItem;
    listWidgetItem = item( i );
    string = string + listWidgetItem->text() + "\n";
  }
  clipboard->setText( string );
  
}