#include "QvkShowClickDialog.h"

#include <QDebug>

QvkShowClickDialog::QvkShowClickDialog( QColor color, bool radiant, double opacity )
{
    newDialog = new QDialog;
    newDialog->setModal( true );

    myUiDialog.setupUi( newDialog );
   
    QPalette *palette = new QPalette();
    
    palette->setColor( QPalette::Button, Qt::white );
    myUiDialog.pushButton_white->setPalette( *palette );
    palette->setColor( QPalette::Button, Qt::black );
    myUiDialog.pushButton_black->setPalette( *palette );
    
    palette->setColor( QPalette::Button, Qt::red );
    myUiDialog.pushButton_red->setPalette( *palette );
    palette->setColor( QPalette::Button, Qt::darkRed );
    myUiDialog.pushButton_darkRed->setPalette( *palette );
    
    palette->setColor( QPalette::Button, Qt::green );
    myUiDialog.pushButton_green->setPalette( *palette );
    palette->setColor( QPalette::Button, Qt::darkGreen );
    myUiDialog.pushButton_darkGreen->setPalette( *palette );
    
    palette->setColor( QPalette::Button, Qt::blue );
    myUiDialog.pushButton_blue->setPalette( *palette );
    palette->setColor( QPalette::Button, Qt::darkBlue );
    myUiDialog.pushButton_darkBlue->setPalette( *palette );

    palette->setColor( QPalette::Button, Qt::cyan );
    myUiDialog.pushButton_cyan->setPalette( *palette );
    palette->setColor( QPalette::Button, Qt::darkCyan );
    myUiDialog.pushButton_darkCyan->setPalette( *palette );

    palette->setColor( QPalette::Button, Qt::magenta );
    myUiDialog.pushButton_magenta->setPalette( *palette );
    palette->setColor( QPalette::Button, Qt::darkMagenta );
    myUiDialog.pushButton_darkMagenta->setPalette( *palette );

    palette->setColor( QPalette::Button, Qt::yellow );
    myUiDialog.pushButton_yellow->setPalette( *palette );
    palette->setColor( QPalette::Button, Qt::darkYellow );
    myUiDialog.pushButton_darkYellow->setPalette( *palette );

    palette->setColor( QPalette::Button, Qt::gray );
    myUiDialog.pushButton_gray->setPalette( *palette );
    palette->setColor( QPalette::Button, Qt::darkGray );
    myUiDialog.pushButton_darkGray->setPalette( *palette );
    
    connect( myUiDialog.pushButton_white, SIGNAL( clicked() ), this, SLOT( white() ) );
    connect( myUiDialog.pushButton_black, SIGNAL( clicked() ), this, SLOT( black() ) );
    connect( myUiDialog.pushButton_red, SIGNAL( clicked() ), this, SLOT( red() ) );
    connect( myUiDialog.pushButton_darkRed, SIGNAL( clicked() ), this, SLOT( darkRed() ) );
    connect( myUiDialog.pushButton_green, SIGNAL( clicked() ), this, SLOT( green() ) );
    connect( myUiDialog.pushButton_darkGreen, SIGNAL( clicked() ), this, SLOT( darkGreen() ) );
    connect( myUiDialog.pushButton_blue, SIGNAL( clicked() ), this, SLOT( blue() ) );
    connect( myUiDialog.pushButton_darkBlue, SIGNAL( clicked() ), this, SLOT( darkBlue() ) );
    connect( myUiDialog.pushButton_cyan, SIGNAL( clicked() ), this, SLOT( cyan() ) );
    connect( myUiDialog.pushButton_darkCyan, SIGNAL( clicked() ), this, SLOT( darkCyan() ) );
    connect( myUiDialog.pushButton_magenta, SIGNAL( clicked() ), this, SLOT( magenta() ) );
    connect( myUiDialog.pushButton_darkMagenta, SIGNAL( clicked() ), this, SLOT( darkMagenta() ) );
    connect( myUiDialog.pushButton_yellow, SIGNAL( clicked() ), this, SLOT( yellow() ) );
    connect( myUiDialog.pushButton_darkYellow, SIGNAL( clicked() ), this, SLOT( darkYellow() ) );
    connect( myUiDialog.pushButton_gray, SIGNAL( clicked() ), this, SLOT( gray() ) );
    connect( myUiDialog.pushButton_darkGray, SIGNAL( clicked() ), this, SLOT( darkGray() ) );

    connect( myUiDialog.horizontalSliderCircle, SIGNAL( valueChanged( int ) ), this, SLOT( valueChangedSliderCircle( int ) ) );
    connect( myUiDialog.horizontalSliderOpacity, SIGNAL( valueChanged( int ) ), this, SLOT( valueChangedOpacity( int ) ) );
    
    connect( myUiDialog.checkBoxRadiant, SIGNAL( stateChanged( int ) ), this, SLOT( stateChangedRadiant( int ) ) );

    connect( myUiDialog.buttonBox, SIGNAL( accepted() ), this, SLOT( ok() ) );
    connect( myUiDialog.buttonBox, SIGNAL( rejected() ), this, SLOT( cancel() ) );
    connect( myUiDialog.pushButtonStandard, SIGNAL( clicked() ), this, SLOT( Standard() ) );
    myUiDialog.pushButtonStandard->setIcon ( QIcon::fromTheme( "edit-undo", QIcon( ":/pictures/undo.png" ) ) );
    
    circleWidget = new QvkCircleWidget( myUiDialog.graphicsView );
    circleWidget->show();
    valueChangedSliderCircle( myUiDialog.horizontalSliderCircle->value() );
    circleWidget->setColor( color );
    circleWidget->setRadiant( radiant );
    circleWidget->setOpacity( opacity );
}

