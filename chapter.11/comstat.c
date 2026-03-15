/* *********************************************************
   COMSTAT.C 
   Copyright (c) 1992 - Billy P. Taylor
   *********************************************************
   Shows how to use interrupt 14h to configure serial ports, 
   read modem and line status, and transmit and receive data.

   NOTE:  this program requires a compiler that supports 
          _interrupt function types, such as Microsoft C version
          5.1 or later, and Borland C++ 2.0 or later.

   To run, type COMSTAT<Enter>.
   ******************************************************* */

/* ******** DEFINES and MACROS ********* */
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define BIT0   1  /* 00000001 */
#define BIT1   2  /* 00000010 */
#define BIT2   4  /* 00000100 */
#define BIT3   8  /* 00001000 */
#define BIT4  16  /* 00010000 */
#define BIT5  32  /* 00100000 */
#define BIT6  64  /* 01000000 */
#define BIT7 128  /* 10000000 */

/* Line status bits */
#define Timeout            BIT7
#define TxShiftRegEmpty    BIT6
#define TxHoldingRegEmpty  BIT5
#define BreakDetected      BIT4
#define FramingError       BIT3
#define ParityError        BIT2
#define ReceiverOverrun    BIT1
#define DataReceived       BIT0

/* Modem status bits */
#define CarrierDetect      BIT7
#define RingIndicate       BIT6
#define DataSetReady       BIT5
#define ClearToSend        BIT4
#define DeltaCD            BIT3
#define TrailingEdgeRI     BIT2
#define DeltaDSR           BIT1
#define DeltaCTS           BIT0

/* Interrupt 14h function 00h bits */
#define BAUD110               0                  /* 00000000 */
#define BAUD150            BIT5                  /* 00100000 */
#define BAUD300            BIT6                  /* 01000000 */
#define BAUD600          ( BIT6 + BIT5 )         /* 01100000 */
#define BAUD1200           BIT7                  /* 10000000 */
#define BAUD2400         ( BIT7 + BIT5 )         /* 10100000 */
#define BAUD4800         ( BIT7 + BIT6 )         /* 11000000 */
#define BAUD9600         ( BIT7 + BIT6 + BIT5 )  /* 11100000 */
#define CLEAR_BAUD_RATE  ( BIT4 + BIT3 + BIT2 + BIT1 + BIT0 )

#define NOPARITY              0                  /* 00000000 */
#define ODDPARITY          BIT3                  /* 00001000 */
#define EVENPARITY       ( BIT4 + BIT3 )         /* 00011000 */
#define CLEAR_PARITY     ( BIT7 + BIT6 + BIT5 + BIT2 + BIT1 + BIT0 )

#define STOP1                 0                  /* 00000000 */
#define STOP2              BIT2                  /* 00000100 */
#define CLEAR_STOP_BITS  ( BIT7 + BIT6 + BIT5 + BIT4 + BIT3 + BIT1 + BIT0 )

#define DATA7              BIT1                  /* 00000010 */
#define DATA8            ( BIT1 + BIT0 )         /* 00000011 */
#define CLEAR_DATA_BITS  ( BIT7 + BIT6 + BIT5 + BIT4 + BIT3 + BIT2 )

#define BACKSPACE  8
#define ENTER     13
#define ESCAPE    27
#define NEWLINE   10

#define boolean unsigned char
#define YES 1
#define NO  0

#define BDA_com_addr (unsigned int  far *)0x00400000
#define BDA_com_to   (unsigned char far *)0x0040007c

#define COM_ADDRESS(which) *(BDA_com_addr+which)
#define COM_TIMEOUT(which) *(BDA_com_to+which)

/* Macro generates ASCII 1 if bit is set, else ASCII 0 */
#define bitOnOff( value, bit ) (value & bit) ? '1' : '0'

/* ********  Global Data  ******** */

union REGS regs;

boolean quitNow = NO,
        converseModeOn = NO;

unsigned char  scratch[100],
               kbdBuffer[81] = "\000",
              *kbdBufferP = kbdBuffer;

unsigned int currentPort = 0,
             lineCol     = 1,
             cursorCol   = 1;

unsigned char cfgWords[4] = 
   { BAUD1200+NOPARITY+STOP1+DATA8,     /* COM1 */
     BAUD1200+NOPARITY+STOP1+DATA8,     /* COM2 */
     BAUD1200+NOPARITY+STOP1+DATA8,     /* COM3 */
     BAUD1200+NOPARITY+STOP1+DATA8 };   /* COM4 */

