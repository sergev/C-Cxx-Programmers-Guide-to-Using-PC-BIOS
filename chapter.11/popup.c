/* ***************************************************
   POPUP.C
   Copyright (c) 1992 - Billy P. Taylor
   ***************************************************
   Uses video pages to produce text mode popup windows
   ************************************************** */

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>


/* Function prototypes */
void copyPage            ( unsigned char, 
                           unsigned char,
                           unsigned char,
                           unsigned char );
void getScreenParameters ( unsigned char *,
                           unsigned char *,
                           unsigned char *,
                           unsigned char * );
void makeVideoPageActive ( unsigned char );
void popup               ( unsigned char * );
void readCell            ( unsigned char,  
                           unsigned char *,  
                           unsigned char * );
void setCursorPosition   ( unsigned char, 
                           unsigned char, 
                           unsigned char );
void writeCell           ( unsigned char, 
                           unsigned char, 
                           unsigned char, 
                           unsigned int );

#define FG_WHITE    7         /* Video attributes */
#define BG_RED     64
#define INTENSE     8
#define POPUP_ATTRIBUTE FG_WHITE + BG_RED + INTENSE

void main( int argc, char *argv[] )
{
if ( argc > 1 )
   popup( argv[1] );
else
   popup( " Press any key to continue " );
}

/* Generates popup window */
void popup( unsigned char *string )
{
unsigned char activePage,    popupPage,
              currentMode,   maxRows,
              maxColumns,    msgRow,
              msgColumn;
/* 
   Must know active video page 
   and numbers of rows and columns
*/
getScreenParameters( &currentMode, 
                     &activePage, 
                     &maxRows, 
                     &maxColumns );

/* Choose unused video page */
popupPage = activePage + 1;

if ( popupPage >= 8 )
   popupPage = 0;

/*
   Copy active video page to spare page
*/
copyPage( activePage, popupPage, maxRows, maxColumns );

/* Center message */
msgRow = maxRows / 2;
msgColumn = (maxColumns - strlen( string )) / 2;

/* Write message to spare page */
while( *string != 0 )
{
   setCursorPosition( popupPage, msgRow, msgColumn );
   writeCell( popupPage, *string, POPUP_ATTRIBUTE, 1 );
   ++string;
   ++msgColumn;
   }

/* Activate spare page and message pops up */
makeVideoPageActive( popupPage );

/* Wait until user says to continue */
getch();

/* Restore original page and popup goes away */
makeVideoPageActive( activePage );
}

/*
   Returns current video mode, active 
   video page, and numbers of rows and columns
*/
void getScreenParameters( unsigned char *videoMode,
                          unsigned char *activePage,
                          unsigned char *textRows,
                          unsigned char *textColumns )
{
#define BDA_rows (unsigned int far *)0x00400084
union REGS regs;

regs.h.ah = 0x0f;               /* AH = 0Fh - get screen parameters */
int86( 0x10, &regs, &regs );    /* Call BIOS */

*videoMode = regs.h.al;         /* AL = video mode */
*activePage = regs.h.bh;        /* BH = active video page */
*textColumns = regs.h.ah;       /* AH = number of columns */
*textRows = *BDA_rows + 1;      /* Number of rows from BDA */
}

/* Activates specified video page */
void makeVideoPageActive( unsigned char whichPage )
{
union REGS regs;

regs.h.ah = 5;                /* AH = 5 - set active video page */
regs.h.al = whichPage;        /* AL = page (0..7) to activate */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}

/* Sets cursor position */
void setCursorPosition( unsigned char videoPage, 
                        unsigned char row, 
                        unsigned char column )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - set cursor position */
regs.h.bh = videoPage;        /* BH = video page */
regs.h.dh = row;              /* DH = row */
regs.h.dl = column;           /* DL = column */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}                             /* Returns nothing */

/* Writes character and attribute to specified video page */
void writeCell( unsigned char videoPage, 
                unsigned char character, 
                unsigned char attribute, 
                unsigned int times )
{
union REGS regs;

regs.h.ah = 9;                /* AH = 9 - write character and attribute */
regs.h.al = character;        /* AL = character */
regs.h.bh = videoPage;        /* BH = video page */
regs.h.bl = attribute;        /* BL = attribute */
regs.x.cx = times;            /* CX = number of repetitions */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}                             /* Returns nothing */

/* Returns character and attribute at current cursor position */
void readCell( unsigned char videoPage,  
               unsigned char *pChar,  
               unsigned char *pAttribute ) 
{ 
union REGS regs; 

regs.h.ah = 8;                /* AH = 8 - read character cell */ 
regs.h.bh = videoPage;        /* BH = video page to read */ 
int86( 0x10, &regs, &regs );  /* Call BIOS */ 
*pChar = regs.h.al;           /* AL = character */ 
*pAttribute = regs.h.ah;      /* AH = attribute */ 
}

/* Copies a video page */
void copyPage( unsigned char source, 
               unsigned char dest,
               unsigned char maxRows,
               unsigned char maxColumns )
{
unsigned char currentRow,
              currentColumn;
unsigned char character,
              attribute;

for( currentRow = 0; currentRow != maxRows; currentRow++ )
{
   for( currentColumn = 0; currentColumn != maxColumns; currentColumn++ )
   {
      setCursorPosition( source, currentRow, currentColumn );
      readCell( source, &character, &attribute );
      setCursorPosition( dest, currentRow, currentColumn );
      writeCell( dest, character, attribute, 1 );
      }
   }
}
