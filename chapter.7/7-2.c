/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-2.  Read String from Keyboard
*/


/* 
   General purpose string input function using interrupt 16h
*/

#include <stdio.h>
#include <ctype.h>
#include <dos.h>

/* Function prototypes */
void getCursorPosition ( unsigned char, 
                         unsigned char *, 
                         unsigned char * );
void getKbdChar        ( unsigned char *,
                         unsigned char * );
void setCursorPosition ( unsigned char, 
                         unsigned char, 
                         unsigned char );
void writeChar         ( unsigned char, 
                         unsigned char, 
                         unsigned int );

/* *** Function starts here *** */
void readKbdString( unsigned char videoPage,   /* Video page */
                    unsigned char row,         /* Starting row */
                    unsigned char column,      /* Starting column */
                    unsigned char maxLength,   /* Maximum length */
                    unsigned char *bufPtr )    /* Output buffer pointer */
{
#define BACKSPACE  8    /* Character code for Backspace key */
#define ENTER_KEY 13    /* Character code for Enter key */
#define WRITE_ONCE 1

union REGS regs;

unsigned char scanCode,
              charCode,
              currentLength;

for( currentLength = 0, *bufPtr = 0; ; )
{
/* Set cursor position for current character */
   setCursorPosition( videoPage, row, column );

/* Read next character from keyboard */
   getKbdChar( &charCode, &scanCode );

   switch ( charCode )
   {
/* Handle Backspace key */
      case BACKSPACE:
         if ( currentLength != 0 )   /* and string length != 0 */
         {
/* Move cursor to previous column */
            --column;
            setCursorPosition( videoPage, row, column );
/* Erase character in that position */
            writeChar( videoPage, ' ', WRITE_ONCE );
/* Decrement buffer pointer and null-terminate string */
            --bufPtr;
            *bufPtr = 0;
/* Decrement current string length */
            --currentLength;
            }
         break;
/* If Enter pressed, just return */
      case ENTER_KEY:
         return;
/* Handle all other keys here */
      default:
/* Buffer more keys if string is not full */
         if ( ( currentLength != maxLength ) && 
/* and if key we just read is printable */
              ( isprint(charCode) ) )
         {
/* Add key to end of buffer */
            *bufPtr = charCode;
/* Increment buffer pointer and null-terminate string */
            ++bufPtr;
            *bufPtr = 0;
/* Echo character to screen */
            writeChar( videoPage, charCode, WRITE_ONCE );
/* Increment cursor position and string length */
            ++column;
            ++currentLength;
            }
         break;
      }
   }
}

/* 
   Returns next scan code/character code 
   from typeahead buffer
*/
void getKbdChar( unsigned char *charCode,
                 unsigned char *scanCode )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 or 10h - read keyboard */
int86( 0x16, &regs, &regs );  /* Call BIOS */
*scanCode = regs.h.ah;        /* AH = scan code */
*charCode = regs.h.al;        /* AL = character code */
}                             /* Returns nothing */

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

/* Returns cursor position */
void getCursorPosition( unsigned char videoPage, 
                        unsigned char *row, 
                        unsigned char *column )
{
union REGS regs;

regs.h.ah = 3;                /* AH = 3 - get cursor position */
regs.h.bh = videoPage;        /* BH = video page */
int86( 0x10, &regs, &regs );  /* Call BIOS */

*row = regs.h.dh;             /* DH = current row */
*column = regs.h.dl;          /* DL = current column */
}

/*
   Writes one character n times 
   starting at current cursor position
*/
void writeChar( unsigned char videoPage, 
                unsigned char character, 
                unsigned int times )
{
union REGS regs;

regs.h.ah = 0x0a;             /* AH = 0x0A - write character */
regs.h.al = character;        /* AL = character */
regs.h.bh = videoPage;        /* BH = video page */
regs.x.cx = times;            /* CX = times to write character */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}                             /* Returns nothing */