/* ********  Data for Command Dispatching  ******** */

struct dispatchEntry
{
   char *command;
   void far *function;
   };

void changeComPorts     ( void );
void sayBadCommand      ( void );
void sendString         ( void );
void setExitFlag        ( void );
void setParity          ( void );
void setSpeed           ( void );
void setStopBits        ( void );
void setTimeout         ( void );
void setWordSize        ( void );
void showHelp           ( void );
void turnConverseModeOn ( void );

struct dispatchEntry dispatchTable[] = 
{
   { "QUIT",      setExitFlag },
   { "HELP",      showHelp },
   { "COM",       changeComPorts },
   { "PARITY",    setParity },
   { "STOPBITS",  setStopBits },
   { "DATABITS",  setWordSize },
   { "SPEED",     setSpeed },
   { "TIMEOUT",   setTimeout },
   { "SEND",      sendString },
   { "CONVERSE",  turnConverseModeOn },
   { 000,         sayBadCommand }
   };

/* *************************  Messages  ************************* */

char *comPortNames[] = { "COM1","COM2","COM3","COM4" };

char blankLine[] = 
"                                                                                ",
     dashes[]    = 
"--------------------------------------------------------------------------------";

char *mainScreen[] =
{
"---------------------- COMSTAT : Serial Port Test Utility ----------------------",
blankLine,
"                       --- LINE Status -------- Bit ------- MODEM Status -------",
"Port      :                       Timeout - . -- 7 -- . - RX carrier detect (CD)",
"Address   :            TX shift reg empty - . -- 6 -- . - Ring indicate (RI)",
"Timeout   :          TX holding reg empty - . -- 5 -- . - Data set ready (DSR)",
"Speed     :             RX Break detected - . -- 4 -- . - Clear to send (CTS)",
"Parity    :              RX framing error - . -- 3 -- . - Delta CD",
"Data Bits :               RX parity error - . -- 2 -- . - Trailing edge RI",
"Stop Bits :              RX overrun error - . -- 1 -- . - Delta DSR",
"                          RX data present - . -- 0 -- . - Delta CTS",
dashes,
blankLine,
blankLine,
blankLine,
blankLine,
blankLine,
blankLine,
blankLine,
dashes,
blankLine,
blankLine,
blankLine,
"Enter a command, or type HELP.",
":",
0 };

/* 
   Define screen rows and columns for displaying status.
   Note that an 80x25 text screen is assumed.  We get
   one by setting video mode 3 in main().
*/
#define TEXT_MODE          3     /* Video mode to set */
#define PORTNAMEROW       12     /* Port name */
#define PSTATROW           3     /* Row where status display begins */
#define LINESTATCOL       44     /* Line status column */
#define MODEMSTATCOL      54     /* Modem status column */
#define LINE_WINTOPROW    12     /* Received data window top row */
#define LINE_WINLEFTCOL    0     /* Received data window left column */
#define LINE_WINBOTTOMROW 18     /* Received data window bottom row */
#define LINE_WINRIGHTCOL  79     /* Received data window right column */

#define CMD_WINTOPROW     20     /* User command window top row */
#define CMD_WINLEFTCOL     0     /* User command window left column */
#define CMD_WINBOTTOMROW  23     /* User command window bottom row */
#define CMD_WINRIGHTCOL   79     /* User command window right column */

#define STATROW            3     /* Port status row */
#define STATCOL           12     /* Port status column */

#define CURSORROW         24     /* User command entry row */

char *helpLines[] =
{
"Quit, Com <1,2,3,4>, Parity <even,odd,none>, StopBits <1,2>",
"DataBits <5,6,7,8>, Speed <110,150,300,600,1200,2400,4800,9600>",
"Timeout <1..255>, Send <string>, Converse",
0 };

/* *******************  Function Prototypes  ******************** */

void          checkForRcvdData       ( void );
void          clearScreen            ( void );
void          dispatchCommand        ( void );
unsigned int  getComPortStatus       ( void );
unsigned char getVideoMode           ( void );
void          say                    ( unsigned int, 
                                       unsigned int, 
                                       char * );
void          scrollCmdWindow        ( void );
void          scrollLineWindow       ( void );
void          serviceKeyboard        ( void );
void          setComParameters       ( void );
void          setCursorPosition      ( unsigned int,
                                       unsigned int );
