
#include "QvkRegionController.h"

QvkRegionController::QvkRegionController()
{
  regionChoise = new QvkRegionChoise();
  regionRecord = new QvkRegionRecord();
}

QvkRegionController::~QvkRegionController()
{
}

void QvkRegionController::show()
{
  regionChoise->show();
}

void QvkRegionController::close()
{
  regionChoise->close();
}

int QvkRegionController::getX()
{
  return regionChoise->geometry().x(); 
}

int QvkRegionController::getY()
{
  return regionChoise->geometry().y();
}

int QvkRegionController::getWidth()
{
  return regionChoise->getWidth();
}

int QvkRegionController::getHeight()
{
  return regionChoise->getHeight();
}

int QvkRegionController::getXRecordArea()
{
  return regionChoise->getXRecordArea();
}

int QvkRegionController::getYRecordArea()
{
  return regionChoise->getYRecordArea();
}

void QvkRegionController::lockFrame( bool status )
{
  regionChoise->lockFrame( status );
  
  if ( status == true )
  {
    regionChoise->hide();
    regionRecord->show();
    regionRecord->setRecordGeometry( getX(), getY(), getWidth(), getHeight() );
  }
  else
  {
    regionRecord->hide();
    regionChoise->show();
  }
    
}
