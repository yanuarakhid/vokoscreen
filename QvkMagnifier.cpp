#include "QvkMagnifier.h" 

using namespace std;


// Lupe Rund, Qadratisch, Oval

//QvkMagnifier::QvkMagnifier( int width, int height, int framewidth )
QvkMagnifier::QvkMagnifier()
{
  QString magnifierVersion = "1.0.5";
  faktor = 2;
  label = new QLabel( this );

  Magnifier400x200();

  resize( 2 * distanzX * faktor, 2 * distanzY * faktor );
  setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
  border = 3;

  label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
  label->setAlignment( Qt::AlignCenter );
  label->setScaledContents( true );
  label->show();
 
  QTimer *timer = new QTimer( this );
  connect( timer, SIGNAL( timeout() ), this, SLOT( mytimer() ) );
  timer->start( 40 );  
}


void QvkMagnifier::closeEvent( QCloseEvent * event )
{
  (void)event;
  emit closeMagnifier();
}


void QvkMagnifier::getDialogMagnifier( QWidget *parent )
{
  QDialog *dialog = new QDialog( parent );
  dialog->resize( 300, 200 );

  QLabel* label = new QLabel( dialog );
  label->setText("");
  label->setGeometry( QRect( 20, 30, 120, 150) );
  label->setAlignment( Qt::AlignCenter );
  label->show();
  QImage* qImage = new QImage( ":/pictures/magnifier.png" );
  label->setPixmap(QPixmap::fromImage(*qImage, Qt::AutoColor));
  label->setScaledContents(true);
  
  radioButton1 = new QRadioButton( dialog );
  radioButton1->setGeometry( 170, 50, 200, 21 );
  radioButton1->setText( "200 x 200" );
  radioButton1->show();
  connect( radioButton1, SIGNAL( clicked() ), SLOT( Magnifier200x200() ) );
  if ( formValue == 1 )
    radioButton1->setChecked( true );
  
  radioButton2 = new QRadioButton( dialog );
  radioButton2->setGeometry( 170, 80, 200, 21 );
  radioButton2->setText( "400 x 200" );
  radioButton2->show();
  connect( radioButton2, SIGNAL( clicked() ), SLOT( Magnifier400x200() ) );
  if ( formValue == 2 )
    radioButton2->setChecked( true );
  
  radioButton3 = new QRadioButton( dialog );
  radioButton3->setGeometry( 170, 110, 200, 21 );
  radioButton3->setText( "600 x 200" );
  radioButton3->show();
  connect( radioButton3, SIGNAL( clicked() ), SLOT( Magnifier600x200() ) );
  if ( formValue == 3 )
    radioButton3->setChecked( true );

  dialog->exec();
}


void QvkMagnifier::Magnifier200x200()
{
  distanzX = 50;
  distanzY = 50;
  resize( 2 * distanzX * faktor, 2 * distanzY * faktor );
  label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
  formValue = 1;
}


void QvkMagnifier::Magnifier400x200()
{
  distanzX = 100;
  distanzY = 50;
  resize( 2 * distanzX * faktor, 2 * distanzY * faktor );
  label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
  formValue = 2;
}


void QvkMagnifier::Magnifier600x200()
{
  distanzX = 150;
  distanzY = 50;
  resize( 2 * distanzX * faktor, 2 * distanzY * faktor );
  label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
  formValue = 3;
}



int QvkMagnifier::getDistanzX()
{
  return distanzX;
}


int QvkMagnifier::getDistanzY()
{
  return distanzY; 
}


int QvkMagnifier::NewDistanzXLeft()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();
  
  int newDistanzX = ( ( desk->screenGeometry().width() / 2 ) - cursor.pos().x() ) * 
                 ( distanzX + ( width() / 2 ) ) / 
                 ( desk->screenGeometry().width() / 2 - distanzX ) - 
                 ( width() / 2 );
  return  newDistanzX;
}


int QvkMagnifier::NewDistanzXRight()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();
  
  int newDistanX = ( ( desk->screenGeometry().width() / 2 ) - cursor.pos().x() ) * 
                 ( -distanzX - ( width() / 2 ) ) / 
                 ( desk->screenGeometry().width() / 2 - distanzX ) - 
                 ( width() / 2 );
  return  newDistanX;
}


QvkMagnifier::~QvkMagnifier()
{
}