void          setVideoMode           ( unsigned char );
void          showPortStatus         ( unsigned int, 
                                       unsigned int, 
                                       unsigned int );
void          showSettings           ( void );
unsigned int  transmitChar           ( unsigned char );
void          writeChar              ( unsigned int, 
                                       unsigned int, 
                                       unsigned char );
void          writeCharToLineWindow  ( char );
void          writeWindowString      ( char * );


/* ********  Main  ******* */
void main( void )
{
unsigned int lineNumber;

/* Paint screen */
setVideoMode( TEXT_MODE );
for( lineNumber=0; mainScreen[lineNumber] != 0; lineNumber++ )
   say( lineNumber, 0, mainScreen[lineNumber] );

/* Show current port configuration */
setComParameters();
showSettings();

/* Main loop */
while ( !quitNow )
{
   serviceKeyboard();
   checkForRcvdData();
   showPortStatus( PSTATROW, LINESTATCOL, MODEMSTATCOL );
   }

/* Back to DOS */
clearScreen();
printf( "\n" );
exit( 0 );
}


/* ******************  Dispatched Functions  ******************** */

/* QUIT command : Set flag so main loop will exit */
void setExitFlag( void )
{
quitNow = YES;
   }

/* HELP command : Display command summary */
void showHelp( void )
{
unsigned int lineNumber;

for( lineNumber=0; helpLines[lineNumber]!=0; lineNumber++ )
   writeWindowString( helpLines[lineNumber] );
}

/* COM command : Switch comm ports */
void changeComPorts( void )
{
boolean portOk;

portOk = NO;

if ( strchr(kbdBuffer,'1') )
{
   currentPort = 0;
   portOk = YES;
   }

if ( strchr(kbdBuffer,'2') )
{
   currentPort = 1;
   portOk = YES;
   }

if ( strchr(kbdBuffer,'3') )
{
   currentPort = 2;
   portOk = YES;
   }

if ( strchr(kbdBuffer,'4') )
{
   currentPort = 3;
   portOk = YES;
   }

if ( portOk )
{
   setComParameters();
   showSettings();
   }
else
   sayBadCommand();
}

/* PARITY command : Change parity type for current port */
void setParity( void )
{
boolean even,odd,none;

strupr( kbdBuffer );

even = ( strstr(kbdBuffer,"EVEN") != 0 );
odd  = ( strstr(kbdBuffer,"ODD") != 0 );
none = ( strstr(kbdBuffer,"NONE") != 0 );

if ( (even+odd+none) != 1 )
   writeWindowString( "Specify only EVEN, ODD, or NONE !" );
else
{
   cfgWords[currentPort] &= CLEAR_PARITY;
   if ( even )
      cfgWords[currentPort] |= EVENPARITY;
   if ( odd )
      cfgWords[currentPort] |= ODDPARITY;
   if ( none )
      cfgWords[currentPort] |= NOPARITY;
   setComParameters();
   showSettings();
   }
}

/* Return address of command line token */
char *findToken( char *stringPtr )
{
while ( (*stringPtr != 0) && (*stringPtr != ' ') )
   stringPtr++;

if ( *stringPtr != 0 )
{
   while ( *stringPtr == ' ' )
      stringPtr++;
   }

return( stringPtr );
}

/* STOPBITS command : Set number of stop bits for current port */
void setStopBits( void )
{
unsigned int stopBits;

stopBits = atoi( findToken(kbdBuffer) );

if ( (stopBits != 1) && (stopBits != 2) )
   writeWindowString( "Specify only 1 or 2 STOP BITS!" );
else
{
   cfgWords[currentPort] &= CLEAR_STOP_BITS;
   if ( stopBits == 1 )
      cfgWords[currentPort] |= STOP1;
   else
      cfgWords[currentPort] |= STOP2;
   setComParameters();
   showSettings();
   }
}

/* DATABITS command : Set number of data bits for current port */
void setWordSize( void )
{
unsigned int wordSize;

wordSize = atoi( findToken(kbdBuffer) );

if ( (wordSize != 7) && (wordSize != 8) )
   writeWindowString( "Specify only WORD SIZE 7 or 8 !" );
else
{
   cfgWords[currentPort] &= CLEAR_DATA_BITS;
   if ( wordSize == 7 )
      cfgWords[currentPort] |= DATA7;
   else
      cfgWords[currentPort] |= DATA8;
   setComParameters();
   showSettings();
   }
}

