/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-8.  Display PC Character Set
*/


/*
   SHOWCSET.C
   Displays PC character set in 
   16 rows of 16 characters each
*/

#include <dos.h>
#include <stdio.h>

#define NORMAL 7           /* Normal white-on-black display */

void getCursorPosition( unsigned char, 
                        unsigned char *, 
                        unsigned char * );
void setCursorPosition( unsigned char, 
                        unsigned char, 
                        unsigned char );
void writeCell( unsigned char, 
                unsigned char, 
                unsigned char, 
                unsigned int );

void main( void )
{
int charValue;
unsigned char row, 
              column;

printf( "\n\n  Character Set Display" );
printf( "\n-------------------------" );

for ( charValue = 0;  
      charValue != 256;  
      charValue++ )
{
   if ( (charValue % 16) == 0 )
   {  /* Print legend after every 16 characters */
      printf( "\n%.3u-%.3u  ", charValue, charValue+15 );
      getCursorPosition( 0, &row, &column );
      }

   writeCell( 0, (unsigned char)charValue, NORMAL, 1 );
   ++column;
   setCursorPosition( 0, row, column );
   }
}

/* Returns current cursor position */ 
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

/* Moves cursor to specified row and column */ 
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

/* Writes character and attribute */ 
void writeCell( unsigned char videoPage,  
                unsigned char character,  
                unsigned char attribute,  
                unsigned int times ) 
{ 
union REGS regs; 

regs.h.ah = 9;                /* AH = 9 - write character cell */ 
regs.h.al = character;        /* AL = character to write */ 
regs.h.bh = videoPage;        /* BH = video page */ 
regs.h.bl = attribute;        /* BL = character's attribute */ 
regs.x.cx = times;            /* CX = number of repetitions */ 
int86( 0x10, &regs, &regs );  /* Call BIOS */ 
}                             /* Returns nothing */