void QvkMagnifier::setMagnifier()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();


  // Lupe an oberen linke Ecke setzen
  if ( ( cursor.pos().x() < distanzX ) and ( cursor.pos().y() <  distanzY ) )
  {
    move( 2 * distanzX,  2 * distanzY );
    return;
  }

  // Lupe obere rechte Ecke setzen 
  if ( ( cursor.pos().x() > ( desk->screenGeometry().width() - distanzX ) ) and ( cursor.pos().y() < distanzY ) )
  {
    move( desk->screenGeometry().width() - 2 * distanzX - width(), 2 * distanzY);
    return;
  }
  
  // Lupe am oberen Rand setzen
  // Linke Hälfte am oberen Rand
  if ( ( cursor.pos().y() < distanzY ) and ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) )
  {
    move( cursor.pos().x() + NewDistanzXLeft(), 2 * distanzY );
    return;
  }
  // Rechte Hälfte am oberen Rand
  if ( ( cursor.pos().y() < distanzY ) and ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) )
  {
    move( cursor.pos().x() - NewDistanzXRight() - width(), 2 * distanzY );
    return;
  }

  // Lupe an untere rechte Ecke setzen
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanzX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanzY ) )
  {
      move( desk->screenGeometry().width() - ( 2 * distanzX + width() ), desk->screenGeometry().height() - ( 2 * distanzY + height() ) );
      return; 
  }
  
  // Lupe am rechten Rand setzen
  // Obere Hälfte am rechten Rand
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanzX ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) )// div 2
  {
    move( desk->screenGeometry().width() - ( 2 * distanzX + width() ), cursor.pos().y() + 1 * distanzY );
    return;
  }
  // untere Hälfte am rechten Rand
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanzX ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
  {
    move( desk->screenGeometry().width() - ( 2 * distanzX + width() ), cursor.pos().y() - distanzY - height() );
    return;
  }

  // Lupe an linken unteren Ecke setzen
  if ( ( cursor.pos().x() < distanzX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanzY ) )
  {
    move( 2 * distanzX, desk->screenGeometry().height() - 2 * distanzY - height() );
    return; 
  }
  
  // Lupe am unteren Rand setzen
  // Linke Hälfte unterer Rand
  if ( ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanzY ) )
  {
    move( cursor.pos().x() + NewDistanzXLeft(), desk->screenGeometry().height() - ( 2 * distanzY + height() ) );
    return; 
  }
  // Rechte Hälfte unterer Rand
  if ( ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanzY ) )
  {
    move( cursor.pos().x() - NewDistanzXRight() - width(), desk->screenGeometry().height() - 2 * distanzY - height() );
    return; 
  }
  
  // Lupe am linken Rand setzen
  // Obere Hälfte am linken Rand
  if ( ( cursor.pos().x() < distanzX ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) ) // div 2
    
  {
    move( 2 * distanzX, cursor.pos().y() + distanzY );
    return;
  }
  // Untere Hälfte am linken Rand
  if ( ( cursor.pos().x() < distanzX ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
  {
    move( 2 * distanzX, cursor.pos().y() - distanzY - height() );
    return; 
  }
  
  // Linke obere Hälfte 
  if ( ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) ) // div 2
    move( cursor.pos().x() + NewDistanzXLeft(), cursor.pos().y() + distanzY );
  
  // Rechte obere Hälfte
  if ( ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) )
    move( cursor.pos().x() - NewDistanzXRight() - width(), cursor.pos().y() + distanzY );
  
  // Linke untere Hälfte
  if ( ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
    move( cursor.pos().x() + NewDistanzXLeft(), cursor.pos().y() - distanzY - height() );
  
  // Rechte untere Hälfte
  if ( ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
    move( cursor.pos().x() - NewDistanzXRight() -width(), cursor.pos().y() - distanzY - width() );
}


void QvkMagnifier::mytimer()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();
//    Qt::Window | Qt::WindowStaysOnTopHint

  setMagnifier();

  // Obere linke Ecke
  if ( ( cursor.pos().x() < distanzX ) and ( cursor.pos().y() <  distanzY ) )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  0, 
					  0, 
					  2 * distanzX ,
					  2 * distanzY );
    label->setPixmap( originalPixmap );    
    return; 
  }
  
  // Obere rechte Ecke
  if ( ( cursor.pos().x() > ( desk->screenGeometry().width() - distanzX ) ) and ( cursor.pos().y() < distanzY ) )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  desk->screenGeometry().width() - 2 * distanzX, 
					  0, 
					  2 * distanzX ,
					  2 * distanzY );
    label->setPixmap( originalPixmap );    
    return; 
  }
  
  // Linke untere Ecke
  if ( ( cursor.pos().x() < distanzX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanzY ) )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  0, 
					  desk->screenGeometry().height() - 2 * distanzY, 
					  2 * distanzX ,
					  2 * distanzY );
    label->setPixmap( originalPixmap );    
    return; 
  }
  
  // Rechte untere Ecke  
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanzX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanzY ) )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  desk->screenGeometry().width() - 2 * distanzX, 
					  desk->screenGeometry().height() - 2 * distanzY, 
					  2 * distanzX ,
					  2 * distanzY );
    label->setPixmap( originalPixmap );    
    return; 
  }  
  
  // Unterer Rand
  if ( cursor.pos().y() > desk->screenGeometry().height() - distanzY )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  cursor.pos().x() - distanzX, 
					  desk->screenGeometry().height() - 2 * distanzY, 
					  2 * distanzX , 
					  2 * distanzY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Oberen Rand
  if ( cursor.pos().y() < distanzY )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  cursor.pos().x() - distanzX, 
					  0, 
					  2 * distanzX, 
					  2 * distanzY );
    label->setPixmap( originalPixmap );
    return;
  }
  
  // Rechter Rand
  if ( cursor.pos().x() > desk->screenGeometry().width() - distanzX )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  desk->screenGeometry().width() - 2 * distanzX, 
					  cursor.pos().y() - distanzY, 
					  2 * distanzX ,
					  2 * distanzY );
    label->setPixmap( originalPixmap );
    return;
  }
  
  // Linker Rand
  if ( cursor.pos().x() < distanzX )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					  0, 
					  cursor.pos().y() - distanzY, 
					  2 * distanzX ,
					  2 * distanzY );
    label->setPixmap( originalPixmap );    
    return;
  }
  
  // Fläche
  QPixmap originalPixmap = QPixmap();
  originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(), 
					cursor.pos().x() - distanzX, 
					cursor.pos().y() - distanzY, 
					2 * distanzX , 
					2 * distanzY );
  label->setPixmap( originalPixmap );
}
