#include "QvkShowkeyGetkey.h"

#include <QCoreApplication>

#include <iostream>
#include <map>
#include <set>
#include <cstdlib>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <X11/Xlocale.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#include <X11/extensions/Xrandr.h>
#include <X11/XKBlib.h> // VK


QvkShowkeyGetkey::QvkShowkeyGetkey()
{
}


QvkShowkeyGetkey::~QvkShowkeyGetkey()
{
}


namespace {
  bool cont = true;             // flag for while loop

  Display *display = 0;         // X server connection

  std::string key_map[256];       // key names
  std::string key_map_upper[256]; // key names, upper case

  std::set<int> caps_set, shift_set, ctrl_set, alt_set, meta_set;
}


/*
 * Gibt die Tastaturtabelle in einer QStringList zurück
 */
QStringList QvkShowkeyGetkey::PrintKeyTable()
{
    bool exprs = false;
    QString line;
    QStringList stringList;

    if (! ( display = XOpenDisplay( NULL ) ) )
    {
       qDebug() << "Can't open display NULL" << '\n';
       return stringList;
    }

    int i;
    int min_keycode, max_keycode, keysyms_per_keycode;
    KeySym *keymap, *origkeymap;

    XDisplayKeycodes (display, &min_keycode, &max_keycode);
    origkeymap = XGetKeyboardMapping( display, min_keycode, (max_keycode - min_keycode + 1), &keysyms_per_keycode );

    if (!origkeymap)
    {
	qDebug() << "unable to get keyboard mapping table.";
	return stringList;
    }

    keymap = origkeymap;
    for (i = min_keycode; i <= max_keycode; i++) {
	int  j, max;
        line = "";
	
        line.append( QString::number( i ) );
	
	max = keysyms_per_keycode - 1;
	while ((max >= 0) && (keymap[max] == NoSymbol))
	    max--;
	
	for (j = 0; j <= max; j++)
	{
	    register KeySym ks = keymap[j];
	    const char *s;
	    if (ks != NoSymbol)
		s = XKeysymToString (ks);
	    else
		s = "NoSymbol";
	    
	    if (!exprs)
	    {
		line.append( " " ); // Delemiter
		line.append( "0x" ); // Identifier 
		QString hexStr = QString::number( (unsigned int)ks, 16 ); // convert to hex
		line.append( hexStr );
		line.append( " " ); // Delemiter
		line.append( "" ).append( s ).append( "" );
	    }
	    else 
	      if (s)
		fprintf (stdout, " %s", s);
	      else
		fprintf (stdout, " 0x%04x", (unsigned int)ks);
	}

	keymap += keysyms_per_keycode;
	stringList <<  line;
	
    }
    XFree ((char *) origkeymap);
    
    return stringList;
}


bool isNumLockOn()
{
    Display *display = XOpenDisplay( NULL );
    bool mumlock_state = false;
    XKeyboardState x;
    XGetKeyboardControl( display, &x );
    mumlock_state = x.led_mask & 2;
    XCloseDisplay( display );
    return mumlock_state;
}


bool isCapsLockOn()
{
    Display *display = XOpenDisplay( NULL );
    bool caps_state = false;
    unsigned n;
    XkbGetIndicatorState( display, XkbUseCoreKbd, &n );
    caps_state = ( n & 0x01 ) == 1;
    XCloseDisplay( display );
    return caps_state;
}


/*
 * gibt den ermittelten Buchstaben zurück einschließliech Umlaute
 */
