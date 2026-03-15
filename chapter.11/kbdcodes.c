/* ***************************************************
   KBDCODES.C 
   Copyright (c) 1992 - Billy P. Taylor
   ***************************************************
   Shows how to use interrupt 16h functions 0, 1, 10h, 
   and 11h to read and display keyboard information.  
   Use it to view the keyboard status bits and watch 
   the scan codes and character codes returned from BIOS.

   Note: this program requires a compiler that supports 
   inline assembly.

   To run, type KBDCODES<Enter>.
   *************************************************** */

#include <ctype.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

char *mainScreen[] = { 
"Interrupt 16h Function 02h   Function 12h   Function 12h ",
"              Register  AL   Register  AL   Register  AH ",
"     Insert :   .              .              .          : Sys Req              ",
"   Caps Lock:   .              .              .          : Caps Lock            ",
"    Num Lock:   .              .              .          : Num Lock             ",
"Scroll Lock :   .              .              .          : Scroll Lock          ",
"    Alt key :   .              .              .          : Right Alt            ",
"   Ctrl key :   .              .              .          : Right Ctrl           ",
" Left Shift :   .              .              .          : Left Alt             ",
"Right Shift :   .              .              .          : Left Ctrl            ",
"                                                                                ",
"Interrupt 16h  Function 11h                                                     ",
"              Scan/Char Code                                                    ",
0 };

/* 
   Define screen rows and columns for displaying status.
   Note that an 80 x 25 text screen is assumed.  We get
   one by setting video mode 3 in main().
*/
#define TEXT_MODE           3  /* Video mode to set */

#define ROW_DYNAMIC_STATUS  2  /* Keyboard status display row */
#define COL_FUNC02h_AL     16  /* Column for int 16h function 2 result */
#define COL_FUNC12h_AL     31  /* Column for int 16h func 12h AL result */
#define COL_FUNC12h_AH     46  /* Column for int 16h func 12h AH result */

#define ROW_TOP_SCAN_CODES 13  /* Keyboard data window top row */
#define ROW_BOT_SCAN_CODES 24  /* Keyboard data window bottom row */
#define COL_FUNC11h_LEFT   15  /* Int 16h func 11h display left column */
#define COL_FUNC11h_RIGHT  25  /* Int 16h func 11h display right column */

#define ONCE   1
#define NORMAL 7

char pressed[]  = "Pressed",
     released[] = "   .   ";

/* FUNCTION PROTOTYPES */
void          displayBits              ( unsigned char, 
                                         unsigned char, 
                                         unsigned char );
unsigned char getCurrentPage           ( void );
unsigned int  getEnhancedKbdChar       ( void );
unsigned int  getKbdChar               ( void );
unsigned char getVideoMode             ( void );
unsigned int  readExtendedShiftStatus  ( void );
unsigned char readShiftStatus          ( void );
void          scrollUp                 ( unsigned char, 
                                         unsigned char, 
                                         unsigned char,
                                         unsigned char, 
                                         unsigned char, 
                                         unsigned char );
void          setCursorPosition        ( unsigned char, 
                                         unsigned char, 
                                         unsigned char );
void          setVideoMode             ( unsigned char );
void          showCodes                ( unsigned char, 
                                         unsigned char, 
                                         unsigned char, 
                                         unsigned char );
unsigned int  testEnhancedKbdCharReady ( void );
unsigned int  testKbdCharReady         ( void );

void main( void )
{
unsigned char status,
              lastStatus,
              flagsRegister;

unsigned int  extendedStatus,
              lastExtendedStatus,
              scanAndCharCode,
              row;

/* Set 80x25 text video mode and prepare screen */
setVideoMode( TEXT_MODE );
for( row=0; mainScreen[row] != 0; row++)
{
   setCursorPosition( getCurrentPage(), row, 0 );
   printf( "%s", mainScreen[row] );
   }

/* Program loops here until you press Ctrl-Break */
for(;;)
{
/* Read 83/84-key keyboard shift status */
   status = readShiftStatus();

/* If it changed since last time, display it */
   if ( status != lastStatus )
   {
      displayBits( ROW_DYNAMIC_STATUS, 
                   COL_FUNC02h_AL, 
                   status );
      lastStatus = status;
      }

/* Read enhanced keyboard shift status */
   extendedStatus = readExtendedShiftStatus();

/* If it changed since last time, display it */
   if ( extendedStatus != lastExtendedStatus )
   {
      displayBits( ROW_DYNAMIC_STATUS, 
                   COL_FUNC12h_AL, 
                   extendedStatus );
      displayBits( ROW_DYNAMIC_STATUS, 
                   COL_FUNC12h_AH, 
                   (extendedStatus >> 8) );
      lastExtendedStatus = extendedStatus;
      }

/* Test if key was pressed */
   scanAndCharCode = testEnhancedKbdCharReady();
   if ( scanAndCharCode != 0 )
   {
      scrollUp( ONCE, 
                NORMAL, 
                ROW_TOP_SCAN_CODES, 
                COL_FUNC11h_LEFT, 
                ROW_BOT_SCAN_CODES, 
                COL_FUNC11h_RIGHT );

      scanAndCharCode = getEnhancedKbdChar();

      showCodes( scanAndCharCode >> 8, 
                 scanAndCharCode, 
                 ROW_BOT_SCAN_CODES, 
                 COL_FUNC11h_LEFT );
      }
   }
}