/* SPEED command : Set line speed for current port */
void setSpeed( void )
{
unsigned int index, speed;

static unsigned int bps[] = 
   { 110,150,300,600,1200,2400,4800,9600,0 };

static unsigned int bits[] = 
   { BAUD110,BAUD150,BAUD300,BAUD600,
     BAUD1200,BAUD2400,BAUD4800,BAUD9600 };

speed = atoi( findToken(kbdBuffer) );

index = 0;
while( (bps[index] != speed) && (bps[index]!=0) )
   index++;

if ( bps[index] == 0 )
   writeWindowString( "SPEED 110,150,300,600,1200,2400,4800,9600 !" );
else
{
   cfgWords[currentPort] &= CLEAR_BAUD_RATE;
   cfgWords[currentPort] |= bits[index];
   setComParameters();
   showSettings();
   }
}

/* TIMEOUT command : set character timeout for current port */
void setTimeout( void )
{
unsigned char timeout;

timeout = atoi( findToken(kbdBuffer) );
COM_TIMEOUT( currentPort ) = timeout;
showSettings();
}

/* SEND command : Transmit string through current port */
void sendString( void )
{
char *stringPtr;

stringPtr = findToken( kbdBuffer );

while ( *stringPtr != NULL )
{
   transmitChar( *stringPtr );
   ++stringPtr;
   }

transmitChar( ENTER );
}

/* CONVERSE command : Enter converse mode */
void turnConverseModeOn( void )
{
converseModeOn = YES;
writeWindowString( "Press ESC to cancel CONVERSE mode" );
}

/* DEFAULT : inform user that command line was bad */
void sayBadCommand( void )
{
writeWindowString( "Type HELP for a list of commands." );
}

/* ********  Dispatcher  ******** */
/* Call routine for command that user specified */
void dispatchCommand( void )
{
void (_interrupt _far *dispatchFunction)( void );

struct dispatchEntry *dispatchTablePtr;

dispatchTablePtr = dispatchTable;

while ( (dispatchTablePtr->command != 0) &&
        (memicmp( kbdBuffer,
                  dispatchTablePtr->command,
                  strlen(dispatchTablePtr->command)) != 0) )
{
   ++dispatchTablePtr;
   }

(unsigned long)dispatchFunction = 
   (unsigned long)dispatchTablePtr->function;

(*dispatchFunction)();
}

/* ********  Support Functions  ******** */
/* Scroll received data display window */
void scrollLineWindow( void )
{
#define WHITE_ON_BLACK 7
#define PAGE_0         0
#define SCROLL_ONCE    1

regs.h.ah = 6;                       /* AH = 6 - scroll screen */
regs.h.al = SCROLL_ONCE;             /* AL = times to scroll */
regs.h.bh = WHITE_ON_BLACK;          /* BH = attribute */
regs.h.bl = PAGE_0;                  /* BL = video page */
regs.h.ch = LINE_WINTOPROW;          /* CH = top row */
regs.h.cl = LINE_WINLEFTCOL;         /* CL = left column */
regs.h.dh = LINE_WINBOTTOMROW;       /* DH = bottom row */
regs.h.dl = LINE_WINRIGHTCOL;        /* DL = right column */
int86( 0x10, &regs, &regs );         /* Call BIOS */
}

/* Scroll command history window */
void scrollCmdWindow( void )
{
#define WHITE_ON_BLACK 7
#define PAGE_0         0
#define SCROLL_ONCE    1

regs.h.ah = 6;                       /* AH = 6 - scroll screen */
regs.h.al = SCROLL_ONCE;             /* AL = times to scroll */
regs.h.bh = WHITE_ON_BLACK;          /* BH = attribute */
regs.h.bl = PAGE_0;                  /* BL = video page */
regs.h.ch = CMD_WINTOPROW;           /* CH = top row */
regs.h.cl = CMD_WINLEFTCOL;          /* CL = left column */
regs.h.dh = CMD_WINBOTTOMROW;        /* DH = bottom row */
regs.h.dl = CMD_WINRIGHTCOL;         /* DL = right column */
int86( 0x10, &regs, &regs );         /* Call BIOS */
}

/* Write string to command history window */
void writeWindowString( char *stringPtr )
{
scrollCmdWindow();
say( CMD_WINBOTTOMROW, CMD_WINLEFTCOL, stringPtr );
}