QString QvkShowkeyGetkey::getKey( QStringList list, int code )
{
  QString keyFromList;
  QStringList splitValuesList;
  QString key;
  
  QRegExp rx( "^" + QString::number( code ) + " " );
  QStringList keyList = list.filter( rx );
  QString keyValues = keyList[0];
  splitValuesList = keyValues.split( " " );
  keyFromList = splitValuesList[ 1 ];
  
  int integer = keyFromList.toInt( 0, 16 );
  key = (QString)integer;
  
  // Bei folgen Tasten Flag setzen und kein Wert zurückgeben
  keyFromList = splitValuesList[ 2 ];
  if ( keyFromList == "Caps_Lock")
  {
     key = "";
     caps_set.insert(code);
  }
  else if ( keyFromList == "Shift_L"   || keyFromList == "Shift_R")
  {
     key = "";
     shift_set.insert(code);
  }
  else if ( keyFromList == "Control_L" || keyFromList == "Control_R")
  {
     key = "";
     ctrl_set.insert(code);
  }
  else if ( keyFromList == "Alt_L"     || keyFromList == "Alt_R")
  {
     key = "";
     alt_set.insert(code);
  }
  else if ( keyFromList == "Meta_L"    || keyFromList == "Meta_R")
  {
     key = "";
     meta_set.insert(code);
  }
  else if ( keyFromList == "Super_L"    || keyFromList == "Super_R")
  {
     key = "";
     meta_set.insert(code);
  }

  // Mit "xset q" kann der status abgefragt werden
  // http://www.qtcentre.org/threads/30180-how-to-determine-if-CapsLock-is-on-crossplatform
  // http://stackoverflow.com/questions/24822505/how-to-tell-if-shift-is-pressed-on-numpad-input-with-numlock-on-or-at-least-get
  
  if ( keyFromList == "F1" ) key = "F1";
  if ( keyFromList == "F2" ) key = "F2";
  if ( keyFromList == "F3" ) key = "F3";
  if ( keyFromList == "F4" ) key = "F4";
  if ( keyFromList == "F5" ) key = "F5";
  if ( keyFromList == "F6" ) key = "F6";
  if ( keyFromList == "F7" ) key = "F7";
  if ( keyFromList == "F8" ) key = "F8";
  if ( keyFromList == "F9" ) key = "F9";
  if ( keyFromList == "F10" ) key = "F10";
  if ( keyFromList == "F11" ) key = "F11";
  if ( keyFromList == "F12" ) key = "F12";
  if ( keyFromList == "Escape" ) key = "Esc";
  if ( keyFromList == "BackSpace" ) key = "Bsp";
  if ( keyFromList == "Tab" ) key = "Tab";
  if ( keyFromList == "Num_Lock" ) key = "";
  if ( keyFromList == "Return" ) key = "Return";
  
  if ( isNumLockOn() == true )
  {
    keyFromList = splitValuesList[ 4 ];
    if ( keyFromList == "KP_0" ) key = "0";
    if ( keyFromList == "KP_1" ) key = "1";
    if ( keyFromList == "KP_2" ) key = "2";
    if ( keyFromList == "KP_3" ) key = "3";
    if ( keyFromList == "KP_4" ) key = "4";
    if ( keyFromList == "KP_5" ) key = "5";
    if ( keyFromList == "KP_6" ) key = "6";
    if ( keyFromList == "KP_7" ) key = "7";
    if ( keyFromList == "KP_8" ) key = "8";
    if ( keyFromList == "KP_9" ) key = "9";
    if ( keyFromList == "KP_Multiply" ) key = "*";
    if ( keyFromList == "KP_Subtract" ) key = "-";
    if ( keyFromList == "KP_Add" ) key = "+";
    if ( keyFromList == "KP_Decimal" ) key = ".";
    if ( keyFromList == "KP_Divide" ) key = "/";
    if ( keyFromList == "KP_Separator" ) key = ",";
  }

  if ( isNumLockOn() == false )
  {
    keyFromList = splitValuesList[ 2 ];
    if ( keyFromList == "KP_Home" )      key = "Home";
    if ( keyFromList == "KP_Up" )        key = "Up";
    if ( keyFromList == "KP_Prior" )     key = "Prior";
    if ( keyFromList == "KP_Left" )      key = "Left";
    if ( keyFromList == "KP_Begin" )     key = "Begin";
    if ( keyFromList == "KP_Right" )     key = "Right";
    if ( keyFromList == "KP_End" )       key = "End";
    if ( keyFromList == "KP_Down" )      key = "Down";
    if ( keyFromList == "KP_Next" )      key = "Next";
    if ( keyFromList == "KP_Insert" )    key = "insert";
    if ( keyFromList == "KP_Delete" )    key = "Delete";
    if ( keyFromList == "KP_Enter" )     key = "Enter";
    if ( keyFromList == "KP_Add" )       key = "+";
    if ( keyFromList == "KP_Subtract" )  key = "-";
    if ( keyFromList == "KP_Multiply" )  key = "*";
    if ( keyFromList == "KP_Divide" )    key = "/";
  }
  
  qDebug() << "Numlook" << isNumLockOn() << "|" << "keycode" << code << "|" << "Keysym" << keyFromList << "|" << "Print key:" << key;
  
  return key;
}