QvkShowClickDialog::~QvkShowClickDialog()
{
}

void QvkShowClickDialog::show()
{
   newDialog->show();
}

void QvkShowClickDialog::cancel()
{
   newDialog->close();
}

void QvkShowClickDialog::Standard()
{
  myUiDialog.horizontalSliderShowtime->setValue( 5 );
  myUiDialog.pushButton_red->click();
  myUiDialog.horizontalSliderCircle->setValue( 70 );
  myUiDialog.horizontalSliderOpacity->setValue( 50 );
  myUiDialog.checkBoxRadiant->setCheckState( Qt::Unchecked );
}

void QvkShowClickDialog::ok()
{
   emit newCircleWidgetValue( circleWidget->getDiameter(), circleWidget->getColor() );
   emit newShowtime( (double) myUiDialog.horizontalSliderShowtime->value()/10 );
   emit newOpacity( circleWidget->getOpacity() );
   emit newRadiant( circleWidget->getRadiant() );
   newDialog->close();
}

void QvkShowClickDialog::valueChangedSliderCircle( int value )
{
  circleWidget->setDiameter( value );
}

void QvkShowClickDialog::valueChangedOpacity( int value )
{
  double value_1 = value;
  circleWidget->setOpacity( value_1 / 100 );
}

void QvkShowClickDialog::stateChangedRadiant( int value )
{
  if ( value == Qt::Unchecked )
    circleWidget->setRadiant( false );
  else
    circleWidget->setRadiant( true );
}

void QvkShowClickDialog::white()
{
  circleWidget->setColor( Qt::white );
}

void QvkShowClickDialog::black()
{
  circleWidget->setColor( Qt::black );
}

void QvkShowClickDialog::red()
{
  circleWidget->setColor( Qt::red );
}

void QvkShowClickDialog::darkRed()
{
  circleWidget->setColor( Qt::darkRed );
}

void QvkShowClickDialog::green()
{
  circleWidget->setColor( Qt::green );
}

void QvkShowClickDialog::darkGreen()
{
  circleWidget->setColor( Qt::darkGreen );
}

void QvkShowClickDialog::blue()
{
  circleWidget->setColor( Qt::blue );
}

void QvkShowClickDialog::darkBlue()
{
  circleWidget->setColor( Qt::darkBlue );
}

void QvkShowClickDialog::cyan()
{
  circleWidget->setColor( Qt::cyan );
}

void QvkShowClickDialog::darkCyan()
{
  circleWidget->setColor( Qt::darkCyan );
}

void QvkShowClickDialog::magenta()
{
  circleWidget->setColor( Qt::magenta );
}

void QvkShowClickDialog::darkMagenta()
{
  circleWidget->setColor( Qt::darkMagenta );
}

void QvkShowClickDialog::yellow()
{
  circleWidget->setColor( Qt::yellow );
}

void QvkShowClickDialog::darkYellow()
{
  circleWidget->setColor( Qt::darkYellow );
}

void QvkShowClickDialog::gray()
{
  circleWidget->setColor( Qt::gray );
}

void QvkShowClickDialog::darkGray()
{
  circleWidget->setColor( Qt::darkGray );
}
