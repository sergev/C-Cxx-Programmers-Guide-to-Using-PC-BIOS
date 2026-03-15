/* ***************************************************
   LPTSTAT.C
   Copyright (c) 1992 - Billy P. Taylor
   ***************************************************
   Shows printer status under varying conditions, such as 
   paper out, printer busy, etc.  For network users, 
   note that your network software (Vines, NetWare, 
   etc.) may intercept interrupt 17h and substitute its 
   own printer driver.  When viewing network printers, 
   the status may not be accurate.

   To run, type LPTSTAT<Enter>.
   ************************************************** */

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define boolean unsigned char
#define YES 1
#define NO  0

#define BDA_lpt_addr (unsigned int  far *)0x00400008
#define BDA_lpt_to   (unsigned char far *)0x00400078

#define LPT_ADDRESS(which) *(BDA_lpt_addr+which)
#define LPT_TIMEOUT(which) *(BDA_lpt_to+which)

#define FORMFEED 12

/* ***********  Data  *********** */
union REGS regs;

boolean sendDataToPrinter = NO,
        somethingPrinted = NO,
        quitNow   = NO;

unsigned int currentPrinter = 0;

/* *********  Messages  ******** */

char *printerNames[] = { "LPT1","LPT2","LPT3" };

char title[]       = 
"                 LPTSTAT : Display Parallel Printer Status Word",
     dashes[]      = 
"--------------------------------------------------------------------------------",
     blankLine[]   = 
"                                                                                ",
     prtDisabled[] = "Printing disabled",
     prtEnabled[]  = "Printing enabled ";

char *screen[] =
{
title,
dashes,
"This utility shows the real-time status word for the selected parallel printer.",
"Turn the printer on and off, set it online and offline, take the paper out, etc.",
"and watch how the status word changes for each condition.",
blankLine,
"Bit State   Function",
" 7  ´ . - Printer Ready",
" 6  ´ . - Acknowledge",
" 5  ´ . - Out of Paper",
" 4  ´ . - Selected",
" 3  ´ . - I/O error",
" 2  ´ . - reserved",
" 1  ´ . - reserved",
" 0  ´ . - Timeout",
blankLine,
blankLine,
blankLine,
blankLine,
blankLine,
blankLine,
blankLine,
"Press 1 = LPT1    W = Write to printer",
"      2 = LPT2    P = Pause printing",
"      3 = LPT3    Q = Quit",
0 };

/* 
   Define screen rows and columns for displaying status.
   Note that an 80 x 25 text screen is assumed.  We get
   one by setting video mode 3 in main().
*/
#define TEXT_MODE   3     /* Video mode to set */

#define PADDROW    18     /* Row to display printer address */
#define PLEGENDROW 20     /* Row to display printer installed message */
#define PSTATROW    7     /* Printer status row */
#define PSTATCOL    6     /* Printer status column */

char stringToPrint[]  = 
"\015AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789\n";

char *nextByteToPrint = stringToPrint;

/* *******  Function Prototypes  ******** */
unsigned int  getPrinterStatus    ( unsigned int );
unsigned char getVideoMode        ( void );
unsigned int  printChar           ( unsigned int, 
                                    unsigned char );
void          say                 ( unsigned int, 
                                    unsigned int, 
                                    char * );
void          sayPrinterName      ( unsigned int );
void          sendByteToPrinter   ( void );
void          serviceKeyboard     ( void );
void          setCursorPosition   ( unsigned int, 
                                    unsigned int );
void          setVideoMode        ( unsigned char );
void          showPrinterStatus   ( unsigned int, 
                                    unsigned int, 
                                    unsigned int );
void          writeChar           ( unsigned int, 
                                    unsigned int, 
                                    unsigned char );

/* ***********  Main  *********** */
void main( void )
{
unsigned int row;

/* Set 80x25 text video mode and prepare screen */
setVideoMode( TEXT_MODE );
for( row=0; screen[row] != 0; row++ )
   say( row, 0, screen[row] );

/* Indicate which printer we are watching */
sayPrinterName( currentPrinter );
say( PLEGENDROW, 0, prtDisabled );

/* Main loop */
while ( !quitNow )
{
/* Process keyboard input */
   serviceKeyboard();

/* Print line of text if needed */
   if ( sendDataToPrinter )
      sendByteToPrinter();

/* Display printer status */
   showPrinterStatus( currentPrinter, PSTATROW, PSTATCOL );
   }

/* Print trailing form feed to empty printer buffer */
if ( somethingPrinted )
   printChar( currentPrinter, FORMFEED );

/* Back to DOS */
setCursorPosition( 24, 0 );
exit( 0 );
}

