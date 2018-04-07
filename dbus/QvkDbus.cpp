#include "QvkDbus.h"
#include "vokoscreenqvkdbus_adaptor.h"


QvkDbus::QvkDbus()
{
}


QvkDbus::QvkDbus( Ui_screencast value )
{
    myUi = value;
    
    new GuiAdaptor( this );
    QDBusConnection dbusConnection = QDBusConnection::sessionBus();
    dbusConnection.registerObject("/gui", this);
    dbusConnection.registerService("org.vokoscreen.screencast");
}


QvkDbus::~QvkDbus()
{  
}


QString QvkDbus::showAllMethods()
{
    QStringList functions;
    for (int n = 0; n < staticMetaObject.methodCount(); n++) {

        functions.append(QString::fromLocal8Bit(staticMetaObject.method(n).name() ) );

        int index = functions.indexOf("destroyed");
        functions.removeAt( index );

        index = functions.indexOf("objectNameChanged");
        functions.removeAt( index );

        index = functions.indexOf("deleteLater");
        functions.removeAt( index );

        index = functions.indexOf("_q_reregisterTimers");
        functions.removeAt( index );

        index = functions.indexOf("vokoscreenFinishLoaded");
        functions.removeAt( index );

        index = functions.indexOf("showAllMethods");
        functions.removeAt( index );

        int value = staticMetaObject.method(n).parameterCount();

        if ( ( value == 1 ) and ( !functions.empty() ) )
        {
           functions.replace( functions.count()-1, QString::fromLocal8Bit( staticMetaObject.method(n).name()).append( " int" ) );
        }
    }

    functions.sort( Qt::CaseInsensitive );

    QString string = functions.join( ", " );

    return string;
}


void QvkDbus::vokoscreenFinishLoaded()
{
    vokoscreenLoaded = "0";

    qDebug() << "[vokoscreen] ---Begin DBus methods--- ";
    QString string = showAllMethods();
    qDebug().noquote() << "[vokoscreen] methods:" << string;
    qDebug() << "[vokoscreen] ---End DBus methods--- ";
    qDebug(" ");
}


QString QvkDbus::isVokoscreenLoaded()
{
  return vokoscreenLoaded;
}


int QvkDbus::startrecord()
{
   if ( myUi.recordButton->isEnabled() == true )
   {
     myUi.recordButton->click();
     return 0;
   }
   else
   {
     return 1;
   }
}


int QvkDbus::stoprecord()
{
   if ( myUi.StopButton->isEnabled() == true )
   {
     myUi.StopButton->click();
     return 0;
   }
   else
   {
     return 1;
   }
}


int QvkDbus::FullScreen()
{
    if ( ( myUi.FullScreenRadioButton->isEnabled() == true ) and ( myUi.FullScreenRadioButton->isChecked() == false ) )
    {
      myUi.FullScreenRadioButton->click();
      return 0;
    }
    else
    {
      return 1;
    }
}


int QvkDbus::Window()
{
   if ( ( myUi.WindowRadioButton->isEnabled() == true ) and ( myUi.WindowRadioButton->isChecked() == false ) )
   {
     myUi.WindowRadioButton->click();
     return 0;
   }
   else
   {
     return 1;
   }
}



int QvkDbus::Area()
{
   if ( ( myUi.AreaRadioButton->isEnabled() == true ) and ( myUi.AreaRadioButton->isChecked() == false ) )
   {
     myUi.AreaRadioButton->click();
     return 0;
   }
   else
   {
     return 1;
   }
}


int QvkDbus::AreaReset()
{
   if ( myUi.areaResetButton->isEnabled() == true )
   {
      myUi.areaResetButton->click();
      return 0;
   }
   else
   {
      return 1;
   }
}


int QvkDbus::AudioOn()
{
   if ( ( myUi.AudioOnOffCheckbox->isEnabled() == true ) ) //and ( myUi.AudioOnOffCheckbox->checkState() == Qt::Unchecked ) )
   {
       myUi.AudioOnOffCheckbox->setCheckState(Qt::Unchecked);
       myUi.AudioOnOffCheckbox->click();
       return 0;
   }
   else
   {
       return 1;
   }
}


int QvkDbus::AudioOff()
{
   if ( ( myUi.AudioOnOffCheckbox->isEnabled() == true ) ) //and ( myUi.AudioOnOffCheckbox->checkState() == Qt::Checked ) )
   {
       myUi.AudioOnOffCheckbox->setCheckState(Qt::Checked);
       myUi.AudioOnOffCheckbox->click();
       return 0;
   }
   else
   {
       return 1;
   }
}


QString QvkDbus::WebcamOn()
{
    if ( myUi.webcamCheckBox->checkState() == Qt::Unchecked )
    {
        myUi.webcamCheckBox->click();
        return "0";
    }
    else
    {
        return "1";
    }
}


QString QvkDbus::WebcamOff()
{
    if ( myUi.webcamCheckBox->checkState() == Qt::Checked  )
    {
        myUi.webcamCheckBox->click();
        return "0";
    }
    else
    {
        return "1";
    }
}


QString QvkDbus::CountDown( QString value )
{
    myUi.CountdownSpinBox->setValue( value.toInt() );
    return "0";
}


QString QvkDbus::Tab( QString value )
{

    if ( value.toInt() < myUi.tabWidget->count() )
    {
      myUi.tabWidget->setCurrentIndex( value.toInt() - 1 );
      return "0";
    }
    else
    {
      return "1";
    }
}


void QvkDbus::quit()
{
    emit close();
}