/* Write character to received data display window */
void writeCharToLineWindow( char character )
{
switch ( character )
{
   case ENTER   : lineCol = LINE_WINLEFTCOL;
                  break;
   case NEWLINE : scrollLineWindow();
                  lineCol = LINE_WINLEFTCOL;
                  break;
   default      : writeChar( LINE_WINBOTTOMROW, lineCol, character );
                  ++lineCol;
                  if ( lineCol > LINE_WINRIGHTCOL )
                  {
                     scrollLineWindow();
                     lineCol = LINE_WINLEFTCOL;
                     }
                  break;
   }
}

/* Move cursor to specified row and column */
void setCursorPosition( unsigned int row, unsigned int column )
{
regs.h.ah = 2;                       /* AH = 2 - move cursor */
regs.h.bh = 0;                       /* BH = video page */
regs.h.dh = row;                     /* DH = row */
regs.h.dl = column;                  /* DL = column */
int86( 0x10, &regs, &regs );         /* Call BIOS */
}

/* Write character at specified row and column */
void writeChar( unsigned int row, 
                unsigned int column, 
                unsigned char character)
{
setCursorPosition( row, column );

regs.h.ah = 0x0a;                    /* AH = 0Ah - write character */
regs.h.al = character;               /* AL = character */
regs.h.bh = 0;                       /* BH = video page */
regs.x.cx = 1;                       /* CX = times to write character */
int86( 0x10, &regs, &regs );         /* Call BIOS */
}

/* Display string at specified row and column */
void say( unsigned int row, 
          unsigned int column, 
          char *stringPtr )
{
while( *stringPtr != 0 )
{
   writeChar( row, column, *stringPtr );
   ++column;
   ++stringPtr;
   }
}

/* Clear screen */
void clearScreen( void )
{
setVideoMode( getVideoMode() );
}

/* Set video mode */
void setVideoMode( unsigned char newMode )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - set video mode */
regs.h.al = newMode;          /* AL = video mode */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}

/* Return current video mode */
unsigned char getVideoMode( void )
{
union REGS regs;

regs.h.ah = 0x0f;             /* AH = 0Fh - read video mode */
int86( 0x10, &regs, &regs );  /* Call BIOS */
return( regs.h.al );          /* AL = current video mode */
}

/* ********************  Keyboard Handler  ********************** */
/* 
   Reads keyboard, constructs command line, and dispatches command
*/
void serviceKeyboard( void )
{
unsigned char byteFromKbd;

setCursorPosition( CURSORROW, cursorCol );

if ( kbhit() )
{
   byteFromKbd = getch();
   if ( converseModeOn )
   {
      if ( byteFromKbd == ESCAPE )
      {
         converseModeOn = NO;
         writeWindowString( "CONVERSE mode cancelled" );
         }
      else
         transmitChar( byteFromKbd );
      }
   else
   {
      switch ( byteFromKbd )
      {
         case ENTER     : if ( strlen(kbdBuffer) != 0 )
                          {
                             writeWindowString( kbdBuffer );
                             while( cursorCol != 0 )
                             {
                                say( CURSORROW, cursorCol, " " );
                                --cursorCol;
                                }
                             ++cursorCol;
                             dispatchCommand();
                             kbdBufferP = kbdBuffer;
                             *kbdBufferP = 0;
                             }
                          break;
         case ESCAPE    : while( cursorCol != 0 )
                          {
                             say( CURSORROW, cursorCol, " " );
                             --cursorCol;
                             }
                          ++cursorCol;
                          kbdBufferP = kbdBuffer;
                          *kbdBufferP = 0;
                          break;
         case BACKSPACE : if ( cursorCol != 1 )
                          {
                             --kbdBufferP;
                             *kbdBufferP = 0;
                             --cursorCol;
                             say( CURSORROW, cursorCol, " " );
                             }
                          break;
         default        : if ( cursorCol != (CMD_WINRIGHTCOL-1) )
                          {
                             *kbdBufferP = byteFromKbd;
                             ++kbdBufferP;
                             *kbdBufferP = 0;
                             ++cursorCol;
                             say( CURSORROW, 1, kbdBuffer );
                             }
                          break;
         }
      }
   }
}

