#ifndef QvkLupe_H
#define QvkLupe_H
#include <QDialog>
#include <QLabel>

class QvkLupe: public QDialog
{ 
Q_OBJECT
public:
  QvkLupe();
  virtual ~QvkLupe();
  
  
public slots:
  void mytimer();  
  void setLupe();
  int NewDistanzXLeft();
  int NewDistanzXRight();

  
private slots:
  void closeEvent( QCloseEvent * event );
  
  
protected:  

  
signals:
    void closeLupe();
  
  
private:
  QLabel *label;
  int border;
  int distanzX;
  int distanzY;
};


#endif
