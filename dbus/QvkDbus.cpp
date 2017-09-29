#include "QvkDbus.h"


QvkDbus::QvkDbus(Ui_screencast GUI )
{
   myUi = GUI;
}

QvkDbus::QvkDbus()
{
}

QvkDbus::~QvkDbus()
{
}


void QvkDbus::startrecord()
{
   qDebug() << "ööööööööööööööööööööööööööööööööööööööööööööööööööööööööööööööö";
   myUi.recordButton->click();
}

/*
void QvkDbus::stoprecord()
{
   myUi.StopButton->click(); 
}


void QvkDbus::setAudioOn()
{
   if ( myUi.AudioOnOffCheckbox->checkState() == Qt::Unchecked )
   {
       myUi.AudioOnOffCheckbox->click();
   }
}


void QvkDbus::setAudioOff()
{
   if ( myUi.AudioOnOffCheckbox->checkState() == Qt::Checked )
   {
       myUi.AudioOnOffCheckbox->click();
   }
}
*/
