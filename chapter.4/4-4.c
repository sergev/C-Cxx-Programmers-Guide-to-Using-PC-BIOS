/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-4.  Read or Set Cursor Position
*/


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
