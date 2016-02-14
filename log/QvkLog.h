#ifndef QvkLog_H 
#define QvkLog_H

//#include "ui_vokoscreen.h" //***************************

#include <QObject>
/*
QT_BEGIN_NAMESPACE
  namespace Ui { class Ui_MainWindow ; }
QT_END_NAMESPACE
*/
class QvkLog : public QObject
{
    Q_OBJECT

public:
  QvkLog();

  virtual ~QvkLog();
  

public slots:
    void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

  
signals:
    void newLogText( QString txt );

  
private slots:

  
private:
    //Ui_MainWindow *GuiVokoscreen;

  
protected:
  
  
};

#endif