QString QvkShowkeyGetkey::getKeyShift( QStringList list, int code )
{
  QRegExp rx( "^" + QString::number( code ) + " " );
  QStringList keyList = list.filter( rx );
  QString keyValues = keyList[0];
  QStringList splitValuesList = keyValues.split( " " );
  QString keyFromList = splitValuesList[ 3 ];
  
  int integer = keyFromList.toInt( 0, 16 );
  QString key = (QString)integer;
  return key;
}

/*
QString QvkShowkeyGetkey::getKeyCaps( QStringList list, int code )
{
    QRegExp rx( "^" + QString::number( code ) + " " );
    QStringList keyList = list.filter( rx );
    QString keyValues = keyList[0];
    QStringList splitValuesList = keyValues.split( " " );
    QString keyFromList = splitValuesList[ 5 ];
  
    int integer = keyFromList.toInt( 0, 16 );
    QString key = (QString)integer;
    return key;
}
*/

void QvkShowkeyGetkey::run()
{
  const timespec sleeptime = { 0, 1000000 };

  /*
   * open the display
   */
  if (! ( display = XOpenDisplay( NULL ) ) )
  {
    std::cerr << "Can't open display NULL" << '\n';
    std::exit(1);
  }

  /*
   * fill up keyboard mapping
   */
  
  //fill_mappings();
  
  // keys aus eigene Routine ermitteln
  QStringList keyList = PrintKeyTable();
  

  /*
   * fill relevant key buffers
   */
  char keys[32];                  // buffer for reading keys in
  char lastkeys[32];              // previous keys state  

  std::fill(lastkeys, lastkeys + sizeof(lastkeys), 0);
		
  /*
   * query keyboard in loop
   */
  bool last_is_nav  = false;    // navigation key indicator
//  bool last_is_char = false;    // spaces adjustment
  while (cont) {
    nanosleep(&sleeptime, 0);   // avoid busy waiting
    
    XQueryKeymap( display, keys );
    
    // read modifiers (caps lock is ignored)
    bool shift = false;
    bool ctrl  = false;
    bool alt   = false;
    bool meta  = false;

    for (unsigned i = 0; i < sizeof(keys); ++i)
      for (unsigned j = 0, test = 1; j < 8; ++j, test *= 2)
        if (keys[i] & test)
	{
          const int code = i*8+j;

          if (shift_set.find(code) != shift_set.end())
	  {
            shift = true;
	    qDebug() << "Shift = true";
	  }
	  
          if (ctrl_set.find(code) != ctrl_set.end())
            ctrl = true;

          if (alt_set.find(code) != alt_set.end())
            alt = true;

          if (meta_set.find(code) != meta_set.end())
            meta = true;
	  
        }
    
    // print changed keys
    for (unsigned i = 0; i < sizeof(keys); ++i)
      if (keys[i] != lastkeys[i]) {
        // check which key got changed
        for (unsigned j = 0, test = 1; j < 8; ++j, test *= 2)
          // if the key was pressed, and it wasn't before, print this
          if ((keys[i] & test) && ((keys[i] & test) != (lastkeys[i] & test)))
	  {
            const int code = i*8+j;
            QString key = getKey( keyList, code );

            const bool key_is_nav = ( key == "Nav" );

            // only print navigation keys once
            if ( ! (last_is_nav && key_is_nav) && key.size() > 0 )
	    {
              // change key according to modifiers
              if ( ! key_is_nav)
	      {
		if ( meta )
                  key = " Meta-" + key + " ";

                if ( alt )
                  key = " Alt-" + key + " ";

                if ( ctrl )
                  key = " Ctrl-" + key + " ";

	        if ( ( not shift ) )
	        {
	          emit pressedKey( key );
	        }
		
		// shift
                if ( shift )
		{
                  emit pressedKey( getKeyShift( keyList, code ) );
		}
		
              }
	      
              last_is_nav = key_is_nav;
            }
          }
      
        lastkeys[i] = keys[i];
      }
  }

  XCloseDisplay(display);
}
