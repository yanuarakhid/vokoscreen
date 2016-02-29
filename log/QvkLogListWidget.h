#ifndef QvkLogListWidget_H
#define QvkLogListWidget_H

#include <QListWidget>
#include <QAction>

class QvkLogListWidget : public QListWidget
{
    Q_OBJECT

public:
  QvkLogListWidget();
  virtual ~QvkLogListWidget();
  

public slots:

  
signals:

  
private slots:
  void copyAll();

  
private:

  
protected:
   void contextMenuEvent( QContextMenuEvent *event );
  
  
};

#endif
