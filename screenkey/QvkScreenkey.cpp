
#include "QvkScreenkey.h" 

using namespace std;

QvkScreenkey::QvkScreenkey()
{
  onOff = true;
}


QvkScreenkey::~QvkScreenkey(){}


void QvkScreenkey::setScreenkeyOff()
{
  onOff = false; 
}


void QvkScreenkey::setScreenkeyOn()
{
  onOff = true; 
}


void QvkScreenkey::readKey()
{
    Display * display;

    char szKey[32];
    char szKeyOld[32] = {0};

    char szBit;
    char szBitOld;
    int  iCheck;

    char szKeysym;
    char *szKeyString;

    int iKeyCode;

    Window focusWin = 0;
    int iReverToReturn = 0;

    display = XOpenDisplay( NULL );

    while( onOff )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents );     
        XQueryKeymap( display, szKey );
	QTest::qSleep(10);
        if( memcmp( szKey, szKeyOld, 32 ) != 0 )
        {
            for( int i = 0; i < (int)sizeof( szKey ); i++ )
            {
                szBit = szKey[i];
                szBitOld = szKeyOld[i];
                iCheck = 1;

                for ( int j = 0 ; j < 8 ; j++ )
                {
                     if ( ( szBit & iCheck ) && !( szBitOld & iCheck ) )
                     {
                         iKeyCode = i * 8 + j ;

                         szKeysym = XkbKeycodeToKeysym( display, iKeyCode, 0, 0 );
                         szKeyString = XKeysymToString( szKeysym );
	 
                         XGetInputFocus( display, &focusWin, &iReverToReturn );
			 
			 QString keyString = szKeyString;
			 if ( keyString == "space" )
			   keyString = " ";
			 if ( keyString == "period" )
			   keyString = ".";
			 if ( keyString == "asciicircum" )
			   keyString = "^";
			 if ( keyString == "minus" )
			   keyString = "-";
			 if ( keyString == "less" )
			   keyString = "<";
			 if ( keyString == "comma" )
			   keyString = ",";
			 if ( keyString == "plus" )
			   keyString = "+";
			 if ( keyString == "numbersign" )
			   keyString = "#";
			 emit pressedKey( keyString );
			 
                     }
                    iCheck = iCheck << 1 ;
                }
            }
        }
        memcpy( szKeyOld, szKey, 32 );
    }
    XCloseDisplay( display );
}

