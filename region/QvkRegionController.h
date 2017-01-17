#ifndef QvkRegionController_H
#define QvkRegionController_H

#include <QObject>

#include "QvkRegionChoise.h"
#include "QvkRegionRecord.h"

class QvkRegionController: public QObject
{
    Q_OBJECT
    
public:
   QvkRegionController();
   virtual ~QvkRegionController();

   int getX();
   int getY();
   int getWidth();
   int getHeight();
   
   int getXRecordArea();
   int getYRecordArea();
   
   void lockFrame( bool status );

   void show();
   void close();
   void hide(); // neu

   
signals:

    
private slots:
  
  
public slots:
  
  
private:
  QvkRegionChoise *regionChoise;
  QvkRegionRecord *regionRecord;
  
  
protected:  

};

#endif