/* Display byte as column of bits */
void displayBits( unsigned char row, 
                  unsigned char col, 
                  unsigned char bitMask )
{
unsigned char bitToTest;
bitToTest = 128;
do
{
   setCursorPosition( getCurrentPage(), row, col );

   if ( (bitMask & bitToTest) )
      printf( "%s", pressed );
   else
      printf( "%s", released );

   ++row;
   bitToTest >>= 1;
   } while ( bitToTest );
}

/* Display scan code and character code */
void showCodes( unsigned char scanCode, 
                unsigned char charCode, 
                unsigned char row, 
                unsigned char col )
{
setCursorPosition( getCurrentPage(), row, col );
printf( "%.2Xh - %.2Xh %c", 
         scanCode, 
         charCode,
         ( isprint(charCode) ) ? charCode : ' ' );
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

/* Return active video page */
unsigned char getCurrentPage( void )
{
union REGS regs;

regs.h.ah = 0x0f;             /* AH = 0Fh - read video mode */
int86( 0x10, &regs, &regs );  /* Call BIOS */
return( regs.h.bh );          /* BH = current video page */
}

/* Move cursor */
void setCursorPosition( unsigned char videoPage, 
                        unsigned char row, 
                        unsigned char column )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - set cursor position */
regs.h.bh = videoPage;        /* BH = video page */
regs.h.dh = row;              /* DH = row number */
regs.h.dl = column;           /* DL = column number */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}

/* Scroll screen up */
void scrollUp( unsigned char times, 
               unsigned char attribute, 
               unsigned char topRow, 
               unsigned char leftColumn, 
               unsigned char bottomRow, 
               unsigned char rightColumn )
{
union REGS regs;

regs.h.ah = 6;                /* AH = 6 - scroll screen up */
regs.h.al = times;            /* AL = number of times to scroll */
regs.h.bh = attribute;        /* BH = attribute for new lines */
regs.h.ch = topRow;           /* CH = top row number */
regs.h.cl = leftColumn;       /* CL = leftmost column number */
regs.h.dh = bottomRow;        /* DH = bottom row number */
regs.h.dl = rightColumn;      /* DL = rightmost column number */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}

/* Return next scan code (high 8 bits) 
   and character code (low 8 bits) 
*/
unsigned int getKbdChar( void )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - get keyboard character */
int86( 0x16, &regs, &regs );  /* Call BIOS */
return( regs.x.ax );          /* AH = scan code, AL = character code */
}

/* Test whether key was pressed.  Returns scan code 
   (high 8 bits) and character code (low 8 bits).
   Must do this in assembly language as it uses the
   Zero flag which is not available in the standard
   C REGS structure.
*/
unsigned int testKbdCharReady( void )
{
unsigned int returnValue;

_asm  mov   AH, 1               /* AH = 1 - check for key pressed */
_asm  int   16h                 /* Call BIOS */
_asm  jnz   endFunc1            /* Zero false = key pressed */
_asm  xor   AX,AX               /* No data ready - return 0 */
endFunc1:
_asm  mov   returnValue, AX     /* AH = scan code, AL = character code */
return( returnValue );          /* Return codes */
}

/* Return conventional (8-bit) shift state */
unsigned char readShiftStatus( void )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - read keyboard shift state */
int86( 0x16, &regs, &regs );  /* Call BIOS */
return( regs.h.al );          /* AL = shift codes */
}

/* Return enhanced scan code (high 8 bits) 
   and character code (low 8 bits) 
*/
unsigned int getEnhancedKbdChar( void )
{
union REGS regs;

regs.h.ah = 0x10;             /* AH = 10h - get enhanced kbd character */
int86( 0x16, &regs, &regs );  /* Call BIOS */
return( regs.x.ax );          /* AH = scan code, AL = character code */
}

/* Test whether key pressed.  Return enhanced scan code 
   (high 8 bits) and character code (low 8 bits) 
   Must do this in assembly language as it uses the
   Zero flag which is not available in the standard
   C REGS structure.
*/
unsigned int testEnhancedKbdCharReady( void )
{
unsigned int  returnValue;

_asm  mov   AH, 11h             /* AH = 0x11 - test enh. key pressed */
_asm  int   16h                 /* Call BIOS */
_asm  jnz   endFunc11           /* Zero false = character ready */
_asm  xor   AX,AX               /* No character - return 0 */
endFunc11:
_asm  mov   returnValue, AX     /* AH = scan code, AL = character code */
return( returnValue );          /* Return codes */
}

/* Return extended shift status */
unsigned int readExtendedShiftStatus( void )
{
union REGS regs;

regs.h.ah = 0x12;             /* AH = 12h - read extended kbd shift state */
int86( 0x16, &regs, &regs );  /* Call BIOS */
return( regs.x.ax );          /* AH = extended shift state */
                              /* AL = conventional shift state */
}
