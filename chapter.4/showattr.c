/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-9.  Display PC Video Attributes
*/


/*
   SHOWATTR.C
   Displays video attributes for active mode
*/

#include <dos.h>
#include <stdio.h>

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
int attribute;
unsigned char row, column;
char scratch[5];

printf( "\n\n    Attribute Display" );
printf( "\n-------------------------" );

for ( attribute = 0;  attribute != 256;  attribute++ )
{
   if ( (attribute % 16) == 0 )
   {  /* Line feed after 16 attributes */
      printf( "\n" );
      getCursorPosition( 0, &row, &column );
      }

   sprintf( scratch, " %.3u", attribute );
   writeCell( 0, scratch[0], (unsigned char)attribute, 1 );

   ++column;
   setCursorPosition( 0, row, column );
   writeCell( 0, scratch[1], (unsigned char)attribute, 1 );

   ++column;
   setCursorPosition( 0, row, column );
   writeCell( 0, scratch[2], (unsigned char)attribute, 1 );

   ++column;
   setCursorPosition( 0, row, column );
   writeCell( 0, scratch[3], (unsigned char)attribute, 1 );

   ++column;
   setCursorPosition( 0, row, column );
   }
}

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

regs.h.ah = 9;                /* AH = 9 - write character and attribute */ 
regs.h.al = character;        /* AL = character */ 
regs.h.bh = videoPage;        /* BH = video page */ 
regs.h.bl = attribute;        /* BL = attribute */ 
regs.x.cx = times;            /* CX = number of repetitions */ 
int86( 0x10, &regs, &regs );  /* Call BIOS */ 
}                             /* Returns nothing */