/* Service keyboard and act on commands entered */
void serviceKeyboard( void )
{
if ( kbhit() )
{
   switch ( getch() )
   {
      case '1' : currentPrinter = 0;
                 sayPrinterName( currentPrinter );
                 break;
      case '2' : currentPrinter = 1;
                 sayPrinterName( currentPrinter );
                 break;
      case '3' : currentPrinter = 2;
                 sayPrinterName( currentPrinter );
                 break;
      case 'p' :
      case 'P' : if ( sendDataToPrinter )
                 {
                    say( PLEGENDROW, 0, prtDisabled );
                    sendDataToPrinter = NO;
                    }
                 break;
      case 'q' :
      case 'Q' : quitNow = YES;
                 break;
      case 'w' :
      case 'W' : if ( LPT_ADDRESS(currentPrinter) != 0 )
                 {
                    say( PLEGENDROW, 0, prtEnabled );
                    sendDataToPrinter = YES;
                    }
                 break;
      }
   }
}

/* Print next stringToPrint[] character and advance 
   pointer for next call.  If end-of-string, return 
   pointer to start of string
*/
void sendByteToPrinter( void )
{
printChar( currentPrinter, *nextByteToPrint );
somethingPrinted = YES;

++nextByteToPrint;
if ( *nextByteToPrint == 0 )
   nextByteToPrint = stringToPrint;
}

/* Print character */
unsigned int printChar( unsigned int whichPrinter, 
                        unsigned char character )
{
regs.h.ah = 0;                     /* AH = 0 - print character */
regs.h.al = character;             /* AL = character */
regs.x.dx = whichPrinter;          /* DX = printer number */
int86( 0x17, &regs, &regs );       /* Call BIOS */
return( regs.h.ah );               /* AH = status */
}

/* Return printer status */
unsigned int getPrinterStatus( unsigned int whichPrinter )
{
regs.h.ah = 2;                     /* AH = 2 - get printer status */
regs.x.dx = whichPrinter;          /* DX = printer number */
int86( 0x17, &regs, &regs );       /* Call BIOS */
return( regs.h.ah );               /* AH = status */
}

/* Set video mode */
void setVideoMode( unsigned char newMode )
{
union REGS regs;

regs.h.ah = 0;                     /* AH = 0 - set video mode */
regs.h.al = newMode;               /* AL = video mode */
int86( 0x10, &regs, &regs );       /* Call BIOS */
}

/* Return current video mode */
unsigned char getVideoMode( void )
{
union REGS regs;

regs.h.ah = 0x0f;                  /* AH = 0Fh - read video state */
int86( 0x10, &regs, &regs );       /* Call BIOS */
return( regs.h.al );               /* AL = current video mode */
}

/* Move cursor to specified row and column */
void setCursorPosition( unsigned int row,
                        unsigned int column )
{
regs.h.ah = 2;                     /* AH = 2 - move cursor */
regs.h.bh = 0;                     /* BH = video page */
regs.h.dh = row;                   /* DH = row */
regs.h.dl = column;                /* DL = column */
int86( 0x10, &regs, &regs );       /* Call BIOS */
}

/* Display character at specified row and column */
void writeChar( unsigned int row, 
                unsigned int column, 
                unsigned char character)
{
setCursorPosition( row, column );

regs.h.ah = 0x0a;                  /* AH = 0Ah - write character */
regs.h.al = character;             /* AL = character */
regs.h.bh = 0;                     /* BH = video page */
regs.x.cx = 1;                     /* CX = times to write character */

int86( 0x10, &regs, &regs );       /* Call BIOS */
}

/* Display specified printer's status */
void showPrinterStatus( unsigned int whichPrinter, 
                        unsigned int row, 
                        unsigned int col )
{
unsigned char status;

status = getPrinterStatus( whichPrinter );

writeChar( row++, col, (char)('0' + ((status & 128) != 0)) );
writeChar( row++, col, (char)('0' + ((status &  64) != 0)) );
writeChar( row++, col, (char)('0' + ((status &  32) != 0)) );
writeChar( row++, col, (char)('0' + ((status &  16) != 0)) );
writeChar( row++, col, (char)('0' + ((status &   8) != 0)) );
writeChar( row++, col, (char)('0' + ((status &   4) != 0)) );
writeChar( row++, col, (char)('0' + ((status &   2) != 0)) );
writeChar( row,   col, (char)('0' + ((status &   1) != 0)) );
}

/* Display printer name and I/O address */
void sayPrinterName( unsigned int whichPrinter )
{
say( PADDROW, 0, blankLine );

setCursorPosition( PADDROW, 0 );
printf( "%s is ", printerNames[whichPrinter] );

if ( LPT_ADDRESS(whichPrinter) == 0 )
{
   printf( "not installed." );
   say( PLEGENDROW, 0, prtDisabled );
   sendDataToPrinter = NO;
   }
else
   printf( "installed at I/O port %.4X - timeout value is %u seconds.",
            LPT_ADDRESS(whichPrinter),
            LPT_TIMEOUT(whichPrinter) );
}

/* Display string at specified row and column */
void say( unsigned int row, 
          unsigned int column, 
          char *stringPtr )
{
setCursorPosition( row, column );
printf( "%s", stringPtr );
}