/* ********  Serial Port Functions  ******** */
/* Display received data */
void checkForRcvdData( void )
{
if ( COM_ADDRESS(currentPort) != 0 )
{
   regs.h.ah = 3;                          /* AH = 3 - get port status */
   regs.x.dx = currentPort;                /* DX = port number */
   int86( 0x14, &regs, &regs );            /* Call BIOS */
   if ( regs.h.ah & DataReceived )         /* AH = line status */
   {
      regs.h.ah = 2;                       /* AH = 2 - read character */
      regs.x.dx = currentPort;             /* DX = port number */
      int86( 0x14, &regs, &regs );         /* Call BIOS */
      writeCharToLineWindow( regs.h.al );  /* AL = character */
      }
   }
}

/* Transmit character */
unsigned int transmitChar( unsigned char character )
{
regs.h.ah = 1;                         /* AH = 1 - send character */
regs.h.al = character;                 /* AL = character */
regs.x.dx = currentPort;               /* DX = port number */
int86( 0x14, &regs, &regs );           /* Call BIOS */
return( regs.h.ah );                   /* AH = line status */
}

/* Return line and modem status */
unsigned int getComPortStatus( void )
{
regs.h.ah = 3;                         /* AH = 3 - get status */
regs.x.dx = currentPort;               /* DX = port number */
int86( 0x14, &regs, &regs );           /* Call BIOS */
return( regs.x.ax );                   /* AH = line status */
                                       /* AL = modem status */
}

/* Display current line and modem status */
void showPortStatus( unsigned int row, 
                     unsigned int lsCol, 
                     unsigned int msCol )
{
unsigned int status;

unsigned char lineStatus,
              modemStatus,
              testBit;

status = getComPortStatus();

/* Upper 8 bits == line status, lower 8 bits == modem status */
lineStatus = status >> 8;     /* Isolate line status */
modemStatus = status;         /* Isolate modem status */

for( testBit = BIT7; testBit != 0; testBit >>= 1 , row++ )
{
   writeChar( row, lsCol, bitOnOff( lineStatus, testBit ) );
   writeChar( row, msCol, bitOnOff( modemStatus, testBit ) );
   }
}

/* Set baud rate, parity, and bit configuration for current port */
void setComParameters( void )
{
regs.h.ah = 0;                         /* AH = 0 - set parameters */
regs.h.al = cfgWords[currentPort];     /* AL = parameters */
regs.x.dx = currentPort;               /* DX = port number */
int86( 0x14, &regs, &regs );           /* Call BIOS */
}

/* Display speed, parity, etc. settings for current port */
void showSettings( void )
{
say( STATROW+0, STATCOL, comPortNames[currentPort] );

if ( COM_ADDRESS(currentPort) == 0 )
   say( STATROW+1, STATCOL, "Not Installed" );
else
{
   sprintf( scratch, "%.4Xh", COM_ADDRESS(currentPort) );
   say( STATROW+1, STATCOL, "             " );
   say( STATROW+1, STATCOL, scratch );
   }

sprintf( scratch, "%.3u", COM_TIMEOUT(currentPort) );
say( STATROW+2, STATCOL, scratch );

switch ( cfgWords[currentPort] & BAUD9600 )
{
   case BAUD110  : say( STATROW+3, STATCOL, "110 " );
                   break;
   case BAUD150  : say( STATROW+3, STATCOL, "150 " );
                   break;
   case BAUD300  : say( STATROW+3, STATCOL, "300 " );
                   break;
   case BAUD600  : say( STATROW+3, STATCOL, "600 " );
                   break;
   case BAUD1200 : say( STATROW+3, STATCOL, "1200" );
                   break;
   case BAUD2400 : say( STATROW+3, STATCOL, "2400" );
                   break;
   case BAUD4800 : say( STATROW+3, STATCOL, "4800" );
                   break;
   case BAUD9600 : say( STATROW+3, STATCOL, "9600" );
                   break;
   }

switch ( cfgWords[currentPort] & EVENPARITY )
{
   case NOPARITY   : say( STATROW+4, STATCOL, "None" );
                     break;
   case ODDPARITY  : say( STATROW+4, STATCOL, "Odd " );
                     break;
   case EVENPARITY : say( STATROW+4, STATCOL, "Even" );
                     break;
   }

switch ( cfgWords[currentPort] & DATA8 )
{
   case DATA7 : say( STATROW+5, STATCOL, "7" );
                break;
   case DATA8 : say( STATROW+5, STATCOL, "8" );
                break;
   }

switch ( cfgWords[currentPort] & STOP2 )
{
   case STOP1 : say( STATROW+6, STATCOL, "1" );
                break;
   case STOP2 : say( STATROW+6, STATCOL, "2" );
                break;
   }
}
